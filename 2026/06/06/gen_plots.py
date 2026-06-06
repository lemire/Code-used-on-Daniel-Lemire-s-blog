#!/usr/bin/env python3
"""
gen_plots.py

Generate the two SVG figures used in post.md from the benchmark results in
./results/benchstat.txt. The numbers below are transcribed from that file
(github.com/RoaringBitmap/roaring/v2 v2.18.2, Go 1.26.2, Xeon Gold 6548N);
each row notes the percentage change versus the GOAMD64=v1 baseline as reported
by benchstat. Changes that benchstat flagged as not statistically significant
("~") are recorded as 0.

No third-party dependencies: the script emits SVG by hand so it runs anywhere a
stock Python 3 is installed.

    python3 gen_plots.py        # writes results/popcount_levels.svg and
                                # results/level_speedups.svg
"""

import os

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "results")

# ---------------------------------------------------------------------------
# Figure 1: absolute time to population-count a bitmap container, per level.
# (Popcount-128 row of benchstat.txt, in nanoseconds.)
# ---------------------------------------------------------------------------
POPCOUNT_NS = [("v1", 40.19), ("v2", 22.81), ("v3", 22.66), ("v4", 22.83)]

# ---------------------------------------------------------------------------
# Figure 2: percentage change versus v1 for a curated set of operations, at
# each of v2/v3/v4. Negative is faster. (vs base columns of benchstat.txt.)
# ---------------------------------------------------------------------------
#                       label                         v2     v3     v4
SPEEDUPS = [
    ("Popcount",                                   -43.2, -43.6, -43.2),
    ("CountTrailingZeros",                         -21.2, -22.4, -22.2),
    ("FromDense array",                            -20.7, -37.9, -37.3),
    ("FromDense bitmaps+runs",                       0.0, -30.0, -29.1),
    ("AndNot array∩run",                        0.0, -25.7, -21.8),
    ("IntersectionCardinality",                      1.0, -22.4, -22.5),
    ("XorDense",                                   -14.1, -12.4, -13.2),
    ("Xor",                                          0.0,  +8.2,  +7.2),
    ("FastOrRunContainers",                         -6.8, +18.0, +18.1),
]

GREEN = "#2e8b57"
RED = "#c0392b"
GRID = "#d0d0d0"
AXIS = "#333333"
SERIES = {"v2": "#9ecae1", "v3": "#4292c6", "v4": "#084594"}


def esc(s):
    return s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")


def write(path, body):
    with open(path, "w") as f:
        f.write(body)
    print("wrote", path)


# ---------------------------------------------------------------------------
def fig_popcount():
    W, H = 560, 320
    ml, mr, mt, mb = 60, 20, 50, 50
    pw, ph = W - ml - mr, H - mt - mb
    ymax = 45.0
    n = len(POPCOUNT_NS)
    bw = pw / n * 0.55
    gap = pw / n
    s = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" '
         f'font-family="-apple-system,Segoe UI,Helvetica,Arial,sans-serif">']
    s.append(f'<rect width="{W}" height="{H}" fill="white"/>')
    s.append(f'<text x="{W/2}" y="26" text-anchor="middle" font-size="16" '
             f'font-weight="bold" fill="{AXIS}">Time to popcount a bitmap container</text>')
    # y gridlines + labels
    for t in range(0, int(ymax) + 1, 10):
        y = mt + ph - (t / ymax) * ph
        s.append(f'<line x1="{ml}" y1="{y:.1f}" x2="{ml+pw}" y2="{y:.1f}" '
                 f'stroke="{GRID}" stroke-width="1"/>')
        s.append(f'<text x="{ml-8}" y="{y+4:.1f}" text-anchor="end" '
                 f'font-size="12" fill="{AXIS}">{t}</text>')
    s.append(f'<text x="16" y="{mt+ph/2}" text-anchor="middle" font-size="12" '
             f'fill="{AXIS}" transform="rotate(-90 16 {mt+ph/2})">nanoseconds (lower is better)</text>')
    for i, (lvl, v) in enumerate(POPCOUNT_NS):
        cx = ml + gap * (i + 0.5)
        x = cx - bw / 2
        h = (v / ymax) * ph
        y = mt + ph - h
        color = RED if lvl == "v1" else GREEN
        s.append(f'<rect x="{x:.1f}" y="{y:.1f}" width="{bw:.1f}" height="{h:.1f}" fill="{color}"/>')
        s.append(f'<text x="{cx:.1f}" y="{y-6:.1f}" text-anchor="middle" '
                 f'font-size="12" font-weight="bold" fill="{AXIS}">{v:.1f}</text>')
        s.append(f'<text x="{cx:.1f}" y="{mt+ph+20:.1f}" text-anchor="middle" '
                 f'font-size="13" fill="{AXIS}">GOAMD64={lvl}</text>')
    s.append('</svg>')
    write(os.path.join(OUT, "popcount_levels.svg"), "\n".join(s))


