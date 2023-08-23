
#include "sse_inet_aton.h"

#include <x86intrin.h> // update if we need to support Windows.
#include <string.h>
/**

This assumes Linux/macOS and a processor with SSE 4.1 (virtually any x64 processor in operation today).

This works is based on research by Mula. (Mula is Polish and his name cannot be coded in ASCII. 
I transliterate his name deliberately.) 

Wojciech Mula, SIMD-ized faster parse of IPv4 addresses, 2023-04-09
http://0x80.pl/notesen/2023-04-09-faster-parse-ipv4.html

In this article, Mula describes several SIMD-based algorithm. Our approach is similar with 
the following differences:

1. Mula distinguishes between the case where we have no 3-digit numbers, or no 2-digit numbers. 
   We have a single case (less branching). We don't expect that this optimization, by Mula, 
   is very useful in practice.
2. Mula has a distinct step to check that the input is all digits and dots. We integrate this check 
   in the main computation.
3. Mula relies on two different hashing functions, an expensive one that reduces memory usage, and 
   a fast one that leads to more memory usage. We opted for a difference choice that is nearly as 
   compact as the slow Mula hashing and nearly as fast as the fastest Mula routine.
4. We validate by comparing the expected length of the address (which we get from the mask) instead 
   of Mula's approach of validating by store the dot mask.

Overall, we get a performance that is similar or better than the fastest Mula's scheme, while using 
relatively little memory.

*/

static const uint8_t patterns_id[256] = {
    38,  65,  255, 56,  73,  255, 255, 255, 255, 255, 255, 3,   255, 255, 6,
    255, 255, 9,   255, 27,  255, 12,  30,  255, 255, 255, 255, 15,  255, 33,
    255, 255, 255, 255, 18,  36,  255, 255, 255, 54,  21,  255, 39,  255, 255,
    57,  255, 255, 255, 255, 255, 255, 255, 255, 24,  42,  255, 255, 255, 60,
    255, 255, 255, 255, 255, 255, 255, 255, 45,  255, 255, 63,  255, 255, 255,
    255, 255, 255, 255, 255, 255, 48,  53,  255, 255, 66,  71,  255, 255, 16,
    255, 34,  255, 255, 255, 255, 255, 255, 255, 52,  255, 255, 22,  70,  40,
    255, 255, 58,  51,  255, 255, 69,  255, 255, 255, 255, 255, 255, 255, 255,
    255, 5,   255, 255, 255, 255, 255, 255, 11,  29,  46,  255, 255, 64,  255,
    255, 72,  0,   77,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 76,  255, 255, 255, 255, 255, 255, 255, 75,  255,
    80,  255, 255, 255, 26,  255, 44,  255, 7,   62,  255, 255, 25,  255, 43,
    13,  31,  61,  255, 255, 255, 255, 255, 255, 255, 255, 255, 2,   19,  37,
    255, 255, 50,  55,  79,  68,  255, 255, 255, 255, 49,  255, 255, 67,  255,
    255, 255, 255, 17,  255, 35,  78,  255, 4,   255, 255, 255, 255, 255, 255,
    10,  23,  28,  41,  255, 255, 59,  255, 255, 255, 8,   255, 255, 255, 255,
    255, 1,   14,  32,  255, 255, 255, 255, 255, 255, 255, 255, 74,  255, 47,
    20,
};

