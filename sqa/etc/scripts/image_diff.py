import sys
import os
from PIL import Image
import imagehash
import cv2
import sys
import os
from skimage.metrics import structural_similarity as ssim
import torch
import lpips
import torchvision.transforms as T
from PIL import Image as PILImage
import numpy as np


LPIPS_AVAILABLE = True


def compare_shapes(img1_path, img2_path, diff_path="diff.png"):
    img1 = cv2.imread(img1_path)
    img2 = cv2.imread(img2_path)

    # Ensure same size
    if img1.shape != img2.shape:
        img2 = cv2.resize(img2, (img1.shape[1], img1.shape[0]))

    # Convert to grayscale & threshold
    gray1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
    gray2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
    _, th1 = cv2.threshold(gray1, 10, 255, cv2.THRESH_BINARY)
    _, th2 = cv2.threshold(gray2, 10, 255, cv2.THRESH_BINARY)

    # XOR to see changed regions
    diff = cv2.bitwise_xor(th1, th2)

    # Overlay red differences on img2
    overlay = img2.copy()
    overlay[diff > 0] = [0, 0, 255]
    cv2.imwrite(diff_path, overlay)

    nonzero = np.count_nonzero(diff)
    print(f"Changed pixels: {nonzero}")
    print(f"Diff image saved at {diff_path}")

    return nonzero == 0

def compare_phash(img1_path, img2_path, threshold=5):
    """Compare two images using perceptual hashing (pHash)."""
    hash1 = imagehash.phash(Image.open(img1_path))
    hash2 = imagehash.phash(Image.open(img2_path))
    diff = hash1 - hash2
    print(f"pHash difference: {diff}")

    if diff <= threshold:
        print("✅ Images are perceptually identical")
        return True
    else:
        print("❌ Images differ")
        return False


def compare_lpips(img1_path, img2_path, threshold=0.1):
    """Compare two images using LPIPS (deep perceptual similarity)."""
    if not LPIPS_AVAILABLE:
        print("⚠️ LPIPS not available. Install with: pip install torch lpips torchvision")
        return False

    loss_fn = lpips.LPIPS(net='alex')  # could also use 'vgg'

    transform = T.Compose([T.Resize((256, 256)), T.ToTensor()])
    img1 = transform(PILImage.open(img1_path).convert("RGB")).unsqueeze(0)
    img2 = transform(PILImage.open(img2_path).convert("RGB")).unsqueeze(0)

    dist = loss_fn(img1, img2)
    score = dist.item()
    print(f"LPIPS distance: {score:.4f}")

    if score < threshold:
        print("✅ Images are perceptually identical")
        return True
    else:
        print("❌ Images differ")
        return False

def compare_cv(img1_path, img2_path, diff_path=None):
    # Default diff file name: <expected>.diff.png
    if diff_path is None:
        base, ext = os.path.splitext(img2_path)
        diff_path = f"{base}.diff2.png"

    # Read images in grayscale (luminance only)
    img1 = cv2.imread(img1_path, cv2.IMREAD_GRAYSCALE)
    img2 = cv2.imread(img2_path, cv2.IMREAD_GRAYSCALE)

    if img1 is None or img2 is None:
        print("❌ Error: Could not read one of the images.")
        return False

    # Resize to same shape if needed
    if img1.shape != img2.shape:
        print("Resizing second image to match first")
        img2 = cv2.resize(img2, (img1.shape[1], img1.shape[0]))

    # Compute SSIM
    score, diff = ssim(img1, img2, full=True)
    print(f"SSIM score: {score:.4f}")

    # Create diff image for debugging
    diff = (diff * 255).astype("uint8")
    cv2.imwrite(diff_path, diff)
    print(f"Diff image saved to: {diff_path}")

    # Decide pass/fail
    if score > 0.99:   # threshold can be adjusted
        print("✅ Images are perceptually identical")
        return True
    else:
        print("❌ Images differ")
        return False

