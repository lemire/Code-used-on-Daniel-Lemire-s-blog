# Bar charts for the post, from the raw output of run.sh and run_queries.sh.
#   python plot.py results_c8a.txt c8a "AMD EPYC 9R45, 192 cores"
#   python plot.py queries results_c8a_queries.txt c8a_queries "AMD EPYC 9R45, 192 cores"
# Writes, as .png and .webp:
#   <stem>_one: one core, each library;
#   <stem>_all: each library at its best number of workers;
#   <stem>_workers: fastsimdjson and msgspec (processes) by number of workers.
import subprocess
import sys
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib import font_manager

SURFACE = "#fff7ef"
INK = "#1c2430"
GRID = "#f0d9c8"
MUTED = "#8a6f5c"
BLUE = "#1d4ed8"
GRAY = "#9a8676"

HERE = Path(__file__).resolve().parent
NAMES = {
    ("json", "process"): "json",
    ("orjson", "process"): "orjson",
    ("msgspec", "thread"): "msgspec (threads)",
    ("msgspec", "process"): "msgspec",
    ("fastsimdjson", "thread"): "fastsimdjson",
}


def font_family():
    have = {f.name for f in font_manager.fontManager.ttflist}
    for name in ("Avenir Next", "Helvetica Neue", "Helvetica"):
        if name in have:
            return name
    return "sans-serif"


def read(path):
    """{(library, kind): {workers: GB/s}}"""
    rows = {}
    for line in open(path):
        f = line.split()
        if len(f) < 7 or f[0].startswith("#") or f[6] != "GB/s":
            continue
        rows.setdefault((f[0], f[1]), {})[int(f[2])] = float(f[5])
    return rows


def save(fig, stem):
    png = HERE / f"{stem}.png"
    fig.savefig(png, dpi=150)
    plt.close(fig)
    subprocess.run(["cwebp", "-quiet", "-q", "85", str(png), "-o",
                    str(HERE / f"{stem}.webp")], check=True)


def figure(height, heading, subtitle):
    """A figure of the given height (inches) with a title and a subtitle."""
    fig = plt.figure(figsize=(13, height))
    fig.text(0.03, 1 - 0.35 / height, heading, ha="left", va="top", fontsize=32,
             fontweight="bold", color=INK)
    fig.text(0.03, 1 - 1.05 / height, subtitle, ha="left", va="top", fontsize=19,
             color=MUTED)
    return fig


def style_axes(ax):
    ax.tick_params(axis="x", labelsize=18, colors=MUTED, length=0)
    ax.tick_params(axis="y", length=0, pad=12)
    for spine in ax.spines.values():
        spine.set_visible(False)


def label(value):
    return f"{value:.1f} GB/s" if value >= 1 else f"{value:.2f} GB/s"


def bars(items, heading, subtitle, stem):
    """items: list of (label, GB/s, note); fastsimdjson in blue."""
    n = len(items)
    height = 1.15 * n + 2.2
    fig = figure(height, heading, subtitle)
    ax = fig.add_axes([0.27, 0.45 / height, 0.70, 1 - 2.2 / height])
    top = max(v for _, v, _ in items)
    for i, (name, value, note) in enumerate(items):
        ours = name == "fastsimdjson"
        ax.barh(i, value, height=0.7, color=BLUE if ours else GRAY, zorder=3)
        text = label(value) + (f"  ({note})" if note else "")
        ax.text(value + top * 0.015, i, text, va="center", ha="left", fontsize=21,
                fontweight="bold" if ours else "normal", color=BLUE if ours else INK,
                zorder=4)
    ax.set_yticks(range(n))
    ax.set_yticklabels([name for name, _, _ in items], fontsize=23)
    for t, (name, _, _) in zip(ax.get_yticklabels(), items):
        if name == "fastsimdjson":
            t.set_fontweight("bold")
            t.set_color(BLUE)
    ax.invert_yaxis()
    ax.set_xlim(0, top * 1.6)
    ax.set_xticks([])
    style_axes(ax)
    save(fig, stem)


