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
loaded db: a15 (Apple A15)
# check: 58823 58823 58823 58823
Trial 1
veq_non_zero_max                         :   4.94 GB/s  308.5 Ma/s   3.24 ns/d   3.52 GHz  11.43 c/d  11.24 i/d   0.71 c/b   0.70 i/b   0.98 i/c
veq_non_zero_mov                         :   4.94 GB/s  308.5 Ma/s   3.24 ns/d   3.53 GHz  11.43 c/d  11.24 i/d   0.71 c/b   0.70 i/b   0.98 i/c
veq_non_zero_narrow                      :   4.93 GB/s  308.0 Ma/s   3.25 ns/d   3.52 GHz  11.44 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c
veq_non_zero_float                       :   3.73 GB/s  233.3 Ma/s   4.29 ns/d   3.57 GHz  15.29 c/d  10.24 i/d   0.96 c/b   0.64 i/b   0.67 i/c
Trial 2
veq_non_zero_max                         :   4.92 GB/s  307.7 Ma/s   3.25 ns/d   3.53 GHz  11.47 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.91 GB/s  307.0 Ma/s   3.26 ns/d   3.52 GHz  11.48 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_narrow                      :   4.91 GB/s  306.8 Ma/s   3.26 ns/d   3.52 GHz  11.49 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 3
veq_non_zero_max                         :   4.92 GB/s  307.7 Ma/s   3.25 ns/d   3.54 GHz  11.49 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.75 GB/s  297.0 Ma/s   3.37 ns/d   3.46 GHz  11.63 c/d  11.24 i/d   0.73 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_narrow                      :   4.90 GB/s  306.4 Ma/s   3.26 ns/d   3.54 GHz  11.54 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 4
veq_non_zero_max                         :   4.88 GB/s  305.0 Ma/s   3.28 ns/d   3.52 GHz  11.55 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_mov                         :   4.90 GB/s  306.4 Ma/s   3.26 ns/d   3.52 GHz  11.49 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_narrow                      :   4.89 GB/s  305.5 Ma/s   3.27 ns/d   3.52 GHz  11.53 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 5
veq_non_zero_max                         :   4.92 GB/s  307.2 Ma/s   3.25 ns/d   3.54 GHz  11.51 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.93 GB/s  308.4 Ma/s   3.24 ns/d   3.52 GHz  11.43 c/d  11.24 i/d   0.71 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_narrow                      :   4.83 GB/s  301.7 Ma/s   3.32 ns/d   3.52 GHz  11.68 c/d  11.24 i/d   0.73 c/b   0.70 i/b   0.96 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 6
veq_non_zero_max                         :   4.91 GB/s  306.9 Ma/s   3.26 ns/d   3.52 GHz  11.48 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.70 GB/s  293.7 Ma/s   3.40 ns/d   3.52 GHz  12.00 c/d  11.24 i/d   0.75 c/b   0.70 i/b   0.94 i/c 
veq_non_zero_narrow                      :   4.83 GB/s  301.9 Ma/s   3.31 ns/d   3.52 GHz  11.67 c/d  11.24 i/d   0.73 c/b   0.70 i/b   0.96 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 7
veq_non_zero_max                         :   4.89 GB/s  305.9 Ma/s   3.27 ns/d   3.53 GHz  11.54 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_mov                         :   4.92 GB/s  307.4 Ma/s   3.25 ns/d   3.52 GHz  11.46 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_narrow                      :   4.95 GB/s  309.3 Ma/s   3.23 ns/d   3.52 GHz  11.39 c/d  11.24 i/d   0.71 c/b   0.70 i/b   0.99 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 8
veq_non_zero_max                         :   4.87 GB/s  304.3 Ma/s   3.29 ns/d   3.53 GHz  11.60 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_mov                         :   4.93 GB/s  308.1 Ma/s   3.25 ns/d   3.54 GHz  11.49 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_narrow                      :   4.78 GB/s  298.6 Ma/s   3.35 ns/d   3.52 GHz  11.80 c/d  11.24 i/d   0.74 c/b   0.70 i/b   0.95 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 9
veq_non_zero_max                         :   4.88 GB/s  305.3 Ma/s   3.28 ns/d   3.52 GHz  11.54 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_mov                         :   4.92 GB/s  307.4 Ma/s   3.25 ns/d   3.52 GHz  11.46 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_narrow                      :   4.75 GB/s  296.9 Ma/s   3.37 ns/d   3.52 GHz  11.87 c/d  11.24 i/d   0.74 c/b   0.70 i/b   0.95 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 10
veq_non_zero_max                         :   4.92 GB/s  307.8 Ma/s   3.25 ns/d   3.53 GHz  11.46 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.96 GB/s  309.8 Ma/s   3.23 ns/d   3.52 GHz  11.38 c/d  11.24 i/d   0.71 c/b   0.70 i/b   0.99 i/c 
veq_non_zero_narrow                      :   4.66 GB/s  291.0 Ma/s   3.44 ns/d   3.52 GHz  12.10 c/d  11.24 i/d   0.76 c/b   0.70 i/b   0.93 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
☁  iszero [master] ⚡  clang-format -i benchmarks/benchmark.cpp 
☁  iszero [master] ⚡  cmake --build build                     
[ 50%] Building CXX object CMakeFiles/benchmark.dir/benchmarks/benchmark.cpp.o
[100%] Linking CXX executable benchmark
[100%] Built target benchmark
☁  iszero [master] ⚡  sudo ./build/benchmark                  
loaded db: a15 (Apple A15)
# check: 58823 58823 58823 58823
Trial 1
veq_non_zero_max                         :   4.86 GB/s  303.9 Ma/s   3.29 ns/d   3.52 GHz  11.59 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_mov                         :   4.79 GB/s  299.4 Ma/s   3.34 ns/d   3.52 GHz  11.76 c/d  11.24 i/d   0.74 c/b   0.70 i/b   0.96 i/c 
veq_non_zero_narrow                      :   4.87 GB/s  304.3 Ma/s   3.29 ns/d   3.52 GHz  11.58 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 2
veq_non_zero_max                         :   7.39 GB/s  462.0 Ma/s   2.16 ns/d   2.83 GHz   6.12 c/d  11.24 i/d   0.38 c/b   0.70 i/b   1.84 i/c 
veq_non_zero_mov                         :   4.91 GB/s  307.2 Ma/s   3.26 ns/d   3.54 GHz  11.51 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_narrow                      :   4.94 GB/s  308.9 Ma/s   3.24 ns/d   3.54 GHz  11.46 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.10 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 3
veq_non_zero_max                         :   4.94 GB/s  308.7 Ma/s   3.24 ns/d   3.52 GHz  11.41 c/d  11.24 i/d   0.71 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.94 GB/s  308.6 Ma/s   3.24 ns/d   3.52 GHz  11.42 c/d  11.24 i/d   0.71 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_narrow                      :   4.92 GB/s  307.5 Ma/s   3.25 ns/d   3.53 GHz  11.49 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.3 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 4
veq_non_zero_max                         :   4.91 GB/s  306.8 Ma/s   3.26 ns/d   3.52 GHz  11.49 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.86 GB/s  303.7 Ma/s   3.29 ns/d   3.54 GHz  11.64 c/d  11.24 i/d   0.73 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_narrow                      :   4.93 GB/s  308.1 Ma/s   3.25 ns/d   3.54 GHz  11.49 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.66 GHz  15.70 c/d  10.24 i/d   0.98 c/b   0.64 i/b   0.65 i/c 
Trial 5
veq_non_zero_max                         :   4.94 GB/s  308.7 Ma/s   3.24 ns/d   3.52 GHz  11.42 c/d  11.24 i/d   0.71 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.88 GB/s  305.2 Ma/s   3.28 ns/d   3.53 GHz  11.58 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_narrow                      :   4.91 GB/s  306.9 Ma/s   3.26 ns/d   3.53 GHz  11.49 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 6
veq_non_zero_max                         :   4.91 GB/s  307.0 Ma/s   3.26 ns/d   3.53 GHz  11.50 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.90 GB/s  306.5 Ma/s   3.26 ns/d   3.53 GHz  11.51 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_narrow                      :   4.90 GB/s  306.1 Ma/s   3.27 ns/d   3.52 GHz  11.52 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 7
veq_non_zero_max                         :   4.70 GB/s  293.6 Ma/s   3.41 ns/d   3.52 GHz  12.01 c/d  11.24 i/d   0.75 c/b   0.70 i/b   0.94 i/c 
veq_non_zero_mov                         :   4.89 GB/s  305.8 Ma/s   3.27 ns/d   3.53 GHz  11.53 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_narrow                      :   4.92 GB/s  307.5 Ma/s   3.25 ns/d   3.53 GHz  11.48 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 8
veq_non_zero_max                         :   4.89 GB/s  305.9 Ma/s   3.27 ns/d   3.52 GHz  11.52 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.93 GB/s  308.4 Ma/s   3.24 ns/d   3.53 GHz  11.43 c/d  11.24 i/d   0.71 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_narrow                      :   4.91 GB/s  307.0 Ma/s   3.26 ns/d   3.53 GHz  11.51 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 9
veq_non_zero_max                         :   4.79 GB/s  299.6 Ma/s   3.34 ns/d   3.52 GHz  11.76 c/d  11.24 i/d   0.73 c/b   0.70 i/b   0.96 i/c 
veq_non_zero_mov                         :   4.86 GB/s  303.9 Ma/s   3.29 ns/d   3.52 GHz  11.59 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.97 i/c 
veq_non_zero_narrow                      :   4.94 GB/s  309.0 Ma/s   3.24 ns/d   3.23 GHz  10.46 c/d  11.24 i/d   0.65 c/b   0.70 i/b   1.07 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
Trial 10
veq_non_zero_max                         :   4.89 GB/s  305.8 Ma/s   3.27 ns/d   3.52 GHz  11.52 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_mov                         :   4.94 GB/s  308.4 Ma/s   3.24 ns/d   3.54 GHz  11.47 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_narrow                      :   4.91 GB/s  306.8 Ma/s   3.26 ns/d   3.52 GHz  11.48 c/d  11.24 i/d   0.72 c/b   0.70 i/b   0.98 i/c 
veq_non_zero_float                       :   3.73 GB/s  233.2 Ma/s   4.29 ns/d   3.52 GHz  15.09 c/d  10.24 i/d   0.94 c/b   0.64 i/b   0.68 i/c 
```
