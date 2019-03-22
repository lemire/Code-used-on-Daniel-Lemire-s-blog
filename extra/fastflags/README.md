Given an array of 16-bit values, we want to count the number of first bit set, the number
of second bit set, and so forth (16 counters in total).

Ref: https://github.com/mklarqvist/FastFlagStats

Naive scalar code:
```

void scalar_naive(const uint16_t *data, size_t n, uint32_t *flags) {
  memset(flags, 0, 16 * sizeof(uint32_t));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 16; ++j) {
      flags[j] += ((data[i] & (1 << j)) >> j);
    }
  }
}
```


Results (Skylake):

```
$ make test
clang++-6.0 -O3 -march=native -o countbits countbits.cpp -Wall -Wextra
clang++-6.0 --version
clang version 6.0.1-svn334776-1~exp1~20190114064905.115 (branches/release_60)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
./countbits

 Using array size = 1000000
500000 500000 500000 500000 500000 500000 499968 499968 499968 499776 499712 499712 499712 499712 492096 491520
scalar_naive(array, len, counter)                 	:  2.300 cycles per input word (best)  2.309 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  2.300 cycles per input word (best)  2.326 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.173 cycles per input word (best)  1.185 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  1.173 cycles per input word (best)  1.188 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  2.513 cycles per input word (best)  2.524 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  2.512 cycles per input word (best)  2.533 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  2.037 cycles per input word (best)  2.047 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  1.457 cycles per input word (best)  1.469 cycles per input word (avg)

 Using array size = 2000000
1000000 1000000 1000000 1000000 1000000 1000000 1000000 999936 999936 999936 999552 999424 999424 999424 999424 984192
scalar_naive(array, len, counter)                 	:  2.301 cycles per input word (best)  2.308 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  2.301 cycles per input word (best)  2.309 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.174 cycles per input word (best)  1.178 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  1.175 cycles per input word (best)  1.183 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  1.857 cycles per input word (best)  1.861 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  1.769 cycles per input word (best)  1.777 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  2.037 cycles per input word (best)  2.040 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  1.458 cycles per input word (best)  1.464 cycles per input word (avg)

 Using array size = 4000000
2000000 2000000 2000000 2000000 2000000 2000000 2000000 2000000 1999872 1999872 1999872 1999104 1998848 1998848 1998848 1998848
scalar_naive(array, len, counter)                 	:  2.304 cycles per input word (best)  2.311 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  2.303 cycles per input word (best)  2.310 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.182 cycles per input word (best)  1.189 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  1.184 cycles per input word (best)  1.191 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  1.536 cycles per input word (best)  1.547 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  1.403 cycles per input word (best)  1.413 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  2.033 cycles per input word (best)  2.041 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  1.463 cycles per input word (best)  1.470 cycles per input word (avg)

 Using array size = 100000000
50000000 50000000 50000000 50000000 50000000 50000000 50000000 50000000 49999872 49999872 49999872 49999872 49999872 49996032 49996032 49996032
scalar_naive(array, len, counter)                 	:  2.310 cycles per input word (best)  2.310 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  2.310 cycles per input word (best)  2.312 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.195 cycles per input word (best)  1.196 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  1.195 cycles per input word (best)  1.196 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  1.285 cycles per input word (best)  1.286 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  1.121 cycles per input word (best)  1.123 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  2.039 cycles per input word (best)  2.041 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  1.471 cycles per input word (best)  1.472 cycles per input word (avg)
```


Results (cannonlake):

