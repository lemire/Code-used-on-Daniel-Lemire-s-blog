# Bar charts for UTF-8 to UTF-16 with U+FFFD replacement.
import matplotlib.pyplot as plt
from matplotlib.patches import PathPatch
from matplotlib.path import Path

text = open("results_icelake.txt").read().splitlines()

def gbs(name, kind, op):
    prefix = f"{name} {kind} {op} "
    for line in text:
        if line.startswith(prefix):
            return float(line.split()[-1])
    raise SystemExit(f"missing {prefix}")

SURFACE, INK, MUTED, GRID = "#fcfcfb", "#0b0b0b", "#52514e", "#e4e3df"
BLUE, ORANGE, GREEN = "#2a78d6", "#eb6834", "#1baf7a"
plt.rcParams.update({"font.family": "Avenir Next", "font.size": 17})


def rounded(x, v, w, ymax):
    rx = min(0.045, w * 0.18)
    ry = ymax * 0.012
    l, r, k = x - w / 2, x + w / 2, 0.45
    verts = [(l, 0), (l, v - ry), (l, v - ry * k), (l + rx * k, v), (l + rx, v),
             (r - rx, v), (r - rx * k, v), (r, v - ry * k), (r, v - ry), (r, 0), (l, 0)]
    codes = [Path.MOVETO, Path.LINETO, Path.CURVE4, Path.CURVE4, Path.CURVE4,
             Path.LINETO, Path.CURVE4, Path.CURVE4, Path.CURVE4, Path.LINETO, Path.CLOSEPOLY]
    return Path(verts, codes)


def grouped(path, groups, series, title, subtitle):
    fig, ax = plt.subplots(figsize=(12, 7), dpi=200)
    fig.patch.set_facecolor(SURFACE)
    ax.set_facecolor(SURFACE)
    n = len(groups)
    k = len(series)
    w = 0.36
    gap = 0.06
    vals = [v for _, _, vs in series for v in vs]
    ymax = max(vals) * 1.18
    for s, (label, color, vs) in enumerate(series):
        shift = (s - (k - 1) / 2) * (w + gap)
        for i, v in enumerate(vs):
            x = i + shift
            ax.add_patch(PathPatch(rounded(x, v, w, ymax), color=color, lw=0, label=label if i == 0 else None))
            ax.text(x, v + ymax * 0.015, f"{v:.1f}", ha="center", va="bottom",
                    color=INK, fontsize=16, fontweight="bold")
    ax.set_xticks(range(n), groups, color=INK, fontsize=16)
    ax.set_xlim(-0.6, n - 0.4)
    ax.set_ylim(0, ymax)
    ax.tick_params(axis="y", colors=MUTED, labelsize=15)
    ax.tick_params(axis="x", length=0, pad=10)
    ax.grid(axis="y", color=GRID, lw=1)
    ax.set_axisbelow(True)
    for spine in ax.spines.values():
        spine.set_visible(False)
    ax.axhline(0, color=MUTED, lw=1)
    leg = ax.legend(frameon=False, ncol=2, loc="upper right", fontsize=16)
    for t in leg.get_texts():
        t.set_color(INK)
    fig.tight_layout(rect=(0, 0, 1, 0.86))
    fig.text(0.012, 0.955, title, ha="left", va="top", fontsize=24, fontweight="bold", color=INK)
    fig.text(0.012, 0.895, subtitle, ha="left", va="top", fontsize=17, color=MUTED)
    fig.savefig(path, facecolor=SURFACE)
    plt.close(fig)


english_kinds = ["no errors", "8 bad bytes", "every 4 KB", "every 64 B"]
kind_keys = ("valid", "eight", "per4k", "per64")
grouped(
    "english.png",
    english_kinds,
    [
        ("scalar", ORANGE, [gbs("english", k, "scalar") for k in kind_keys]),
        ("simdutf", BLUE, [gbs("english", k, "replacement") for k in kind_keys]),
    ],
    "UTF-8 to UTF-16 with replacement",
    "English Wikipedia, Xeon Gold 6548N, icelake, one core, GB/s, higher is better",
)
grouped(
    "japanese.png",
    english_kinds,
    [
        ("scalar", ORANGE, [gbs("japanese", k, "scalar") for k in kind_keys]),
        ("simdutf", BLUE, [gbs("japanese", k, "replacement") for k in kind_keys]),
    ],
    "UTF-8 to UTF-16 with replacement",
    "Japanese Wikipedia, Xeon Gold 6548N, icelake, one core, GB/s, higher is better",
)

langs = ["english", "chinese", "arabic", "hindi", "japanese", "russian"]
labels = ["English", "Chinese", "Arabic", "Hindi", "Japanese", "Russian"]
grouped(
    "valid.png",
    labels,
    [
        ("convert_utf8_to_utf16", BLUE, [gbs(n, "valid", "plain") for n in langs]),
        ("with replacement", GREEN, [gbs(n, "valid", "replacement") for n in langs]),
    ],
    "Valid UTF-8, no errors in the input",
    "Xeon Gold 6548N, icelake, one core, GB/s, higher is better",
)
