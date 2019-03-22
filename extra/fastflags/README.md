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
clang++-6.0 --version
clang version 6.0.1-svn334776-1~exp1~20190114064905.115 (branches/release_60)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
./countbits

 Using array size = 1000000
500000 500000 500000 500000 500000 500000 499968 499968 499968 499776 499712 499712 499712 499712 492096 491520
scalar_naive(array, len, counter)                 	:  2.300 cycles per input word (best)  2.322 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  2.301 cycles per input word (best)  2.321 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.217 cycles per input word (best)  1.232 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  1.174 cycles per input word (best)  1.184 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  2.516 cycles per input word (best)  2.529 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  2.511 cycles per input word (best)  2.524 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  2.037 cycles per input word (best)  2.051 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  1.459 cycles per input word (best)  1.462 cycles per input word (avg)

 Using array size = 100000000
50000000 50000000 50000000 50000000 50000000 50000000 50000000 50000000 49999872 49999872 49999872 49999872 49999872 49996032 49996032 49996032
scalar_naive(array, len, counter)                 	:  2.309 cycles per input word (best)  2.311 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  2.310 cycles per input word (best)  2.310 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.236 cycles per input word (best)  1.236 cycles per input word (avg)
morefastavx2(array, len, counter)                 	:  1.194 cycles per input word (best)  1.195 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  1.290 cycles per input word (best)  1.303 cycles per input word (avg)
flag_stats_avx2 is buggy
flag_stats_avx2_naive_counter(array, len, counter)	:  1.108 cycles per input word (best)  1.111 cycles per input word (avg)
flag_stats_avx2_naive_counter is buggy
flag_stats_avx2_single(array, len, counter)       	:  2.039 cycles per input word (best)  2.072 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  1.473 cycles per input word (best)  1.475 cycles per input word (avg)

```
