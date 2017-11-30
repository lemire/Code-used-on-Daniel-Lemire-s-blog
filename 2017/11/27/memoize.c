// gcc -O3 -o memoize memoize.c && ./memoize
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "benchmark.h"

int getlength(uint8_t x) {
  return 4 + (x & 0b11) + ((x>>2) & 0b11) + ((x>>4) & 0b11) + (x>>6) ;
}

//credit:aqrit and KWillets
int getlength_aqrit(uint8_t key) {
  return ((UINT32_C(0x11011000) * (((uint32_t)key * UINT32_C(0x0401)) & UINT32_C(0x00033033))) >> 28)  + 4;
}


static uint8_t lengthTable[256] ={
  4,  5,  6,  7,  5,  6,  7,  8,  6,  7,  8,  9,  7,  8,  9, 10,
  5,  6,  7,  8,  6,  7,  8,  9,  7,  8,  9, 10,  8,  9, 10, 11,
  6,  7,  8,  9,  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12,
  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13,
  5,  6,  7,  8,  6,  7,  8,  9,  7,  8,  9, 10,  8,  9, 10, 11,
  6,  7,  8,  9,  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12,
  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13,
  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14,
  6,  7,  8,  9,  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12,
  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13,
  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14,
  9, 10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14, 12, 13, 14, 15,
  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13,
  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14,
  9, 10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14, 12, 13, 14, 15,
 10, 11, 12, 13, 11, 12, 13, 14, 12, 13, 14, 15, 13, 14, 15, 16,
 };

int getlength_lookup(uint8_t key) {
  return lengthTable[key];
}

uint32_t getlength4_aqrit(uint32_t key) {
  uint32_t v = ((key >> 2) & 0x33333333) + (key & 0x33333333);
  v = ((v >> 4) & 0x0F0F0F0F) + (v & 0x0F0F0F0F);
  return v + 0x04040404;
}

uint32_t sum_aqrit(uint8_t * key, size_t N) {
  uint32_t x = 0;
  for(size_t i = 0; i < N; i++) {
    x += getlength_aqrit(key[i]);
  }
  return x;
}


uint32_t sum(uint8_t * key, size_t N) {
  uint32_t x = 0;
  for(size_t i = 0; i < N; i++) {
    x += getlength(key[i]);
  }
  return x;
}

uint32_t sum_lookup(uint8_t * key, size_t N) {
  uint32_t x = 0;
  for(size_t i = 0; i < N; i++) {
    x += getlength_lookup(key[i]);
  }
  return x;
}

uint32_t sum4_aqrit(uint8_t * key, size_t N) {
  uint32_t x = 0;
  size_t i = 0;
  for(; i + 3 < N; i+=4) {
    uint32_t tmp;
    memcpy(&tmp, key +i, 4);
    uint32_t packed = getlength4_aqrit(tmp);
    x += ( packed & 0xFF ) + ( (packed >> 8) & 0xFF ) + ( (packed >> 16) & 0xFF ) + (packed >> 24);
  }
  for(; i  < N; i++) {
    x += getlength_lookup(key[i]);
  }

  return x;
}
void demo(size_t N) {
  uint8_t * keys = (uint8_t *) malloc(N * sizeof(uint8_t));
  for(size_t k = 0; k < 256; k++) {
    assert(getlength_lookup(k) == getlength(k));
    assert(getlength_lookup(k) == getlength_aqrit(k));
  }
  uint32_t expected = 0;
  for(size_t k = 0; k < N; k++) {
    keys[k] = rand() % 256;
    expected += getlength_lookup(keys[k]);
  }

  int repeat = 5;
  BEST_TIME(sum(keys,N),expected, , repeat, N, true);
  BEST_TIME(sum_aqrit(keys,N),expected, , repeat, N, true);
  BEST_TIME(sum_lookup(keys,N),expected, , repeat, N, true);
  BEST_TIME(sum4_aqrit(keys,N),expected,  , repeat, N, true);

  free(keys);
}


int main() {
  printf("clocks per s = %d \n", CLOCKS_PER_SEC);
  demo(64);
  return 0;
}
