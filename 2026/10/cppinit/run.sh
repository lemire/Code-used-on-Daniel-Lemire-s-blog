# Builds and runs the benchmark with span() = default and with span() {}.
# usage: run.sh [compiler]   (default: g++)
CXX=${1:-g++}
$CXX --version | head -1
$CXX -O2 -march=x86-64 init.cpp consume.cpp benchmark.cpp -o bench_default
$CXX -O2 -march=x86-64 -DBRACES init.cpp consume.cpp benchmark.cpp -o bench_braces
echo "span() = default: $(./bench_default)"
echo "span() {}       : $(./bench_braces)"
