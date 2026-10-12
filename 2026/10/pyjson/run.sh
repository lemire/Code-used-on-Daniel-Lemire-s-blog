#!/bin/bash
# Downloads the GH Archive hours, creates two Python 3.14 environments with uv
# (the default build with the GIL, and the free-threaded build 3.14t), runs
# bench.py in both and draws the charts. Needs uv, curl and a Linux machine
# (the process methods fork).
#   ./run.sh                    # 8 hours of events, 64 workers
#   HOURS=4 WORKERS=16 ./run.sh
#   CPUS=0-63 ./run.sh          # pin to these CPUs (taskset), e.g. one per core
set -euo pipefail
cd "$(dirname "$0")"
HOURS=${HOURS:-8}
WORKERS=${WORKERS:-64}
PIN=${CPUS:+taskset -c $CPUS}
OUT=results/$(hostname -s)
mkdir -p data "$OUT"
for h in $(seq 12 $((11 + HOURS))); do
  f=data/2024-03-13-$h.json.gz
  [ -s "$f" ] || curl -sSfL -o "$f" "https://data.gharchive.org/2024-03-13-$h.json.gz"
done
[ -d venv-gil ] || { uv venv -q -p 3.14 venv-gil && uv pip install -q -p venv-gil -r requirements-gil.txt; }
[ -d venv-ft ] || { uv venv -q -p 3.14t venv-ft && uv pip install -q -p venv-ft -r requirements-ft.txt; }
(lscpu 2>/dev/null || sysctl -n machdep.cpu.brand_string) > "$OUT/cpu.txt"
$PIN venv-gil/bin/python bench.py --hours "$HOURS" --workers "$WORKERS" --rounds 7 | tee "$OUT/gil.csv"
$PIN venv-ft/bin/python bench.py --hours "$HOURS" --workers "$WORKERS" --rounds 7 | tee "$OUT/ft.csv"
venv-gil/bin/python plot.py "$OUT"
