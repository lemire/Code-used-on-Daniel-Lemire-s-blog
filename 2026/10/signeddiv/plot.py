# Latency charts for the signed 32-bit division post.
# Numbers are the latency columns of the two tables in post.md.
# Writes intel-latency and runtime-latency as PNG and WebP.
import subprocess
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import font_manager

SURFACE = "#fff7ef"
INK = "#1c2430"
GRID = "#f0d9c8"

HERE = Path(__file__).resolve().parent

# Saturated colors that still hold white labels.
BLUE = "#1d4ed8"
TEAL = "#0f9b8e"
ROSE = "#e11d48"
ORANGE = "#ea580c"


def font_family():
    have = {f.name for f in font_manager.fontManager.ttflist}
    for name in ("Avenir Next", "Helvetica Neue", "Helvetica"):
        if name in have:
            return name
    return "sans-serif"


def style():
    plt.rcParams.update({
        "font.family": font_family(),
        "font.size": 28,
        "figure.facecolor": SURFACE,
        "savefig.facecolor": SURFACE,
        "axes.facecolor": SURFACE,
        "text.color": INK,
    })


def grouped(names, series, xlabel, xlim, stem, heading, left):
    # series: (legend, color, values), drawn top to bottom.
    n = len(names)
    k = len(series)
    bar_h = 0.80
    inner = 0.10
    gap = 0.58
    pitch = k * bar_h + (k - 1) * inner + gap
    y0 = np.arange(n) * pitch
    offsets = np.arange(k) * (bar_h + inner)

    top, bottom = 0.78, 0.09
    data_range = n * pitch - gap + 0.25
    # Bar thickness in points, so the value labels stay large on the page.
    target_bar_pt = 58
    fig_h = target_bar_pt * data_range / (bar_h * (top - bottom) * 72)
    fig_w = 12.4
    fig, ax = plt.subplots(figsize=(fig_w, fig_h))

    for (legend, color, values), off in zip(series, offsets):
        bars = ax.barh(
            y0 + off, values, height=bar_h, color=color,
            label=legend, zorder=3,
        )
        for bar, value in zip(bars, values):
            yc = bar.get_y() + bar.get_height() / 2
            ax.text(
                value - xlim * 0.02, yc, f"{value:.1f}",
                va="center", ha="right", color="white",
                fontsize=32, fontweight="bold", zorder=4,
            )

    ax.set_yticks(y0 + offsets.mean())
    ax.set_yticklabels(names, fontsize=34, fontweight="bold", color=INK)
    ax.invert_yaxis()
    ax.set_xlim(0, xlim)
    ax.set_ylim(data_range - 0.12, -0.28)
    ax.set_xlabel(xlabel, fontsize=32, color=INK, labelpad=16)
    ax.grid(axis="x", color=GRID, lw=1.4, zorder=0)
    ax.set_axisbelow(True)
    ax.tick_params(axis="x", labelsize=26, colors=INK, length=0, pad=8)
    ax.tick_params(axis="y", length=0, pad=12)
    for spine in ax.spines.values():
        spine.set_visible(False)

    legend = fig.legend(
        *ax.get_legend_handles_labels(),
        loc="upper left", bbox_to_anchor=(0.055, 0.905),
        frameon=False, fontsize=30, ncol=2,
        handlelength=1.35, handleheight=0.85, columnspacing=1.6,
        borderaxespad=0.0,
    )
    for text, (_, color, _) in zip(legend.get_texts(), series):
        text.set_color(color)
        text.set_fontweight("bold")

    fig.text(
        0.055, 0.975, heading,
        ha="left", va="top", fontsize=46, fontweight="bold", color=INK,
    )
    fig.subplots_adjust(left=left, right=0.985, top=top, bottom=bottom)
    png = HERE / f"{stem}.png"
    webp = HERE / f"{stem}.webp"
    fig.savefig(png, dpi=140)
    plt.close(fig)
    subprocess.run(
        ["cwebp", "-quiet", "-q", "85", str(png), "-o", str(webp)],
        check=True,
    )


def main():
    style()
    grouped(
        ["7", "60", "365", "1000"],
        [
            ("GCC, x / d", BLUE, [10.0, 10.0, 10.0, 8.0]),
            ("GCC, div32", TEAL, [8.0, 8.0, 8.0, 8.0]),
            ("clang, x / d", ROSE, [10.4, 10.4, 10.6, 8.0]),
            ("clang, div32", ORANGE, [8.0, 8.0, 8.0, 8.0]),
        ],
        "cycles",
        13.2,
        "intel-latency",
        "Latency on Intel",
        0.14,
    )
    grouped(
        ["Intel, GCC 14", "Intel, clang 21", "Apple M4"],
        [
            ("division instruction", BLUE, [4.0, 4.0, 2.2]),
            ("fastmod 0.1.0", ROSE, [4.2, 3.4, 2.0]),
            ("fastmod 0.2.0", TEAL, [3.2, 2.9, 2.0]),
        ],
        "nanoseconds",
        5.6,
        "runtime-latency",
        "Latency, divisor known at runtime",
        0.30,
    )


if __name__ == "__main__":
    main()