```
$ make test
g++-8 -O3 -mavx2 -march=native -o countbits countbits.cpp -Wall -Wextra
g++-8 --version
g++-8 (Homebrew GCC 8.2.0) 8.2.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

./countbits

 Using array size = 1000000
500000 500000 500000 500000 500000 500000 499968 499968 499968 499776 499712 499712 499712 499712 492096 491520
scalar_naive(array, len, counter)                 	:  14.616 cycles per input word (best)  14.674 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  13.521 cycles per input word (best)  13.749 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.484 cycles per input word (best)  1.505 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  0.755 cycles per input word (best)  0.772 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  2.341 cycles per input word (best)  2.372 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  2.377 cycles per input word (best)  2.407 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  2.247 cycles per input word (best)  2.276 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  0.693 cycles per input word (best)  0.725 cycles per input word (avg)

 Using array size = 2000000
1000000 1000000 1000000 1000000 1000000 1000000 1000000 999936 999936 999936 999552 999424 999424 999424 999424 984192
scalar_naive(array, len, counter)                 	:  13.682 cycles per input word (best)  13.697 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  13.104 cycles per input word (best)  13.395 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.487 cycles per input word (best)  1.500 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  0.772 cycles per input word (best)  0.799 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  1.498 cycles per input word (best)  1.519 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  1.676 cycles per input word (best)  1.695 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  2.224 cycles per input word (best)  2.256 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  0.715 cycles per input word (best)  0.743 cycles per input word (avg)

 Using array size = 4000000
2000000 2000000 2000000 2000000 2000000 2000000 2000000 2000000 1999872 1999872 1999872 1999104 1998848 1998848 1998848 1998848
scalar_naive(array, len, counter)                 	:  13.257 cycles per input word (best)  13.447 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  13.126 cycles per input word (best)  13.262 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.494 cycles per input word (best)  1.502 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  0.765 cycles per input word (best)  0.787 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  1.062 cycles per input word (best)  1.093 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  1.327 cycles per input word (best)  1.341 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  2.224 cycles per input word (best)  2.251 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  0.735 cycles per input word (best)  0.756 cycles per input word (avg)

 Using array size = 100000000
50000000 50000000 50000000 50000000 50000000 50000000 50000000 50000000 49999872 49999872 49999872 49999872 49999872 49996032 49996032 49996032
scalar_naive(array, len, counter)                 	:  13.279 cycles per input word (best)  13.323 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  13.125 cycles per input word (best)  13.141 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.496 cycles per input word (best)  1.499 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  0.764 cycles per input word (best)  0.767 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  0.719 cycles per input word (best)  0.720 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  1.037 cycles per input word (best)  1.041 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  1.965 cycles per input word (best)  1.971 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  0.734 cycles per input word (best)  0.736 cycles per input word (avg)
```


Skylake-x processor (the compiler seems not to autovec):

```

$ make test
g++ -O3 -mavx2 -march=native -o countbits countbits.cpp -Wall -Wextra
g++ --version
g++ (Ubuntu 8.1.0-5ubuntu1~16.04) 8.1.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

./countbits

 Using array size = 1000000
500000 500000 500000 500000 500000 500000 499968 499968 499968 499776 499712 499712 499712 499712 492096 491520
scalar_naive(array, len, counter)                 	:  19.663 cycles per input word (best)  20.595 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  19.474 cycles per input word (best)  19.538 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.936 cycles per input word (best)  2.001 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  1.150 cycles per input word (best)  1.156 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  3.369 cycles per input word (best)  3.449 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  3.377 cycles per input word (best)  3.439 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  3.316 cycles per input word (best)  3.343 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  1.435 cycles per input word (best)  1.496 cycles per input word (avg)

 Using array size = 2000000
1000000 1000000 1000000 1000000 1000000 1000000 1000000 999936 999936 999936 999552 999424 999424 999424 999424 984192
scalar_naive(array, len, counter)                 	:  19.349 cycles per input word (best)  19.364 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  19.171 cycles per input word (best)  19.183 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.930 cycles per input word (best)  1.937 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  1.147 cycles per input word (best)  1.151 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  2.218 cycles per input word (best)  2.253 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  2.536 cycles per input word (best)  2.573 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  3.144 cycles per input word (best)  3.152 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  1.434 cycles per input word (best)  1.460 cycles per input word (avg)

 Using array size = 4000000
2000000 2000000 2000000 2000000 2000000 2000000 2000000 2000000 1999872 1999872 1999872 1999104 1998848 1998848 1998848 1998848
scalar_naive(array, len, counter)                 	:  19.352 cycles per input word (best)  19.371 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  19.182 cycles per input word (best)  19.197 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.933 cycles per input word (best)  1.945 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  1.159 cycles per input word (best)  1.171 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  1.600 cycles per input word (best)  1.661 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  1.942 cycles per input word (best)  2.008 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  3.032 cycles per input word (best)  3.059 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  1.446 cycles per input word (best)  1.471 cycles per input word (avg)

 Using array size = 100000000
50000000 50000000 50000000 50000000 50000000 50000000 50000000 50000000 49999872 49999872 49999872 49999872 49999872 49996032 49996032 49996032
scalar_naive(array, len, counter)                 	:  19.371 cycles per input word (best)  19.375 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  19.219 cycles per input word (best)  19.226 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.976 cycles per input word (best)  1.977 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  1.260 cycles per input word (best)  1.261 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  1.163 cycles per input word (best)  1.167 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  1.671 cycles per input word (best)  1.674 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  3.014 cycles per input word (best)  3.029 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  1.536 cycles per input word (best)  1.538 cycles per input word (avg)
```
scalar_naive(array, len, counter)                 	:  13.279 cycles per input word (best)  13.323 cycles per input word (avg)
