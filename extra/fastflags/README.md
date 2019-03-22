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
g++ --version
g++ (Ubuntu 8.1.0-5ubuntu1~16.04) 8.1.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

./countbits
500000 500000 500000 500000 500000 500000 499968 499968 499968 499776 499712 499712 499712 499712 492096 491520
scalar_naive(array, len, counter)                 	:  3.459 cycles per input word (best)  3.973 cycles per input word (avg)
scalar_morenaive(array, len, counter)             	:  3.414 cycles per input word (best)  3.496 cycles per input word (avg)
fastavx2(array, len, counter)                     	:  1.165 cycles per input word (best)  1.201 cycles per input word (avg)
flag_stats_avx2(array, len, counter)              	:  3.436 cycles per input word (best)  3.490 cycles per input word (avg)
flag_stats_avx2_naive_counter(array, len, counter)	:  3.483 cycles per input word (best)  3.496 cycles per input word (avg)
flag_stats_avx2_single(array, len, counter)       	:  3.319 cycles per input word (best)  3.382 cycles per input word (avg)
fastavx2mula(array, len, counter)                 	:  1.738 cycles per input word (best)  1.778 cycles per input word (avg)
```
