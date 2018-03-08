// gcc -O3 -o simdbitmapdecode simdbitmapdecode.c  -march=native -lm
#include "benchmark.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

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

size_t bitmap_decode_block3(uint64_t *bitmap, size_t bitmapsize, uint32_t *out) {
  size_t pos = 0;
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    while (bitset != 0) {
        switch (bitset & 0x7) {
            case 0:
                break;

            case 1:
                out[pos++] = p;
                break;

            case 2:
                out[pos++] = p + 1;
                break;

            case 3:
                out[pos++] = p;
                out[pos++] = p + 1;
                break;

            case 4:
                out[pos++] = p + 2;
                break;

            case 5:
                out[pos++] = p;
                out[pos++] = p + 2;
                break;

            case 6:
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                break;

            case 7:
                out[pos++] = p;
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                break;
        } // switch

        bitset >>= 3;
        p += 3;
    }
  }
  return pos;
}

size_t bitmap_decode_block4(uint64_t *bitmap, size_t bitmapsize, uint32_t *out) {
  size_t pos = 0;
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    while (bitset != 0) {
        switch (bitset & 0xf) {
            case 0:
                break;

            case 1:
                out[pos++] = p;
                break;

            case 2:
                out[pos++] = p + 1;
                break;

            case 3:
                out[pos++] = p;
                out[pos++] = p + 1;
                break;

            case 4:
                out[pos++] = p + 2;
                break;

            case 5:
                out[pos++] = p;
                out[pos++] = p + 2;
                break;

            case 6:
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                break;

            case 7:
                out[pos++] = p;
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                break;

            case 8:
                out[pos++] = p + 3;
                break;

            case 9:
                out[pos++] = p;
                out[pos++] = p + 3;
                break;

            case 10:
                out[pos++] = p + 1;
                out[pos++] = p + 3;
                break;

            case 11:
                out[pos++] = p;
                out[pos++] = p + 1;
                out[pos++] = p + 3;
                break;

            case 12:
                out[pos++] = p + 2;
                out[pos++] = p + 3;
                break;

            case 13:
                out[pos++] = p;
                out[pos++] = p + 2;
                out[pos++] = p + 3;
                break;

            case 14:
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                out[pos++] = p + 3;
                break;

            case 15:
                out[pos++] = p;
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                out[pos++] = p + 3;
                break;
        }

        bitset >>= 4;
        p += 4;
    }
  }
  return pos;
}

#include "bitmap_tables.h"

int bitmap_decode_avx2(uint64_t * array, size_t sizeinwords, uint32_t *out) {
	uint32_t *initout = out;
	__m256i baseVec = _mm256_set1_epi32(-1);
	__m256i incVec = _mm256_set1_epi32(64);
	__m256i add8 = _mm256_set1_epi32(8);

	for (int i = 0; i < sizeinwords; ++i) {
		uint64_t w = array[i];
		if (w == 0) {
			baseVec = _mm256_add_epi32(baseVec, incVec);
		} else {
			for (int k = 0; k < 4; ++k) {
				uint8_t byteA = (uint8_t) w;
				uint8_t byteB = (uint8_t)(w >> 8);
				w >>= 16;
				__m256i vecA = _mm256_load_si256((const __m256i *) vecDecodeTable[byteA]);
				__m256i vecB = _mm256_load_si256((const __m256i *) vecDecodeTable[byteB]);
				uint8_t advanceA = lengthTable[byteA];
				uint8_t advanceB = lengthTable[byteB];
				vecA = _mm256_add_epi32(baseVec, vecA);
				baseVec = _mm256_add_epi32(baseVec, add8);
				vecB = _mm256_add_epi32(baseVec, vecB);
				baseVec = _mm256_add_epi32(baseVec, add8);
				_mm256_storeu_si256((__m256i *) out, vecA);
				out += advanceA;
				_mm256_storeu_si256((__m256i *) out, vecB);
				out += advanceB;
			}
		}
	}
	return out - initout;
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

    BEST_TIME(bitmap_decode_ctz(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);
    BEST_TIME(bitmap_decode_avx2(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);


  }

  free(bitmap);
  free(receiver);
}

int main() {
  bitmap_decoding();
  return EXIT_SUCCESS;
}
