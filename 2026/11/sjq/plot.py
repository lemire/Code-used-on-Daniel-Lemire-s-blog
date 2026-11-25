# Bar charts for the post, from the hyperfine exports in results/ (run_bench.sh).
#   python plot.py
# Writes ndjson.png/.webp (1 GB of NDJSON) and document.png/.webp (one 840 MB
# document): the speed of jq, sjq (one thread, the default) and sjq -P, in GB/s.
import json
import subprocess
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib import font_manager

SURFACE = "#fff7ef"
INK = "#1c2430"
MUTED = "#8a6f5c"
BLUE = "#1d4ed8"
LIGHT_BLUE = "#7fa3f0"
GRAY = "#9a8676"

HERE = Path(__file__).resolve().parent
SIZES = {"tweets.ndjson": 1073745346, "timeline.json": 840821077}
SERIES = [("jq", "jq 1.8.2", GRAY), ("sjq1", "sjq, one thread", LIGHT_BLUE), ("sjq", "sjq -P", BLUE)]


def font_family():
    have = {f.name for f in font_manager.fontManager.ttflist}
    for name in ("Avenir Next", "Helvetica Neue", "Helvetica"):
        if name in have:
            return name
    return "sans-serif"


def read():
    """[(workload, input, {series: GB/s})]"""
    rows = []
    for line in open(HERE / "results" / "index.tsv"):
        wid, label, data = line.rstrip("\n").split("\t")
        speed = {}
        for key, _, _ in SERIES:
            r = json.load(open(HERE / "results" / f"{wid}-{key}.json"))["results"][0]
            seconds = r["summary"]["time_wall_clock"]["mean"]
            speed[key] = SIZES[data] / seconds / 1e9
        rows.append((label, data, speed))
    return rows


def save(fig, stem):
    png = HERE / f"{stem}.png"
    fig.savefig(png, dpi=150)
    plt.close(fig)
    subprocess.run(["cwebp", "-quiet", "-q", "85", str(png), "-o", str(HERE / f"{stem}.webp")],
                   check=True)


def label(value):
    return f"{value:.1f} GB/s" if value >= 1 else f"{value:.2f} GB/s"


def chart(rows, heading, subtitle, stem):
    n = len(rows)
    height = 1.55 * n + 2.6
    fig = plt.figure(figsize=(13, height))
    fig.text(0.03, 1 - 0.35 / height, heading, ha="left", va="top", fontsize=32,
             fontweight="bold", color=INK)
    fig.text(0.03, 1 - 1.05 / height, subtitle, ha="left", va="top", fontsize=19, color=MUTED)
    ax = fig.add_axes([0.31, 0.35 / height, 0.66, 1 - 2.6 / height])
    top = max(v for _, _, s in rows for v in s.values())
    bar = 0.26
    for i, (name, _, speed) in enumerate(rows):
        for j, (key, _, color) in enumerate(SERIES):
            y = i + (j - 1) * bar
            v = speed[key]
            ax.barh(y, v, height=bar * 0.88, color=color, zorder=3)
            ax.text(v + top * 0.012, y, label(v), va="center", ha="left", fontsize=15,
                    color=BLUE if key == "sjq" else INK,
                    fontweight="bold" if key == "sjq" else "normal", zorder=4)
    ax.set_yticks(range(n))
    ax.set_yticklabels([name for name, _, _ in rows], fontsize=20, color=INK)
    ax.invert_yaxis()
    ax.set_xlim(0, top * 1.22)
    ax.set_xticks([])
    ax.tick_params(axis="y", length=0, pad=12)
    for spine in ax.spines.values():
        spine.set_visible(False)
    for key, name, color in SERIES:
        ax.barh(0, 0, color=color, label=name)
    leg = fig.legend(*ax.get_legend_handles_labels(), loc="upper left", ncol=3,
                     bbox_to_anchor=(0.025, 1 - 1.5 / height), frameon=False, fontsize=17,
                     columnspacing=1.6)
    for t, (key, _, _) in zip(leg.get_texts(), SERIES):
        t.set_color(BLUE if key == "sjq" else INK)
    save(fig, stem)


def main():
    plt.rcParams.update({"font.family": font_family(), "text.color": INK,
                         "figure.facecolor": SURFACE, "savefig.facecolor": SURFACE,
                         "axes.facecolor": SURFACE})
    rows = read()
    machine = "2 x Intel Xeon Gold 6548N, 64 cores"
    chart([r for r in rows if r[1] == "tweets.ndjson"], "1 GB of NDJSON",
          f"230,138 tweets, one per line, GB/s, {machine}", "ndjson")
    chart([r for r in rows if r[1] == "timeline.json"], "One 840 MB document",
          f"133,229 tweets in one array, GB/s, {machine}", "document")


if __name__ == "__main__":
    main()
