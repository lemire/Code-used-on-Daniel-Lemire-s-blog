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

g++ -O3 -march=native -o countbits countbits.cpp && ./countbits


My fastavx2 function uses a naive vectorized algorithm that is not 'quite' correct (it is off by a bit
because I was lazy in my implementation), but it seems to do quite well...

```
$ g++ -O3 -march=native -o countbits countbits.cpp && ./countbits
scalar_naive(array, len, counter)       	:  2.271 cycles per input word (best)  2.307 cycles per input word (avg)
scalar_morenaive(array, len, counter)   	:  2.309 cycles per input word (best)  2.311 cycles per input word (avg)
fastavx2(array, len, counter)           	:  1.125 cycles per input word (best)  1.153 cycles per input word (avg)
flag_stats_avx2(array, len, counter)    	:  2.230 cycles per input word (best)  2.295 cycles per input word (avg)
```
