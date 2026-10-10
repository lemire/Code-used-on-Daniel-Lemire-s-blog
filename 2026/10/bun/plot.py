# Bar charts for the WebSocket server post.
# Numbers are the C-client ping-pong medians in
# jswebsocket_bench/results/results_c_big4.md, at the rounding used in post.md.
# Writes roundtrips, latency, ws-roundtrips and ws-latency as PNG and WebP.
import subprocess
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import font_manager

SURFACE = "#fff6ee"
INK = "#1c1917"
GRID = "#f0ddd0"

# One colour per server, top to bottom, same order as the post table.
NAMES = [
    "C relay",
    "Node.js 26 + uWS",
    "Deno 2.9.7",
    "Bun 1.4.2",
]
COLORS = ["#0e7490", "#15803d", "#1d4ed8", "#c2410c"]
TRIPS = [54000, 49000, 40000, 37000]
MEDIAN_US = [18.4, 20.4, 25.0, 26.8]
P99_US = [23.0, 24.8, 32.0, 46.4]

HERE = Path(__file__).resolve().parent


def font_family():
    have = {f.name for f in font_manager.fontManager.ttflist}
    for name in ("Avenir Next", "Helvetica Neue", "Helvetica"):
        if name in have:
            return name
    return "sans-serif"


def style():
    plt.rcParams.update({
        "font.family": font_family(),
        "font.size": 22,
        "figure.facecolor": SURFACE,
        "savefig.facecolor": SURFACE,
        "axes.facecolor": SURFACE,
        "text.color": INK,
    })


def bare(ax):
    ax.grid(axis="x", color=GRID, lw=1.2, zorder=0)
    ax.set_axisbelow(True)
    ax.tick_params(axis="x", labelsize=20, colors=INK, length=0)
    ax.tick_params(axis="y", length=0, pad=10)
    for spine in ax.spines.values():
        spine.set_visible(False)


def label_inside(ax, bars, texts):
    for bar, text in zip(bars, texts):
        width = bar.get_width()
        yc = bar.get_y() + bar.get_height() / 2
        ax.text(
            width - width * 0.03,
            yc,
            text,
            va="center",
            ha="right",
            color="white",
            fontsize=26,
            fontweight="bold",
            zorder=4,
        )


def title(fig, text):
    fig.text(
        0.06, 0.94, text,
        ha="left", va="top", fontsize=36, fontweight="bold", color=INK,
    )


def save(fig, stem):
    png = HERE / f"{stem}.png"
    webp = HERE / f"{stem}.webp"
    fig.savefig(png, dpi=160)
    plt.close(fig)
    subprocess.run(
        ["cwebp", "-quiet", "-q", "85", str(png), "-o", str(webp)],
        check=True,
    )


def trips_chart(names, colors, values, stem, fig_h, left):
    y = np.arange(len(names))
    fig, ax = plt.subplots(figsize=(12.6, fig_h))
    bars = ax.barh(y, values, height=0.68, color=colors, zorder=3)
    ax.set_yticks(y)
    ax.set_yticklabels(names, fontsize=28, fontweight="bold")
    ax.invert_yaxis()
    ax.set_xlim(0, 68000)
    ax.set_xticks([0, 20000, 40000, 60000])
    ax.set_xticklabels(["0", "20,000", "40,000", "60,000"])
    ax.set_xlabel("round trips per second", fontsize=24, color=INK, labelpad=12)
    bare(ax)
    for label, color in zip(ax.get_yticklabels(), colors):
        label.set_color(color)
    label_inside(ax, bars, [f"{v:,}" for v in values])
    title(fig, "Round trips per second")
    fig.subplots_adjust(left=left, right=0.97, top=0.82, bottom=0.14)
    save(fig, stem)


def latency_chart(names, medians, tails, stem, fig_h, left):
    y = np.arange(len(names))
    height = 0.36
    fig, ax = plt.subplots(figsize=(12.6, fig_h))
    # Smaller y is the upper bar after invert_yaxis, so the median sits on top.
    median = ax.barh(
        y - height / 2, medians, height=height, color="#2563eb",
        label="median", zorder=3,
    )
    tail = ax.barh(
        y + height / 2, tails, height=height, color="#e11d48",
        label="99th percentile", zorder=3,
    )
    ax.set_yticks(y)
    ax.set_yticklabels(names, fontsize=28, fontweight="bold", color=INK)
    ax.invert_yaxis()
    ax.set_xlim(0, 58)
    ax.set_xlabel("microseconds", fontsize=24, color=INK, labelpad=12)
    bare(ax)
    label_inside(ax, median, [f"{v:.1f}" for v in medians])
    label_inside(ax, tail, [f"{v:.1f}" for v in tails])
    legend = fig.legend(
        *ax.get_legend_handles_labels(),
        loc="upper left", bbox_to_anchor=(left, 0.90),
        frameon=False, fontsize=24, ncol=2,
        handlelength=1.4, handleheight=0.8, columnspacing=1.4,
    )
    for text in legend.get_texts():
        text.set_color(INK)
        text.set_fontweight("bold")
    title(fig, "Time per round trip")
    fig.subplots_adjust(left=left, right=0.97, top=0.78, bottom=0.12)
    save(fig, stem)


# Same colours as the first figure: Node green, Deno blue, Bun orange.
WS_NAMES = ["Node.js 26 + ws", "Bun 1.4.2 + ws", "Deno 2.9.7 + ws"]
WS_COLORS = ["#15803d", "#c2410c", "#1d4ed8"]
WS_TRIPS = [45000, 41000, 34000]
WS_MEDIAN = [21.7, 22.5, 29.6]
WS_P99 = [31.5, 36.9, 45.9]


def main():
    style()
    trips_chart(NAMES, COLORS, TRIPS, "roundtrips", 7.0, 0.30)
    latency_chart(NAMES, MEDIAN_US, P99_US, "latency", 8.0, 0.30)
    trips_chart(WS_NAMES, WS_COLORS, WS_TRIPS, "ws-roundtrips", 6.0, 0.34)
    latency_chart(WS_NAMES, WS_MEDIAN, WS_P99, "ws-latency", 7.0, 0.34)


if __name__ == "__main__":
    main()
