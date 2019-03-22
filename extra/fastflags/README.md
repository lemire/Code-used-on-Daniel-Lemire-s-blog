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


Results:

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
