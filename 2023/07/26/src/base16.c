
#include "base16.h"

#include <stdint.h>
#include <x86intrin.h> // update if we need to support Windows.

/// Source: Faster Base64 Encoding and Decoding Using AVX2 Instructions,
///         https://arxiv.org/abs/1704.00605
size_t base16hex_simd(uint8_t *dst, const uint8_t *src) {
  static int8_t zero_masks[32] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                  0,  0,  0,  0,  0,  -1, -1, -1, -1, -1, -1,
                                  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  bool valid = true;
  const __m128i delta_check = _mm_setr_epi8(-16, -32, -47, 71, 58, -96, 26,
                                            -128, 0, 0, 0, 0, 0, 0, 0, 0);
  const __m128i delta_rebase =
      _mm_setr_epi8(0, 0, -47, -47, -54, 0, -86, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  const uint8_t *srcinit = src;
  do {
    __m128i v = _mm_loadu_si128((__m128i *)src);
    __m128i vm1 = _mm_add_epi8(v, _mm_set1_epi8(-1));
    __m128i hash_key =
        _mm_and_si128(_mm_srli_epi32(vm1, 4), _mm_set1_epi8(0x0F));
    __m128i check = _mm_add_epi8(_mm_shuffle_epi8(delta_check, hash_key), vm1);
    v = _mm_add_epi8(vm1, _mm_shuffle_epi8(delta_rebase, hash_key));
    unsigned int m = (unsigned)_mm_movemask_epi8(check);
    if (m) {
      int length = __builtin_ctzll(m);
      if (length == 0) {
        break;
      }
      src += length;
      __m128i zero_mask =
          _mm_loadu_si128((__m128i *)(zero_masks + 16 - length));
      v = _mm_andnot_si128(zero_mask, v);
      valid = false;
    } else { // common case
      src += 16;
    }
    const __m128i t3 = _mm_maddubs_epi16(v, _mm_set1_epi16(0x0110));
    const __m128i t5 =
        _mm_shuffle_epi8(t3, _mm_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, -1, -1,
                                           -1, -1, -1, -1, -1, -1));

    /**
    // Alternative: has four instructions.

  const __m128i t1 = _mm_srli_epi32(v, 4);
  const __m128i t3 = _mm_or_si128(t0, t1);
  const __m128i t4 = _mm_and_si128(
      t3, _mm_set1_epi16(0x00ff)); // keep just lower bytes in words
  const __m128i t5 =
      _mm_packus_epi16(t4, t4); // no
    */
    _mm_storeu_si128((__m128i *)dst, t5);
    dst += 8;
  } while (valid);
  return (size_t)(src - srcinit);
}

// Parsing hex numbers with validation
// http://0x80.pl/notesen/2022-01-17-validating-hex-parse.html
size_t base16hex_simd_geoff(uint8_t *dst, const uint8_t *src) {
  static int8_t zero_masks[32] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                  0,  0,  0,  0,  0,  -1, -1, -1, -1, -1, -1,
                                  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  bool valid = true;
  const uint8_t *srcinit = src;

  do {
    __m128i v = _mm_loadu_si128((const __m128i *)src);
    const __m128i t1 = _mm_add_epi8(
        v, _mm_set1_epi8((
               char)(0xff - '9'))); // puts 0-9 at top of unsigned integer range
    const __m128i t2 =
        _mm_subs_epu8(t1, _mm_set1_epi8(6)); // now, put 6 blank spots above 0-9
    const __m128i t3 = _mm_sub_epi8(t2, _mm_set1_epi8((char)(0xf0)));
    const __m128i t4 =
        _mm_and_si128(v, _mm_set1_epi8((char)(0xdf))); // squash case
    const __m128i t5 =
        _mm_sub_epi8(t4, _mm_set1_epi8('A')); // put the 'A-F' at 0..5
    const __m128i t6 = _mm_adds_epu8(
        t5, _mm_set1_epi8(10)); // now put A-F at 10..15, with a guarantee that
                                // nothing is smaller
    __m128i t7 = _mm_min_epu8(t3, t6); // t7 is normalized hex nibbles
    const __m128i t8 = _mm_adds_epu8(
        t7,
        _mm_set1_epi8(
            127 - 15)); // t8 has high_bit == 1 iff what we had wasn't valid hex

    unsigned int m = (unsigned)_mm_movemask_epi8(t8);
    if (m) {
      int length = __builtin_ctzll(m);
      if (length == 0) {
        break;
      }
      src += length;
      __m128i zero_mask =
          _mm_loadu_si128((__m128i *)(zero_masks + 16 - length));
      t7 = _mm_andnot_si128(zero_mask, t7);
      valid = false;
    } else { // common case
      src += 16;
    }

    // now each byte of result have value 0 .. 15, we're going to merge nibbles:
    {
      const __m128i t0 = _mm_maddubs_epi16(t7, _mm_set1_epi16(0x0110));
      const __m128i t1 = _mm_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, -1, -1, -1,
                                       -1, -1, -1, -1, -1);
      const __m128i t3 = _mm_shuffle_epi8(t0, t1);
      _mm_storeu_si128((__m128i *)dst, t3);
      dst += 8;
    }
  } while (valid);
  return (size_t)(src - srcinit);
}
size_t base16hex_simple(uint8_t *dst, const uint8_t *src) {

  static const signed char digittoval[256] = {
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,
      6,  7,  8,  9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1};
  const uint8_t *srcinit = src;
  do {
    // assume that they come in pairs
    int8_t n1 = digittoval[src[0]];
    int8_t n2 = digittoval[src[1]];
    if ((n1 < 0) || (n2 < 0)) {
      break;
    }
    src += 2;
    *dst = (uint8_t)((n1 << 4) | n2);
    dst += 1;
  } while (true);
  return (size_t)(src - srcinit);
}