static const uint8_t patterns[81][16] = {
  {0, 128, 2, 128, 4, 128, 6, 128, 128, 128, 128, 128, 128, 128, 128, 128},
  {0, 128, 2, 128, 4, 128, 7, 6, 128, 128, 128, 128, 128, 128, 128, 6},
  {0, 128, 2, 128, 4, 128, 8, 7, 128, 128, 128, 128, 128, 128, 6, 6},
  {0, 128, 2, 128, 5, 4, 7, 128, 128, 128, 128, 128, 128, 4, 128, 128},
  {0, 128, 2, 128, 5, 4, 8, 7, 128, 128, 128, 128, 128, 4, 128, 7},
  {0, 128, 2, 128, 5, 4, 9, 8, 128, 128, 128, 128, 128, 4, 7, 7},
  {0, 128, 2, 128, 6, 5, 8, 128, 128, 128, 128, 128, 4, 4, 128, 128},
  {0, 128, 2, 128, 6, 5, 9, 8, 128, 128, 128, 128, 4, 4, 128, 8},
  {0, 128, 2, 128, 6, 5, 10, 9, 128, 128, 128, 128, 4, 4, 8, 8},
  {0, 128, 3, 2, 5, 128, 7, 128, 128, 128, 128, 2, 128, 128, 128, 128},
  {0, 128, 3, 2, 5, 128, 8, 7, 128, 128, 128, 2, 128, 128, 128, 7},
  {0, 128, 3, 2, 5, 128, 9, 8, 128, 128, 128, 2, 128, 128, 7, 7},
  {0, 128, 3, 2, 6, 5, 8, 128, 128, 128, 128, 2, 128, 5, 128, 128},
  {0, 128, 3, 2, 6, 5, 9, 8, 128, 128, 128, 2, 128, 5, 128, 8},
  {0, 128, 3, 2, 6, 5, 10, 9, 128, 128, 128, 2, 128, 5, 8, 8},
  {0, 128, 3, 2, 7, 6, 9, 128, 128, 128, 128, 2, 5, 5, 128, 128},
  {0, 128, 3, 2, 7, 6, 10, 9, 128, 128, 128, 2, 5, 5, 128, 9},
  {0, 128, 3, 2, 7, 6, 11, 10, 128, 128, 128, 2, 5, 5, 9, 9},
  {0, 128, 4, 3, 6, 128, 8, 128, 128, 128, 2, 2, 128, 128, 128, 128},
  {0, 128, 4, 3, 6, 128, 9, 8, 128, 128, 2, 2, 128, 128, 128, 8},
  {0, 128, 4, 3, 6, 128, 10, 9, 128, 128, 2, 2, 128, 128, 8, 8},
  {0, 128, 4, 3, 7, 6, 9, 128, 128, 128, 2, 2, 128, 6, 128, 128},
  {0, 128, 4, 3, 7, 6, 10, 9, 128, 128, 2, 2, 128, 6, 128, 9},
  {0, 128, 4, 3, 7, 6, 11, 10, 128, 128, 2, 2, 128, 6, 9, 9},
  {0, 128, 4, 3, 8, 7, 10, 128, 128, 128, 2, 2, 6, 6, 128, 128},
  {0, 128, 4, 3, 8, 7, 11, 10, 128, 128, 2, 2, 6, 6, 128, 10},
  {0, 128, 4, 3, 8, 7, 12, 11, 128, 128, 2, 2, 6, 6, 10, 10},
  {1, 0, 3, 128, 5, 128, 7, 128, 128, 0, 128, 128, 128, 128, 128, 128},
  {1, 0, 3, 128, 5, 128, 8, 7, 128, 0, 128, 128, 128, 128, 128, 7},
  {1, 0, 3, 128, 5, 128, 9, 8, 128, 0, 128, 128, 128, 128, 7, 7},
  {1, 0, 3, 128, 6, 5, 8, 128, 128, 0, 128, 128, 128, 5, 128, 128},
  {1, 0, 3, 128, 6, 5, 9, 8, 128, 0, 128, 128, 128, 5, 128, 8},
  {1, 0, 3, 128, 6, 5, 10, 9, 128, 0, 128, 128, 128, 5, 8, 8},
  {1, 0, 3, 128, 7, 6, 9, 128, 128, 0, 128, 128, 5, 5, 128, 128},
  {1, 0, 3, 128, 7, 6, 10, 9, 128, 0, 128, 128, 5, 5, 128, 9},
  {1, 0, 3, 128, 7, 6, 11, 10, 128, 0, 128, 128, 5, 5, 9, 9},
  {1, 0, 4, 3, 6, 128, 8, 128, 128, 0, 128, 3, 128, 128, 128, 128},
  {1, 0, 4, 3, 6, 128, 9, 8, 128, 0, 128, 3, 128, 128, 128, 8},
  {1, 0, 4, 3, 6, 128, 10, 9, 128, 0, 128, 3, 128, 128, 8, 8},
  {1, 0, 4, 3, 7, 6, 9, 128, 128, 0, 128, 3, 128, 6, 128, 128},
  {1, 0, 4, 3, 7, 6, 10, 9, 128, 0, 128, 3, 128, 6, 128, 9},
  {1, 0, 4, 3, 7, 6, 11, 10, 128, 0, 128, 3, 128, 6, 9, 9},
  {1, 0, 4, 3, 8, 7, 10, 128, 128, 0, 128, 3, 6, 6, 128, 128},
  {1, 0, 4, 3, 8, 7, 11, 10, 128, 0, 128, 3, 6, 6, 128, 10},
  {1, 0, 4, 3, 8, 7, 12, 11, 128, 0, 128, 3, 6, 6, 10, 10},
  {1, 0, 5, 4, 7, 128, 9, 128, 128, 0, 3, 3, 128, 128, 128, 128},
  {1, 0, 5, 4, 7, 128, 10, 9, 128, 0, 3, 3, 128, 128, 128, 9},
  {1, 0, 5, 4, 7, 128, 11, 10, 128, 0, 3, 3, 128, 128, 9, 9},
  {1, 0, 5, 4, 8, 7, 10, 128, 128, 0, 3, 3, 128, 7, 128, 128},
  {1, 0, 5, 4, 8, 7, 11, 10, 128, 0, 3, 3, 128, 7, 128, 10},
  {1, 0, 5, 4, 8, 7, 12, 11, 128, 0, 3, 3, 128, 7, 10, 10},
  {1, 0, 5, 4, 9, 8, 11, 128, 128, 0, 3, 3, 7, 7, 128, 128},
  {1, 0, 5, 4, 9, 8, 12, 11, 128, 0, 3, 3, 7, 7, 128, 11},
  {1, 0, 5, 4, 9, 8, 13, 12, 128, 0, 3, 3, 7, 7, 11, 11},
  {2, 1, 4, 128, 6, 128, 8, 128, 0, 0, 128, 128, 128, 128, 128, 128},
  {2, 1, 4, 128, 6, 128, 9, 8, 0, 0, 128, 128, 128, 128, 128, 8},
  {2, 1, 4, 128, 6, 128, 10, 9, 0, 0, 128, 128, 128, 128, 8, 8},
  {2, 1, 4, 128, 7, 6, 9, 128, 0, 0, 128, 128, 128, 6, 128, 128},
  {2, 1, 4, 128, 7, 6, 10, 9, 0, 0, 128, 128, 128, 6, 128, 9},
  {2, 1, 4, 128, 7, 6, 11, 10, 0, 0, 128, 128, 128, 6, 9, 9},
  {2, 1, 4, 128, 8, 7, 10, 128, 0, 0, 128, 128, 6, 6, 128, 128},
  {2, 1, 4, 128, 8, 7, 11, 10, 0, 0, 128, 128, 6, 6, 128, 10},
  {2, 1, 4, 128, 8, 7, 12, 11, 0, 0, 128, 128, 6, 6, 10, 10},
  {2, 1, 5, 4, 7, 128, 9, 128, 0, 0, 128, 4, 128, 128, 128, 128},
  {2, 1, 5, 4, 7, 128, 10, 9, 0, 0, 128, 4, 128, 128, 128, 9},
  {2, 1, 5, 4, 7, 128, 11, 10, 0, 0, 128, 4, 128, 128, 9, 9},
  {2, 1, 5, 4, 8, 7, 10, 128, 0, 0, 128, 4, 128, 7, 128, 128},
  {2, 1, 5, 4, 8, 7, 11, 10, 0, 0, 128, 4, 128, 7, 128, 10},
  {2, 1, 5, 4, 8, 7, 12, 11, 0, 0, 128, 4, 128, 7, 10, 10},
  {2, 1, 5, 4, 9, 8, 11, 128, 0, 0, 128, 4, 7, 7, 128, 128},
  {2, 1, 5, 4, 9, 8, 12, 11, 0, 0, 128, 4, 7, 7, 128, 11},
  {2, 1, 5, 4, 9, 8, 13, 12, 0, 0, 128, 4, 7, 7, 11, 11},
  {2, 1, 6, 5, 8, 128, 10, 128, 0, 0, 4, 4, 128, 128, 128, 128},
  {2, 1, 6, 5, 8, 128, 11, 10, 0, 0, 4, 4, 128, 128, 128, 10},
  {2, 1, 6, 5, 8, 128, 12, 11, 0, 0, 4, 4, 128, 128, 10, 10},
  {2, 1, 6, 5, 9, 8, 11, 128, 0, 0, 4, 4, 128, 8, 128, 128},
  {2, 1, 6, 5, 9, 8, 12, 11, 0, 0, 4, 4, 128, 8, 128, 11},
  {2, 1, 6, 5, 9, 8, 13, 12, 0, 0, 4, 4, 128, 8, 11, 11},
  {2, 1, 6, 5, 10, 9, 12, 128, 0, 0, 4, 4, 8, 8, 128, 128},
  {2, 1, 6, 5, 10, 9, 13, 12, 0, 0, 4, 4, 8, 8, 128, 12},
  {2, 1, 6, 5, 10, 9, 14, 13, 0, 0, 4, 4, 8, 8, 12, 12},
};

