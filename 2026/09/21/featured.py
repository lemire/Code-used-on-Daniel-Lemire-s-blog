# 1200x630 Open Graph / WordPress featured image for the post.
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
miss = [1.37, 2.25, 3.06, 2.05, 2.04, 2.27, 1.55]
order = sorted(range(len(cpus)), key=lambda i: miss[i])
names = [cpus[i] for i in order]
vals = [miss[i] for i in order]

INK, MUTED, BAR = "#0b0b0b", "#52514e", "#c45c26"
fig = plt.figure(figsize=(12, 6.3), dpi=100)
fig.patch.set_facecolor("#f7f6f2")
ax = fig.add_axes([0.22, 0.14, 0.72, 0.68])
y = range(len(names))
ax.barh(y, vals, height=0.62, color=BAR)
ax.set_xlim(0, 3.5)
ax.set_xticks([0, 1, 2, 3])
ax.set_xticklabels(["0", "1", "2", "3"], color=MUTED)
ax.set_yticks(list(y))
ax.set_yticklabels(names, color=INK, fontsize=13)
ax.invert_yaxis()
ax.set_xlabel("cycles per iteration  ·  always miss", color=MUTED)
ax.grid(axis="x", color="#dddcd8", lw=0.6)
ax.set_axisbelow(True)
ax.tick_params(axis="both", length=0)
ax.set_facecolor("#f7f6f2")
for s in ("top", "right", "left", "bottom"):
    ax.spines[s].set_visible(False)
for i, v in enumerate(vals):
    ax.text(v + 0.06, i, f"{v:.2f}", va="center", color=INK, fontsize=12)
fig.text(0.04, 0.92, "Densely packed taken branches", fontsize=22, color=INK, weight="bold")
fig.savefig("featured.png", dpi=100)
plt.close(fig)
subprocess.run(["cwebp", "-quiet", "-q", "85", "featured.png", "-o", "featured.webp"], check=False)
