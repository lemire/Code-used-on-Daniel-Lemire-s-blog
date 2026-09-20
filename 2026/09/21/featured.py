# Super simple featured image: two lines meeting, like a taken
# branch joining a straight path. 1200x630, no text.
import subprocess
from PIL import Image, ImageDraw

W, H = 1200, 630
BG, INK = "#f7f6f2", "#1a1a1a"
img = Image.new("RGB", (W, H), BG)
d = ImageDraw.Draw(img)

w = 22
# Straight path (not-taken): left to right through the middle.
y0 = H // 2
d.line([(140, y0), (1060, y0)], fill=INK, width=w)

# Second branch coming in from above and meeting the path.
# Rounded join by drawing a quadratic-ish polyline.
pts = [(280, 130), (430, 130), (560, y0)]
d.line(pts, fill=INK, width=w, joint="curve")

# Round the visible endpoints.
r = w // 2
for x, y in ((140, y0), (1060, y0), (280, 130)):
    d.ellipse((x - r, y - r, x + r, y + r), fill=INK)

img.save("featured.png")
subprocess.run(
    ["cwebp", "-quiet", "-q", "90", "featured.png", "-o", "featured.webp"],
    check=False,
)
