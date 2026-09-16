# std::flat_map vs std::map / std::unordered_map

Comparing C++23's `std::flat_map` against `std::map` (a red-black tree) and
`std::unordered_map` (a hash table) on building, lookup, iteration, erasure
and memory usage, with random 64-bit keys and 64-bit values.

You need a standard library that ships `<flat_map>`: libstdc++ from GCC 15 or
better, or libc++ from LLVM 20 or better.

Under Linux and macOS, you may run:

```
cmake -B build
cmake --build build
./build/benchmark
```

To use a specific compiler:

```
CXX=g++-15 cmake -B build
```

The binary accepts optional element counts; without one it sweeps
1,000 / 100,000 / 1,000,000 / 10,000,000. The word `mem` restricts the run to
the memory-usage report.

```
./build/benchmark 1000000
./build/benchmark mem 1000000 10000000
```

One-at-a-time insertion and erasure into a flat map is quadratic; those
cases are skipped above 100,000 elements.

Memory usage is measured by replacing the global `operator new`/`delete` and
asking the allocator for the size of each block (`malloc_usable_size` plus the
chunk header with glibc, `malloc_size` on macOS). Large blocks are charged
their requested size because macOS may hand out a recycled block that is
larger than requested.

Results in `../results_big4_gcc16.txt` and `../results_big4_clang21.txt` were
collected with GCC 16.1.0 (libstdc++) and clang 21.1 (with the libstdc++ from
gcc-toolset-15) on an Intel Xeon Gold 6548N, pinned to one core with
`taskset -c 8`. The `../results_mac_m4max_*.txt` files were collected on an
Apple M4 Max with Homebrew GCC 15.2 (libstdc++) and Homebrew clang 22.1
(libc++); hardware counters are only available to root on macOS, so those
files report wall-clock time only.
