//Don't use this code, go to https://github.com/lemire/fastvalidate-utf-8
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"
#include <x86intrin.h>
/*
 * legal utf-8 byte sequence
 * http://www.unicode.org/versions/Unicode6.0.0/ch03.pdf - page 94
 *
 *  Code Points        1st       2s       3s       4s
 * U+0000..U+007F     00..7F
 * U+0080..U+07FF     C2..DF   80..BF
 * U+0800..U+0FFF     E0       A0..BF   80..BF
 * U+1000..U+CFFF     E1..EC   80..BF   80..BF
 * U+D000..U+D7FF     ED       80..9F   80..BF
 * U+E000..U+FFFF     EE..EF   80..BF   80..BF
 * U+10000..U+3FFFF   F0       90..BF   80..BF   80..BF
 * U+40000..U+FFFFF   F1..F3   80..BF   80..BF   80..BF
 * U+100000..U+10FFFF F4       80..8F   80..BF   80..BF
 *
 */

// Copyright (c) 2008-2010 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1
#define SHIFTLESS_UTF8_REJECT 16


static const uint8_t utf8d[] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 00..1f
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 20..3f
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 40..5f
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 60..7f
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   9,   9,   9,   9,   9,   9,
    9,   9,   9,   9,   9,   9,   9,   9,   9,   9, // 80..9f
    7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
    7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
    7,   7,   7,   7,   7,   7,   7,   7,   7,   7, // a0..bf
    8,   8,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2, // c0..df
    0xa, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
    0x3, 0x3, 0x4, 0x3, 0x3, // e0..ef
    0xb, 0x6, 0x6, 0x6, 0x5, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,
    0x8, 0x8, 0x8, 0x8, 0x8 // f0..ff
};
  static const uint8_t utf8d_transition[] = {
    0x0, 0x1, 0x2, 0x3, 0x5, 0x8, 0x7, 0x1, 0x1, 0x1, 0x4,
    0x6, 0x1, 0x1, 0x1, 0x1, // s0..s0
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   0,   1,   1,   1,   1,
    1,   0,   1,   0,   1,   1,   1,   1,   1,   1, // s1..s2
    1,   2,   1,   1,   1,   1,   1,   2,   1,   2,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   2,   1,   1,   1,   1,   1,   1,   1,   1, // s3..s4
    1,   2,   1,   1,   1,   1,   1,   1,   1,   2,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   3,   1,   3,   1,   1,   1,   1,   1,   1, // s5..s6
    1,   3,   1,   1,   1,   1,   1,   3,   1,   3,   1,
    1,   1,   1,   1,   1,   1,   3,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1, // s7..s8
};
static const uint8_t shifted_utf8d_transition[] = {0x0, 0x10, 0x20, 0x30, 0x50, 0x80, 0x70, 0x10, 0x10, 0x10, 0x40, 0x60, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0, 0x10, 0x0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
};
static uint32_t inline decode(uint32_t *state, uint32_t *codep, uint32_t byte) {
  uint32_t type = utf8d[byte];
  *codep = (*state != UTF8_ACCEPT) ? (byte & 0x3fu) | (*codep << 6)
                                   : (0xff >> type) & (byte);
  *state = utf8d_transition[16 * *state + type];
  return *state;
}

static uint32_t inline shiftless_decode(uint32_t *state, uint32_t *codep, uint32_t byte) {
  uint32_t type = utf8d[byte];
  *codep = (*state != UTF8_ACCEPT) ? (byte & 0x3fu) | (*codep << 6)
                                   : (0xff >> type) & (byte);
  *state = shifted_utf8d_transition[*state + type];
  return *state;
}

static uint32_t inline updatestate(uint32_t *state, uint32_t byte) {
  uint32_t type = utf8d[byte];
  *state = utf8d_transition[16 * *state + type];
  return *state;
}

static uint32_t inline shiftless_updatestate(uint32_t *state, uint32_t byte) {
  uint32_t type = utf8d[byte];
  *state = shifted_utf8d_transition[*state + type];
  return *state;
}


bool is_ascii(const char *c, size_t len) {
  for (size_t i = 0; i < len; i++) {
    if(c[i] < 0) return false;
  }
  return true;
}


