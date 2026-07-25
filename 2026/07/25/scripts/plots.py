#!/usr/bin/env python3
"""Figures for the memory-level-parallelism blog post."""
import os, re, csv, math
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator

SP  = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.dirname(SP)
RES = os.path.join(OUT, "results", "pass2-1GiB")

# ---- validated palette (dataviz reference instance, light surface) ----
SURFACE   = "#fcfcfb"
INK       = "#0b0b0b"
INK2      = "#52514e"
MUTED     = "#898781"
GRID      = "#e1e0d9"
AXIS      = "#c3c2b7"
VENDOR    = {"Intel": "#2a78d6", "AMD": "#eb6834", "Graviton": "#1baf7a"}
# ordinal blue ramp, steps 250/350/450/550/700 -- validated --ordinal
RAMP5     = ["#86b6ef", "#5598e7", "#2a78d6", "#1c5cab", "#0d366b"]
RAMP4     = ["#86b6ef", "#3987e5", "#1c5cab", "#0d366b"]

plt.rcParams.update({
    "font.family": "sans-serif",
    "font.sans-serif": ["Helvetica Neue", "Helvetica", "Arial", "DejaVu Sans"],
    "figure.facecolor": SURFACE, "axes.facecolor": SURFACE,
    "savefig.facecolor": SURFACE,
    "text.color": INK, "axes.labelcolor": INK2, "axes.edgecolor": AXIS,
    "xtick.color": MUTED, "ytick.color": MUTED,
    "xtick.labelcolor": INK2, "ytick.labelcolor": INK2,
    "axes.grid": True, "grid.color": GRID, "grid.linewidth": 0.8,
    "axes.spines.top": False, "axes.spines.right": False,
    "font.size": 10, "axes.titlesize": 11, "figure.dpi": 130,
})

# instance -> (vendor, short chip label, year the family launched)
SYS = {
    "m4.large":  ("Intel",    "Broadwell",      2016),
    "m5.large":  ("Intel",    "Cascade Lake",   2017),
    "m6i.large": ("Intel",    "Ice Lake",       2021),
    "m7i.large": ("Intel",    "Sapphire Rapids",2023),
    "m8i.large": ("Intel",    "Xeon 6 (GNR)",   2025),
    "m5a.large": ("AMD",      "Zen 1 (Naples)", 2018),
    "m6a.large": ("AMD",      "Zen 3 (Milan)",  2021),
    "m7a.large": ("AMD",      "Zen 4 (Genoa)",  2023),
    "m8a.large": ("AMD",      "Zen 5 (Turin)",  2025),
    "a1.large":  ("Graviton", "Graviton1",      2018),
    "m6g.large": ("Graviton", "Graviton2",      2020),
    "m7g.large": ("Graviton", "Graviton3",      2022),
    "m8g.large": ("Graviton", "Graviton4",      2024),
    "m9g.large": ("Graviton", "Graviton5",      2026),
}
GROUPS = {
    "Intel":    ["m4.large","m5.large","m6i.large","m7i.large","m8i.large"],
    "AMD":      ["m5a.large","m6a.large","m7a.large","m8a.large"],
    "Graviton": ["a1.large","m6g.large","m7g.large","m8g.large","m9g.large"],
}

def load_csv(t):
    """pass 2: MLP_CSV=1 -> header row of lane counts, one data row of MiB/s"""
    p = os.path.join(RES, t + ".csv")
    if not os.path.exists(p): return None
    rows = [r for r in csv.reader(open(p)) if r]
    if len(rows) < 2: return None
    lanes = [int(x) for x in rows[0][1:]]
    bw    = [float(x) for x in rows[1][1:]]
    n = min(len(lanes), len(bw))
    return lanes[:n], bw[:n]

def metrics(lanes, bw):
    peak = max(bw)
    imax = bw.index(peak)
    k90 = next(l for l, b in zip(lanes, bw) if b >= 0.90 * peak)
    # MLP by Little's law: concurrency = throughput x latency.
    # peak/bw[0] is exactly (peak_bytes_per_s * lat1) / 64.
    return dict(peak_gbs=peak/1024.0, peak_lanes=lanes[imax], k90=k90,
                lat1=61035.156/bw[0], mlp=peak/bw[0])

DATA = {}
for t in SYS:
    d = load_csv(t)
    if d:
        DATA[t] = dict(lanes=d[0], bw=d[1], **metrics(*d))

missing = [t for t in SYS if t not in DATA]
if missing:
    print("MISSING pass-2 data:", missing)

# ------------------------------------------------------------------ figure 1
def declutter(ys, gap):
    """nudge label y-positions apart, preserving order, minimal total movement"""
    order = sorted(range(len(ys)), key=lambda i: ys[i])
    out = list(ys)
    for k in range(1, len(order)):
        prev, cur = order[k-1], order[k]
        if out[cur] - out[prev] < gap:
            out[cur] = out[prev] + gap
    return out

