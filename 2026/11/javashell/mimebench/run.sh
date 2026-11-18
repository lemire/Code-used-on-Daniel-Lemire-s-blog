#!/bin/bash
# MIME base64 decoding benchmark (big4). Run from this directory:
#   PIN="taskset -c 8" ./run.sh > results_big4_run1.txt
# rank.py takes the median of several runs.
# Tool paths below are the ones on big4; override them with environment variables.
set -e
T=~/mimebench_tools
: ${JAVA:=$T/jdk27/bin/java} ${JAVAC:=$T/jdk27/bin/javac}
: ${NODE:=~/.nvm/versions/node/v26.13.1/bin/node} ${BUN:=~/.bun/bin/bun} ${DENO:=~/.deno/bin/deno}
: ${PHP:=$T/php85/bin/php} ${PHPPR:=$T/phppr/install/bin/php}
: ${DOTNET:=$T/dotnet11/dotnet} ${PYTHON:=build/venv/bin/python}
: ${GO:=~/opt/go/bin/go} ${ZIG:=~/opt/zig-x86_64-linux-0.17.1/zig}
: ${CXX:=g++} ${PIN:=}
export DOTNET_ROOT=$(dirname $DOTNET) DOTNET_CLI_TELEMETRY_OPTOUT=1 DOTNET_NOLOGO=1
mkdir -p build
[ -f mime.txt ] || python3 gen.py
$JAVAC -d build Bench.java
$CXX -O3 -std=c++17 -Isimdutf bench.cpp simdutf/simdutf.cpp -o build/bench_cpp
$DOTNET build -c Release dotnet -o build/dotnet > /dev/null
(cd go && $GO build -o ../build/gobench .)
(cd zig && $ZIG build-exe -O ReleaseFast -lc main.zig -femit-bin=../build/zigbench)
uname -srm; grep -m1 "model name" /proc/cpuinfo; $CXX --version | head -1; head -1 expected.txt
$PIN ./build/bench_cpp
$PIN build/gobench
$PIN build/zigbench 2>&1
$PIN $JAVA -cp build Bench
$PIN $NODE bench.mjs
$PIN $BUN bench.mjs
$PIN $DENO run -A bench.mjs
$PIN $PHP bench.php
PHPLABEL=" (PR #25476)" $PIN $PHPPR bench.php
$PIN $DOTNET build/dotnet/dotnet.dll
FASTPYLABEL=" (PR #5)" $PIN $PYTHON bench.py
