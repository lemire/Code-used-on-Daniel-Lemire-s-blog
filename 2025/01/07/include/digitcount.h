#ifndef DIGITCOUNT_H
#define DIGITCOUNT_H

#include <cstdint>


int int_log2(uint64_t x) { return 63 - __builtin_clzll(x | 1); }

int digit_count(uint32_t x) {
   static uint32_t table[] = {9, 99, 999, 9999, 99999,
    999999, 9999999, 99999999, 999999999};
    int y = (9 * int_log2(x)) >> 5;
    y += x > table[y];
    return y + 1;
}

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

int alternative_digit_count(uint32_t x) {
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

int alternative_digit_count(uint64_t x) {
static uint64_t table[64][2] = {
    { 0x01, 0xfffffffffffffff6ULL },
    { 0x01, 0xfffffffffffffff6ULL },
    { 0x01, 0xfffffffffffffff6ULL },
    { 0x01, 0xfffffffffffffff6ULL },
    { 0x02, 0xffffffffffffff9cULL },
    { 0x02, 0xffffffffffffff9cULL },
    { 0x02, 0xffffffffffffff9cULL },
    { 0x03, 0xfffffffffffffc18ULL },
    { 0x03, 0xfffffffffffffc18ULL },
    { 0x03, 0xfffffffffffffc18ULL },
    { 0x04, 0xffffffffffffd8f0ULL },
    { 0x04, 0xffffffffffffd8f0ULL },
    { 0x04, 0xffffffffffffd8f0ULL },
    { 0x04, 0xffffffffffffd8f0ULL },
    { 0x05, 0xfffffffffffe7960ULL },
    { 0x05, 0xfffffffffffe7960ULL },
    { 0x05, 0xfffffffffffe7960ULL },
    { 0x06, 0xfffffffffff0bdc0ULL },
    { 0x06, 0xfffffffffff0bdc0ULL },
    { 0x06, 0xfffffffffff0bdc0ULL },
    { 0x07, 0xffffffffff676980ULL },
    { 0x07, 0xffffffffff676980ULL },
    { 0x07, 0xffffffffff676980ULL },
    { 0x07, 0xffffffffff676980ULL },
    { 0x08, 0xfffffffffa0a1f00ULL },
    { 0x08, 0xfffffffffa0a1f00ULL },
    { 0x08, 0xfffffffffa0a1f00ULL },
    { 0x09, 0xffffffffc4653600ULL },
    { 0x09, 0xffffffffc4653600ULL },
    { 0x09, 0xffffffffc4653600ULL },
    { 0x0a, 0xfffffffdabf41c00ULL },
    { 0x0a, 0xfffffffdabf41c00ULL },
    { 0x0a, 0xfffffffdabf41c00ULL },
    { 0x0a, 0xfffffffdabf41c00ULL },
    { 0x0b, 0xffffffe8b7891800ULL },
    { 0x0b, 0xffffffe8b7891800ULL },
    { 0x0b, 0xffffffe8b7891800ULL },
    { 0x0c, 0xffffff172b5af000ULL },
    { 0x0c, 0xffffff172b5af000ULL },
    { 0x0c, 0xffffff172b5af000ULL },
    { 0x0d, 0xfffff6e7b18d6000ULL },
    { 0x0d, 0xfffff6e7b18d6000ULL },
    { 0x0d, 0xfffff6e7b18d6000ULL },
    { 0x0d, 0xfffff6e7b18d6000ULL },
    { 0x0e, 0xffffa50cef85c000ULL },
    { 0x0e, 0xffffa50cef85c000ULL },
    { 0x0e, 0xffffa50cef85c000ULL },
    { 0x0f, 0xfffc72815b398000ULL },
    { 0x0f, 0xfffc72815b398000ULL },
    { 0x0f, 0xfffc72815b398000ULL },
    { 0x10, 0xffdc790d903f0000ULL },
    { 0x10, 0xffdc790d903f0000ULL },
    { 0x10, 0xffdc790d903f0000ULL },
    { 0x10, 0xffdc790d903f0000ULL },
    { 0x11, 0xfe9cba87a2760000ULL },
    { 0x11, 0xfe9cba87a2760000ULL },
    { 0x11, 0xfe9cba87a2760000ULL },
    { 0x12, 0xf21f494c589c0000ULL },
    { 0x12, 0xf21f494c589c0000ULL },
    { 0x12, 0xf21f494c589c0000ULL },
    { 0x13, 0x7538dcfb76180000ULL },
    { 0x13, 0x7538dcfb76180000ULL },
    { 0x13, 0x7538dcfb76180000ULL },
    { 0x13, 0x7538dcfb76180000ULL },
};
  int log = int_log2(x);
  uint64_t low = table[log][1];
  uint64_t high = table[log][0];
  return (x + low < x ) + high;
}


#endif