"""Feature image, 1200x630: the input, and the store that overflows."""
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle, FancyBboxPatch

BG = "#16130f"
INK = "#f5efe6"
MUTED = "#a8a29e"
DIM = "#3a342e"
TEAL = "#2dd4bf"
TEAL_DARK = "#0f766e"
RED = "#f43f5e"
AMBER = "#f59e0b"

fig = plt.figure(figsize=(12, 6.3), dpi=100)
fig.patch.set_facecolor(BG)
ax = fig.add_axes([0, 0, 1, 1])
ax.set_xlim(0, 120); ax.set_ylim(0, 63); ax.axis("off"); ax.set_facecolor(BG)

# The input string: 128 A's and one '=' (four rows of 32, '=' after the last A).
mono = {"fontname": "Menlo", "fontsize": 18, "fontweight": "bold"}
row = "A" * 32
for i, yy in enumerate((55, 50, 45, 40)):
    ax.text(6, yy, row, color=MUTED, ha="left", va="center", **mono)
ax.text(6, 40, " " * 32 + "=", color=RED, ha="left", va="center", **mono)

# Byte strip: 160 bytes. The array is the first 98. The tail store starts at 96.
x0, w, y, h = 6, 108 / 160, 16, 8
for b in range(160):
    if b < 96:
        face = TEAL_DARK
    elif b < 98:
        face = AMBER
    else:
        face = RED
    ax.add_patch(Rectangle((x0 + b * w, y), w, h, facecolor=face, edgecolor=BG, linewidth=0.6))
ax.add_patch(Rectangle((x0, y - 1.1), 98 * w, h + 2.2, fill=False, edgecolor=TEAL, linewidth=2.5))
xe = x0 + 98 * w
ax.plot([xe, xe], [y - 3.6, y + h + 3.6], color=INK, lw=2, ls=(0, (3, 2.5)))

sans = {"fontname": "Avenir Next"}
ax.text(x0 + 49 * w, y - 4.6, "98-byte array", color=TEAL, ha="center", va="center", fontsize=18, fontweight="heavy", **sans)
ax.text(x0 + 129 * w, y - 4.6, "62 bytes past the end", color=RED, ha="center", va="center", fontsize=18, fontweight="heavy", **sans)
ax.text(x0 + 129 * w, y + h / 2, "64-byte masked store", color=INK, ha="center", va="center", fontsize=15, fontweight="demibold", **sans)

# Small caption, bottom-right.
ax.text(114, 7, "Java4Shell · CVE-2026-114938", color=MUTED, ha="right", va="center", fontsize=15,
        fontweight="medium", **sans)

fig.savefig("feature.png", dpi=100, facecolor=BG)
