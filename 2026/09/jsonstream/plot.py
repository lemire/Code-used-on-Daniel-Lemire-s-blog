# Bar charts for streaming compressed NDJSON through simdjson.
import matplotlib.pyplot as plt
from matplotlib.patches import PathPatch
from matplotlib.path import Path

rows = {}
for line in open("results_big4.txt"):
    if line.startswith("| ") and not line.startswith("| configuration"):
        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        rows[" | ".join(cells[:-1])] = float(cells[-1]) / 1000  # GB/s


def gbs(label):
    if label not in rows:
        raise SystemExit(f"missing {label}")
    return rows[label]


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


def grouped(path, groups, series, title, subtitle, w=0.36):
    fig, ax = plt.subplots(figsize=(12, 7), dpi=200)
    fig.patch.set_facecolor(SURFACE)
    ax.set_facecolor(SURFACE)
    n = len(groups)
    k = len(series)
    gap = 0.04
    vals = [v for _, _, vs in series for v in vs]
    ymax = max(vals) * 1.18
    for s, (label, color, vs) in enumerate(series):
        shift = (s - (k - 1) / 2) * (w + gap)
        for i, v in enumerate(vs):
            x = i + shift
            ax.add_patch(PathPatch(rounded(x, v, w, ymax), color=color, lw=0, label=label if i == 0 else None))
            ax.text(x, v + ymax * 0.015, f"{v:.1f}", ha="center", va="bottom",
                    color=INK, fontsize=15, fontweight="bold")
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
    if k > 1:
        leg = ax.legend(frameon=False, ncol=k, loc="upper left", fontsize=16)
        for t in leg.get_texts():
            t.set_color(INK)
    fig.tight_layout(rect=(0, 0, 1, 0.86))
    fig.text(0.012, 0.955, title, ha="left", va="top", fontsize=24, fontweight="bold", color=INK)
    fig.text(0.012, 0.895, subtitle, ha="left", va="top", fontsize=17, color=MUTED)
    fig.savefig(path, facecolor=SURFACE)
    plt.close(fig)


threads = [1, 2, 4, 8, 16, 32, 64]
grouped(
    "uncompressed.png",
    [str(t) for t in threads],
    [("uncompressed", BLUE, [gbs(f"uncompressed file, threads={t}, chunk 64 KiB") for t in threads])],
    "Parsing an uncompressed NDJSON file with simdjson",
    "812 MB, number of threads, Xeon Gold 6548N (2 sockets), GB/s, higher is better",
    w=0.6,
)

# gzip with 1 thread: the single-threaded loop. With more threads: one thread
# inflates and the others parse.
grouped(
    "formats.png",
    ["uncompressed", "gzip", "zstd frames", "lz4 frames"],
    [
        ("1 thread", ORANGE, [gbs("uncompressed file, threads=1, chunk 64 KiB"),
                              gbs("gzip file, chunk 256 KiB"),
                              gbs("zst frames, threads=1"), gbs("lz4 frames, threads=1")]),
        ("8 threads", GREEN, [gbs("uncompressed file, threads=8, chunk 64 KiB"),
                              gbs("gzip file, threads=8 (+1 inflating), chunk 256 KiB"),
                              gbs("zst frames, threads=8"), gbs("lz4 frames, threads=8")]),
        ("64 threads", BLUE, [gbs("uncompressed file, threads=64, chunk 64 KiB"),
                              gbs("gzip file, threads=64 (+1 inflating), chunk 256 KiB"),
                              gbs("zst frames, threads=64"), gbs("lz4 frames, threads=64")]),
    ],
    "Decompressing and parsing NDJSON with simdjson",
    "812 MB of NDJSON, Xeon Gold 6548N (2 sockets), GB/s of JSON, higher is better",
    w=0.26,
)
