# Taken branches per cycle

Tight loops written in inline assembly (ARM64 and x64) to measure how the
rate of taken branches limits a loop: a sum of 64-bit integers with the loop
unrolled 1x, 2x, 4x, 8x and 16x, an empty counting loop, and counting loops
padded with unconditional jumps (dense: a jump every few bytes; sparse: one
jump per 64-byte line).

Hardware counters come from the [counters](https://github.com/lemire/counters)
library: they work on Linux without special privileges (given perf access),
and on macOS only as root.

```
cmake -B build
cmake --build build
./build/benchmark          # Linux
sudo ./build/benchmark     # macOS, to get cycle counts
```

An optional argument sets the number of 64-bit words to sum (a positive
multiple of 16; default 2048, i.e. 16 kB).

The `experiments/` directory holds one-off experiments: `spacing_x64.cpp` and
`spacing_arm64.cpp` vary the distance between consecutive taken branches;
`loop_shapes_x64.cpp` and `alu_ops_x64.cpp` vary the number and kind of
instructions in a tiny loop (used to understand the Ice Lake results);
`if_loop.cpp` is a C++ loop with an if: two taken branches per iteration
when the if is always a miss. `ifloop/` is the same idea in Go (`go run .`
from that directory). The C++ experiments need the `counters` headers on
the include path, e.g.
`clang++ -O2 -std=c++20 -I build/_deps/counters-src/include experiments/spacing_arm64.cpp`.
