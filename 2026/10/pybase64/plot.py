"""Bar charts of base64 decoding speed in Python, from results.csv.

Usage: python plot.py   (writes plain.png, mime.png and validate.png, and
.webp copies when Pillow is available)
"""
import csv
from collections import defaultdict

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt

# Categorical slots 1-3 of a validated palette (distinct under color blindness).
COLORS = {"fastpybase64": "#2a78d6", "pybase64": "#eb6834", "base64": "#1baf7a"}
LABELS = {"fastpybase64": "fastpybase64", "pybase64": "pybase64", "base64": "base64 (standard library)"}
ORDER = ["base64", "pybase64", "fastpybase64"]
INK, MUTED, GRID, SURFACE = "#0b0b0b", "#5f5e5a", "#e6e5e0", "#fcfcfb"
SIZES = [1024, 65536, 1 << 20]
SIZE_LABELS = {64: "64 B", 1024: "1 KiB", 4096: "4 KiB", 65536: "64 KiB", 1 << 20: "1 MiB"}


def load():
    speed = defaultdict(dict)
    with open("results.csv") as f:
        for r in csv.DictReader(f):
            speed[(r["format"], r["validate"] == "True")][(int(r["size"]), r["library"])] = float(r["gbps"])
    return speed


def chart(data, title, subtitle, filename):
    plt.rcParams.update({"font.size": 18, "font.family": "sans-serif"})
    fig, ax = plt.subplots(figsize=(12, 7.2), dpi=150)
    fig.patch.set_facecolor(SURFACE)
    ax.set_facecolor(SURFACE)
    width = 0.27
    vmax = max(data[(s, lib)] for s in SIZES for lib in ORDER)
    for i, lib in enumerate(ORDER):
        xs = [k + (i - 1) * width for k in range(len(SIZES))]
        values = [data[(s, lib)] for s in SIZES]
        ax.bar(xs, values, width=width - 0.03, color=COLORS[lib], label=LABELS[lib], zorder=3)
        for x, v in zip(xs, values):
            ax.text(x, v + vmax * 0.012, f"{v:.1f}" if v >= 1 else f"{v:.2f}", ha="center",
                    va="bottom", fontsize=17, color=INK, fontweight="bold" if lib == "fastpybase64" else None)
    ax.set_xticks(range(len(SIZES)))
    ax.set_xticklabels([SIZE_LABELS[s] for s in SIZES], fontsize=20, color=INK)
    ax.set_ylabel("GB/s of decoded data", fontsize=19, color=INK)
    ax.set_ylim(0, vmax * 1.12)
    ax.tick_params(axis="y", labelsize=16, colors=MUTED, length=0)
    ax.tick_params(axis="x", length=0, pad=10)
    ax.yaxis.grid(True, color=GRID, linewidth=1)
    ax.set_axisbelow(True)
    for side in ax.spines.values():
        side.set_visible(False)
    ax.legend(loc="upper left", fontsize=17, frameon=False)
    fig.text(0.015, 0.965, title, fontsize=27, color=INK, fontweight="bold", va="top")
    fig.text(0.015, 0.895, subtitle, fontsize=17, color=MUTED, va="top")
    fig.tight_layout(rect=(0, 0, 1, 0.84))
    fig.savefig(filename, facecolor=SURFACE)
    plt.close(fig)
    try:
        from PIL import Image

        Image.open(filename).save(filename.replace(".png", ".webp"), quality=90)
    except ImportError:
        pass


def main():
    speed = load()
    chart(speed[("plain", False)], "Decoding base64 in Python",
          "b64decode(s), the default (validate=False)", "plain.png")
    chart(speed[("mime", False)], "Decoding base64 with line breaks",
          "b64decode(s) on MIME data: a newline every 76 characters", "mime.png")
    chart(speed[("plain", True)], "Decoding base64 with validation",
          "b64decode(s, validate=True)", "validate.png")


if __name__ == "__main__":
    main()