int sse_inet_aton(const char* ipv4_string, const size_t ipv4_string_length, uint32_t * destination) {
  // This function always reads 16 bytes. With AVX-512 we can do a mask
  // load, but it is not generally available with SSE 4.1.
  const __m128i input = _mm_loadu_si128((const __m128i *)ipv4_string);
  if (ipv4_string_length > 15) {
    return 0;
  }
  // locate dots
  uint16_t dotmask;
  {
    const __m128i dot = _mm_set1_epi8('.');
    const __m128i t0 = _mm_cmpeq_epi8(input, dot);
    dotmask = (uint16_t)_mm_movemask_epi8(t0);
    uint16_t mask = (uint16_t)(1 << ipv4_string_length);
    dotmask &= mask - 1;
    dotmask |= mask;
  }

  // build a hashcode

  const uint8_t hashcode = (uint8_t)((6639 * dotmask) >> 13);
  // grab the index of the shuffle mask
  const uint8_t id = patterns_id[hashcode];
  if (id >= 81) {
    return 0;
  }
  const uint8_t *pat = &patterns[id][0];

  const __m128i pattern = _mm_loadu_si128((const __m128i *)pat);
  // The value of the shuffle mask at a specific index points at the last digit,
  // we check that it matches the length of the input.
  const __m128i ascii0 = _mm_set1_epi8('0');
  const __m128i t0 = input;

  __m128i t1 = _mm_shuffle_epi8(t0, pattern);
  // check that leading digits of 2- 3- numbers are not zeros.
  {
    const __m128i eq0 = _mm_cmpeq_epi8(t1, ascii0);
    if (!_mm_testz_si128(eq0, _mm_set_epi8(-1, 0, -1, 0, -1, 0, -1, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0))) {
      return 0;
    }
  }
  // replace null values with '0'
  __m128i t1b = _mm_blendv_epi8(t1, ascii0, pattern);

  // subtract '0'
  const __m128i t2 = _mm_sub_epi8(t1b, ascii0);
  // check that everything was in the range '0' to '9'
  {
    // There are two ways to do it, the second one saves one instruction
    // under LLVM clang 16. Under both GCC and clang, they have the same performance:
    /*const __m128i c9 = _mm_set1_epi8('9' - '0');
    const __m128i t2m = _mm_max_epu8(t2, c9);
    const __m128i t2me = _mm_cmpeq_epi8(t2m, c9);
    if (!_mm_test_all_ones(t2me)) {
      return 0;
    }*/
    const __m128i t2z = _mm_add_epi8(t2,_mm_set1_epi8(-128) );
    const __m128i c9 = _mm_set1_epi8('9' - '0' - 128);
    const __m128i t2me = _mm_cmpgt_epi8(t2z, c9);
    if (!_mm_test_all_zeros(t2me, t2me)) {
      return 0;
    }
  }
  // We do the computation, the Mula way.
  const __m128i weights =
      _mm_setr_epi8(1, 10, 1, 10, 1, 10, 1, 10, 100, 0, 100, 0, 100, 0, 100, 0);
  const __m128i t3 = _mm_maddubs_epi16(t2, weights);
  // In t3, we have 8 16-bit values, the first four combine the two first digits, and
  // the 4 next 16-bit valued are made of the third digits.
  const __m128i t4 = _mm_alignr_epi8(t3, t3, 8);
  const __m128i t5 = _mm_add_epi16(t4, t3);
  // Test that we don't overflow (over 255)
  if (!_mm_testz_si128(t5, _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1,
                                        0, -1, 0, -1, 0))) {
    return 0;
  }
  // pack and we are done!
  const __m128i t6 = _mm_packus_epi16(t5, t5);
  *destination = (uint32_t)_mm_cvtsi128_si32(t6);
  return ((int)ipv4_string_length - (int)pat[6]);
}


