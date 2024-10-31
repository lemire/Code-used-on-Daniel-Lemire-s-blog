#include <inttypes.h>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
int int_log2(uint32_t x) { return 31 - __builtin_clz(x | 1); }

int fast_digit_count(uint32_t x) {
  static uint64_t table[] = {
      4294967296,  8589934582,  8589934582,  8589934582,  12884901788,
      12884901788, 12884901788, 17179868184, 17179868184, 17179868184,
      21474826480, 21474826480, 21474826480, 21474826480, 25769703776,
      25769703776, 25769703776, 30063771072, 30063771072, 30063771072,
      34349738368, 34349738368, 34349738368, 34349738368, 38554705664,
      38554705664, 38554705664, 41949672960, 41949672960, 41949672960,
      42949672960, 42949672960};
  return (x + table[int_log2(x)]) >> 32;
}
int int_log2(uint64_t x) { return 63 - __builtin_clzll(x | 1); }

int digit_count(uint64_t x) {
  static uint64_t table[] = {9,
                             99,
                             999,
                             9999,
                             99999,
                             999999,
                             9999999,
                             99999999,
                             999999999,
                             9999999999,
                             99999999999,
                             999999999999,
                             9999999999999,
                             99999999999999,
                             999999999999999ULL,
                             9999999999999999ULL,
                             99999999999999999ULL,
                             999999999999999999ULL,
                             9999999999999999999ULL};
  int y = (19 * int_log2(x) >> 6);
  y += x > table[y];
  return y + 1;
}

int main() {
  size_t counter = 0;
  printf("%" PRIu64 " \n", 10000000000000000000ULL);
  for (uint64_t i = 1;; i *= 10) {
    ++counter;
    printf("%" PRIu64 " %d == %zu\n", i - 1, digit_count(i - 1), counter - 1);

    printf("%" PRIu64 " %d == %zu\n", i, digit_count(i), counter);
    printf("%" PRIu64 " %d == %zu\n", i + 1, digit_count(i + 1), counter);

    if (i * 10 < i) {
      break;
    }
  }
  printf("%" PRIu64 " %d\n", 0xffffffffffffffff,
         digit_count(0xffffffffffffffff));

  return 0;
}