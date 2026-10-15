"""Bar charts of server-side time per command: string bitmaps vs Redis Roaring.

Usage: python plot.py results_big4.json
"""
import json
import sys

import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter, MaxNLocator

results = json.load(open(sys.argv[1]))

# The hashed dataset is 50 to 100 times slower than the others with strings, so it
# gets its own charts; on a shared linear axis the other bars would vanish.
DATASETS = [("dau_0.1pct", "0.1% random"), ("dense_50pct", "50% random"),
            ("runs", "runs")]
HASHED = [("hashed_2e32", "hashed 32-bit ids")]
SERIES = [("string", "bitset", "#2a78d6"), ("Roaring", "roaring", "#eb6834")]
COMMANDS = [("BITCOUNT", "bitcount"), ("BITOP AND", "bitop_and"),
            ("BITOP OR", "bitop_or")]

plt.rcParams.update({"font.size": 18, "axes.titlesize": 24, "axes.labelsize": 20,
                     "xtick.labelsize": 18, "ytick.labelsize": 16,
                     "legend.fontsize": 18, "font.family": "sans-serif"})


def fmt(us):
    if us >= 1000:
        ms = us / 1000
        return f"{ms:,.0f} ms" if ms >= 10 else f"{ms:.1f} ms"
    return f"{us:.0f} µs" if us >= 10 else f"{us:.1f} µs"


def plot(op, datasets, title, fname):
    fig, ax = plt.subplots(figsize=(4 + 3.3 * len(datasets), 8))
    width = 0.38
    times = []
    for i, (label, mode, colour) in enumerate(SERIES):
        for j, (ds, _) in enumerate(datasets):
            t = results[ds]["results"][mode][op]
            times.append(t)
            x = j + (i - 0.5) * width
            ax.bar(x, t, width=width - 0.04, color=colour,
                   label=label if j == 0 else None)
            ax.annotate(fmt(t), (x, t), xytext=(0, 5), textcoords="offset points",
                        ha="center", va="bottom", fontsize=15, color="#0b0b0b")
    hi = max(times)
    ax.set_ylim(0, hi * 1.08)
    ax.yaxis.set_major_locator(MaxNLocator(nbins=6))
    if hi >= 1000:
        ax.yaxis.set_major_formatter(FuncFormatter(lambda t, _: f"{t / 1000:,.0f} ms"))
    else:
        ax.yaxis.set_major_formatter(FuncFormatter(lambda t, _: f"{t:,.0f} µs"))
    ax.set_xticks(range(len(datasets)), [d for _, d in datasets])
    ax.set_xlim(-0.6, len(datasets) - 0.4)
    ax.set_ylabel("time per command (lower is better)")
    ax.set_title(title, pad=50)
    ax.grid(axis="y", color="#e4e3df", linewidth=1)
    ax.set_axisbelow(True)
    for side in ("top", "right"):
        ax.spines[side].set_visible(False)
    ax.legend(loc="lower center", bbox_to_anchor=(0.5, 1.0), frameon=False,
              ncol=2, handlelength=1.2, columnspacing=1.5)
    fig.tight_layout()
    fig.savefig(f"{fname}.png", dpi=150)
    plt.close(fig)
    print(f"{fname}.png")


for op, fname in COMMANDS:
    plot(op, DATASETS, op, fname)
    plot(op, HASHED, f"{op}, hashed ids", f"{fname}_hashed")