def by_workers(rows, heading, subtitle, stem):
    ours = rows[("fastsimdjson", "thread")]
    theirs = rows[("msgspec", "process")]
    workers = sorted(set(ours) & set(theirs))
    height = 6.4
    fig = figure(height, heading, subtitle)
    ax = fig.add_axes([0.08, 0.75 / height, 0.89, 1 - 2.5 / height])
    w = 0.38
    top = max(max(ours.values()), max(theirs.values()))
    for k, n in enumerate(workers):
        for dx, value, color in ((-w / 2, theirs[n], GRAY), (w / 2, ours[n], BLUE)):
            ax.bar(k + dx, value, width=w, color=color, zorder=3)
            ax.text(k + dx, value + top * 0.015, f"{value:.0f}" if value >= 10 else f"{value:.1f}",
                    ha="center", va="bottom", fontsize=15, color=color,
                    fontweight="bold" if color == BLUE else "normal")
    ax.set_xticks(range(len(workers)))
    ax.set_xticklabels([str(n) for n in workers], fontsize=18, color=INK)
    ax.set_xlabel("processes or threads", fontsize=18, color=MUTED, labelpad=8)
    ax.set_ylim(0, top * 1.15)
    ax.set_yticks([])
    ax.grid(axis="y", color=GRID, lw=1.2, zorder=0)
    style_axes(ax)
    ax.bar(0, 0, color=BLUE, label="fastsimdjson (threads)")
    ax.bar(0, 0, color=GRAY, label="msgspec (processes)")
    leg = ax.legend(loc="upper left", frameon=False, fontsize=18)
    for t, c in zip(leg.get_texts(), (BLUE, MUTED)):
        t.set_color(c)
    save(fig, stem)


QUERY_LIBS = [("orjson", "processes", "#c9b8a8"), ("msgspec", "processes", GRAY),
              ("fastsimdjson", "threads", BLUE)]


def read_queries(path):
    """{query: {library: (best GB/s, workers)}}"""
    best = {}
    for line in open(path):
        f = line.split()
        if len(f) < 8 or f[0].startswith("#") or f[7] != "GB/s":
            continue
        q, lib, w, v = f[0], f[1], int(f[3]), float(f[6])
        if v > best.setdefault(q, {}).get(lib, (0, 0))[0]:
            best[q][lib] = (v, w)
    return best


def queries_chart(path, machine, stem):
    best = read_queries(path)
    queries = list(best)
    height = 7.0
    fig = figure(height, "Six queries, six files",
                 f"GB/s, best number of processes or threads, {machine}")
    ax = fig.add_axes([0.04, 0.95 / height, 0.94, 1 - 3.0 / height])
    w = 0.27
    top = max(v for q in best.values() for v, _ in q.values())
    for k, q in enumerate(queries):
        for j, (lib, _, color) in enumerate(QUERY_LIBS):
            v = best[q][lib][0]
            x = k + (j - 1) * w
            ax.bar(x, v, width=w * 0.92, color=color, zorder=3)
            ax.text(x, v + top * 0.012, f"{v:.0f}" if v >= 10 else f"{v:.1f}", ha="center",
                    va="bottom", fontsize=14, color=BLUE if lib == "fastsimdjson" else INK,
                    fontweight="bold" if lib == "fastsimdjson" else "normal")
    ax.set_xticks(range(len(queries)))
    ax.set_xticklabels(queries, fontsize=18, color=INK)
    ax.set_ylim(0, top * 1.15)
    ax.set_yticks([])
    ax.grid(axis="y", color=GRID, lw=1.2, zorder=0)
    style_axes(ax)
    for lib, kind, color in QUERY_LIBS:
        ax.bar(0, 0, color=color, label=f"{lib} ({kind})")
    leg = fig.legend(*ax.get_legend_handles_labels(), loc="upper left", ncol=3,
                     bbox_to_anchor=(0.025, 1 - 1.6 / height), frameon=False, fontsize=17,
                     columnspacing=1.6)
    for t, (lib, _, color) in zip(leg.get_texts(), QUERY_LIBS):
        t.set_color(BLUE if lib == "fastsimdjson" else INK)
    save(fig, stem)


def main():
    if sys.argv[1] == "queries":
        plt.rcParams.update({"font.family": font_family(), "text.color": INK,
                             "figure.facecolor": SURFACE, "savefig.facecolor": SURFACE,
                             "axes.facecolor": SURFACE})
        queries_chart(sys.argv[2], sys.argv[4], sys.argv[3])
        return
    path, stem, machine = sys.argv[1], sys.argv[2], sys.argv[3]
    plt.rcParams.update({"font.family": font_family(), "text.color": INK,
                         "figure.facecolor": SURFACE, "savefig.facecolor": SURFACE,
                         "axes.facecolor": SURFACE})
    rows = read(path)
    one = [(NAMES[k], rows[k][1], "") for k in NAMES
           if k in rows and 1 in rows[k] and k != ("msgspec", "thread")]
    bars(one, "Counting stars on one core", f"7.6 GB of GitHub events, {machine}",
         f"{stem}_one")
    best = []
    for k, name in NAMES.items():
        if k in rows:
            w, v = max(rows[k].items(), key=lambda kv: kv[1])
            best.append((name, v, f"{w} {'processes' if k[1] == 'process' else 'threads'}"))
    bars(best, "Counting stars on all cores",
         f"7.6 GB of GitHub events, {machine}, best number of workers", f"{stem}_all")
    by_workers(rows, "More cores", f"GB/s, 7.6 GB of GitHub events, {machine}",
               f"{stem}_workers")


if __name__ == "__main__":
    main()
