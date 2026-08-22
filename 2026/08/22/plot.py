#!/usr/bin/env python3
"""Java String.indexOf vs Two-Way on the two AArch64 adversaries."""
import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt

plt.rcParams.update(
    {
        "font.size": 16,
        "axes.titlesize": 17,
        "axes.labelsize": 16,
        "xtick.labelsize": 15,
        "ytick.labelsize": 15,
        "legend.fontsize": 14,
        "lines.linewidth": 3.5,
        "lines.markersize": 11,
    }
)

n = 1 << 20  # 1 MiB haystack
m = [512, 1024, 2048, 4096]
# measured ns/char, converted to total search time in seconds
indexof_first = [t * n / 1e9 for t in [139.654, 273.034, 542.592, 1075.952]]
twoway_first = [t * n / 1e9 for t in [0.296, 0.318, 0.278, 0.313]]

fig, ax = plt.subplots(figsize=(8.2, 5.4))
ax.plot(m, indexof_first, "o-", color="#c0392b", label="indexOf",
        linewidth=3.5, markersize=11)
ax.plot(m, twoway_first, "o-", color="#1f6aa5", label="Two-Way",
        linewidth=3.5, markersize=11)
ax.set_xscale("log", base=2)
ax.set_xticks(m)
ax.set_xticklabels([str(x) for x in m])
ax.set_ylim(bottom=0)
ax.set_xlabel("needle length m")
ax.set_ylabel("search time (s)")
ax.set_title("1 MiB haystack of 'a'  ·  OpenJDK 25, Apple M4 Max")
ax.legend(frameon=False)
for s in ("top", "right"):
    ax.spines[s].set_visible(False)
ax.yaxis.grid(True, linestyle=":", alpha=0.55)
ax.set_axisbelow(True)
fig.tight_layout()
fig.savefig("indexof-quadratic.png", dpi=170)
print("wrote indexof-quadratic.png")
