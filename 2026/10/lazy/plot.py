# Grouped bars for the lazy-import startup post.
# Writes startup.png and startup.webp beside this file.
import subprocess
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

SURFACE = "#fff7ef"
INK = "#1c2430"
EAGER = "#f04438"
LAZY = "#0ea5a0"
GRID = "#f0d9c8"

COMMANDS = ["--version", "mean", "stats"]
EAGER_MS = [295, 296, 299]
LAZY_MS = [20, 24, 224]

HERE = Path(__file__).resolve().parent


def main():
    plt.rcParams.update({
        "font.family": "Avenir Next",
        "font.size": 22,
        "figure.facecolor": SURFACE,
        "savefig.facecolor": SURFACE,
        "axes.facecolor": SURFACE,
    })

    y = np.arange(len(COMMANDS))
    height = 0.36
    fig, ax = plt.subplots(figsize=(11.2, 6.4))

    # Smaller y is the upper bar after invert_yaxis, so eager sits on top.
    eager_bars = ax.barh(
        y - height / 2, EAGER_MS, height=height, color=EAGER,
        label="eager imports", zorder=3,
    )
    lazy_bars = ax.barh(
        y + height / 2, LAZY_MS, height=height, color=LAZY,
        label="lazy imports", zorder=3,
    )

    ax.set_yticks(y)
    ax.set_yticklabels(COMMANDS, fontsize=28, fontweight="bold", color=INK)
    ax.invert_yaxis()
    ax.set_xlim(0, 360)
    ax.set_xlabel("milliseconds", fontsize=24, color=INK, labelpad=12)
    ax.tick_params(axis="x", labelsize=20, colors=INK, length=0)
    ax.tick_params(axis="y", length=0)
    ax.grid(axis="x", color=GRID, lw=1.1, zorder=0)
    ax.set_axisbelow(True)
    for spine in ax.spines.values():
        spine.set_visible(False)

    def annotate(bars, color):
        for bar in bars:
            value = bar.get_width()
            yc = bar.get_y() + bar.get_height() / 2
            text = f"{int(value)}"
            if value >= 80:
                ax.text(
                    value - 12, yc, text, va="center", ha="right",
                    color="white", fontsize=24, fontweight="bold",
                )
            else:
                ax.text(
                    value + 10, yc, text, va="center", ha="left",
                    color=color, fontsize=24, fontweight="bold",
                )

    annotate(eager_bars, EAGER)
    annotate(lazy_bars, LAZY)

    handles, labels = ax.get_legend_handles_labels()
    legend = fig.legend(
        handles, labels, loc="upper left", bbox_to_anchor=(0.055, 0.86),
        frameon=False, fontsize=26, ncol=2,
        handlelength=1.5, handleheight=0.85, columnspacing=1.6,
    )
    for text in legend.get_texts():
        text.set_color(INK)
        text.set_fontweight("bold")

    fig.text(
        0.055, 0.95, "Time to run the tool",
        ha="left", va="top", fontsize=36, fontweight="bold", color=INK,
    )
    fig.subplots_adjust(left=0.18, right=0.97, top=0.74, bottom=0.14)

    png = HERE / "startup.png"
    webp = HERE / "startup.webp"
    fig.savefig(png, dpi=160)
    plt.close(fig)
    subprocess.run(
        ["cwebp", "-quiet", "-q", "85", str(png), "-o", str(webp)],
        check=True,
    )


if __name__ == "__main__":
    main()