# ---------------------------------------------------------------------------
def fig_speedups():
    rows = SPEEDUPS
    n = len(rows)
    rowh = 46
    ml, mr, mt, mb = 200, 30, 60, 36
    pw = 420
    W = ml + pw + mr
    H = mt + n * rowh + mb
    lo, hi = -50.0, 25.0  # percent axis range

    def xfor(p):
        return ml + (p - lo) / (hi - lo) * pw

    s = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" '
         f'font-family="-apple-system,Segoe UI,Helvetica,Arial,sans-serif">']
    s.append(f'<rect width="{W}" height="{H}" fill="white"/>')
    s.append(f'<text x="{W/2}" y="24" text-anchor="middle" font-size="16" '
             f'font-weight="bold" fill="{AXIS}">Speed vs. GOAMD64=v1 baseline</text>')
    s.append(f'<text x="{W/2}" y="42" text-anchor="middle" font-size="12" '
             f'fill="{AXIS}">left of zero = faster, right = slower</text>')
    # vertical gridlines every 10%
    t = -50
    while t <= 25:
        x = xfor(t)
        s.append(f'<line x1="{x:.1f}" y1="{mt}" x2="{x:.1f}" y2="{H-mb}" '
                 f'stroke="{GRID}" stroke-width="1"/>')
        s.append(f'<text x="{x:.1f}" y="{H-mb+16}" text-anchor="middle" '
                 f'font-size="11" fill="{AXIS}">{t:+d}%</text>')
        t += 10
    # zero line
    zx = xfor(0)
    s.append(f'<line x1="{zx:.1f}" y1="{mt}" x2="{zx:.1f}" y2="{H-mb}" '
             f'stroke="{AXIS}" stroke-width="1.5"/>')
    barh = rowh / 4.2
    for i, (label, v2, v3, v4) in enumerate(rows):
        ry = mt + i * rowh
        s.append(f'<text x="{ml-10}" y="{ry+rowh/2+4:.1f}" text-anchor="end" '
                 f'font-size="12.5" fill="{AXIS}">{esc(label)}</text>')
        for j, (lvl, val) in enumerate([("v2", v2), ("v3", v3), ("v4", v4)]):
            by = ry + 6 + j * (barh + 1.5)
            x0 = xfor(min(0, val))
            x1 = xfor(max(0, val))
            w = max(0.6, x1 - x0)
            s.append(f'<rect x="{x0:.1f}" y="{by:.1f}" width="{w:.1f}" '
                     f'height="{barh:.1f}" fill="{SERIES[lvl]}"/>')
            tx = (x1 + 4) if val >= 0 else (x0 - 4)
            anc = "start" if val >= 0 else "end"
            txt = "~" if abs(val) < 0.5 else f"{val:+.0f}%"
            s.append(f'<text x="{tx:.1f}" y="{by+barh-1.5:.1f}" text-anchor="{anc}" '
                     f'font-size="10" fill="{AXIS}">{txt}</text>')
    # legend
    lx = ml + pw - 150
    ly = mt + 6
    for k, (lvl, col) in enumerate(SERIES.items()):
        s.append(f'<rect x="{lx+k*50}" y="{ly}" width="12" height="12" fill="{col}"/>')
        s.append(f'<text x="{lx+k*50+16}" y="{ly+11}" font-size="12" fill="{AXIS}">{lvl}</text>')
    s.append('</svg>')
    write(os.path.join(OUT, "level_speedups.svg"), "\n".join(s))


if __name__ == "__main__":
    fig_popcount()
    fig_speedups()
