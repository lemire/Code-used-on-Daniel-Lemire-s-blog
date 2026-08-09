#!/usr/bin/env python3
"""Go PGO on encoding/json: which document you profile decides what you gain.

One stacked panel per benchmarked document; within a panel, one bar per
choice of training document.
"""
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

plt.rcParams.update({
    'font.size': 14,
    'axes.titlesize': 15,
    'axes.labelsize': 14,
    'xtick.labelsize': 13,
    'ytick.labelsize': 13,
})

tests = ['twitter.json', 'canada.json', 'citm_catalog.json']
base = {'twitter.json': 111.7, 'canada.json': 73.7, 'citm_catalog.json': 115.8}
# measured MB/s: [profiled twitter, profiled canada, profiled citm]
measured = {
    'twitter.json':      [115.2, 112.2, 114.3],
    'canada.json':       [75.2, 77.2, 73.5],
    'citm_catalog.json': [119.1, 115.3, 116.7],
}
trained = ['profiled\ntwitter', 'profiled\ncanada', 'profiled\ncitm_catalog']

fig, axes = plt.subplots(3, 1, figsize=(7.6, 11.0), sharex=True)
for ax, doc in zip(axes, tests):
    b = base[doc]
    pct = [(v / b - 1) * 100 for v in measured[doc]]
    # the bar whose profile matches the document being parsed
    colors = ['#2e7d32' if tests[i] == doc else '#8d9db6' for i in range(3)]
    bars = ax.bar(trained, pct, color=colors, width=0.55)
    for bar, p in zip(bars, pct):
        ax.text(bar.get_x() + bar.get_width() / 2,
                p + (0.18 if p >= 0 else -0.52),
                f'{p:+.1f}%', ha='center', fontsize=13)
    ax.axhline(0, color='black', linewidth=1.0)
    ax.set_title(f'parsing {doc}   (baseline {b:.0f} MB/s)', pad=10)
    ax.set_ylim(-1.6, 6.2)
    ax.set_ylabel('faster (%)')
    for s in ('top', 'right'):
        ax.spines[s].set_visible(False)
    ax.yaxis.grid(True, linestyle=':', alpha=0.55)
    ax.set_axisbelow(True)

fig.suptitle('Go 1.26: profile-guided optimization of encoding/json',
             fontsize=16, y=0.985)
fig.tight_layout(rect=(0, 0, 1, 0.972))
fig.savefig('go-pgo.png', dpi=170)
print('wrote go-pgo.png')
