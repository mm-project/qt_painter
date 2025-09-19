import cv2
import sys
import os
from skimage.metrics import structural_similarity as ssim

def compare_images(img1_path, img2_path, diff_path=None):
    # Default diff file name: <expected>.diff.png
    if diff_path is None:
        base, ext = os.path.splitext(img2_path)
        diff_path = f"{base}.diff.png"

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
        print("Usage: python compare_images.py <expected.png> <actual.png> [diff.png]")
        sys.exit(1)

    img1_path = sys.argv[1]
    img2_path = sys.argv[2]
    diff_path = sys.argv[3] if len(sys.argv) > 3 else None

    success = compare_images(img1_path, img2_path, diff_path)
    sys.exit(0 if success else 1)