bool validate_utf8_branchless(const char *c, size_t len) {
  const unsigned char *cu = (const unsigned char *)c;
  uint32_t state = 0;
  for (size_t i = 0; i < len; i++) {
    uint32_t byteval = (uint32_t)cu[i];
    updatestate(&state, byteval);
  }
  return state != UTF8_REJECT;
}

bool shiftless_validate_utf8_branchless(const char *c, size_t len) {
  const unsigned char *cu = (const unsigned char *)c;
  uint32_t state = 0;
  for (size_t i = 0; i < len; i++) {
    uint32_t byteval = (uint32_t)cu[i];
    shiftless_updatestate(&state, byteval);
  }
  return state != SHIFTLESS_UTF8_REJECT;
}

bool validate_utf8(const char *c, size_t len) {
  const unsigned char *cu = (const unsigned char *)c;
  uint32_t state = 0;
  for (size_t i = 0; i < len; i++) {
    uint32_t byteval = (uint32_t)cu[i];
    if (updatestate(&state, byteval) == UTF8_REJECT)
      return false;
  }
  return true;
}

bool shiftless_validate_utf8(const char *c, size_t len) {
  const unsigned char *cu = (const unsigned char *)c;
  uint32_t state = 0;
  for (size_t i = 0; i < len; i++) {
    uint32_t byteval = (uint32_t)cu[i];
    if (shiftless_updatestate(&state, byteval) == SHIFTLESS_UTF8_REJECT)
      return false;
  }
  return true;
}

// credit: Travis Downs
bool validate_utf8_double(const char *c, size_t len) {
  size_t half = len / 2;
  while((unsigned char )c[half] <= 0xBF && (unsigned char ) c[half] > 0x80 && half > 0 ) {
    half --;
  }
  uint32_t s1 = 0, s2 = 0;
  for (size_t i = 0, j = half; i < half; i++, j++) {
    updatestate(&s1, (unsigned char )c[i]);
    updatestate(&s2, (unsigned char )c[j]);
  }
  for (int j = half * 2; j < len; j++) {
    updatestate(&s2, (unsigned char )c[j]);
  }
  return (s1 != UTF8_REJECT) && (s2 != UTF8_REJECT);
}


