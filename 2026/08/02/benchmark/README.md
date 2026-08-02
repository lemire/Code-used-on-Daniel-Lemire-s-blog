Comparing `std::vector`, `std::list` and `std::hive` on appending, iterating
and erasing.

No standard library ships `std::hive` yet (GCC 16 has no `<hive>` header), so we
use [plf::hive](https://github.com/mattreecebentley/plf_hive), the reference
implementation the proposal is based on. CMake fetches it for you.

Under Linux and macOS, you may run:

```
cmake -B build
cmake --build build
./build/benchmark          # baseline instruction set
./build/benchmark_native   # built with -march=native
```

Both binaries accept an optional element count; without one they sweep
100,000 / 1,000,000 / 10,000,000.

```
./build/benchmark_native 1000000
```

Results in `../results.txt` were collected with GCC 16.1.0 on an Intel Xeon
Gold 6548N, pinned to one core with `taskset -c 8`. The main tables are from
`./build/benchmark_native`. Appendix material in that file (smaller hive block
limits; `MALLOC_MMAP_THRESHOLD_` raised) was measured with one-off runs on the
same machine, not by this binary alone.
