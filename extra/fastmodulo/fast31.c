#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "benchmark.h"


static inline uint32_t maxLiteralLengthModulus(uint32_t n) {
   uint32_t m = (n & UINT32_C(0xC1F07C1F)) + ((n >> 5) & UINT32_C(0xC1F07C1F));
   m = (m >> 15) + (m & UINT32_C(0x00007FFF));
   if (m <= 31)
       return m == 31 ? 0 : m;
   m = (m >> 5) + (m & UINT32_C(0x0000001F));
   if (m <= 31)
       return m == 31 ? 0 : m;
   m = (m >> 5) + (m & UINT32_C(0x0000001F));
   if (m <= 31)
       return m == 31 ? 0 : m;
   m = (m >> 5) + (m & UINT32_C(0x0000001F));
   if (m <= 31)
       return m == 31 ? 0 : m;
   m = (m >> 5) + (m & UINT32_C(0x0000001F));
   if (m <= 31)
       return m == 31 ? 0 : m;
   m = (m >> 5) + (m & UINT32_C(0x0000001F));
   return m == 31 ? 0 : m;
}

static uint32_t sum31(uint32_t * data, size_t length) {
  uint32_t x = 0;
  for(size_t i = 0; i < length; ++i) x += (data[i] % 31);
  return x;
}

static uint32_t fastsum31(uint32_t * data, size_t length) {
  uint32_t x = 0;
  for(size_t i = 0; i < length; ++i) x += maxLiteralLengthModulus(data[i]);
  return x;
}
int main() {
  const int N = 1000;
  uint32_t * data = malloc(sizeof(uint32_t) * N);
  for(size_t i = 0 ; i < N; ++i)
    data[i] = rand();
  const int repeat = 50;
  uint32_t expected = sum31(data,N);
  BEST_TIME(sum31(data,N), expected, , repeat,  N, true);
  BEST_TIME(fastsum31(data,N), expected, , repeat,  N, true);

}
