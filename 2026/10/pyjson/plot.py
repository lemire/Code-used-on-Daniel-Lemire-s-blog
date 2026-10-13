"""Bar charts for the post: python plot.py results/big4

one_thread.png  the methods on one core (Python 3.14)
many_cores.png  the methods on 64 cores, one process (free-threaded Python 3.14t)
"""
import csv
import glob
import os
import sys

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import Patch

OUT = os.path.dirname(os.path.abspath(__file__))
rows, size, events = {}, 0, 0
for path in glob.glob(os.path.join(sys.argv[1], "*.csv")):
    for r in csv.DictReader(line for line in open(path) if not line.startswith("#")):
        rows[r["method"]] = float(r["gbps"])
        size, events = float(r["gbps"]) * float(r["seconds"]), int(r["events"])

# Colours by library (validated categorical slots; every bar carries its value).
COLOR = {"fastsimdjson": "#2a78d6", "msgspec": "#eb6834", "orjson": "#1baf7a", "json": "#eda100",
         "jiter": "#e87ba4", "ssrjson": "#7c3aed"}
INK, MUTED, GRID, SURFACE = "#0b0b0b", "#52514e", "#e4e3df", "#fcfcfb"

ONE = [  # (method, label, library)
    ("fastsimdjson-lazy", "fastsimdjson map_many (lazy views)", "fastsimdjson"),
    ("msgspec", "msgspec (typed Struct)", "msgspec"),
    ("fastsimdjson", "fastsimdjson map_many", "fastsimdjson"),
    ("orjson", "orjson", "orjson"),
    ("ssrjson", "ssrjson", "ssrjson"),
    ("jiter", "jiter (string cache)", "jiter"),
    ("jiter-nocache", "jiter (no string cache)", "jiter"),
    ("json", "json (standard library)", "json"),
]
MANY = [
    ("threads-fastsimdjson-lazy", "fastsimdjson map_many (lazy views)\n64 threads", "fastsimdjson"),
    ("threads-fastsimdjson", "fastsimdjson map_many\n64 threads", "fastsimdjson"),
    ("threads-msgspec", "msgspec (typed Struct)\n64 threads", "msgspec"),
    ("threads-json", "json (standard library)\n64 threads", "json"),
    ("threads-ssrjson", "ssrjson\n64 threads", "ssrjson"),
    ("threads-jiter", "jiter (no string cache)\n64 threads", "jiter"),
    ("threads-jiter-cache", "jiter (string cache)\n64 threads", "jiter"),
]


def chart(name, items, title, subtitle, height):
    items = [(m, l, lib) for m, l, lib in items if m in rows]
    items.sort(key=lambda x: rows[x[0]], reverse=True)
    plt.rcParams.update({"font.family": "DejaVu Sans", "font.size": 17})
    fig, ax = plt.subplots(figsize=(13, height), dpi=150, facecolor=SURFACE)
    ax.set_facecolor(SURFACE)
    ys = list(range(len(items)))[::-1]
    vmax = max(rows[m] for m, _, _ in items)
    for y, (m, label, lib) in zip(ys, items):
        v = rows[m]
        ax.barh(y, v, height=0.7, color=COLOR[lib], zorder=3)
        ax.text(v + vmax * 0.012, y, f"{v:.2f} GB/s" if v < 10 else f"{v:.1f} GB/s",
                va="center", ha="left", fontsize=18, fontweight="bold", color=INK, zorder=4)
    ax.set_yticks(ys, [l for _, l, _ in items], fontsize=17, color=INK)
    ax.set_xlim(0, vmax * 1.2)
    ax.set_xlabel("GB/s of JSON (higher is better)", fontsize=17, color=MUTED, labelpad=10)
    ax.tick_params(axis="x", colors=MUTED, labelsize=15, length=0)
    ax.tick_params(axis="y", length=0, pad=10)
    ax.grid(axis="x", color=GRID, linewidth=1.2, zorder=0)
    for side in ("top", "right", "left"):
        ax.spines[side].set_visible(False)
    ax.spines["bottom"].set_color(GRID)
    libs = []
    for _, _, lib in items:
        if lib not in libs:
            libs.append(lib)
    ax.legend(handles=[Patch(color=COLOR[l], label=l) for l in libs], loc="lower right",
              frameon=False, fontsize=16)
    fig.text(0.015, 1 - 0.38 / height, title, ha="left", va="center", fontsize=27,
             fontweight="bold", color=INK)
    fig.text(0.015, 1 - 0.82 / height, subtitle, ha="left", va="center", fontsize=16, color=MUTED)
    fig.subplots_adjust(left=0.35, right=0.97, top=1 - 1.15 / height, bottom=0.95 / height)
    fig.savefig(os.path.join(OUT, name), facecolor=SURFACE)
    plt.close(fig)
    print("wrote", name)


chart("one_thread.png", ONE, "One core",
      f"{events / 1e6:.1f} million GitHub events ({size:.1f} GB): count them by type, "
      "and pull requests by language",
      7.8)
chart("many_cores.png", MANY, "64 cores, one process",
      "Same task, free-threaded Python 3.14t, one CPU per core",
      8.2)