// credit: Travis Downs
bool shiftless_validate_utf8_double(const char *c, size_t len) {
  size_t half = len / 2;
  while((unsigned char )c[half] <= 0xBF && (unsigned char ) c[half] > 0x80 && half > 0 ) {
    half --;
  }
  uint32_t s1 = 0, s2 = 0;
  for (size_t i = 0, j = half; i < half; i++, j++) {
    shiftless_updatestate(&s1, (unsigned char )c[i]);
    shiftless_updatestate(&s2, (unsigned char )c[j]);
  }
  for (int j = half * 2; j < len; j++) {
    shiftless_updatestate(&s2, (unsigned char )c[j]);
  }
  return (s1 != SHIFTLESS_UTF8_REJECT) && (s2 != SHIFTLESS_UTF8_REJECT);
}
// can be vectorized: https://woboq.com/blog/utf-8-processing-using-simd.html
bool validate_utf8_sse_nocheating(const char *src, size_t len) {
  const char *end = src + len;
  while (src + 16 < end) {
    __m128i chunk = _mm_loadu_si128((const __m128i *)(src));

    int asciiMask = _mm_movemask_epi8(chunk);

    __m128i chunk_signed = _mm_add_epi8(chunk, _mm_set1_epi8(0x80));
    __m128i cond2 =
        _mm_cmplt_epi8(_mm_set1_epi8(0xc2 - 1 - 0x80), chunk_signed);
    __m128i state = _mm_set1_epi8((char)(0x0 | 0x80));
    state = _mm_blendv_epi8(state, _mm_set1_epi8((char)(0x2 | 0xc0)), cond2);

    __m128i cond3 =
        _mm_cmplt_epi8(_mm_set1_epi8(0xe0 - 1 - 0x80), chunk_signed);

    state = _mm_blendv_epi8(state, _mm_set1_epi8((char)(0x3 | 0xe0)), cond3);
    __m128i mask3 = _mm_slli_si128(cond3, 1);

    __m128i cond4 =
        _mm_cmplt_epi8(_mm_set1_epi8(0xf0 - 1 - 0x80), chunk_signed);

    // Fall back to the scalar processing
    if (_mm_movemask_epi8(cond4)) {
      break;
    }

    __m128i count = _mm_and_si128(state, _mm_set1_epi8(0x7));

    __m128i count_sub1 = _mm_subs_epu8(count, _mm_set1_epi8(0x1));

    __m128i counts = _mm_add_epi8(count, _mm_slli_si128(count_sub1, 1));

    __m128i shifts = count_sub1;
    shifts = _mm_add_epi8(shifts, _mm_slli_si128(shifts, 1));
    counts = _mm_add_epi8(
        counts, _mm_slli_si128(_mm_subs_epu8(counts, _mm_set1_epi8(0x2)), 2));
    shifts = _mm_add_epi8(shifts, _mm_slli_si128(shifts, 2));

    if (asciiMask ^ _mm_movemask_epi8(_mm_cmpgt_epi8(counts, _mm_set1_epi8(0))))
      return false; // error
    shifts = _mm_add_epi8(shifts, _mm_slli_si128(shifts, 4));

    if (_mm_movemask_epi8(_mm_cmpgt_epi8(
            _mm_sub_epi8(_mm_slli_si128(counts, 1), counts), _mm_set1_epi8(1))))
      return false; // error

    shifts = _mm_add_epi8(shifts, _mm_slli_si128(shifts, 8));

    __m128i mask = _mm_and_si128(state, _mm_set1_epi8(0xf8));
    shifts =
        _mm_and_si128(shifts, _mm_cmplt_epi8(counts, _mm_set1_epi8(2))); // <=1

    chunk =
        _mm_andnot_si128(mask, chunk); // from now on, we only have usefull bits

    shifts = _mm_blendv_epi8(shifts, _mm_srli_si128(shifts, 1),
                             _mm_srli_si128(_mm_slli_epi16(shifts, 7), 1));

    __m128i chunk_right = _mm_slli_si128(chunk, 1);

    __m128i chunk_low = _mm_blendv_epi8(
        chunk,
        _mm_or_si128(chunk, _mm_and_si128(_mm_slli_epi16(chunk_right, 6),
                                          _mm_set1_epi8(0xc0))),
        _mm_cmpeq_epi8(counts, _mm_set1_epi8(1)));

    __m128i chunk_high =
        _mm_and_si128(chunk, _mm_cmpeq_epi8(counts, _mm_set1_epi8(2)));

    shifts = _mm_blendv_epi8(shifts, _mm_srli_si128(shifts, 2),
                             _mm_srli_si128(_mm_slli_epi16(shifts, 6), 2));
    chunk_high = _mm_srli_epi32(chunk_high, 2);

    shifts = _mm_blendv_epi8(shifts, _mm_srli_si128(shifts, 4),
                             _mm_srli_si128(_mm_slli_epi16(shifts, 5), 4));
    chunk_high = _mm_or_si128(
        chunk_high, _mm_and_si128(_mm_and_si128(_mm_slli_epi32(chunk_right, 4),
                                                _mm_set1_epi8(0xf0)),
                                  mask3));
    int c = _mm_extract_epi16(counts, 7);
    int source_advance = !(c & 0x0200) ? 16 : !(c & 0x02) ? 15 : 14;

    __m128i high_bits = _mm_and_si128(chunk_high, _mm_set1_epi8(0xf8));
    if (!_mm_testz_si128(
            mask3,
            _mm_or_si128(_mm_cmpeq_epi8(high_bits, _mm_set1_epi8(0x00)),
                         _mm_cmpeq_epi8(high_bits, _mm_set1_epi8(0xd8)))))
      return false;

    shifts = _mm_blendv_epi8(shifts, _mm_srli_si128(shifts, 8),
                             _mm_srli_si128(_mm_slli_epi16(shifts, 4), 8));

    chunk_high = _mm_slli_si128(chunk_high, 1);

    __m128i shuf =
        _mm_add_epi8(shifts, _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5,
                                          4, 3, 2, 1, 0));

    chunk_low = _mm_shuffle_epi8(chunk_low, shuf);
    chunk_high = _mm_shuffle_epi8(chunk_high, shuf);
    __m128i utf16_low = _mm_unpacklo_epi8(chunk_low, chunk_high);
    __m128i utf16_high = _mm_unpackhi_epi8(chunk_low, chunk_high);

    if (_mm_cmpestrc(_mm_cvtsi64_si128(0xfdeffdd0fffffffe), 4, utf16_high, 8,
                     _SIDD_UWORD_OPS | _SIDD_CMP_RANGES) |
        _mm_cmpestrc(_mm_cvtsi64_si128(0xfdeffdd0fffffffe), 4, utf16_low, 8,
                     _SIDD_UWORD_OPS | _SIDD_CMP_RANGES)) {
      return false;
    }

    src += source_advance;
  }
  return validate_utf8(src, end - src);
}bool validate_utf8_sse(const char *src, size_t len) {
  const char *end = src + len;
  while (src + 16 < end) {
    __m128i chunk = _mm_loadu_si128((const __m128i *)(src));

    int asciiMask = _mm_movemask_epi8(chunk);
    if (!asciiMask) {
      src += 16;
      continue;
    }

    __m128i chunk_signed = _mm_add_epi8(chunk, _mm_set1_epi8(0x80));
    __m128i cond2 =
        _mm_cmplt_epi8(_mm_set1_epi8(0xc2 - 1 - 0x80), chunk_signed);
    __m128i state = _mm_set1_epi8((char)(0x0 | 0x80));
    state = _mm_blendv_epi8(state, _mm_set1_epi8((char)(0x2 | 0xc0)), cond2);

    __m128i cond3 =
        _mm_cmplt_epi8(_mm_set1_epi8(0xe0 - 1 - 0x80), chunk_signed);

    state = _mm_blendv_epi8(state, _mm_set1_epi8((char)(0x3 | 0xe0)), cond3);
    __m128i mask3 = _mm_slli_si128(cond3, 1);

    __m128i cond4 =
        _mm_cmplt_epi8(_mm_set1_epi8(0xf0 - 1 - 0x80), chunk_signed);

    // Fall back to the scalar processing
    if (_mm_movemask_epi8(cond4)) {
      break;
    }

    __m128i count = _mm_and_si128(state, _mm_set1_epi8(0x7));

    __m128i count_sub1 = _mm_subs_epu8(count, _mm_set1_epi8(0x1));

    __m128i counts = _mm_add_epi8(count, _mm_slli_si128(count_sub1, 1));

    __m128i shifts = count_sub1;
    shifts = _mm_add_epi8(shifts, _mm_slli_si128(shifts, 1));
    counts = _mm_add_epi8(
        counts, _mm_slli_si128(_mm_subs_epu8(counts, _mm_set1_epi8(0x2)), 2));
    shifts = _mm_add_epi8(shifts, _mm_slli_si128(shifts, 2));

    if (asciiMask ^ _mm_movemask_epi8(_mm_cmpgt_epi8(counts, _mm_set1_epi8(0))))
      return false; // error
    shifts = _mm_add_epi8(shifts, _mm_slli_si128(shifts, 4));

    if (_mm_movemask_epi8(_mm_cmpgt_epi8(
            _mm_sub_epi8(_mm_slli_si128(counts, 1), counts), _mm_set1_epi8(1))))
      return false; // error

    shifts = _mm_add_epi8(shifts, _mm_slli_si128(shifts, 8));

    __m128i mask = _mm_and_si128(state, _mm_set1_epi8(0xf8));
    shifts =
        _mm_and_si128(shifts, _mm_cmplt_epi8(counts, _mm_set1_epi8(2))); // <=1

    chunk =
        _mm_andnot_si128(mask, chunk); // from now on, we only have usefull bits

    shifts = _mm_blendv_epi8(shifts, _mm_srli_si128(shifts, 1),
                             _mm_srli_si128(_mm_slli_epi16(shifts, 7), 1));

    __m128i chunk_right = _mm_slli_si128(chunk, 1);

    __m128i chunk_low = _mm_blendv_epi8(
        chunk,
        _mm_or_si128(chunk, _mm_and_si128(_mm_slli_epi16(chunk_right, 6),
                                          _mm_set1_epi8(0xc0))),
        _mm_cmpeq_epi8(counts, _mm_set1_epi8(1)));

    __m128i chunk_high =
        _mm_and_si128(chunk, _mm_cmpeq_epi8(counts, _mm_set1_epi8(2)));

    shifts = _mm_blendv_epi8(shifts, _mm_srli_si128(shifts, 2),
                             _mm_srli_si128(_mm_slli_epi16(shifts, 6), 2));
    chunk_high = _mm_srli_epi32(chunk_high, 2);

    shifts = _mm_blendv_epi8(shifts, _mm_srli_si128(shifts, 4),
                             _mm_srli_si128(_mm_slli_epi16(shifts, 5), 4));
    chunk_high = _mm_or_si128(
        chunk_high, _mm_and_si128(_mm_and_si128(_mm_slli_epi32(chunk_right, 4),
                                                _mm_set1_epi8(0xf0)),
                                  mask3));
    int c = _mm_extract_epi16(counts, 7);
    int source_advance = !(c & 0x0200) ? 16 : !(c & 0x02) ? 15 : 14;

    __m128i high_bits = _mm_and_si128(chunk_high, _mm_set1_epi8(0xf8));
    if (!_mm_testz_si128(
            mask3,
            _mm_or_si128(_mm_cmpeq_epi8(high_bits, _mm_set1_epi8(0x00)),
                         _mm_cmpeq_epi8(high_bits, _mm_set1_epi8(0xd8)))))
      return false;

    shifts = _mm_blendv_epi8(shifts, _mm_srli_si128(shifts, 8),
                             _mm_srli_si128(_mm_slli_epi16(shifts, 4), 8));

    chunk_high = _mm_slli_si128(chunk_high, 1);

    __m128i shuf =
        _mm_add_epi8(shifts, _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5,
                                          4, 3, 2, 1, 0));

    chunk_low = _mm_shuffle_epi8(chunk_low, shuf);
    chunk_high = _mm_shuffle_epi8(chunk_high, shuf);
    __m128i utf16_low = _mm_unpacklo_epi8(chunk_low, chunk_high);
    __m128i utf16_high = _mm_unpackhi_epi8(chunk_low, chunk_high);

    if (_mm_cmpestrc(_mm_cvtsi64_si128(0xfdeffdd0fffffffe), 4, utf16_high, 8,
                     _SIDD_UWORD_OPS | _SIDD_CMP_RANGES) |
        _mm_cmpestrc(_mm_cvtsi64_si128(0xfdeffdd0fffffffe), 4, utf16_low, 8,
                     _SIDD_UWORD_OPS | _SIDD_CMP_RANGES)) {
      return false;
    }

    src += source_advance;
  }
  return validate_utf8(src, end - src);
}

