# Figures for the blog post on compile-time perfect hashing.
# Numbers: ConstexprCore/perfect_hash at commit 862ec56, Apple M3 Max,
# AppleClang 15, -O3, `sudo ./build/benchmarks/bench_protocol --filter hits`.
import matplotlib.pyplot as plt

OURS = "#2a78d6"
OTHER = "#b9b8b2"
INK = "#0b0b0b"
INK2 = "#52514e"
SURFACE = "#fcfcfb"

plt.rcParams.update({
    "font.family": "sans-serif",
    "font.size": 12,
    "axes.edgecolor": INK2,
    "axes.labelcolor": INK2,
    "xtick.color": INK2,
    "ytick.color": INK,
    "figure.facecolor": SURFACE,
    "axes.facecolor": SURFACE,
})

def clean(ax):
    for s in ("top", "right", "left"):
        ax.spines[s].set_visible(False)
    ax.tick_params(axis="y", length=0)
    ax.xaxis.grid(True, color="#e6e5e0", linewidth=0.8)
    ax.set_axisbelow(True)

# Figure 1: HTTP headers (20 keys), all methods.
methods = [
    ("ours (perfect_hash)", 1.266), ("kronuz::phf", 7.825), ("gperf", 7.704),
    ("pthash", 12.234), ("absl::flat_hash_map", 10.854),
    ("naive if-chain", 12.657), ("frozen::unordered_map", 14.970),
    ("std::unordered_map", 16.592), ("ankerl::unordered_dense", 18.574),
]
methods.sort(key=lambda m: m[1], reverse=True)
fig, ax = plt.subplots(figsize=(8, 4.6))
names = [m[0] for m in methods]
vals = [m[1] for m in methods]
colors = [OURS if n.startswith("ours") else OTHER for n in names]
bars = ax.barh(names, vals, color=colors, height=0.72)
for b, v in zip(bars, vals):
    ax.text(v + 0.25, b.get_y() + b.get_height() / 2, f"{v:.1f} ns",
            va="center", fontsize=11, color=INK)
ax.set_xlabel("nanoseconds per lookup (lower is better)")
ax.set_title("HTTP header names (20 keys), Apple M3 Max", loc="left",
             fontsize=13, color=INK)
ax.set_xlim(0, 21)
clean(ax)
fig.tight_layout()
fig.savefig("http_headers.png", dpi=200)

# Figure 2: every key set, ours vs gperf vs std::unordered_map.
sets = ["URL protocols (6)", "C++ keywords (15)", "HTTP headers (20)",
        "MIME types (15)", "JS reserved words (45)", "HTTP headers (50)",
        "S&P 100 tickers (100)"]
ours = [1.187, 1.330, 1.266, 1.441, 1.528, 1.987, 1.769]
gperf = [5.312, 5.319, 7.704, 8.142, 25.580, 8.595, 5.612]
umap = [11.915, 11.631, 16.592, 11.657, 15.770, 15.359, 11.919]
series = [("std::unordered_map", umap, "#d9d8d2"),
          ("gperf", gperf, "#8c8b85"), ("ours", ours, OURS)]
fig, ax = plt.subplots(figsize=(8, 6))
h = 0.26
y = list(range(len(sets)))[::-1]
for i, (label, data, c) in enumerate(series):
    off = (i - 1) * h
    ax.barh([v + off for v in y], data, height=h - 0.03, color=c, label=label)
    if label == "ours":
        for yy, v in zip(y, data):
            ax.text(v + 0.3, yy + off, f"{v:.1f}", va="center",
                    fontsize=10, color=INK)
ax.set_yticks(y)
ax.set_yticklabels(sets)
ax.set_xlabel("nanoseconds per lookup (lower is better)")
ax.set_title("Seven key sets, Apple M3 Max", loc="left",
             fontsize=13, color=INK)
handles, labels = ax.get_legend_handles_labels()
ax.legend(handles[::-1], labels[::-1], frameon=False, loc="lower right")
clean(ax)
fig.tight_layout()
fig.savefig("all_keysets.png", dpi=200)

# Figure 3: what one lookup costs (separate counter capture on URL protocols).
metrics = [("cycles\nper lookup", 4.86, 48.42), ("instructions\nin the code", 63, 250),
           ("calls\nin the code", 0, 32), ("branch misses\nper lookup", 0.00, 1.01)]
fig, axes = plt.subplots(1, 4, figsize=(10, 3.2))
for ax, (label, a, b) in zip(axes, metrics):
    bars = ax.bar(["ours", "std::\nunordered_map"], [a, b],
                  color=[OURS, OTHER], width=0.6)
    for bar, v in zip(bars, [a, b]):
        txt = f"{v:g}" if v else "0"
        ax.text(bar.get_x() + bar.get_width() / 2, v + b * 0.02, txt,
                ha="center", va="bottom", fontsize=11, color=INK)
    ax.set_title(label, fontsize=12, color=INK)
    ax.set_ylim(0, b * 1.18)
    ax.set_yticks([])
    for s in ("top", "right", "left"):
        ax.spines[s].set_visible(False)
    ax.tick_params(axis="x", length=0, labelsize=10)
fig.suptitle("Cost of one lookup, URL protocols (6 keys)", x=0.01,
             ha="left", fontsize=13, color=INK)
fig.tight_layout()
fig.savefig("lookup_cost.png", dpi=200)
