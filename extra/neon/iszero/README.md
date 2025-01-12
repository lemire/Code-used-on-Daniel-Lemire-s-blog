We are assuming an aarch64 system.

Usage:
```
cmake -B build
cmake --build build
./build/benchmark
```

Potential result:
```
sudo ./build/benchmark
Password:
loaded db: a15 (Apple A15)
# check: 58823 58823 58823 58823
Trial 1
veq_non_zero_max                         :   4.92 GB/s  307.7 Ma/s   3.25 ns/d   3.52 GHz  11.45 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.60 GB/s  287.7 Ma/s   3.48 ns/d   3.52 GHz  12.24 c/d  11.24 i/d   0.77 c/b   0.70 i/b   0.92 i/c 
veq_non_zero_narrow                      :   4.90 GB/s  306.1 Ma/s   3.27 ns/d   3.54 GHz  11.56 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 2
veq_non_zero_max                         :   4.89 GB/s  305.6 Ma/s   3.27 ns/d   3.52 GHz  11.53 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_mov                         :   6.48 GB/s  405.2 Ma/s   2.47 ns/d   3.23 GHz   7.98 c/d  11.27 i/d   0.50 c/b   0.70 i/b   1.41 i/c 
veq_non_zero_narrow                      :   4.88 GB/s  305.2 Ma/s   3.28 ns/d   3.53 GHz  11.55 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 3
veq_non_zero_max                         :   5.55 GB/s  347.1 Ma/s   2.88 ns/d   3.38 GHz   9.73 c/d  11.24 i/d   0.61 c/b   0.70 i/b   1.15 i/c 
veq_non_zero_mov                         :   4.88 GB/s  304.8 Ma/s   3.28 ns/d   3.53 GHz  11.58 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_narrow                      :   7.71 GB/s  481.7 Ma/s   2.08 ns/d   3.45 GHz   7.15 c/d  11.24 i/d   0.45 c/b   0.70 i/b   1.57 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.56 GHz  15.25 c/d  10.24 i/d   0.95 c/b   0.64 i/b   0.67 i/c 
branchy
Trial 1
veq_non_zero_max                         :  23.53 GB/s  1470.6 Ma/s   0.68 ns/d   3.59 GHz   2.44 c/d  17.24 i/d   0.15 c/b   1.08 i/b   7.06 i/c 
veq_non_zero_mov                         :  22.32 GB/s  1395.0 Ma/s   0.72 ns/d   3.60 GHz   2.58 c/d  16.18 i/d   0.16 c/b   1.01 i/b   6.27 i/c 
veq_non_zero_narrow                      :  22.30 GB/s  1393.6 Ma/s   0.72 ns/d   3.59 GHz   2.58 c/d  16.18 i/d   0.16 c/b   1.01 i/b   6.27 i/c 
veq_non_zero_float                       :  26.30 GB/s  1643.5 Ma/s   0.61 ns/d   3.61 GHz   2.20 c/d  15.11 i/d   0.14 c/b   0.94 i/b   6.88 i/c 
Trial 2
veq_non_zero_max                         :  23.53 GB/s  1470.6 Ma/s   0.68 ns/d   3.60 GHz   2.45 c/d  17.24 i/d   0.15 c/b   1.08 i/b   7.03 i/c 
veq_non_zero_mov                         :  22.30 GB/s  1393.6 Ma/s   0.72 ns/d   3.60 GHz   2.58 c/d  16.18 i/d   0.16 c/b   1.01 i/b   6.27 i/c 
veq_non_zero_narrow                      :  22.30 GB/s  1393.6 Ma/s   0.72 ns/d   3.60 GHz   2.58 c/d  16.18 i/d   0.16 c/b   1.01 i/b   6.27 i/c 
veq_non_zero_float                       :  26.30 GB/s  1643.5 Ma/s   0.61 ns/d   3.62 GHz   2.20 c/d  15.11 i/d   0.14 c/b   0.94 i/b   6.87 i/c 
Trial 3
veq_non_zero_max                         :  23.55 GB/s  1472.1 Ma/s   0.68 ns/d   3.60 GHz   2.45 c/d  17.24 i/d   0.15 c/b   1.08 i/b   7.05 i/c 
veq_non_zero_mov                         :  22.30 GB/s  1393.6 Ma/s   0.72 ns/d   3.59 GHz   2.58 c/d  16.18 i/d   0.16 c/b   1.01 i/b   6.28 i/c 
veq_non_zero_narrow                      :  22.30 GB/s  1393.6 Ma/s   0.72 ns/d   3.59 GHz   2.58 c/d  16.18 i/d   0.16 c/b   1.01 i/b   6.27 i/c 
veq_non_zero_float                       :  26.27 GB/s  1641.6 Ma/s   0.61 ns/d   3.61 GHz   2.20 c/d  15.11 i/d   0.14 c/b   0.94 i/b   6.87 i/c 
```
