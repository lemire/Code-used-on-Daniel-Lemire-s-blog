# Store-forwarding benchmark

`examples.c` (one directory up) is the small C. The timed loops are in
`forward.c`. `bench.cpp` checks those loops and counts cycles with
[lemire/counters](https://github.com/lemire/counters).

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/benchmark
```

On the Intel machine, pin one core:

```sh
taskset -c 8 ./build/benchmark
```

Vectorization is off, so the register version of a loop is not also an
autovectorized loop. The latency tests are a dependent chain: each loaded
value is the next iteration's input.
