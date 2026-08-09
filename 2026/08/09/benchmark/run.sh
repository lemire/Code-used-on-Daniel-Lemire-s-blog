#!/bin/bash
# Baseline vs PGO, training on ONE document and measuring all three.
set -e
cd "$(dirname "$0")"
DATA=${DATA:-$HOME/nodecomp/bench/data}
TRAIN=${TRAIN:-twitter.json}
CPU=${CPU:-0}

rm -f default.pgo
echo "building baseline (no profile)"; go build -o /tmp/gj_base .
echo "collecting a CPU profile while parsing $TRAIN"
taskset -c $CPU /tmp/gj_base -dir $DATA -profile /tmp/cpu.pprof -train $TRAIN
echo "rebuilding with -pgo"; go build -pgo=/tmp/cpu.pprof -o /tmp/gj_pgo .

echo
echo "trained on: $TRAIN"
echo "--- baseline ---"
taskset -c $CPU /tmp/gj_base -dir $DATA -bench
echo "--- PGO ---"
taskset -c $CPU /tmp/gj_pgo -dir $DATA -bench
