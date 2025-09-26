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


LPIPS_AVAILABLE = True


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
    else:
        success = compare_cv(img1_path, img2_path)

    sys.exit(1)
    #sys.exit(0 if success else 1)
