This assumes a recent AVX-512 processor.

The `parse_unsigned_avx512` function is 2.3x faster than conventional number parsing routines. Importantly, it does not 'cheat': it does not assume that the input is padded in any way. However, we require that you pass a pointer to the end of the digits.
It is based on an initial design by John Keiser.



Note that this is without constant hoisting, we pay the full price of function calls.

```
$ make
parse_unsigned_avx512                    :   1.80 GB/s  189.9 Ma/s   5.27 ns/d   3.20 GHz  16.83 c/d  57.74 i/d   1.78 c/b   6.09 i/b   3.43 i/c 
std::from_chars                          :   0.78 GB/s   81.9 Ma/s  12.20 ns/d   3.19 GHz  38.98 c/d  128.76 i/d   4.11 c/b  13.58 i/b   3.30 i/c 
parse_unsigned                           :   0.76 GB/s   80.3 Ma/s  12.45 ns/d   3.19 GHz  39.76 c/d  108.84 i/d   4.20 c/b  11.48 i/b   2.74 i/c 
parse_unsigned_bounded                   :   0.72 GB/s   75.6 Ma/s  13.22 ns/d   3.19 GHz  42.23 c/d  112.84 i/d   4.46 c/b  11.91 i/b   2.67 i/c 
$ ./benchmark
parse_unsigned_avx512                    :   1.95 GB/s  205.3 Ma/s   4.87 ns/d   3.20 GHz  15.57 c/d  49.79 i/d   1.64 c/b   5.25 i/b   3.20 i/c 
std::from_chars                          :   0.78 GB/s   82.2 Ma/s  12.17 ns/d   3.19 GHz  38.87 c/d  128.76 i/d   4.10 c/b  13.58 i/b   3.31 i/c 
parse_unsigned                           :   0.79 GB/s   83.7 Ma/s  11.95 ns/d   3.19 GHz  38.17 c/d  107.84 i/d   4.03 c/b  11.38 i/b   2.83 i/c 
parse_unsigned_bounded                   :   0.77 GB/s   80.9 Ma/s  12.36 ns/d   3.19 GHz  39.47 c/d  111.84 i/d   4.16 c/b  11.80 i/b   2.83 i/c 
``````
