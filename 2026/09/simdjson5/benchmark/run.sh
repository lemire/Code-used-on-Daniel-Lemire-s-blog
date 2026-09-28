#!/bin/bash
source ~/usegcc16.sh >/dev/null
cd ~/simdjson5bench
D=v5/build/_deps/simdjson-data-src/jsonexamples
FILES="$D/twitter.json $D/citm_catalog.json $D/canada.json $D/github_events.json $D/gsoc-2018.json $D/marine_ik.json $D/mesh.json $D/numbers.json $D/random.json $D/twitterescaped.json $D/update-center.json $D/instruments.json $D/apache_builds.json"
mkdir -p results
for r in 1 2 3; do
  for v in v4 v5; do
    echo "round $r $v"
    taskset -c 2 ./$v/build/benchmark/dom/parse -t -n 200 $FILES > results/parse_${v}_r$r.tsv 2>&1
    taskset -c 2 ./serialize_$v $FILES > results/serialize_${v}_r$r.txt 2>&1
    (cd $v/build/benchmark && taskset -c 2 ./bench_ondemand --benchmark_filter=-\<THREADED\|accessor --benchmark_repetitions=5 \
       --benchmark_out=$HOME/simdjson5bench/results/ondemand_${v}_r$r.json --benchmark_out_format=json > $HOME/simdjson5bench/results/ondemand_${v}_r$r.txt 2>&1)
  done
done
echo RUN_DONE
