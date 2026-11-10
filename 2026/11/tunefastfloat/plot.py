# Bar charts for post.md: millions of numbers parsed per second, for four
# kinds of data and four choices of performance hints. One chart per machine.
# The values come from the results files.
import subprocess
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import font_manager

SURFACE = "#fff7ef"
INK = "#1c2430"
GRID = "#f0d9c8"
# Validated with the dataviz skill's validate_palette.js (light, on SURFACE).
ORANGE = "#ea580c"
BLUE = "#2563eb"
GREEN = "#16a34a"
PURPLE = "#9333ea"

HERE = Path(__file__).resolve().parent

DATASETS = [
    ("price_changes", "price changes\n-731.51"),
    ("account_balances", "account balances\n-731271.51"),
    ("sensor_readings", "sensor readings\n42.5, 0.375, 7"),
    ("regular_prices", "regular prices\n12.34"),
]
SERIES = [
    ("no hint", "no hint", ORANGE),
    ("sign + nearest", "unpredictable_sign + assume_round_to_nearest", BLUE),
    ("short + nearest", "short_numbers + assume_round_to_nearest", GREEN),
    ("all three", "all three hints", PURPLE),
]


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


def load(name):
    """{(data set, hints): millions of numbers per second}"""
    out = {}
    for line in (HERE / name).read_text().splitlines():
        if line.startswith("#") or not line.strip():
            continue
        dataset = line[:19].strip()
        hints = line[19:44].strip()
        rate = float(line[44:].split("ns")[1].split("M/s")[0])
        out[(dataset, hints)] = rate
    return out


def chart(results, stem, xlim):
    names = [label for _, label in DATASETS]
    n = len(names)
    k = len(SERIES)
    bar_h = 0.80
    inner = 0.10
    gap = 0.70
    pitch = k * bar_h + (k - 1) * inner + gap
    y0 = np.arange(n) * pitch
    offsets = np.arange(k) * (bar_h + inner)

    top, bottom = 0.84, 0.08
    data_range = n * pitch - gap + 0.25
    target_bar_pt = 46
    fig_h = target_bar_pt * data_range / (bar_h * (top - bottom) * 72)
    fig, ax = plt.subplots(figsize=(15.0, fig_h))

    for (key, legend, color), off in zip(SERIES, offsets):
        values = [results[(dataset, key)] for dataset, _ in DATASETS]
        bars = ax.barh(y0 + off, values, height=bar_h, color=color,
                       label=legend, zorder=3)
        for bar, value in zip(bars, values):
            yc = bar.get_y() + bar.get_height() / 2
            ax.text(value - xlim * 0.015, yc, f"{value:.0f}",
                    va="center", ha="right", color="white",
                    fontsize=27, fontweight="bold", zorder=4)

    ax.set_yticks(y0 + offsets.mean())
    ax.set_yticklabels(names, fontsize=30, fontweight="bold", color=INK,
                       linespacing=1.3)
    ax.invert_yaxis()
    ax.set_xlim(0, xlim)
    ax.set_ylim(data_range - 0.12, -0.28)
    ax.set_xlabel("millions of numbers per second (higher is better)",
                  fontsize=30, color=INK, labelpad=16)
    ax.grid(axis="x", color=GRID, lw=1.4, zorder=0)
    ax.set_axisbelow(True)
    ax.tick_params(axis="x", labelsize=26, colors=INK, length=0, pad=8)
    ax.tick_params(axis="y", length=0, pad=14)
    for spine in ax.spines.values():
        spine.set_visible(False)

    legend = fig.legend(
        *ax.get_legend_handles_labels(),
        loc="upper left", bbox_to_anchor=(0.02, 0.985),
        frameon=False, fontsize=26, ncol=1,
        handlelength=1.35, handleheight=0.85, labelspacing=0.35,
        borderaxespad=0.0,
    )
    for text in legend.get_texts():
        text.set_color(INK)
        text.set_fontweight("bold")

    fig.subplots_adjust(left=0.30, right=0.985, top=top, bottom=bottom)
    png = HERE / f"{stem}.png"
    webp = HERE / f"{stem}.webp"
    fig.savefig(png, dpi=160)
    plt.close(fig)
    subprocess.run(["cwebp", "-quiet", "-q", "85", str(png), "-o", str(webp)],
                   check=True)


def main():
    style()
    chart(load("results_big3_icelake_gcc15.txt"), "icelake", 170)
    chart(load("results_mac_m4max_appleclang17.txt"), "m4", 380)


if __name__ == "__main__":
    main()
