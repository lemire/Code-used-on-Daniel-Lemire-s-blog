# Signed 32-bit division on 64-bit systems

`include/div32.h` provides `div32<d>(x)`, which computes `x / d` for a signed
32-bit `x` and a constant divisor `d > 1` that is not a power of two.

Under Linux and macOS, you may run:

```
cmake -B build
cmake --build build
./build/tests
./build/benchmark
```

The benchmark also compares the division instruction with
[fastmod](https://github.com/lemire/fastmod) for a divisor given at runtime
(`./build/benchmark 7`).

To use a specific compiler:

```
CXX=clang++ cmake -B build
```

The `asm` directory has examples of the generated assembly.