def fig_curves():
    # one panel per vendor, stacked vertically, shared y so the three
    # families are directly comparable; fonts sized for a blog-width image.
    fig, axes = plt.subplots(3, 1, figsize=(9.0, 12.6), sharey=True)
    for ax, (vendor, members) in zip(axes, GROUPS.items()):
        present = [t for t in members if t in DATA]
        ramp = RAMP5 if len(present) >= 5 else RAMP4
        ends = []
        for i, t in enumerate(present):
            d = DATA[t]
            y = [b/1024.0 for b in d["bw"]]
            ax.plot(d["lanes"], y, lw=2.6, color=ramp[i], zorder=3,
                    solid_capstyle="round")
            ends.append((y[-1], SYS[t][1], ramp[i]))
        lab_y = declutter([e[0] for e in ends], gap=2.1)
        for (yv, name, col), ly in zip(ends, lab_y):
            ax.annotate(name, xy=(102, ly), xytext=(0, 0),
                        textcoords="offset points", va="center", ha="left",
                        fontsize=15, color=INK2, annotation_clip=False)
            if abs(ly - yv) > 0.2:    # leader line when the label was moved
                ax.plot([99.5, 101.5], [yv, ly], lw=1.0, color=AXIS,
                        zorder=2, clip_on=False)
        ax.set_title(vendor, color=INK, fontweight="bold", loc="left",
                     pad=10, fontsize=21)
        ax.set_ylabel("bandwidth (GiB/s)", fontsize=17)
        ax.set_xlim(0, 129)           # right margin holds the chip labels
        ax.set_xticks([1, 25, 50, 75, 99])
        ax.set_ylim(0, 26)
        ax.tick_params(labelsize=15)
        ax.grid(axis="both", alpha=0.9)
        ax.set_axisbelow(True)
    axes[-1].set_xlabel("independent memory lanes", fontsize=17)
    fig.text(0.005, 0.004,
             "1 GiB random pointer chase, transparent huge pages, AWS EC2 *.large, us-east-1.",
             ha="left", fontsize=12, color=MUTED)
    fig.tight_layout(rect=[0, 0.022, 1, 1])
    fig.subplots_adjust(hspace=0.28)
    fig.savefig(os.path.join(OUT, "mlp-curves.png"), bbox_inches="tight")
    print("wrote mlp-curves.png")

# ------------------------------------------------------------------ figure 2
def fig_race():
    fig, axes = plt.subplots(1, 3, figsize=(14.2, 4.6))
    panels = [("lat1",     "one-lane latency, ns  (lower is better)",
                           "1. Latency is going nowhere"),
              ("mlp",      "concurrent cache-line fetches",
                           "2. So they add concurrency"),
              ("peak_gbs", "peak single-core bandwidth (GiB/s)",
                           "3. Which is what you actually get")]
    for ax, (key, ylab, title) in zip(axes, panels):
        for vendor, members in GROUPS.items():
            pts = [(SYS[t][2], DATA[t][key], t) for t in members if t in DATA]
            pts.sort()
            ax.plot([p[0] for p in pts], [p[1] for p in pts], lw=2,
                    marker="o", ms=7, color=VENDOR[vendor], zorder=3,
                    markeredgecolor=SURFACE, markeredgewidth=2, label=vendor)
            if pts:
                ax.annotate(vendor, xy=(pts[-1][0], pts[-1][1]),
                            xytext=(6, 0), textcoords="offset points",
                            va="center", fontsize=9.5, color=INK2,
                            fontweight="bold")
        ax.set_title(title, color=INK, fontweight="bold", loc="left", pad=8)
        ax.set_ylabel(ylab)
        ax.set_xlabel("year the instance family launched")
        ax.set_xlim(2015.2, 2027.4)
        ax.xaxis.set_major_locator(MaxNLocator(integer=True))
        ax.set_ylim(bottom=0)
        ax.set_axisbelow(True)
    axes[0].legend(frameon=False, loc="upper left", fontsize=9,
                   labelcolor=INK2, handlelength=1.6)
    fig.suptitle("The memory-level-parallelism race on AWS, 2016-2026",
                 x=0.008, ha="left", fontsize=12.5, fontweight="bold", color=INK)
    fig.text(0.008, 0.005,
             "One core each, 1 GiB random pointer chase. Concurrency is "
             "peak bandwidth x one-lane latency / 64 B (Little's law).",
             ha="left", fontsize=8.5, color=MUTED)
    fig.tight_layout(rect=[0, 0.03, 1, 0.92])
    fig.savefig(os.path.join(OUT, "mlp-race.png"), bbox_inches="tight")
    print("wrote mlp-race.png")

# ------------------------------------------------------------------ table
def dump_table():
    print(f"{'instance':<10} {'chip':<17} {'yr':>4} {'lat1':>6} {'peakGiB/s':>9} "
          f"{'@lanes':>6} {'k90':>4} {'MLP':>5}")
    for vendor, members in GROUPS.items():
        for t in members:
            if t not in DATA: continue
            d = DATA[t]
            print(f"{t:<10} {SYS[t][1]:<17} {SYS[t][2]:>4} {d['lat1']:>6.1f} "
                  f"{d['peak_gbs']:>9.1f} {d['peak_lanes']:>6} {d['k90']:>4} "
                  f"{d['mlp']:>5.1f}")

if __name__ == "__main__":
    os.makedirs(OUT, exist_ok=True)
    dump_table()
    if len(DATA) >= 3:
        fig_curves(); fig_race()
