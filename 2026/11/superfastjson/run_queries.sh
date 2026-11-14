#!/bin/bash
# The six queries: orjson and msgspec on processes (Python with the GIL),
# fastsimdjson on threads (free-threaded Python).
# Usage: bash run_queries.sh DATADIR [worker counts...]
D=${1:?data directory}; shift
W=${@:-1 16 32 64 96}
G=venv-gil/bin/python; T=venv-nogil/bin/python
echo "# $(lscpu | grep 'Model name' | sed 's/ \+/ /g'), $(nproc) logical processors"
for q in twitter bestbuy google_map nspl walmart wiki; do
  for w in $W; do
    export RUNS=$([ $w = 1 ] && echo 2 || echo 5)
    $G queries.py $D $q orjson $w
    $G queries.py $D $q msgspec $w
    $T queries.py $D $q fastsimdjson $w
  done
done
