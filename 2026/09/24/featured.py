# Featured image: a short integer, as a string. 1200x630, no title.
import subprocess
from PIL import Image, ImageDraw, ImageFont

W, H = 1200, 630
BG, INK = "#f7f6f2", "#1a1a1a"
TEXT = '"12345678"'

img = Image.new("RGB", (W, H), BG)
d = ImageDraw.Draw(img)
font = ImageFont.truetype(
    "/System/Library/Fonts/HelveticaNeue.ttc", 188, index=1
)
box = d.textbbox((0, 0), TEXT, font=font)
x = (W - (box[2] - box[0])) / 2 - box[0]
y = (H - (box[3] - box[1])) / 2 - box[1]
d.text((x, y), TEXT, font=font, fill=INK)

img.save("featured.png")
subprocess.run(
    ["cwebp", "-quiet", "-q", "90", "featured.png", "-o", "featured.webp"],
    check=False,
)
