#!/bin/bash
# Usage: ./run.sh path/to/python3.15  (run from this directory)
PY=${1:-.venv/bin/python}
$PY -VV
echo "## whole-process timings"
taskset -c 8 $PY bench.py $PY
echo "## modules loaded"
$PY modules.py $PY
echo "## access overhead"
for i in 1 2 3; do
  taskset -c 8 $PY access.py eager
  taskset -c 8 $PY access.py lazy
done
echo "## real use"
taskset -c 8 $PY realuse.py $PY
