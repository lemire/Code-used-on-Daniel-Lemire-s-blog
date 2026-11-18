"""Ranks the decoders by the median of several result files.

Usage: python3 rank.py results_big4_run*.txt > ranking_big4.md
"""
import re, statistics, sys
from collections import defaultdict

line_re = re.compile(r"^(.*?) mime (.*): ([0-9.]+) GB/s$")
speeds = defaultdict(list)
for path in sys.argv[1:]:
    for line in open(path):
        m = line_re.match(line.strip())
        if m:
            speeds[(m.group(1), m.group(2))].append(float(m.group(3)))

rows = sorted(((statistics.median(v), k, v) for k, v in speeds.items()), reverse=True)
print(f"MIME base64 decoding, 1 MB decoded (76-character lines, CRLF), GB/s of input, median of {len(sys.argv) - 1} runs.\n")
print("| rank | system | function | GB/s | runs |")
print("|---:|---|---|---:|---|")
for i, (med, (system, func), v) in enumerate(rows, 1):
    print(f"| {i} | {system} | `{func}` | {med:.2f} | {' '.join(f'{x:.2f}' for x in v)} |")