def extract_boxes(img_path):
    """Load image, threshold, and extract bounding boxes of shapes."""
    img = cv2.imread(img_path)
    if img is None:
        raise FileNotFoundError(f"❌ Could not load image: {img_path}")

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    _, th = cv2.threshold(gray, 10, 255, cv2.THRESH_BINARY)

    contours, _ = cv2.findContours(th, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    boxes = [cv2.boundingRect(c) for c in contours]  # (x, y, w, h)
    return img, boxes


def iou(boxA, boxB):
    """Intersection over Union for two rectangles."""
    xA = max(boxA[0], boxB[0])
    yA = max(boxA[1], boxB[1])
    xB = min(boxA[0]+boxA[2], boxB[0]+boxB[2])
    yB = min(boxA[1]+boxA[3], boxB[1]+boxB[3])

    inter = max(0, xB - xA) * max(0, yB - yA)
    areaA = boxA[2] * boxA[3]
    areaB = boxB[2] * boxB[3]
    return inter / float(areaA + areaB - inter + 1e-6)


def compare_shapes2(img1_path, img2_path, diff_path="diff.png", iou_threshold=0.5):
    """Compare shapes between two images using bounding boxes + IoU."""
    img1, boxes1 = extract_boxes(img1_path)
    img2, boxes2 = extract_boxes(img2_path)

    matched = []
    unmatched1 = []
    unmatched2 = list(range(len(boxes2)))  # all are unmatched initially

    # Try to match each box in img1 with best box in img2
    for i, b1 in enumerate(boxes1):
        best_match = -1
        best_iou = 0
        for j, b2 in enumerate(boxes2):
            score = iou(b1, b2)
            if score > best_iou:
                best_iou = score
                best_match = j
        if best_iou >= iou_threshold:
            matched.append((i, best_match, best_iou))
            if best_match in unmatched2:
                unmatched2.remove(best_match)
        else:
            unmatched1.append(i)

    # Draw results on copy of second image
    diff_img = img2.copy()
    for i in unmatched2:
        x, y, w, h = boxes2[i]
        cv2.rectangle(diff_img, (x, y), (x+w, y+h), (0, 0, 255), 3)  # extra shapes in red
    for i in unmatched1:
        x, y, w, h = boxes1[i]
        cv2.rectangle(diff_img, (x, y), (x+w, y+h), (255, 0, 0), 3)  # missing shapes in blue
    for (i, j, score) in matched:
        x, y, w, h = boxes2[j]
        cv2.rectangle(diff_img, (x, y), (x+w, y+h), (0, 255, 0), 2)  # matched in green

    cv2.imwrite(diff_path, diff_img)
    print(f"🖼️ Diff image saved at {diff_path}")

    # Print summary
    print("=== Comparison Results ===")
    for (i, j, score) in matched:
        print(f"✅ Shape {i} (img1) matched with Shape {j} (img2), IoU={score:.2f}")
    for i in unmatched1:
        print(f"❌ Shape {i} from img1 missing in img2")
    for j in unmatched2:
        print(f"➕ Extra Shape {j} in img2 not in img1")

    return len(unmatched1) == 0 and len(unmatched2) == 0

def extract_contours(img_path):
    """Load image, threshold, and extract contours."""
    img = cv2.imread(img_path)
    if img is None:
        raise FileNotFoundError(f"❌ Could not load {img_path}")

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    _, th = cv2.threshold(gray, 10, 255, cv2.THRESH_BINARY)

    contours, _ = cv2.findContours(th, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    return img, contours


def compare_contours(img1_path, img2_path, diff_path="diff.png", threshold=0.01):
    """Compare contours between two images using cv2.matchShapes."""
    img1, cnts1 = extract_contours(img1_path)
    img2, cnts2 = extract_contours(img2_path)

    matched = []
    unmatched1 = []
    unmatched2 = list(range(len(cnts2)))

    for i, c1 in enumerate(cnts1):
        best_score = 1e9
        best_match = -1
        for j, c2 in enumerate(cnts2):
            score = cv2.matchShapes(c1, c2, cv2.CONTOURS_MATCH_I1, 0)
            if score < best_score:
                best_score = score
                best_match = j
        if best_score < threshold:
            matched.append((i, best_match, best_score))
            if best_match in unmatched2:
                unmatched2.remove(best_match)
        else:
            unmatched1.append(i)

    # Visualize
    diff_img = img2.copy()
    for i in unmatched2:
        cv2.drawContours(diff_img, [cnts2[i]], -1, (0, 0, 255), 3)  # red = extra
    for i in unmatched1:
        cv2.drawContours(diff_img, [cnts1[i]], -1, (255, 0, 0), 3)  # blue = missing
    for (i, j, score) in matched:
        cv2.drawContours(diff_img, [cnts2[j]], -1, (0, 255, 0), 2)  # green = matched

    cv2.imwrite(diff_path, diff_img)
    print(f"🖼️ Diff saved at {diff_path}")

    # Report
    print("=== Results ===")
    for (i, j, score) in matched:
        print(f"✅ Contour {i} (img1) matched with {j} (img2), score={score:.5f}")
    for i in unmatched1:
        print(f"❌ Contour {i} from img1 missing in img2")
    for j in unmatched2:
        print(f"➕ Extra contour {j} in img2 not in img1")

    return len(unmatched1) == 0 and len(unmatched2) == 0

def compare_graphics(
    img1_path,
    img2_path,
    diff_path=None,
    bin_threshold=10,     # treat pixels >10 as "ink"
    open_kernel=3,        # 3x3 opening removes 1px dots/lines
    min_blob_area=50,     # ignore tiny residuals (< 50 px)
    max_diff_ratio=0.001  # allow up to 0.1% of pixels as noise
):
    """
    Robust compare for synthetic graphics (blocks, lines, UI renders).

    Returns True if images are "same" within tolerance, else False.
    Saves a diff overlay image where remaining differences are marked red.
    """
    img1 = cv2.imread(img1_path)
    img2 = cv2.imread(img2_path)
    if img1 is None or img2 is None:
        print("❌ Could not read one of the images.")
        return False

    # 1) Align sizes by cropping to common area (avoids resize artifacts)
    h = min(img1.shape[0], img2.shape[0])
    w = min(img1.shape[1], img2.shape[1])
    img1 = img1[:h, :w]
    img2 = img2[:h, :w]

    # 2) Ink masks (background here is near-black)
    g1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
    g2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
    _, th1 = cv2.threshold(g1, bin_threshold, 255, cv2.THRESH_BINARY)
    _, th2 = cv2.threshold(g2, bin_threshold, 255, cv2.THRESH_BINARY)

    # 3) Remove thin noise (1px grids/AA) with a small opening
    k = cv2.getStructuringElement(cv2.MORPH_RECT, (open_kernel, open_kernel))
    th1 = cv2.morphologyEx(th1, cv2.MORPH_OPEN, k, iterations=1)
    th2 = cv2.morphologyEx(th2, cv2.MORPH_OPEN, k, iterations=1)

    # 4) XOR to get changed regions
    diff_mask = cv2.bitwise_xor(th1, th2)

    # 5) Drop tiny blobs by area
    num, labels, stats, _ = cv2.connectedComponentsWithStats(diff_mask, connectivity=8)
    keep = np.zeros_like(diff_mask)
    kept_boxes = []
    for i in range(1, num):  # skip background
        area = stats[i, cv2.CC_STAT_AREA]
        if area >= min_blob_area:
            keep[labels == i] = 255
            x = stats[i, cv2.CC_STAT_LEFT]
            y = stats[i, cv2.CC_STAT_TOP]
            w_ = stats[i, cv2.CC_STAT_WIDTH]
            h_ = stats[i, cv2.CC_STAT_HEIGHT]
            kept_boxes.append((x, y, w_, h_, area))

    # 6) Decide, and visualize on top of img2
    diff_pixels = int(np.count_nonzero(keep))
    diff_ratio = diff_pixels / float(h * w)
    overlay = img2.copy()
    overlay[keep > 0] = [0, 0, 255]  # mark diffs red

    if diff_path is None:
        base, _ = os.path.splitext(img2_path)
        diff_path = f"{base}.diff.graphics.png"
    cv2.imwrite(diff_path, overlay)

    print(f"Pixels different (after filtering): {diff_pixels}  ({diff_ratio*100:.4f}%)")
    if kept_boxes:
        print("Remaining diff blobs (x, y, w, h, area):")
        for b in kept_boxes:
            print("  ", b)
    print(f"🖼️ Diff saved: {diff_path}")

    return diff_ratio <= max_diff_ratio

def compare_graphics2(
    img1_path,
    img2_path,
    diff_path=None,
    bin_threshold=10,
    min_blob_area=50,
    max_diff_ratio=0.001
):
    img1 = cv2.imread(img1_path)
    img2 = cv2.imread(img2_path)
    if img1 is None or img2 is None:
        print("❌ Could not read one of the images.")
        return False

    # Align sizes
    h = min(img1.shape[0], img2.shape[0])
    w = min(img1.shape[1], img2.shape[1])
    img1 = img1[:h, :w]
    img2 = img2[:h, :w]

    # Grayscale + threshold
    g1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
    g2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
    _, th1 = cv2.threshold(g1, bin_threshold, 255, cv2.THRESH_BINARY)
    _, th2 = cv2.threshold(g2, bin_threshold, 255, cv2.THRESH_BINARY)

    # XOR mask
    diff_mask = cv2.bitwise_xor(th1, th2)

    # Morphology: closing first, then opening
    k = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
    diff_mask = cv2.morphologyEx(diff_mask, cv2.MORPH_CLOSE, k, iterations=1)
    diff_mask = cv2.morphologyEx(diff_mask, cv2.MORPH_OPEN, k, iterations=1)

    # Connected components
    num, labels, stats, _ = cv2.connectedComponentsWithStats(diff_mask, connectivity=8)
    keep = np.zeros_like(diff_mask)
    kept_boxes = []
    for i in range(1, num):
        area = stats[i, cv2.CC_STAT_AREA]
        if area >= min_blob_area:
            keep[labels == i] = 255
            x = stats[i, cv2.CC_STAT_LEFT]
            y = stats[i, cv2.CC_STAT_TOP]
            w_ = stats[i, cv2.CC_STAT_WIDTH]
            h_ = stats[i, cv2.CC_STAT_HEIGHT]
            kept_boxes.append((x, y, w_, h_, area))

    # Count & ratio
    diff_pixels = int(np.count_nonzero(keep))
    diff_ratio = diff_pixels / float(h * w)

    # Overlay diffs in red
    overlay = img2.copy()
    overlay[keep > 0] = [0, 0, 255]

    if diff_path is None:
        base, _ = os.path.splitext(img2_path)
        diff_path = f"{base}.diff.graphics.png"
    cv2.imwrite(diff_path, overlay)

    print(f"Pixels different (after filtering): {diff_pixels}  ({diff_ratio*100:.4f}%)")
    if kept_boxes:
        print("Remaining diff blobs (x, y, w, h, area):")
        for b in kept_boxes:
            print("  ", b)
    print(f"🖼️ Diff saved: {diff_path}")

    return diff_ratio <= max_diff_ratio

def extract_boxes1(img_path):
    """Extract bounding boxes of objects from an image."""
    img = cv2.imread(img_path)
    if img is None:
        raise FileNotFoundError(f"❌ Could not load {img_path}")

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    _, th = cv2.threshold(gray, 10, 255, cv2.THRESH_BINARY)

    contours, _ = cv2.findContours(th, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    boxes = [cv2.boundingRect(c) for c in contours]
    return img, boxes

def iou1(boxA, boxB):
    """Intersection over Union for two rectangles."""
    xA = max(boxA[0], boxB[0])
    yA = max(boxA[1], boxB[1])
    xB = min(boxA[0]+boxA[2], boxB[0]+boxB[2])
    yB = min(boxA[1]+boxA[3], boxB[1]+boxB[3])

    inter = max(0, xB-xA) * max(0, yB-yA)
    areaA = boxA[2]*boxA[3]
    areaB = boxB[2]*boxB[3]
    return inter / float(areaA + areaB - inter + 1e-6)

def compare_rectangles(img1_path, img2_path, diff_path="diff_rect.png", iou_threshold=0.5):
    img1, boxes1 = extract_boxes1(img1_path)
    img2, boxes2 = extract_boxes1(img2_path)

    matched, missing, extra = [], [], list(range(len(boxes2)))

    for i, b1 in enumerate(boxes1):
        best_match, best_iou = -1, 0
        for j, b2 in enumerate(boxes2):
            score = iou1(b1, b2)
            if score > best_iou:
                best_iou, best_match = score, j
        if best_iou >= iou_threshold:
            matched.append((i, best_match, best_iou))
            if best_match in extra:
                extra.remove(best_match)
        else:
            missing.append(i)

    # Draw visualization
    diff_img = img2.copy()
    for j in extra:
        x,y,w,h = boxes2[j]
        cv2.rectangle(diff_img, (x,y), (x+w,y+h), (0,0,255), 3)  # red = extra
    for i in missing:
        x,y,w,h = boxes1[i]
        cv2.rectangle(diff_img, (x,y), (x+w,y+h), (255,0,0), 3)  # blue = missing
    for i,j,score in matched:
        x,y,w,h = boxes2[j]
        cv2.rectangle(diff_img, (x,y), (x+w,y+h), (0,255,0), 2)  # green = matched

    cv2.imwrite(diff_path, diff_img)
    print(f"🖼️ Diff saved: {diff_path}")

    print("=== Results ===")
    for i,j,score in matched:
        print(f"✅ Box {i} (img1) matched with Box {j} (img2), IoU={score:.2f}")
    for i in missing:
        print(f"❌ Box {i} missing in img2")
    for j in extra:
        print(f"➕ Extra Box {j} in img2")

    return len(missing)==0 and len(extra)==0

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python compare_images.py <expected.png> <actual.png> [method]")
        sys.exit(1)

    img1_path = sys.argv[1]
    img2_path = sys.argv[2]
    method = sys.argv[3]

    if method == "method1":
        success = compare_lpips(img1_path, img2_path)
    elif method == "method2":
        success = compare_phash(img1_path, img2_path)
    elif method == "method3":
        success = compare_cv(img1_path, img2_path)
    elif method == "method4":
        success = compare_shapes(img1_path, img2_path)
    elif method == "method5":
        success = compare_shapes2(img1_path, img2_path)
    elif method == "method6":
        success = compare_contours(img1_path, img2_path)
    elif method == "method7":
        success = compare_graphics(img1_path, img2_path)
    elif method == "method8":
        success = compare_graphics2(img1_path, img2_path)
    elif method == "method9":
        success = compare_rectangles(img1_path, img2_path)

    if success:
        print("")
        print("✅ same")
        sys.exit(0)
    else:
        print("")
        print("❌ different")
        sys.exit(1)

