We are assuming an aarch64 system.

Usage:
```
cmake -B build
cmake --build build
./build/benchmark
```

Potential result:
```

Trial 1
veq_non_zero_max                         :   4.86 GB/s  303.5 Ma/s   3.29 ns/d   3.53 GHz  11.63 c/d  12.27 i/d   0.73 c/b   0.77 i/b   1.06 i/c 
veq_non_zero_mov                         :   4.83 GB/s  302.0 Ma/s   3.31 ns/d   3.53 GHz  11.68 c/d  12.27 i/d   0.73 c/b   0.77 i/b   1.05 i/c 
veq_non_zero_narrow                      :   4.89 GB/s  305.6 Ma/s   3.27 ns/d   3.53 GHz  11.55 c/d  12.27 i/d   0.72 c/b   0.77 i/b   1.06 i/c 
veq_non_zero_float                       :   3.80 GB/s  237.3 Ma/s   4.21 ns/d   3.52 GHz  14.84 c/d  11.27 i/d   0.93 c/b   0.70 i/b   0.76 i/c 
Trial 2
veq_non_zero_max                         :   4.89 GB/s  305.6 Ma/s   3.27 ns/d   3.53 GHz  11.55 c/d  12.27 i/d   0.72 c/b   0.77 i/b   1.06 i/c 
veq_non_zero_mov                         :   4.88 GB/s  304.7 Ma/s   3.28 ns/d   3.54 GHz  11.60 c/d  12.27 i/d   0.73 c/b   0.77 i/b   1.06 i/c 
veq_non_zero_narrow                      :   4.86 GB/s  304.0 Ma/s   3.29 ns/d   3.53 GHz  11.61 c/d  12.27 i/d   0.73 c/b   0.77 i/b   1.06 i/c 
veq_non_zero_float                       :   3.79 GB/s  237.0 Ma/s   4.22 ns/d   3.53 GHz  14.88 c/d  11.27 i/d   0.93 c/b   0.70 i/b   0.76 i/c 
Trial 3
veq_non_zero_max                         :   4.87 GB/s  304.2 Ma/s   3.29 ns/d   3.54 GHz  11.63 c/d  12.27 i/d   0.73 c/b   0.77 i/b   1.05 i/c 
veq_non_zero_mov                         :   4.91 GB/s  307.0 Ma/s   3.26 ns/d   3.52 GHz  11.48 c/d  12.27 i/d   0.72 c/b   0.77 i/b   1.07 i/c 
veq_non_zero_narrow                      :   4.91 GB/s  306.9 Ma/s   3.26 ns/d   3.54 GHz  11.55 c/d  12.27 i/d   0.72 c/b   0.77 i/b   1.06 i/c 
veq_non_zero_float                       :   3.78 GB/s  236.5 Ma/s   4.23 ns/d   3.52 GHz  14.88 c/d  11.27 i/d   0.93 c/b   0.70 i/b   0.76 i/c 
branchy
Trial 1
veq_non_zero_max                         :  19.97 GB/s  1248.2 Ma/s   0.80 ns/d   3.59 GHz   2.88 c/d  18.08 i/d   0.18 c/b   1.13 i/b   6.28 i/c 
veq_non_zero_mov                         :  19.99 GB/s  1249.4 Ma/s   0.80 ns/d   3.59 GHz   2.88 c/d  18.08 i/d   0.18 c/b   1.13 i/b   6.28 i/c 
veq_non_zero_narrow                      :  19.95 GB/s  1246.9 Ma/s   0.80 ns/d   3.60 GHz   2.88 c/d  18.08 i/d   0.18 c/b   1.13 i/b   6.27 i/c 
veq_non_zero_float                       :  23.50 GB/s  1468.8 Ma/s   0.68 ns/d   3.61 GHz   2.46 c/d  16.89 i/d   0.15 c/b   1.06 i/b   6.86 i/c 
Trial 2
veq_non_zero_max                         :  19.95 GB/s  1246.9 Ma/s   0.80 ns/d   3.60 GHz   2.89 c/d  18.08 i/d   0.18 c/b   1.13 i/b   6.26 i/c 
veq_non_zero_mov                         :  19.97 GB/s  1248.2 Ma/s   0.80 ns/d   3.59 GHz   2.88 c/d  18.08 i/d   0.18 c/b   1.13 i/b   6.28 i/c 
veq_non_zero_narrow                      :  19.97 GB/s  1248.2 Ma/s   0.80 ns/d   3.60 GHz   2.88 c/d  18.08 i/d   0.18 c/b   1.13 i/b   6.27 i/c 
veq_non_zero_float                       :  22.86 GB/s  1428.9 Ma/s   0.70 ns/d   3.52 GHz   2.46 c/d  16.89 i/d   0.15 c/b   1.06 i/b   6.86 i/c 
Trial 3
veq_non_zero_max                         :  19.95 GB/s  1246.9 Ma/s   0.80 ns/d   3.59 GHz   2.88 c/d  18.08 i/d   0.18 c/b   1.13 i/b   6.28 i/c 
veq_non_zero_mov                         :  19.95 GB/s  1246.9 Ma/s   0.80 ns/d   3.59 GHz   2.88 c/d  18.08 i/d   0.18 c/b   1.13 i/b   6.28 i/c 
veq_non_zero_narrow                      :  19.91 GB/s  1244.5 Ma/s   0.80 ns/d   3.60 GHz   2.89 c/d  18.08 i/d   0.18 c/b   1.13 i/b   6.26 i/c 
veq_non_zero_float                       :  22.91 GB/s  1432.1 Ma/s   0.70 ns/d   3.51 GHz   2.45 c/d  16.89 i/d   0.15 c/b   1.06 i/b   6.89 i/c 
branchy zeros
Trial 1
veq_non_zero_max                         :  38.21 GB/s  2387.9 Ma/s   0.42 ns/d   3.68 GHz   1.54 c/d   9.77 i/d   0.10 c/b   0.61 i/b   6.34 i/c 
veq_non_zero_mov                         :  38.20 GB/s  2387.8 Ma/s   0.42 ns/d   3.68 GHz   1.54 c/d   9.77 i/d   0.10 c/b   0.61 i/b   6.35 i/c 
veq_non_zero_narrow                      :  38.21 GB/s  2387.9 Ma/s   0.42 ns/d   3.68 GHz   1.54 c/d   9.77 i/d   0.10 c/b   0.61 i/b   6.34 i/c 
veq_non_zero_float                       :  45.93 GB/s  2870.8 Ma/s   0.35 ns/d   3.63 GHz   1.26 c/d   9.77 i/d   0.08 c/b   0.61 i/b   7.73 i/c 
Trial 2
veq_non_zero_max                         :  37.22 GB/s  2326.2 Ma/s   0.43 ns/d   3.57 GHz   1.53 c/d   9.77 i/d   0.10 c/b   0.61 i/b   6.37 i/c 
veq_non_zero_mov                         :  37.92 GB/s  2369.9 Ma/s   0.42 ns/d   3.67 GHz   1.55 c/d   9.77 i/d   0.10 c/b   0.61 i/b   6.31 i/c 
veq_non_zero_narrow                      :  38.13 GB/s  2383.3 Ma/s   0.42 ns/d   3.68 GHz   1.54 c/d   9.77 i/d   0.10 c/b   0.61 i/b   6.33 i/c 
veq_non_zero_float                       :  45.93 GB/s  2870.8 Ma/s   0.35 ns/d   3.62 GHz   1.26 c/d   9.77 i/d   0.08 c/b   0.61 i/b   7.74 i/c 
Trial 3
veq_non_zero_max                         :  38.06 GB/s  2378.8 Ma/s   0.42 ns/d   3.74 GHz   1.57 c/d   9.77 i/d   0.10 c/b   0.61 i/b   6.21 i/c 
veq_non_zero_mov                         :  37.15 GB/s  2322.0 Ma/s   0.43 ns/d   3.64 GHz   1.57 c/d   9.77 i/d   0.10 c/b   0.61 i/b   6.23 i/c 
veq_non_zero_narrow                      :  37.22 GB/s  2326.2 Ma/s   0.43 ns/d   3.58 GHz   1.54 c/d   9.77 i/d   0.10 c/b   0.61 i/b   6.35 i/c 
veq_non_zero_float                       :  45.93 GB/s  2870.8 Ma/s   0.35 ns/d   3.62 GHz   1.26 c/d   9.77 i/d   0.08 c/b   0.61 i/b   7.74 i/c 
```
