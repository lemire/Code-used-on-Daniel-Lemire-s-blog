# Stage-1 gain of the SVE2 `match` classifier over NEON, file by file,
# from predicate/results/{g4,g5}: bridge (svsel extraction) vs base.
# Produces speedup_gcc.png and speedup_clang.png.
import math
import matplotlib.pyplot as plt

def load(path):
    d = {}
    for line in open(path):
        p = line.rstrip('\n').split('\t')
        if len(p) < 8:
            continue
        d[p[0].strip('"')] = float(p[6])  # stage 1 throughput
    return d

def best(m, c, t):
    acc = {}
    for rep in (1, 2, 3):
        for k, v in load(f"predicate/results/{m}/{c}.{t}.{rep}.tsv").items():
            acc[k] = max(acc.get(k, 0), v)
    return acc

MACHINES = [("g4", "Graviton 4"), ("g5", "Graviton 5")]
COMPILERS = [("gcc15", "GCC", "speedup_gcc.png"), ("clang21", "clang", "speedup_clang.png")]
gain = {}
for m, _ in MACHINES:
    for c, _, _ in COMPILERS:
        a, b = best(m, c, "base"), best(m, c, "bridge")
        gain[(m, c)] = {k: 100 * (b[k] / a[k] - 1) for k in a}

# one file order for both figures: sorted by the average gain over all four configurations
files = sorted(gain[("g4", "gcc15")], key=lambda k: sum(g[k] for g in gain.values()))

BLUE, INK, MUTED = "#2a78d6", "#0b0b0b", "#52514e"
plt.rcParams.update({"font.size": 15})
y = range(len(files))
for c, cname, out in COMPILERS:
    fig, axes = plt.subplots(1, 2, figsize=(12, 9), sharey=True)
    for ax, (m, mname) in zip(axes, MACHINES):
        g = gain[(m, c)]
        vals = [g[k] for k in files]
        geo = 100 * (math.exp(sum(math.log(1 + v / 100) for v in vals) / len(vals)) - 1)
        ax.barh(y, vals, height=0.6, color=BLUE)
        ax.axvline(0, color=INK, lw=0.8)
        ax.axvline(geo, color=MUTED, lw=1.2, ls="--")
        ax.set_title(f"{mname}, {cname}\ngeometric mean: {geo:+.0f}%", color=INK)
        ax.set_xlim(-5, 18)
        ax.set_xticks([0, 5, 10, 15])
        ax.set_xticklabels(["0", "5%", "10%", "15%"], color=MUTED)
        ax.grid(axis="x", color="#dddcd8", lw=0.6)
        ax.set_axisbelow(True)
        ax.tick_params(axis="both", length=0)
        for s in ("top", "right", "left", "bottom"):
            ax.spines[s].set_visible(False)
    axes[0].set_yticks(list(y))
    axes[0].set_yticklabels(files, color=INK)
    fig.supxlabel("indexing (stage 1) throughput gain of SVE2 match over NEON", color=MUTED)
    fig.tight_layout()
    fig.savefig(out, dpi=200)
