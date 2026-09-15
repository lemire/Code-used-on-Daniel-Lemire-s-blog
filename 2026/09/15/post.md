# Subnormal floating-point numbers are expensive... on Intel processors

We represent floating-point numbers using the IEEE standard. For very small numbers, the standard uses special *subnormal* numbers. Unfortunately, they have
a reputation of making operations slow. Thus video game programmers and machine learning specialists sometimes avoid computing with subnormal numbers for performance.

How slow are they? Let me measure. I wrote a small C++ benchmark with a few kernels
over arrays of 16384 values (small enough to fit in cache):

- multiply each value by 0.75,
- add two arrays,
- divide each value by 3,
- multiply normal values by a tiny constant (2<sup>-1030</sup>) so that the
  *inputs* are normal but the *outputs* are subnormal,
- a dependent chain `x *= 0.9999` repeated 16384 times.

For each kernel, I feed either normal values (in `[0.5, 1)`), subnormal values,
or normal values where one value in a hundred is subnormal. The compiler is allowed to autovectorize the array computations. 
I use GCC 15 with `-O3 -march=native` on Linux and Apple
clang 17 with the same flags on macOS. I also checked with clang 21 on Linux to make sure.

I ran the benchmark on five processors:

- Intel Xeon 6975P-C (Granite Rapids), on an AWS `c8i.xlarge` instance,
- Intel Xeon Gold 6548N (Emerald Rapids), a server in my lab,
- AMD EPYC 9R45 (Zen 5), on an AWS `c8a.xlarge` instance,
- AWS Graviton 5 (Arm Neoverse V3), on a `c9g.xlarge` instance,
- Apple M4 Max.

Here are the results for `double` values, in nanoseconds per element (or per
step). 

*Intel Granite Rapids*

| kernel                     | normal | 1% subnormal | subnormal |
| -------------------------- | -----: | -----------: | --------: |
| multiply by 0.75           |   0.17 |         0.44 |      8.35 |
| add two arrays             |   0.20 |         0.18 |      0.18 |
| divide by 3                |   0.51 |         0.85 |      9.38 |
| normal in, subnormal out   |   0.17 |              |      8.58 |
| dependent chain            |   0.77 |              |     32.69 |

*Intel Emerald Rapids*

| kernel                     | normal | 1% subnormal | subnormal |
| -------------------------- | -----: | -----------: | --------: |
| multiply by 0.75           |   0.21 |         0.49 |      9.25 |
| add two arrays             |   0.23 |         0.25 |      0.25 |
| divide by 3                |   0.57 |         0.94 |     10.40 |
| normal in, subnormal out   |   0.21 |              |      9.27 |
| dependent chain            |   1.14 |              |     36.51 |

*AMD Zen 5*

| kernel                     | normal | 1% subnormal | subnormal |
| -------------------------- | -----: | -----------: | --------: |
| multiply by 0.75           |   0.07 |         0.10 |      0.08 |
| add two arrays             |   0.09 |         0.09 |      0.09 |
| divide by 3                |   0.11 |         0.24 |      0.25 |
| normal in, subnormal out   |   0.07 |              |      0.07 |
| dependent chain            |   0.66 |              |      0.88 |

*AWS Graviton 5*

| kernel                     | normal | 1% subnormal | subnormal |
| -------------------------- | -----: | -----------: | --------: |
| multiply by 0.75           |   0.17 |         0.17 |      0.16 |
| add two arrays             |   0.19 |         0.20 |      0.20 |
| divide by 3                |   0.30 |         0.30 |      0.30 |
| normal in, subnormal out   |   0.17 |              |      0.17 |
| dependent chain            |   0.91 |              |      0.91 |

*Apple M4 Max*

| kernel                     | normal | 1% subnormal | subnormal |
| -------------------------- | -----: | -----------: | --------: |
| multiply by 0.75           |   0.06 |         0.06 |      0.06 |
| add two arrays             |   0.12 |         0.12 |      0.12 |
| divide by 3                |   0.11 |         0.11 |      0.11 |
| normal in, subnormal out   |   0.06 |              |      0.06 |
| dependent chain            |   0.72 |              |      0.75 |

On Intel processors, a multiplication involving a subnormal number is about 45
to 50 times slower than a multiplication over normal numbers. A division is 18
times slower. The dependent chain, where each multiplication waits for the
previous one, goes from about 1 ns to over 30 ns per step.
A normal multiplication in the dependent chain has
a latency of 4 cycles. With a subnormal, it has a latency of 128 cycles.
It does not matter whether the subnormal is an input or an output: multiplying
normal numbers into a subnormal result is just as slow as multiplying subnormal
numbers.
The exception is additions and subtractions: they run at full speed.
Even if subnormals are rare (1%), the cost on Intel can be significant because when the compiler vectorizes the computation, a single subnormal can slow down a whole block of computations.

AMD does much better. On Zen 5, multiplications and additions run at full
speed regardless of the inputs. The dependent multiplication chain is a third
slower (0.66 ns to 0.88 ns per step): the multiplier needs an extra cycle or so
to handle a subnormal. Divisions are about twice as slow. Interestingly, with
divisions, having one subnormal in a hundred is almost as slow as having all
subnormals.
The two Arm processors, the Graviton 5 and the Apple M4 Max, do not care at
all. Subnormal numbers are handled at full speed. 

Thus it appears that on the latest AMD and ARM processors subnormals might not be a concern. But they remain very much a performance issue under Intel processors.

[My source code is available](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/09/15).
