# Cycles per iteration of lastHit: always-hit vs always-miss.
# Writes hits.png/webp and misses.png/webp with a shared x scale.
import subprocess
import matplotlib.pyplot as plt

cpus = [
    "M4 Max",
    "Graviton 5",
    "Zen 4",
    "Zen 5",
    "Ice Lake",
    "Emerald Rapids",
    "Granite Rapids",
]
hit = [1.36, 1.26, 2.05, 1.36, 1.54, 1.86, 1.56]
miss = [1.37, 2.25, 3.06, 2.05, 2.04, 2.27, 1.55]

INK, MUTED = "#0b0b0b", "#52514e"
HIT_COLOR, MISS_COLOR = "#2a78d6", "#c45c26"
XMAX = 3.5
plt.rcParams.update({"font.size": 14})

def one(values, color, title, out):
    order = sorted(range(len(cpus)), key=lambda i: values[i])
    names = [cpus[i] for i in order]
    vals = [values[i] for i in order]
    fig, ax = plt.subplots(figsize=(8.2, 4.6))
    y = range(len(names))
    ax.barh(y, vals, height=0.62, color=color)
    ax.set_xlim(0, XMAX)
    ax.set_xticks([0, 1, 2, 3])
    ax.set_xticklabels(["0", "1", "2", "3"], color=MUTED)
    ax.set_yticks(list(y))
    ax.set_yticklabels(names, color=INK)
    ax.invert_yaxis()
    ax.set_xlabel("cycles per iteration", color=MUTED)
    ax.set_title(title, color=INK, loc="left")
    ax.grid(axis="x", color="#dddcd8", lw=0.6)
    ax.set_axisbelow(True)
    ax.tick_params(axis="both", length=0)
    for s in ("top", "right", "left", "bottom"):
        ax.spines[s].set_visible(False)
    for i, v in enumerate(vals):
        ax.text(v + 0.06, i, f"{v:.2f}", va="center", color=INK, fontsize=12)
    fig.tight_layout()
    fig.savefig(out + ".png", dpi=160)
    plt.close(fig)
    subprocess.run(
        ["cwebp", "-quiet", "-q", "85", out + ".png", "-o", out + ".webp"],
        check=False,
    )

one(hit, HIT_COLOR, "Always hit (one taken branch)", "hits")
one(miss, MISS_COLOR, "Always miss (two taken branches)", "misses")
