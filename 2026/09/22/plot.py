# One speedup-over-time figure per library. Each series is normalized to its
# first measurement; lines are steps because performance only changes at a
# commit. Reads the per-commit TSVs in this directory and history/, writes
# <name>.png and <name>.webp.
import csv
import subprocess
from collections import defaultdict
from datetime import date

import matplotlib.dates as mdates
import matplotlib.pyplot as plt

INK, MUTED, GRID = "#0b0b0b", "#52514e", "#dddcd8"
COLORS = ["#2a78d6", "#eb6834", "#1baf7a", "#eda100"]
plt.rcParams.update({"font.size": 13})
X0, X1 = date(2024, 8, 1), date(2026, 9, 30)


def d(s):
    return date.fromisoformat(s[:10])


def rows(path):
    with open(path) as f:
        return list(csv.DictReader(f, delimiter="\t"))


# ---- per-library loaders: each returns [(label, xs, ys)] with ys = speedup

def ada():
    r = rows("ada_history.tsv")
    base = float(r[0]["gbps"])
    return [("href", [d(x["date"]) for x in r], [float(x["gbps"]) / base for x in r])]


def fast_float():
    r = rows("fast_float_history.tsv")
    xs = [d(x["date"]) for x in r]
    out = []
    for label, col in (("canada.txt", "canada_mbs"), ("mesh.txt", "mesh_mbs")):
        base = float(r[0][col])
        out.append((label, xs, [float(x[col]) / base for x in r]))
    return out


def simdjson(kind):
    """kind is 'serialization' or 'parsing'. Bytes per cycle relative to the
    paper's commit af2a4361 (index 005). clang-p2996 builds commits up to index
    63 and GCC 16 from index 32 on; the two are chained at 63."""
    r = rows("simdjson_reflection_history.tsv")
    PAPER, SPLICE = "005", "063"
    out = []
    for label, bench in (("twitter.json", f"{kind}_twitter"), ("citm_catalog.json", f"{kind}_citm")):
        clang = {x["idx"]: x for x in r if x["tc"] == "clangpaper" and x["bench"] == bench}
        gcc = {x["idx"]: x for x in r if x["tc"] == "gcc16" and x["bench"] == bench}
        cb0 = float(clang[PAPER]["cb"])
        xs, ys = [], []
        for idx in sorted(clang):
            if idx >= PAPER:
                xs.append(d(clang[idx]["date"]))
                ys.append(cb0 / float(clang[idx]["cb"]))
        at_splice, gcb0 = ys[-1], float(gcc[SPLICE]["cb"])
        for idx in sorted(gcc):
            if idx > SPLICE:
                xs.append(d(gcc[idx]["date"]))
                ys.append(at_splice * gcb0 / float(gcc[idx]["cb"]))
        out.append((label, xs, ys))
    return out


def sweep(project, metrics, higher_is_better):
    """Series from history/<project>.tsv as written by history/sweep.py."""
    s = defaultdict(list)
    for x in rows(f"history/{project}.tsv"):
        s[x["metric"]].append((d(x["date"]), float(x["value"])))
    out = []
    for label, metric in metrics:
        pts = s[metric]
        base = pts[0][1]
        ys = [(v / base) if higher_is_better else (base / v) for _, v in pts]
        out.append((label, [p[0] for p in pts], ys))
    return out


# ---- drawing

def panel(ax, title, series, ymax):
    ends = []
    for k, (label, xs, ys) in enumerate(series):
        c = COLORS[k]
        ax.step(xs, ys, where="post", color=c, lw=2)
        ax.plot([xs[-1], X1], [ys[-1], ys[-1]], color=c, lw=2)
        ends.append([ys[-1], label, c])
    gap = (ymax - 0.8) * 0.085
    order = sorted(range(len(ends)), key=lambda i: ends[i][0])
    lab = [ends[i][0] for i in order]
    for j in range(1, len(lab)):
        lab[j] = max(lab[j], lab[j - 1] + gap)
    for j in range(len(lab) - 2, -1, -1):
        lab[j] = min(lab[j], lab[j + 1] - gap)
    for j, i in enumerate(order):
        y, label, c = ends[i]
        ax.text(X1, lab[j], f"  {label}  {y:.2f}×", va="center", color=INK, fontsize=10.5)
    ax.axhline(1, color=MUTED, lw=0.8)
    if title:
        ax.set_title(title, color=INK, loc="left", fontsize=13)
    ax.set_ylim(0.8, ymax)
    ax.set_ylabel("speedup", color=MUTED)
    ax.grid(axis="y", color=GRID, lw=0.6)
    ax.set_axisbelow(True)
    ax.tick_params(axis="both", length=0, colors=MUTED)
    for s in ("top", "right", "left", "bottom"):
        ax.spines[s].set_visible(False)


def figure(panels, out):
    h = 3.4 * len(panels) + 0.4
    fig, axes = plt.subplots(len(panels), 1, figsize=(8.2, h), sharex=True)
    if len(panels) == 1:
        axes = [axes]
    for ax, (title, series, ymax) in zip(axes, panels):
        panel(ax, title, series, ymax)
    axes[-1].set_xlim(X0, X1)
    axes[-1].xaxis.set_major_locator(mdates.MonthLocator(bymonth=[1, 7]))
    axes[-1].xaxis.set_major_formatter(mdates.DateFormatter("%b %Y"))
    fig.tight_layout()
    fig.subplots_adjust(right=0.66)
    fig.savefig(out + ".png", dpi=160)
    subprocess.run(["cwebp", "-quiet", "-q", "85", out + ".png", "-o", out + ".webp"], check=False)
    plt.close(fig)


figure([("ada, URL parsing", ada(), 2.6)], "ada")
figure([("fast_float, number parsing", fast_float(), 1.9)], "fast_float")
figure([("simdjson, serialization with C++26 reflection", simdjson("serialization"), 2.3),
        ("simdjson, deserialization with C++26 reflection", simdjson("parsing"), 2.3)], "simdjson")
figure([("simdutf, Unicode validation and transcoding",
         sweep("simdutf", [("validate_ascii", "validate_ascii"), ("validate_utf16le", "validate_utf16le"),
                           ("base64_decode", "base64_decode"), ("utf8_to_utf16le", "utf8_to_utf16le")], True), 2.3)],
       "simdutf")
figure([("CRoaring, compressed bitmaps in C",
         sweep("CRoaring", [("contains, census-income", "contains/census-income"),
                            ("to_array, weather", "to_array/weather_sept_85"),
                            ("iterate64, census-income", "iterate64/census-income"),
                            ("cardinality64, weather", "cardinality64/weather_sept_85")], False), 5.6)],
       "croaring")
figure([("roaring, compressed bitmaps in Go",
         sweep("roaring", [("iterate_many, census-income", "iterate_many/census-income"),
                           ("fast_or, census-income", "fast_or/census-income"),
                           ("to_array, weather", "to_array/weather_sept_85"),
                           ("and_cardinality, weather", "and_cardinality/weather_sept_85")], False), 6.6)],
       "roaring")
