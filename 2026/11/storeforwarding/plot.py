# Cycles for a dependent store followed by a load.
# The latency chart draws the store and the load on one 8-byte window.
# Every number is the cycle counter, rounded to the nearest cycle.
import subprocess
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import font_manager
from matplotlib.patches import FancyBboxPatch

SURFACE = "#fff7ef"
INK = "#1c2430"
GRID = "#f0d9c8"
BLUE = "#1d4ed8"
INDIGO = "#4338ca"
ORANGE = "#c2410c"
GREEN = "#047857"
ROSE = "#e11d48"
TEAL = "#0f766e"
TRACK = "#e7d2c0"

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
        "font.size": 28,
        "figure.facecolor": SURFACE,
        "savefig.facecolor": SURFACE,
        "axes.facecolor": SURFACE,
        "text.color": INK,
    })


def label(value):
    if abs(value - round(value)) < 0.05:
        return f"{value:.0f}"
    return f"{value:.1f}"


def grouped(names, series, xlabel, xlim, stem, heading, left):
    n = len(names)
    k = len(series)
    bar_h = 0.80
    inner = 0.12
    gap = 0.62
    pitch = k * bar_h + (k - 1) * inner + gap
    y0 = np.arange(n) * pitch
    offsets = np.arange(k) * (bar_h + inner)

    top, bottom = 0.78, 0.10
    data_range = n * pitch - gap + 0.25
    target_bar_pt = 52
    fig_h = target_bar_pt * data_range / (bar_h * (top - bottom) * 72)
    fig, ax = plt.subplots(figsize=(13.2, fig_h))

    for (legend, color, values), off in zip(series, offsets):
        bars = ax.barh(
            y0 + off, values, height=bar_h, color=color,
            label=legend, zorder=3,
        )
        for bar, value in zip(bars, values):
            yc = bar.get_y() + bar.get_height() / 2
            text = label(value)
            if value < xlim * 0.16:
                ax.text(
                    value + xlim * 0.015, yc, text,
                    va="center", ha="left", color=INK,
                    fontsize=30, fontweight="bold", zorder=4,
                )
            else:
                ax.text(
                    value - xlim * 0.02, yc, text,
                    va="center", ha="right", color="white",
                    fontsize=30, fontweight="bold", zorder=4,
                )

    ax.set_yticks(y0 + offsets.mean())
    ax.set_yticklabels(names, fontsize=32, fontweight="bold", color=INK)
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
        loc="upper left", bbox_to_anchor=(0.055, 0.90),
        frameon=False, fontsize=30, ncol=2,
        handlelength=1.35, handleheight=0.85, columnspacing=1.6,
        borderaxespad=0.0,
    )
    for text, (_, color, _) in zip(legend.get_texts(), series):
        text.set_color(color)
        text.set_fontweight("bold")

    fig.text(
        0.055, 0.975, heading,
        ha="left", va="top", fontsize=42, fontweight="bold", color=INK,
    )
    fig.subplots_adjust(left=left, right=0.985, top=top, bottom=bottom)
    png = HERE / f"{stem}.png"
    webp = HERE / f"{stem}.webp"
    fig.savefig(png, dpi=160)
    plt.close(fig)
    subprocess.run(
        ["cwebp", "-quiet", "-q", "85", str(png), "-o", str(webp)],
        check=True,
    )


def axes_in(fig, box, fig_w, fig_h):
    x, y, w, h = box
    ax = fig.add_axes([x / fig_w, y / fig_h, w / fig_w, h / fig_h])
    ax.set_facecolor(SURFACE)
    return ax


def span_patch(ax, start, end, y, height, color):
    # Rounding is in data units. The axes box is sized so one x unit matches
    # one y unit, and the corners stay circular after the blog column shrinks.
    radius = min(0.28, height / 2.2, max(end - start, 0.01) / 2.2)
    ax.add_patch(
        FancyBboxPatch(
            (start, y),
            end - start,
            height,
            boxstyle=f"round,pad=0,rounding_size={radius}",
            facecolor=color,
            edgecolor="none",
            linewidth=0,
            zorder=3,
            clip_on=False,
        )
    )


def memory_row(fig, box, stores, load, fig_w, fig_h, store_track=True):
    """Two tracks of the same eight bytes: stores on top, the load below.

    Ranges are in bytes, origin at the left. A byte the access does not touch
    stays the track color. Two adjacent stores are separate pills on the page
    background, since together they cover the whole window.
    """
    ax = axes_in(fig, box, fig_w, fig_h)
    ax.set_xlim(-0.35, 8.35)
    ax.set_ylim(0, 8.7 * (box[3] / box[2]))
    ax.axis("off")

    span_x = 8.7
    span_y = span_x * (box[3] / box[2])
    pill_h = 0.50 / box[3] * span_y
    gap = 0.12 / box[3] * span_y
    pair = 2 * pill_h + gap
    y_load = (span_y - pair) / 2
    y_store = y_load + pill_h + gap

    if store_track:
        span_patch(ax, 0, 8, y_store, pill_h, TRACK)
    span_patch(ax, 0, 8, y_load, pill_h, TRACK)
    for start, end in stores:
        span_patch(ax, start, end, y_store, pill_h, INK)
    span_patch(ax, load[0], load[1], y_load, pill_h, TEAL)
    return y_store, y_load, pill_h, span_y


