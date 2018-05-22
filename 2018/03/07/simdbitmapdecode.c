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

uint64_t bytepopcount(uint64_t x) {
   const uint64_t c1 = UINT64_C(0x5555555555555555);
   const uint64_t c2 = UINT64_C(0x3333333333333333);
   const uint64_t c4 = UINT64_C(0x0F0F0F0F0F0F0F0F);
   x = (x & c1) + ((x >> 1) & c1);
   x = (x & c2) + ((x >> 2) & c2);
   x = (x & c4) + ((x >> 4) & c4);
   return x;
}

int bitmap_decode_avx2_turbo_thin(uint64_t * array, size_t sizeinwords, uint32_t *out) {
	uint32_t *initout = out;
	__m256i baseVec = _mm256_set1_epi32(-1);
	__m256i incVec = _mm256_set1_epi32(64);
	__m256i add8 = _mm256_set1_epi32(8);

	for (int i = 0; i < sizeinwords; ++i) {
		uint64_t w = array[i];
		if (w == 0) {
			baseVec = _mm256_add_epi32(baseVec, incVec);
		} else {
                        uint64_t pop = bytepopcount(w);
			for (int k = 0; k < 4; ++k) {
				uint8_t byteA = (uint8_t) w;
				uint8_t byteB = (uint8_t)(w >> 8);
				w >>= 16;
				__m256i vecA =  _mm256_cvtepu8_epi32(_mm_cvtsi64_si128(*(uint64_t *)(vecDecodeTableByte[byteA])));
				__m256i vecB =  _mm256_cvtepu8_epi32(_mm_cvtsi64_si128(*(uint64_t *)(vecDecodeTableByte[byteB])));
				uint8_t advanceA = (uint8_t) pop;
				uint8_t advanceB = (uint8_t) (pop >> 8);
                                pop >>= 16;
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

int bitmap_decode_avx2_turbo(uint64_t * array, size_t sizeinwords, uint32_t *out) {
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
				__m256i vecA =  _mm256_cvtepu8_epi32(_mm_cvtsi64_si128(*(uint64_t *)(vecDecodeTableByte[byteA])));
				__m256i vecB =  _mm256_cvtepu8_epi32(_mm_cvtsi64_si128(*(uint64_t *)(vecDecodeTableByte[byteB])));
				uint8_t advanceA = __builtin_popcount(byteA);
				uint8_t advanceB = __builtin_popcount(byteB);
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

int bitmap_decode_avx2_turbo_nopopcnt(uint64_t * array, size_t sizeinwords, uint32_t *out) {
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
				__m256i vecA =  _mm256_cvtepu8_epi32(_mm_cvtsi64_si128(*(uint64_t *)(vecDecodeTableByte[byteA])));
				__m256i vecB =  _mm256_cvtepu8_epi32(_mm_cvtsi64_si128(*(uint64_t *)(vecDecodeTableByte[byteB])));
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

#ifdef  __AVX512BW__
size_t bitmap_decode_bmi2_avx512(uint64_t *bitmap, size_t bitmapsize, uint32_t *out) {
	size_t pos = 0;
	uint32_t *initout = out;
	uint64_t bitset;
	uint64_t msk_1 = 0xffffffff00000000ULL;
	uint64_t msk_2 = 0xffff0000ffff0000ULL;
	uint64_t msk_3 = 0xff00ff00ff00ff00ULL;
	uint64_t msk_4 = 0xf0f0f0f0f0f0f0f0ULL;
	uint64_t msk_5 = 0xccccccccccccccccULL;
	uint64_t msk_6 = 0xaaaaaaaaaaaaaaaaULL;
	__m512i v1_bit = _mm512_set1_epi8(1);
	__m512i v2_bit = _mm512_set1_epi8(2);
	__m512i v4_bit = _mm512_set1_epi8(4);
	__m512i v8_bit = _mm512_set1_epi8(8);
	__m512i v16_bit = _mm512_set1_epi8(16);
	__m512i v32_bit = _mm512_set1_epi8(32);
	for (size_t k = 0; k < bitmapsize; ++k) {
		uint64_t v = bitmap[k];
		bitset = bitmap[k];
		uint64_t v1 = _pext_u64(msk_1, v);
		uint64_t v2 = _pext_u64(msk_2, v);
		uint64_t v3 = _pext_u64(msk_3, v);
		uint64_t v4 = _pext_u64(msk_4, v);
		uint64_t v5 = _pext_u64(msk_5, v);
		uint64_t v6 = _pext_u64(msk_6, v);
		uint8_t advance = __builtin_popcountll(v);
		__m512i vec;
		// a miracle appears
		vec = _mm512_maskz_add_epi8(v1, v32_bit, _mm512_set1_epi8(0));
		vec = _mm512_mask_add_epi8(vec, v2, v16_bit, vec);
		vec = _mm512_mask_add_epi8(vec, v3, v8_bit, vec);
		vec = _mm512_mask_add_epi8(vec, v4, v4_bit, vec);
		vec = _mm512_mask_add_epi8(vec, v5, v2_bit, vec);
		vec = _mm512_mask_add_epi8(vec, v6, v1_bit, vec);

		__m512i base = _mm512_set1_epi32(k*64);
		__m512i r1 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(vec,0));
		__m512i r2 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(vec,1));
		__m512i r3 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(vec,2));
		__m512i r4 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(vec,3));

		r1 = _mm512_add_epi32(r1, base);
		r2 = _mm512_add_epi32(r2, base);
		r3 = _mm512_add_epi32(r3, base);
		r4 = _mm512_add_epi32(r4, base);
		_mm512_storeu_si512((__m512i *)out, r1);
		_mm512_storeu_si512((__m512i *)(out + 16), r2);
		_mm512_storeu_si512((__m512i *)(out + 32), r3);
		_mm512_storeu_si512((__m512i *)(out + 48), r4);
		out += advance;
	}
	return out - initout;
}
#endif

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
  printf("[bitmap decoding]\n");
  int repeat = 50;
  size_t N = 1000;
  uint64_t *bitmap = malloc(N * sizeof(uint64_t));
  uint32_t *receiver = malloc((N + 64) * 64 * sizeof(uint32_t));
  double ratios[] = {0.03125, 0.125, 0.25, 0.5, 0.9};
  for (int k = 0 ; k < 5; k++) {
    double ratio = ratios[k];
    for (size_t k = 0; k < N; ++k) {
      bitmap[k] = 0;
    }
    size_t appbitcount = (size_t)ceil(ratio * N * 64);
    //if (appbitcount > N * 64 / 2)
    //  break;
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
    BEST_TIME(bitmap_decode_avx2_turbo(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);
    BEST_TIME(bitmap_decode_avx2_turbo_thin(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);
    BEST_TIME(bitmap_decode_avx2_turbo_nopopcnt(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);

#ifdef  __AVX512BW__
    BEST_TIME(bitmap_decode_bmi2_avx512(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);
#endif

  }

  free(bitmap);
  free(receiver);
}

int main() {
  bitmap_decoding();
  return EXIT_SUCCESS;
}
