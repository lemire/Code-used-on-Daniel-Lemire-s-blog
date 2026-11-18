"""Bar chart of MIME base64 decoding speed (mimebench/ranking_big4.md).

Usage: python3 speed.py  (writes speed.png and speed.webp)
"""
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch, Patch

SIMDUTF, OTHER = "#2a78d6", "#eb6834"
INK, MUTED, GRID, SURFACE = "#0b0b0b", "#52514e", "#e4e3df", "#fcfcfb"

# (label, GB/s, uses simdutf), fastest first.
DATA = [
    ("C++ (simdutf)", 18.4, True),
    ("PHP + my PR", 17.1, True),
    ("Python + fastpybase64", 16.4, True),
    ("Bun", 14.2, True),
    ("Node.js", 13.3, True),
    ("Deno", 8.4, True),
    (".NET", 4.9, False),
    ("Go", 1.6, False),
    ("Java", 1.2, False),
    ("Zig", 0.85, False),
    ("PHP", 0.61, False),
]

plt.rcParams.update({"font.size": 20, "font.family": "DejaVu Sans"})
fig, ax = plt.subplots(figsize=(13, 10), dpi=150)
fig.patch.set_facecolor(SURFACE)
ax.set_facecolor(SURFACE)

height = 0.66
xmax = 21.5
for i, (label, speed, fast) in enumerate(DATA):
    y = len(DATA) - 1 - i
    # Square at the baseline, rounded at the data end.
    r = 0.12
    ax.add_patch(FancyBboxPatch((0, y - height / 2), speed, height,
                                boxstyle=f"round,pad=0,rounding_size={r}",
                                mutation_aspect=height / xmax * 13,
                                facecolor=SIMDUTF if fast else OTHER, linewidth=0))
    ax.add_patch(plt.Rectangle((0, y - height / 2), min(r * 2, speed), height,
                               facecolor=SIMDUTF if fast else OTHER, linewidth=0))
    ax.text(speed + 0.25, y, f"{speed:g}", va="center", ha="left",
            color=INK, fontsize=20, fontweight="bold")

ax.set_yticks(range(len(DATA)))
ax.set_yticklabels([d[0] for d in reversed(DATA)], fontsize=21, color=INK)
ax.set_ylim(-0.6, len(DATA) - 0.4)
ax.set_xlim(0, xmax)
ax.set_xlabel("Decoding speed (GB/s of input, higher is better)", fontsize=20,
              color=MUTED, labelpad=12)
ax.set_xticks(range(0, 21, 5))
ax.tick_params(axis="x", colors=MUTED, labelsize=18, length=0)
ax.tick_params(axis="y", length=0, pad=10)
ax.xaxis.grid(True, color=GRID, linewidth=1)
ax.set_axisbelow(True)
for side in ("top", "right", "left"):
    ax.spines[side].set_visible(False)
ax.spines["bottom"].set_color(GRID)

fig.text(0.015, 0.965, "Decoding MIME base64 (1 MB, CRLF every 76 characters)",
         fontsize=24, color=INK, va="top")
fig.text(0.015, 0.915, "Intel Xeon Gold 6548N, one core", fontsize=18, color=MUTED, va="top")
ax.legend(handles=[Patch(color=SIMDUTF, label="uses simdutf"),
                   Patch(color=OTHER, label="does not")],
          loc="lower right", fontsize=20, frameon=False)

fig.tight_layout(rect=(0, 0, 1, 0.88))
fig.savefig("speed.png", facecolor=SURFACE)
fig.savefig("speed.webp", facecolor=SURFACE)