def latency():
    # Byte ranges on one 8-byte window. The split store is two writes of 4,
    # with a thin seam so it does not look like one 8-byte store.
    # store ranges, load range, draw the store track
    patterns = [
        ([(0, 8)], (0, 8), True),
        ([(0, 8)], (4, 8), True),
        ([(0, 4)], (0, 8), True),
        # A thin seam, not a hole. The two writes still cover all eight bytes.
        ([(0, 3.92), (4.08, 8)], (0, 8), True),
    ]
    # Nearest cycle. Same order as the legend, top bar first.
    series = [
        ("Xeon Gold 6548N", BLUE, [1, 5, 19, 20]),
        ("Xeon 6975P-C", INDIGO, [1, 5, 19, 20]),
        ("EPYC 9R45", ORANGE, [1, 7, 14, 15]),
        ("Graviton 5", GREEN, [5, 6, 5, 5]),
        ("Apple M4 Max", ROSE, [5, 5, 5, 8]),
    ]
    n = len(patterns)
    nseries = len(series)
    fig_w = 16.8
    group_h = 2.20
    group_gap = 0.34
    rows_h = n * group_h + (n - 1) * group_gap
    top = 1.02
    bottom = 1.22
    fig_h = top + rows_h + bottom

    diag_w = 4.15
    diag_h = 1.36
    label_w = 1.40
    left = 0.18
    gutter = 0.42
    right = 0.28
    diag_x = left + label_w
    bar_x = diag_x + diag_w + gutter
    bar_w = fig_w - bar_x - right

    fig = plt.figure(figsize=(fig_w, fig_h))
    rows_bottom = bottom
    rows_top = bottom + rows_h

    pitch = group_h + group_gap
    pad = (group_h / 2) / pitch
    ax = axes_in(fig, (bar_x, rows_bottom, bar_w, rows_h), fig_w, fig_h)
    ax.set_ylim(-pad, (n - 1) + pad)
    bar_h = 0.34 / pitch
    gap = 0.055 / pitch
    step = bar_h + gap
    centers = []
    for i, (stores, load, store_track) in enumerate(patterns):
        center = n - 1 - i
        centers.append(center)
        box_y = rows_top - (i + 1) * group_h - i * group_gap
        box_y += (group_h - diag_h) / 2
        y_store, y_load, pill_h, span_y = memory_row(
            fig, (diag_x, box_y, diag_w, diag_h),
            stores, load, fig_w, fig_h, store_track,
        )

        def fig_y(y_data, box_y=box_y):
            return box_y + (y_data + pill_h / 2) / span_y * diag_h

        fig.text(
            (diag_x - 0.14) / fig_w, fig_y(y_store) / fig_h,
            "store", ha="right", va="center",
            fontsize=26, fontweight="bold", color=INK,
        )
        fig.text(
            (diag_x - 0.16) / fig_w, fig_y(y_load) / fig_h,
            "load", ha="right", va="center",
            fontsize=26, fontweight="bold", color=TEAL,
        )

    xlim = 26
    for i, (legend, color, values) in enumerate(series):
        center_offset = ((nseries - 1) / 2 - i) * step
        dy = center_offset - bar_h / 2
        ys = [c + dy + bar_h / 2 for c in centers]
        bars = ax.barh(ys, values, height=bar_h, color=color, zorder=3, align="center")
        for bar, value in zip(bars, values):
            yc = bar.get_y() + bar.get_height() / 2
            text = label(value)
            if value < 3:
                ax.text(
                    value + xlim * 0.012, yc, text,
                    va="center", ha="left", color=INK,
                    fontsize=20, fontweight="bold", zorder=4,
                )
            else:
                ax.text(
                    value - xlim * 0.015, yc, text,
                    va="center", ha="right", color="white",
                    fontsize=20, fontweight="bold", zorder=4,
                )
    swatch_y = fig_h - 0.78
    legend_x = [0.18, 3.55, 6.55, 9.15, 12.15]
    for (legend, color, _), sx in zip(series, legend_x):
        sw = axes_in(fig, (sx, swatch_y, 0.38, 0.28), fig_w, fig_h)
        sw.set_xlim(0, 1)
        sw.set_ylim(0, 1)
        sw.axis("off")
        sw.add_patch(
            FancyBboxPatch(
                (0, 0), 1, 1,
                boxstyle="round,pad=0,rounding_size=0.18",
                facecolor=color, edgecolor="none",
            )
        )
        fig.text(
            (sx + 0.50) / fig_w, (swatch_y + 0.14) / fig_h,
            legend, ha="left", va="center",
            fontsize=20, fontweight="bold", color=color,
        )

    ax.set_xlim(0, xlim)
    ax.set_yticks([])
    ax.set_xlabel("cycles per dependent reload", fontsize=32, color=INK, labelpad=14)
    ax.grid(axis="x", color=GRID, lw=1.4, zorder=0)
    ax.set_axisbelow(True)
    ax.tick_params(axis="x", labelsize=26, colors=INK, length=0, pad=8)
    ax.tick_params(axis="y", length=0)
    for spine in ax.spines.values():
        spine.set_visible(False)

    png = HERE / "latency.png"
    webp = HERE / "latency.webp"
    fig.savefig(png, dpi=160)
    plt.close(fig)
    subprocess.run(
        ["cwebp", "-quiet", "-q", "85", str(png), "-o", str(webp)],
        check=True,
    )


def main():
    style()
    latency()
    grouped(
        [
            "byte store, reload",
            "update a register",
            "two stores, reload",
            "join in registers",
        ],
        [
            ("Xeon Gold 6548N", BLUE, [16.0, 1.0, 17.0, 1.2]),
            ("Apple M4 Max", ROSE, [1.1, 1.1, 1.8, 1.2]),
        ],
        "cycles per element",
        22,
        "throughput",
        "Using the word you just wrote",
        0.36,
    )


if __name__ == "__main__":
    main()
