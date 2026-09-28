#!/bin/bash
set -x
HERE="$(cd "$(dirname "$0")" && pwd)"
source ~/usegcc16.sh # GCC 16.1
mkdir -p ~/simdjson5bench && cd ~/simdjson5bench
[ -d v4 ] || git clone -q https://github.com/simdjson/simdjson.git v4
[ -d v5 ] || git clone -q https://github.com/simdjson/simdjson.git v5
(cd v4 && git checkout -q v4.0.0 && git log -1 --oneline)
(cd v5 && git fetch -q origin v5_candidate && git checkout -q FETCH_HEAD && git log -1 --oneline)
for v in v4 v5; do
  (cd $v && cmake -B build -DCMAKE_BUILD_TYPE=Release -DSIMDJSON_DEVELOPER_MODE=ON -DSIMDJSON_COMPETITION=OFF -DBUILD_SHARED_LIBS=OFF > cmake.log 2>&1; \
   cmake --build build -j 64 --target bench_ondemand parse > build.log 2>&1; echo "$v build exit $?")
done
for v in v4 v5; do
  g++ -O3 -DNDEBUG -std=c++20 -I $v/singleheader "$HERE"/serialize.cpp $v/singleheader/simdjson.cpp -o serialize_$v
done
ls -la v4/build/benchmark/bench_ondemand v5/build/benchmark/bench_ondemand v4/build/benchmark/dom/parse v5/build/benchmark/dom/parse
echo SETUP_DONE
