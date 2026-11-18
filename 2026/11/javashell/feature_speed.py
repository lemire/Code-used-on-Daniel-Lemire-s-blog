"""Feature image for the MIME base64 speed post, 1200x630.

Top: real MIME base64 (76 characters, then CRLF). Bottom: fastest vs slowest decoder, to scale.
"""
import base64
import random

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch

BG = "#16130f"
INK = "#f5efe6"
MUTED = "#a8a29e"
TEXT = "#6b625a"
TEAL = "#2dd4bf"
RED = "#f43f5e"

FAST, SLOW = 18.4, 0.61

random.seed(2026)
b64 = base64.b64encode(bytes(random.getrandbits(8) for _ in range(57 * 6))).decode()
lines = [b64[i:i + 76] for i in range(0, len(b64), 76)][:6]

fig = plt.figure(figsize=(12, 6.3), dpi=100)
fig.patch.set_facecolor(BG)
ax = fig.add_axes([0, 0, 1, 1])
ax.set_xlim(0, 1200); ax.set_ylim(0, 630); ax.axis("off")

mono = {"fontname": "Menlo", "fontsize": 15.5}
x0, y0, dy = 60, 575, 31
for i, line in enumerate(lines):
    y = y0 - i * dy
    ax.text(x0, y, line, color=TEXT, ha="left", va="center", **mono)
    ax.text(x0, y, " " * 76 + "\\r\\n", color=RED, ha="left", va="center", fontweight="bold", **mono)

sans = {"fontname": "Avenir Next"}
left, full = 60, 1080 * 0.80  # bar length of the fastest decoder


def bar(y, value, color, label):
    w = full * value / FAST
    ax.add_patch(FancyBboxPatch((left, y - 22), w, 44, boxstyle="round,pad=0,rounding_size=6",
                                facecolor=color, edgecolor="none"))
    ax.text(left + w + 16, y, f"{value:g} GB/s", color=color, ha="left", va="center",
            fontsize=26, fontweight="heavy", **sans)
    ax.text(left, y + 42, label, color=MUTED, ha="left", va="center", fontsize=17,
            fontweight="demibold", **sans)


bar(270, FAST, TEAL, "C++ with simdutf: line endings skipped in the vector loop")
bar(150, SLOW, RED, "PHP 8.5 base64_decode: slow path at every line ending")

ax.text(1140, 60, "30× apart", color=INK, ha="right", va="center", fontsize=34,
        fontweight="heavy", **sans)
ax.text(60, 60, "Decoding MIME base64, one core", color=MUTED, ha="left", va="center",
        fontsize=17, fontweight="medium", **sans)

fig.savefig("feature_speed.png", dpi=100, facecolor=BG)