int sse_inet_aton_16(const char* ipv4_string, uint32_t * destination, size_t* restrict ipv4_string_length) {
  const __m128i input = _mm_loadu_si128((const __m128i *)ipv4_string);
  const __m128i dot = _mm_set1_epi8('.');
  // locate dots
  uint16_t dotmask;
  {
    const __m128i ascii0_9 = _mm_setr_epi8(
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 0, 0, 0, 0, 0, 0);
    const __m128i digits = _mm_cmpeq_epi8(input, _mm_shuffle_epi8(ascii0_9, input));
    
    const __m128i t0 = _mm_cmpeq_epi8(input, dot);
    dotmask = (uint16_t)_mm_movemask_epi8(t0);
    const uint16_t digit_mask = (uint16_t)_mm_movemask_epi8(digits);
    uint16_t m = digit_mask | dotmask;
    // credit @aqrit
    m ^= (m + 1); // mask of lowest clear bit and below
    dotmask = ~digit_mask & m;
    *ipv4_string_length = (size_t)__builtin_popcount(m) - 1;
  }
  // build a hashcode
  const uint8_t hashcode = (uint8_t)((6639 * dotmask) >> 13);
  // grab the index of the shuffle mask
  const uint8_t id = patterns_id[hashcode];
  if (id >= 81) {
    return 0;
  }
  const uint8_t *pat = &patterns[id][0];

  const __m128i pattern = _mm_loadu_si128((const __m128i *)pat);
  // The value of the shuffle mask at a specific index points at the last digit,
  // we check that it matches the length of the input.
  const __m128i ascii0 = _mm_set1_epi8('0');
  const __m128i t0 = input;

  __m128i t1 = _mm_shuffle_epi8(t0, pattern);
  // check that leading digits of 2- 3- numbers are not zeros.
  {
    const __m128i eq0 = _mm_cmpeq_epi8(t1, ascii0);
    if (!_mm_testz_si128(eq0, _mm_set_epi8(-1, 0, -1, 0, -1, 0, -1, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0))) {
      return 0;
    }
  }
  // subtract '0'
  const __m128i t2 = _mm_subs_epu8(t1, ascii0);
  // check that there is no dot
  {
    const __m128i t2me = _mm_cmpeq_epi8(t1, dot);
    if (!_mm_test_all_zeros(t2me, t2me)) {
      return 0;
    }
  }
  // We do the computation, the Mula way.
  const __m128i weights =
      _mm_setr_epi8(1, 10, 1, 10, 1, 10, 1, 10, 100, 0, 100, 0, 100, 0, 100, 0);
  const __m128i t3 = _mm_maddubs_epi16(t2, weights);
  // In t3, we have 8 16-bit values, the first four combine the two first digits, and
  // the 4 next 16-bit valued are made of the third digits.
  const __m128i t4 = _mm_alignr_epi8(t3, t3, 8);
  const __m128i t5 = _mm_add_epi16(t4, t3);
  // Test that we don't overflow (over 255)
  if (!_mm_testz_si128(t5, _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1,
                                        0, -1, 0, -1, 0))) {
    return 0;
  }
  // pack and we are done!
  const __m128i t6 = _mm_packus_epi16(t5, t5);
  *destination = (uint32_t)_mm_cvtsi128_si32(t6);
  return (int)(*ipv4_string_length - (int)pat[6]);
}


