#!/bin/bash
# Builds and runs every benchmark. Usage: ./run_all.sh [pin-prefix, e.g. "taskset -c 8"]
set -e
cd "$(dirname "$0")"
PIN="$@"
CXX=${CXX:-clang++}
$CXX -O3 -std=c++20 -o bench_cpp bench.cpp
(cd rust && cargo build --release -q)
go build -o bench_go bench.go
nim c -d:danger --hints:off -o:bench_nim bench.nim > /dev/null
$PIN ${PYTHON:-python3} bench.py
$PIN node bench.js
$PIN bun bench.js
$PIN ./bench_cpp
$PIN ./rust/target/release/strbench
$PIN ./bench_go
$PIN ./bench_nim
