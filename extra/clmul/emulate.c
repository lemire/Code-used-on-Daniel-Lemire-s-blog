// gcc -O3 -o emulate emulate.c -march=native
#include <stdint.h>
#include <string.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <x86intrin.h>
#include <assert.h>

uint64_t getmask1(uint64_t quote_bits) {
  uint64_t quote_mask = _mm_cvtsi128_si64(_mm_clmulepi64_si128(
      _mm_set_epi64x(0ULL, quote_bits), _mm_set1_epi8(0xFF), 0));
  return quote_mask;
}

uint64_t getmask2(uint64_t quote_bits) {
  quote_bits = quote_bits ^ (quote_bits << 1);
  quote_bits = quote_bits ^ (quote_bits << 2);
  quote_bits = quote_bits ^ (quote_bits << 4);
  quote_bits = quote_bits ^ (quote_bits << 8);
  quote_bits = quote_bits ^ (quote_bits << 16);
  quote_bits = quote_bits ^ (quote_bits << 32);
  return quote_bits;
}

int main() {
  for(uint64_t x = 1 ; x < 0xFFFFFFFF; x++) {
    if((x % 10000000) == 0) {printf("\r %.1f %%",(double)x * 100.0/0xFFFFFFFF); fflush(NULL);}
    assert(getmask1(x)== getmask2(x));
    assert(getmask1(x | (1ULL << 63))== getmask2(x | (1ULL << 63)));
    assert((getmask1(x) << 32)== (getmask2(x)<< 32));

  }
  printf("\n ok\n");
}
