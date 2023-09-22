This assumes a recent AVX-512 processor.

The `parse_unsigned_avx512` function is between 1.5x (GCC) to 3x faster (clang) than conventional number parsing routines. Importantly, it does not 'cheat': it does not assume that the input is padded in any way. However, we require that you pass a pointer to the end of the digits.
It is based on an initial design by John Keiser.



Note that this is without constant hoisting, we pay the full price of function calls.

```
$ make
$ ./benchmark-clang
parse_unsigned_avx512                    :   2.18 GB/s  230.4 Ma/s   4.34 ns/d   3.20 GHz  13.87 c/d  47.74 i/d   1.46 c/b   5.04 i/b   3.44 i/c 
std::from_chars                          :   0.78 GB/s   82.0 Ma/s  12.20 ns/d   3.19 GHz  38.97 c/d  128.76 i/d   4.11 c/b  13.58 i/b   3.30 i/c 
parse_unsigned                           :   0.77 GB/s   81.1 Ma/s  12.33 ns/d   3.19 GHz  39.39 c/d  105.84 i/d   4.16 c/b  11.17 i/b   2.69 i/c 
parse_unsigned_bounded                   :   0.73 GB/s   77.2 Ma/s  12.96 ns/d   3.19 GHz  41.39 c/d  109.84 i/d   4.37 c/b  11.59 i/b   2.65 i/c 
$ ./benchmark
parse_unsigned_avx512                    :   1.16 GB/s  122.2 Ma/s   8.18 ns/d   3.19 GHz  26.13 c/d  65.74 i/d   2.76 c/b   6.94 i/b   2.52 i/c 
std::from_chars                          :   0.78 GB/s   82.1 Ma/s  12.18 ns/d   3.19 GHz  38.91 c/d  128.76 i/d   4.11 c/b  13.58 i/b   3.31 i/c 
parse_unsigned                           :   0.74 GB/s   78.2 Ma/s  12.79 ns/d   3.19 GHz  40.86 c/d  114.84 i/d   4.31 c/b  12.12 i/b   2.81 i/c 
parse_unsigned_bounded                   :   0.67 GB/s   70.3 Ma/s  14.22 ns/d   3.19 GHz  45.41 c/d  117.84 i/d   4.79 c/b  12.43 i/b   2.59 i/c 
``````
