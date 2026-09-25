# Plots the integer-to-string results from results_m4max.txt as a bar chart.
import re
import matplotlib.pyplot as plt
from matplotlib.patches import PathPatch
from matplotlib.path import Path

text = open("results_m4max.txt").read()

def ns(section, op):
    block = text[text.index(section):]
    return float(re.search(re.escape(op) + r"\s+([\d.]+) ns", block).group(1))

# one colour per language, in the palette's fixed order
COLORS = {"C++": "#2a78d6", "Nim": "#eb6834", "Go": "#1baf7a",
          "JavaScript": "#eda100", "Rust": "#e87ba4", "Python": "#008300"}

# (language, tick label, measured value)
data = [
    ("C++", "C++\nstd::to_string", ns("C++", "to_string(i)")),
    ("Nim", "Nim\n$i", ns("Nim", "$i")),
    ("Go", "Go\nstrconv.Itoa", ns("Go", "strconv.Itoa(i)")),
    ("JavaScript", "Node.js\nString(i)", ns("Node", "String(i)")),
    ("Rust", "Rust\nitoa", ns("Rust", "itoa + to_owned()")),
    ("JavaScript", "Bun\nString(i)", ns("Bun", "String(i)")),
    ("Rust", "Rust\nto_string()", ns("Rust", "i.to_string()")),
    ("Python", "Python\nstr(i)", ns("Python", "str(i)")),
]
data.sort(key=lambda d: d[2])  # fastest on the left

SURFACE, INK, MUTED, GRID = "#fcfcfb", "#0b0b0b", "#52514e", "#e4e3df"
plt.rcParams.update({"font.family": "Avenir Next", "font.size": 17})
fig, ax = plt.subplots(figsize=(12, 7), dpi=200)
fig.patch.set_facecolor(SURFACE)
ax.set_facecolor(SURFACE)

w = 0.72
ymax = max(d[2] for d in data) * 1.12
rx = 0.09                                   # corner radius in x units
ry = rx * ymax / len(data) * 1.35           # same radius in y units (aspect)

def bar(x, v):
    l, r, k = x - w / 2, x + w / 2, 0.45    # k: bezier handle factor
    verts = [(l, 0), (l, v - ry), (l, v - ry * k), (l + rx * k, v), (l + rx, v),
             (r - rx, v), (r - rx * k, v), (r, v - ry * k), (r, v - ry), (r, 0), (l, 0)]
    codes = [Path.MOVETO, Path.LINETO, Path.CURVE4, Path.CURVE4, Path.CURVE4,
             Path.LINETO, Path.CURVE4, Path.CURVE4, Path.CURVE4, Path.LINETO, Path.CLOSEPOLY]
    return Path(verts, codes)

for x, (lang, _, v) in enumerate(data):
    ax.add_patch(PathPatch(bar(x, v), color=COLORS[lang], lw=0))
    ax.text(x, v + ymax * 0.015, f"{v:.1f}", ha="center", va="bottom",
            color=INK, fontsize=18, fontweight="bold")

ax.set_xticks(range(len(data)), [d[1] for d in data], color=INK, fontsize=16)
ax.set_xlim(-0.6, len(data) - 0.4)
ax.set_ylim(0, ymax)
ax.tick_params(axis="y", colors=MUTED, labelsize=15)
ax.tick_params(axis="x", length=0, pad=10)
ax.grid(axis="y", color=GRID, lw=1)
ax.set_axisbelow(True)
for s in ax.spines.values():
    s.set_visible(False)
ax.axhline(0, color=MUTED, lw=1)
fig.tight_layout(rect=(0, 0, 1, 0.86))
fig.text(0.012, 0.955, "Converting an integer to a new string", ha="left", va="top",
         fontsize=26, fontweight="bold", color=INK)
fig.text(0.012, 0.895, "Apple M4 Max, nanoseconds per string, lower is better",
         ha="left", va="top", fontsize=18, color=MUTED)
fig.savefig("strings.png", facecolor=SURFACE)
