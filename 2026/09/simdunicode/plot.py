# Bar charts for the SimdUnicode UTF-16 toWellFormed post.
# One figure per machine: validate-xeon, validate-m4, buffer-xeon, buffer-m4.
import subprocess
from decimal import Decimal, ROUND_HALF_UP
import matplotlib.pyplot as plt

INK, MUTED = "#0b0b0b", "#52514e"
SURFACE, GRID = "#fcfcfb", "#e4e3df"
BLUE, ORANGE, COPY = "#2a78d6", "#c45c26", "#3f3e3c"

FILES = ["Latin-Lipsum", "Chinese-Lipsum", "twitter.json", "Emoji-Lipsum"]
LABELS = ["Latin", "Chinese", "twitter.json", "Emoji"]

COLS = {
    "copy": 2, "index_copy": 4, "simd_buf": 5,
    "index_val": 12, "simd_val": 13,
}

def load(path):
    out = {}
    for line in open(path):
        if not line.startswith("| ") or line.startswith("| file") or line.startswith("|---"):
            continue
        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        if cells[1] != "0":
            continue
        out[cells[0]] = [float(c) for c in cells[2:]]
    return out

def get(table, name, col):
    return table[name][COLS[col] - 2]

def label_of(v):
    quantum = Decimal("0.1") if v < 10 else Decimal("1")
    text = Decimal(str(v)).quantize(quantum, rounding=ROUND_HALF_UP)
    return f"{text:.1f}" if v < 10 else f"{text:.0f}"

AVX512 = load("results/big4-avx512-final.md")
NEON = load("results/m4max-neon-final.md")

def style(ax):
    ax.set_facecolor(SURFACE)
    ax.grid(axis="x", color=GRID, lw=0.9)
    ax.set_axisbelow(True)
    ax.tick_params(axis="both", length=0)
    ax.tick_params(axis="x", colors=MUTED, labelsize=16)
    ax.tick_params(axis="y", colors=INK, labelsize=20)
    for spine in ax.spines.values():
        spine.set_visible(False)
    ax.axvline(0, color="#d0cec8", lw=1)

def grouped(ax, series, xlim):
    """series is (name, values, color), drawn top to bottom within each group."""
    n = len(series)
    height = 0.46
    gap = 0.72
    ybase = []
    y = 0.0
    for _ in LABELS:
        ybase.append(y)
        y += n * height + gap
    for i, (name, vals, color) in enumerate(series):
        ys = [b + i * height for b in ybase]
        ax.barh(ys, vals, height=height * 0.84, color=color, label=name)
        for yi, v in zip(ys, vals):
            text = label_of(v)
            if v >= 12:
                ax.text(v - xlim * 0.018, yi, text, va="center", ha="right",
                        color="white", fontsize=18, fontweight="medium")
            else:
                ax.text(v + xlim * 0.018, yi, text, va="center", ha="left",
                        color=INK, fontsize=18)
    centers = [b + (n - 1) * height / 2 for b in ybase]
    ax.set_yticks(centers)
    ax.set_yticklabels(LABELS)
    ax.set_xlim(0, xlim)
    ax.invert_yaxis()

def finish(fig, path, title, subtitle):
    fig.text(0.02, 0.985, title, ha="left", va="top",
             fontsize=26, fontweight="bold", color=INK)
    fig.text(0.02, 0.915, subtitle, ha="left", va="top", fontsize=17, color=MUTED)
    fig.savefig(path + ".png", dpi=160, facecolor=SURFACE)
    plt.close(fig)
    subprocess.run(
        ["cwebp", "-quiet", "-q", "85", path + ".png", "-o", path + ".webp"],
        check=False,
    )

def one(path, title, subtitle, table, series, xlim, figsize):
    fig, ax = plt.subplots(figsize=figsize)
    grouped(ax, series(table), xlim)
    style(ax)
    ax.set_xlabel("GB/s", color=MUTED, fontsize=18, labelpad=8)
    handles, names = ax.get_legend_handles_labels()
    fig.legend(handles, names, loc="upper left", frameon=False,
               bbox_to_anchor=(0.015, 0.855), ncol=len(names), fontsize=17,
               handlelength=1.5, handleheight=0.85, columnspacing=1.3,
               borderaxespad=0)
    fig.tight_layout(rect=(0.01, 0.01, 0.99, 0.78))
    finish(fig, path, title, subtitle)

def validate_series(table):
    return [
        ("IndexOfAnyInRange", [get(table, f, "index_val") for f in FILES], ORANGE),
        ("SimdUnicode", [get(table, f, "simd_val") for f in FILES], BLUE),
    ]

def buffer_series(table):
    return [
        ("plain copy", [get(table, f, "copy") for f in FILES], COPY),
        ("SimdUnicode", [get(table, f, "simd_buf") for f in FILES], BLUE),
        ("copy + IndexOfAnyInRange", [get(table, f, "index_copy") for f in FILES], ORANGE),
    ]

def main():
    plt.rcParams.update({
        "font.family": "Avenir Next",
        "font.size": 18,
        "figure.facecolor": SURFACE,
    })
    one("validate-xeon", "Intel Xeon Gold 6548N",
        "Is the UTF-16 string well formed? AVX-512.",
        AVX512, validate_series, 142, (10.2, 7.2))
    one("validate-m4", "Apple M4 Max",
        "Is the UTF-16 string well formed? NEON.",
        NEON, validate_series, 162, (10.2, 7.2))
    one("buffer-xeon", "Intel Xeon Gold 6548N",
        "Copy the string, replace lone surrogates. AVX-512.",
        AVX512, buffer_series, 58, (10.2, 8.2))
    one("buffer-m4", "Apple M4 Max",
        "Copy the string, replace lone surrogates. NEON.",
        NEON, buffer_series, 106, (10.2, 8.2))

if __name__ == "__main__":
    main()
