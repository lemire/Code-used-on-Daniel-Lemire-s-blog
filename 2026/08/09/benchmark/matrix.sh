#!/bin/bash
# Train on each document in turn (and on all three merged); measure all three.
set -e
cd "$(dirname "$0")"
DATA=${DATA:-$HOME/nodecomp/bench/data}
CPU=${CPU:-0}
DOCS="twitter.json canada.json citm_catalog.json"

go build -o /tmp/gj_base .
echo "=== baseline (no PGO)   binary $(stat -c %s /tmp/gj_base) bytes ==="
taskset -c $CPU /tmp/gj_base -dir $DATA -bench

for t in $DOCS; do
  taskset -c $CPU /tmp/gj_base -dir $DATA -profile /tmp/p_$t.pprof -train $t 2>/dev/null
  go build -pgo=/tmp/p_$t.pprof -o /tmp/gj_$t .
  echo "=== PGO trained on $t   binary $(stat -c %s /tmp/gj_$t) bytes ==="
  taskset -c $CPU /tmp/gj_$t -dir $DATA -bench
done

go tool pprof -proto /tmp/p_twitter.json.pprof /tmp/p_canada.json.pprof \
  /tmp/p_citm_catalog.json.pprof > /tmp/p_all.pprof 2>/dev/null
go build -pgo=/tmp/p_all.pprof -o /tmp/gj_all .
echo "=== PGO trained on all three   binary $(stat -c %s /tmp/gj_all) bytes ==="
taskset -c $CPU /tmp/gj_all -dir $DATA -bench
