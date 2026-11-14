#!/bin/bash
# The sweep: one line per library and number of workers.
# Usage: bash run.sh [file] [worker counts...]
F=${1:-/dev/shm/events.ndjson}; shift
W=${@:-1 8 16 32 64 128}
G=venv-gil/bin/python; T=venv-nogil/bin/python
echo "# $(lscpu | grep 'Model name' | sed 's/ \+/ /g'), $(nproc) logical processors"
echo "# $(ls -l $(readlink -f $F) | awk '{print $5}') bytes in $F"
for w in $W; do
  export RUNS=$([ $w = 1 ] && echo 1 || echo 5)
  $G bench.py $F json $w process
  $G bench.py $F orjson $w process
  $G bench.py $F msgspec $w process
  $T bench.py $F msgspec $w thread
  $T bench.py $F fastsimdjson $w
done
