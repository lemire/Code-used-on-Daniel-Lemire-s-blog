# Bar chart of the time to link the Node.js binary (big4, median of 6 runs).
import statistics
import matplotlib.pyplot as plt

times = {}
for f in ["results_big4_purelink.txt", "results_big4_purelink_threads.txt",
          "results_big4_purelink_lld.txt"]:
    for line in open(f):
        name, t = line.split()
        times.setdefault(name, []).append(float(t))
labels = {"bfd": "GNU ld (bfd) 2.41", "gold": "GNU gold 2.41",
          "lld-threads1": "LLVM lld 21.1, 1 thread", "lld": "LLVM lld 21.1, 128 threads",
          "mold-threads1": "mold 3.0.0, 1 thread", "mold": "mold 3.0.0, 128 threads"}
order = ["bfd", "gold", "lld-threads1", "lld", "mold-threads1", "mold"]
med = [statistics.median(times[k]) for k in order]

fig, ax = plt.subplots(figsize=(7, 4.2), dpi=150)
colors = ["#2a78d6" if k.startswith("mold") else "#b5b4ae" for k in order]
bars = ax.barh([labels[k] for k in order], med, color=colors, height=0.6)
ax.invert_yaxis()
for b, v in zip(bars, med):
    ax.text(v + 0.04, b.get_y() + b.get_height() / 2, f"{v:.2f} s",
            va="center", fontsize=10, color="#0b0b0b")
ax.set_xlabel("time to link the node binary (seconds)", color="#52514e")
ax.set_xlim(0, max(med) * 1.15)
ax.set_title("Linking Node.js (GCC 14.3, Xeon Gold 6548N)", loc="left", fontsize=11)
for s in ["top", "right", "left"]:
    ax.spines[s].set_visible(False)
ax.spines["bottom"].set_color("#b5b4ae")
ax.tick_params(axis="y", length=0)
ax.tick_params(colors="#52514e")
ax.xaxis.grid(True, color="#e6e5e0", linewidth=0.8)
ax.set_axisbelow(True)
fig.tight_layout()
fig.savefig("link_big4.png")