int sse_inet_aton_16_branchless(const char* ipv4_string, uint32_t * destination, size_t* restrict ipv4_string_length) {

    __m128i v = _mm_loadu_si128((const __m128i *)ipv4_string);

    __m128i is_dot = _mm_cmpeq_epi8(v, _mm_set1_epi8(0x2E));
    uint32_t dot_mask = (uint32_t)_mm_movemask_epi8(is_dot);

    // set non-digits to 0x80..0x89, set digits to 0x00..0x09
    const __m128i saturation_distance = _mm_set1_epi8(0x76); // 0x7F - 9
    v = _mm_xor_si128(v, _mm_set1_epi8(0x30)); // ascii '0'
    v = _mm_adds_epu8(v, saturation_distance);
    uint32_t non_digit_mask = (uint32_t)_mm_movemask_epi8(v);
    v = _mm_subs_epi8(v, saturation_distance);

    uint32_t bad_mask = dot_mask ^ non_digit_mask;
    uint32_t clip_mask = bad_mask ^ (bad_mask - 1);
    uint32_t partition_mask = non_digit_mask & clip_mask;

    const uint32_t length = (uint32_t)_mm_popcnt_u32(clip_mask) - 1;

    uint32_t hash_key = (partition_mask * 0x00CF7800) >> 24;
    uint8_t hash_id = patterns_id[hash_key];
    if (hash_id >= 81) {
        return 0;
    }
    const uint8_t* const pattern_ptr = &patterns[hash_id][0];

    __m128i shuf = _mm_loadu_si128((const __m128i *)pattern_ptr);
    v = _mm_shuffle_epi8(v, shuf);

    const __m128i mul_weights =
        _mm_set_epi8(0,100, 0,100, 0,100, 0,100, 10,1, 10,1, 10,1, 10,1);
    __m128i acc = _mm_maddubs_epi16(mul_weights, v);
    __m128i swapped = _mm_shuffle_epi32(acc, _MM_SHUFFLE(1,0,3,2));
    acc = _mm_adds_epu16(acc, swapped);

    // check `v` for leading zeros in each partition, ignore lanes if partition has only one digit
    // if hibyte of `acc` then bad_char or overflow
    __m128i check_lz = _mm_xor_si128(_mm_cmpeq_epi8(_mm_setzero_si128(), v), shuf);
    __m128i check_of = _mm_adds_epu16(_mm_set1_epi16(0x7F00), acc);
    __m128i checks = _mm_or_si128(check_lz, check_of);
    uint32_t check_mask = (uint32_t)_mm_movemask_epi8(checks);
    check_mask &= 0x0000AA00; // the only lanes wanted

    // pack and we are done!
    uint32_t address = (uint32_t)_mm_cvtsi128_si32(_mm_packus_epi16(acc, acc));
    *ipv4_string_length = length;
    memcpy(destination, &address, 4);
    return (int)(length + check_mask - pattern_ptr[6]);
}