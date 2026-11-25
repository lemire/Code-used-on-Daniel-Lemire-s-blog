#!/bin/bash
# Times jq and sjq (-P, and the default: one thread) with hyperfine.
# Usage: run_bench.sh DATA_DIR RESULTS_DIR JQ SJQ HYPERFINE
#
# Environment:
#   PIN_DOCUMENT  a command prefix for the runs on one thread (jq, sjq
#                 without -P, and sjq -P on the single document), for example
#                 "taskset -c 1-127:2": on a machine with several sockets, the
#                 time of a one-thread run depends on the socket it lands on
#                 (up to 25%), so they all run on the same one.
#   ONLY          workload numbers to run, for example "07 08 09" (the others
#                 keep their results; index.tsv is rewritten in full).
#   SKIP_JQ       1: time sjq only, and keep jq's results (after a change to
#                 sjq, with the same jq, data and machine).
set -e
DATA=$1; OUT=$2; JQ=$3; SJQ=$4; HF=$5
mkdir -p "$OUT"
n=0
bench() {
  local label=$1 file=$2; shift 2
  n=$((n + 1))
  local id=$(printf %02d $n)
  printf '%s\t%s\t%s\n' "$id" "$label" "$file" >> "$OUT/index.tsv.new"
  if [ -n "${ONLY:-}" ] && ! echo " $ONLY " | grep -q " $id "; then return; fi
  local args=""
  for a in "$@"; do args="$args '$a'"; done
  local pin="${PIN_DOCUMENT:-}" pin_sjq=""
  if [ "$file" = timeline.json ]; then pin_sjq="$pin"; fi
  if [ "${SKIP_JQ:-}" != 1 ]; then
    "$HF" --shell=default --style basic --warmup 1 --runs 3 --export-json "$OUT/$id-jq.json" -n "jq" "$pin $JQ $args $DATA/$file > /dev/null"
  fi
  "$HF" --shell=default --style basic --warmup 1 --runs 10 --export-json "$OUT/$id-sjq.json" -n "sjq -P" "$pin_sjq $SJQ -P $args $DATA/$file > /dev/null"
  "$HF" --shell=default --style basic --warmup 1 --runs 10 --export-json "$OUT/$id-sjq1.json" -n "sjq" "$pin $SJQ $args $DATA/$file > /dev/null"
}
rm -f "$OUT/index.tsv.new"
bench "minify (-c .)" tweets.ndjson -c .
bench "pretty-print (.)" tweets.ndjson .
bench "extract a field" tweets.ndjson -r .user.screen_name
bench "select and build objects" tweets.ndjson -c 'select(.retweet_count > 100) | {id, user: .user.screen_name}'
bench "string interpolation" tweets.ndjson -r '"\(.id)\t\(.user.screen_name)"'
bench "regular expression" tweets.ndjson -r '.text | ascii_downcase | select(test("rt"))'
bench "count elements" timeline.json '.statuses | length'
bench "minify (-c .)" timeline.json -c .
bench "group_by" timeline.json -c '.statuses | group_by(.user.lang) | map({lang: .[0].user.lang, n: length})'
mv "$OUT/index.tsv.new" "$OUT/index.tsv"
