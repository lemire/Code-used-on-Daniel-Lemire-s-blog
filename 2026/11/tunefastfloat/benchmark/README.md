# fast_float performance hints benchmark

`bench.cpp` generates four data sets (price changes, account balances, sensor
readings, regular prices) and parses them with `fast_float::from_chars`,
without hints and with combinations of `fast_float::performance_hints`. Each
parsing loop is compiled from `kernel.cpp` in its own translation unit.
The sums are checked: every configuration must give the same result.

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/benchmark
```

On Linux, pin one core and the
[lemire/counters](https://github.com/lemire/counters) library also reports
cycles, instructions and branch misses per number:

```sh
taskset -c 8 ./build/benchmark
```

The performance hints are in a fast_float
[pull request](https://github.com/fastfloat/fast_float/pull/434): CMake fetches
its branch, `performance-hints`.

`plot.py` (one directory up) draws the charts from the results files.
