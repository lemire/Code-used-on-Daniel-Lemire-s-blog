// gcc -O3 -o bitmapdecode bitmapdecode.c  -march=native -lm
#include "benchmark.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

size_t bitmap_decode_supernaive(uint64_t *bitmap, size_t bitmapsize,
                                uint32_t *out) {
  size_t pos = 0;
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    for (int i = 0; i < 64; i++) {
      if ((bitset >> i) & 0x1)
        out[pos++] = p + i;
    }
  }
  return pos;
}

size_t bitmap_decode_naive(uint64_t *bitmap, size_t bitmapsize, uint32_t *out) {
  size_t pos = 0;
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    while (bitset != 0) {
      if (bitset & 0x1) {
        out[pos++] = p;
      }
      bitset >>= 1;
      p += 1;
    }
  }
  return pos;
}

size_t bitmap_decode_ctz(uint64_t *bitmap, size_t bitmapsize, uint32_t *out) {
  size_t pos = 0;
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    while (bitset != 0) {
      uint64_t t = bitset & -bitset;
      int r = __builtin_ctzll(bitset);
      out[pos++] = k * 64 + r;
      bitset ^= t;
    }
  }
  return pos;
}


size_t bitmap_decode_supernaive_callback(uint64_t *bitmap, size_t bitmapsize,
                                         void (*callback)(int)) {
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    for (int i = 0; i < 64; i++) {
      if ((bitset >> i) & 0x1)
        callback(p + i);
    }
  }
  return 0;
}

size_t bitmap_decode_naive_callback(uint64_t *bitmap, size_t bitmapsize,
                                    void (*callback)(int)) {
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    while (bitset != 0) {
      if (bitset & 0x1) {
        callback(p);
      }
      bitset >>= 1;
      p += 1;
    }
  }
  return 0;
}

size_t bitmap_decode_ctz_callpack(uint64_t *bitmap, size_t bitmapsize,
                                  void (*callback)(int)) {
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    while (bitset != 0) {
      uint64_t t = bitset & -bitset;
      int r = __builtin_ctzll(bitset);
      callback(k * 64 + r);
      bitset ^= t;
    }
  }
  return 0;
}

size_t bitmap_count(uint64_t *bitmap, size_t bitmapcount) {
  uint64_t count = 0;
  for (size_t i = 0; i < bitmapcount; ++i) {
    count += __builtin_popcountll(bitmap[i]);
  }
  return count;
}

int globalcount = 0;
void defaultcallback(int x) { globalcount += x; }

void bitmap_decoding() {
  printf("[bitmap decoding]");
  int repeat = 5;
  size_t N = 1000;
  uint64_t *bitmap = malloc(N * sizeof(uint64_t));
  uint32_t *receiver = malloc((N + 64) * 64 * sizeof(uint32_t));

  for (double ratio = 0.03125; ratio < 2; ratio = ratio * 2) {
    for (size_t k = 0; k < N; ++k) {
      bitmap[k] = 0;
    }
    size_t appbitcount = (size_t)ceil(ratio * N * 64);
    if (appbitcount > N * 64 / 2)
      break;
    size_t ccount = 0;
    while (ccount < appbitcount) {
      int bit = rand() % (N * 64);
      uint64_t bef = bitmap[bit / 64];
      uint64_t aft = bef | (UINT64_C(1) << (bit % 64));
      if (bef != aft)
        ccount++;
      bitmap[bit / 64] = aft;
    }
    size_t bitcount = bitmap_count(bitmap, N);
    assert(bitcount == ccount);

    printf("bitmap density %3.2f  \n", bitcount / (N * sizeof(uint64_t) * 8.0));
    BEST_TIME(bitmap_decode_supernaive(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);
    BEST_TIME(bitmap_decode_naive(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);
    BEST_TIME(bitmap_decode_ctz(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);

    BEST_TIME(bitmap_decode_supernaive_callback(bitmap, N, defaultcallback), 0,
              , repeat, bitcount, true);
    BEST_TIME(bitmap_decode_naive_callback(bitmap, N, defaultcallback), 0, ,
              repeat, bitcount, true);
    BEST_TIME(bitmap_decode_ctz_callpack(bitmap, N, defaultcallback), 0, ,
              repeat, bitcount, true);
  }

  free(bitmap);
  free(receiver);
}

int main() {
  bitmap_decoding();
  return EXIT_SUCCESS;
}
