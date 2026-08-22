#!/bin/sh
# Usage: ./run.sh [haystack_length]      (default 1048576)
set -e
N=${1:-1048576}
HERE=$(cd "$(dirname "$0")" && pwd)
javac -d "$HERE" "$HERE/Adversarial.java" "$HERE/TwoWay.java" "$HERE/Benchmark.java"
java -cp "$HERE" Benchmark "$N"