void test() {
  char *goodsequences[] = {"a", "\xc3\xb1", "\xe2\x82\xa1", "\xf0\x90\x8c\xbc"};
  char *badsequences[] = {
      "\xc3\x28",         "\xa0\xa1",         "\xe2\x28\xa1",    "\xe2\x82\x28",
      "\xf0\x28\x8c\xbc", "\xf0\x90\x28\xbc", "\xf0\x28\x8c\x28"};
  for (size_t i = 0; i < 4; i++) {
    size_t len = strlen(goodsequences[i]);
    assert(validate_utf8(goodsequences[i], len));
  }
  for (size_t i = 0; i < 7; i++) {
    size_t len = strlen(badsequences[i]);
    assert(!validate_utf8(badsequences[i], len));
  }
}

void populate(char *data, size_t N) {
  for (size_t i = 0; i < N; i++)
    data[i] = rand() & 0x7f;
}

void demo(size_t N) {
  printf("string size = %zu \n", N);
  char *data = (char *)malloc(N);
  bool expected = true; // it is all ascii?
  int repeat = 5;
  printf("We are feeding ascii so it is always going to be ok.\n");
  BEST_TIME(is_ascii(data, N), expected,populate(data,N) , repeat, N, true);

  BEST_TIME(validate_utf8(data, N), expected,populate(data,N) , repeat, N, true);
  BEST_TIME(validate_utf8_branchless(data, N), expected,populate(data,N) , repeat, N, true);
  BEST_TIME(validate_utf8_double(data, N), expected,populate(data,N) , repeat, N, true);

  BEST_TIME(shiftless_validate_utf8(data, N), expected,populate(data,N) , repeat, N, true);
  BEST_TIME(shiftless_validate_utf8_branchless(data, N), expected,populate(data,N) , repeat, N, true);
  BEST_TIME(shiftless_validate_utf8_double(data, N), expected,populate(data,N) , repeat, N, true);


  BEST_TIME(validate_utf8_sse_nocheating(data, N), expected,populate(data,N) , repeat, N, true);
  BEST_TIME(validate_utf8_sse(data, N), expected,populate(data,N) , repeat, N, true);
  free(data);
}

int main() {
  test();
  demo(32);
  demo(80);
  demo(512);
  printf("We are feeding ascii so it is always going to be ok.\n");
}
