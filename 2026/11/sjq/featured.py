# Feature image for the post. featured.png is a picture, 1200x630.
# This writes the JPEG and WebP siblings. It does not draw the image.
import subprocess

from PIL import Image

PNG = "featured.png"
im = Image.open(PNG).convert("RGB")
im.save("featured.jpg", quality=90)
subprocess.run(["cwebp", "-quiet", "-q", "90", PNG, "-o", "featured.webp"], check=True)
