#!/bin/sh
# Runs all four adversarial substring-search benchmarks.
# Usage: ./run_all.sh [haystack_length]      (default 1048576)
set -e
N=${1:-1048576}
HERE=$(cd "$(dirname "$0")" && pwd)

echo "=============================================================="
echo " 1. macOS C library (strstr / memmem)"
echo "=============================================================="
cc -O2 -fno-builtin -o "$HERE/c/adversarial" "$HERE/c/adversarial.c"
"$HERE/c/adversarial" "$N"

echo
echo "=============================================================="
echo " 2. Java (HotSpot String.indexOf)"
echo "=============================================================="
javac -d "$HERE/java" "$HERE/java/Adversarial.java"
java -cp "$HERE/java" Adversarial "$N"

echo
echo "--- same benchmark with the indexOf intrinsics disabled -------"
java -XX:+UnlockDiagnosticVMOptions \
     -XX:DisableIntrinsic=_indexOfL,_indexOfU,_indexOfUL,_indexOfIL,_indexOfIU,_indexOfIUL,_indexOfL_char,_indexOfU_char \
     -cp "$HERE/java" Adversarial "$N"

echo
echo "=============================================================="
echo " 3. Node.js / V8 (String.prototype.indexOf)"
echo "=============================================================="
node "$HERE/node/adversarial.mjs" "$N"

echo
echo "=============================================================="
echo " 4. .NET (string.IndexOf, Ordinal)"
echo "=============================================================="
dotnet run -c Release --project "$HERE/dotnet/Adversarial" -- "$N"
