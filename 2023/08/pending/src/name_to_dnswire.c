
#include "name_to_dnswire.h"

#include <stdint.h>
#include <x86intrin.h> // update if we need to support Windows.

size_t name_to_dnswire(const char *src, uint8_t *dst) {
  const char *srcinit = src;

  static const bool is_alphanumdash[256] = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
      0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t *counter = dst++;
  do {

    while (is_alphanumdash[(uint8_t)*src]) {
      *dst++ = (uint8_t)*src;
      src++;
    }
    *counter = (uint8_t)(dst - counter -
                         1); // should check for overflow and zero values.
    counter = dst++;
    if (*src != '.') {
      break;
    }
    src++;
  } while (true);
  *counter = 0;
  return (size_t)(src - srcinit);
}

// 256 * 8 bytes = 2kB, easily fits in cache.
static uint64_t thintable_epi8[256] = {
    0x0706050403020100, 0x0007060504030201, 0x0007060504030200,
    0x0000070605040302, 0x0007060504030100, 0x0000070605040301,
    0x0000070605040300, 0x0000000706050403, 0x0007060504020100,
    0x0000070605040201, 0x0000070605040200, 0x0000000706050402,
    0x0000070605040100, 0x0000000706050401, 0x0000000706050400,
    0x0000000007060504, 0x0007060503020100, 0x0000070605030201,
    0x0000070605030200, 0x0000000706050302, 0x0000070605030100,
    0x0000000706050301, 0x0000000706050300, 0x0000000007060503,
    0x0000070605020100, 0x0000000706050201, 0x0000000706050200,
    0x0000000007060502, 0x0000000706050100, 0x0000000007060501,
    0x0000000007060500, 0x0000000000070605, 0x0007060403020100,
    0x0000070604030201, 0x0000070604030200, 0x0000000706040302,
    0x0000070604030100, 0x0000000706040301, 0x0000000706040300,
    0x0000000007060403, 0x0000070604020100, 0x0000000706040201,
    0x0000000706040200, 0x0000000007060402, 0x0000000706040100,
    0x0000000007060401, 0x0000000007060400, 0x0000000000070604,
    0x0000070603020100, 0x0000000706030201, 0x0000000706030200,
    0x0000000007060302, 0x0000000706030100, 0x0000000007060301,
    0x0000000007060300, 0x0000000000070603, 0x0000000706020100,
    0x0000000007060201, 0x0000000007060200, 0x0000000000070602,
    0x0000000007060100, 0x0000000000070601, 0x0000000000070600,
    0x0000000000000706, 0x0007050403020100, 0x0000070504030201,
    0x0000070504030200, 0x0000000705040302, 0x0000070504030100,
    0x0000000705040301, 0x0000000705040300, 0x0000000007050403,
    0x0000070504020100, 0x0000000705040201, 0x0000000705040200,
    0x0000000007050402, 0x0000000705040100, 0x0000000007050401,
    0x0000000007050400, 0x0000000000070504, 0x0000070503020100,
    0x0000000705030201, 0x0000000705030200, 0x0000000007050302,
    0x0000000705030100, 0x0000000007050301, 0x0000000007050300,
    0x0000000000070503, 0x0000000705020100, 0x0000000007050201,
    0x0000000007050200, 0x0000000000070502, 0x0000000007050100,
    0x0000000000070501, 0x0000000000070500, 0x0000000000000705,
    0x0000070403020100, 0x0000000704030201, 0x0000000704030200,
    0x0000000007040302, 0x0000000704030100, 0x0000000007040301,
    0x0000000007040300, 0x0000000000070403, 0x0000000704020100,
    0x0000000007040201, 0x0000000007040200, 0x0000000000070402,
    0x0000000007040100, 0x0000000000070401, 0x0000000000070400,
    0x0000000000000704, 0x0000000703020100, 0x0000000007030201,
    0x0000000007030200, 0x0000000000070302, 0x0000000007030100,
    0x0000000000070301, 0x0000000000070300, 0x0000000000000703,
    0x0000000007020100, 0x0000000000070201, 0x0000000000070200,
    0x0000000000000702, 0x0000000000070100, 0x0000000000000701,
    0x0000000000000700, 0x0000000000000007, 0x0006050403020100,
    0x0000060504030201, 0x0000060504030200, 0x0000000605040302,
    0x0000060504030100, 0x0000000605040301, 0x0000000605040300,
    0x0000000006050403, 0x0000060504020100, 0x0000000605040201,
    0x0000000605040200, 0x0000000006050402, 0x0000000605040100,
    0x0000000006050401, 0x0000000006050400, 0x0000000000060504,
    0x0000060503020100, 0x0000000605030201, 0x0000000605030200,
    0x0000000006050302, 0x0000000605030100, 0x0000000006050301,
    0x0000000006050300, 0x0000000000060503, 0x0000000605020100,
    0x0000000006050201, 0x0000000006050200, 0x0000000000060502,
    0x0000000006050100, 0x0000000000060501, 0x0000000000060500,
    0x0000000000000605, 0x0000060403020100, 0x0000000604030201,
    0x0000000604030200, 0x0000000006040302, 0x0000000604030100,
    0x0000000006040301, 0x0000000006040300, 0x0000000000060403,
    0x0000000604020100, 0x0000000006040201, 0x0000000006040200,
    0x0000000000060402, 0x0000000006040100, 0x0000000000060401,
    0x0000000000060400, 0x0000000000000604, 0x0000000603020100,
    0x0000000006030201, 0x0000000006030200, 0x0000000000060302,
    0x0000000006030100, 0x0000000000060301, 0x0000000000060300,
    0x0000000000000603, 0x0000000006020100, 0x0000000000060201,
    0x0000000000060200, 0x0000000000000602, 0x0000000000060100,
    0x0000000000000601, 0x0000000000000600, 0x0000000000000006,
    0x0000050403020100, 0x0000000504030201, 0x0000000504030200,
    0x0000000005040302, 0x0000000504030100, 0x0000000005040301,
    0x0000000005040300, 0x0000000000050403, 0x0000000504020100,
    0x0000000005040201, 0x0000000005040200, 0x0000000000050402,
    0x0000000005040100, 0x0000000000050401, 0x0000000000050400,
    0x0000000000000504, 0x0000000503020100, 0x0000000005030201,
    0x0000000005030200, 0x0000000000050302, 0x0000000005030100,
    0x0000000000050301, 0x0000000000050300, 0x0000000000000503,
    0x0000000005020100, 0x0000000000050201, 0x0000000000050200,
    0x0000000000000502, 0x0000000000050100, 0x0000000000000501,
    0x0000000000000500, 0x0000000000000005, 0x0000000403020100,
    0x0000000004030201, 0x0000000004030200, 0x0000000000040302,
    0x0000000004030100, 0x0000000000040301, 0x0000000000040300,
    0x0000000000000403, 0x0000000004020100, 0x0000000000040201,
    0x0000000000040200, 0x0000000000000402, 0x0000000000040100,
    0x0000000000000401, 0x0000000000000400, 0x0000000000000004,
    0x0000000003020100, 0x0000000000030201, 0x0000000000030200,
    0x0000000000000302, 0x0000000000030100, 0x0000000000000301,
    0x0000000000000300, 0x0000000000000003, 0x0000000000020100,
    0x0000000000000201, 0x0000000000000200, 0x0000000000000002,
    0x0000000000000100, 0x0000000000000001, 0x0000000000000000,
    0x0000000000000000,
};

#include <stdio.h>
void print8(const char *name, __m128i x) {
  printf("%.32s : ", name);
  uint8_t buffer[16];
  _mm_storeu_si128((__m128i *)buffer, x);
  for (size_t i = 0; i < 16; i++) {
    printf("%02x ", buffer[i]);
  }
  printf("\n");
}

static inline __m128i left_shift_bytes(__m128i x, int count) {
  // We would like to shift by count bytes, but it cannot be done directly
  // without immediates
  __m128i p1 = _mm_sll_epi64(x, _mm_cvtsi64_si128(count * 8));
  __m128i p2 = _mm_srl_epi64(_mm_unpacklo_epi64(_mm_setzero_si128(), x),
                             _mm_cvtsi64_si128(64 - count * 8));
  return _mm_or_si128(p1, p2);
}

// This version processes at most 15 bytes from the input. A fallback would
// be necessary to use such code in production. TODO.
size_t name_to_dnswire_simd(const char *src, uint8_t *dst) {
  const char *srcinit = src;
  // Each label may contain from 1 to 63 octets. The empty label is
  // reserved for the root node and when fully qualified is expressed
  // as the empty label terminated by a dot.
  // The full domain name may not exceed a total length of 253 ASCII characters
  // in its textual representation.
  //
  // It is likely that many name sfits under 16 bytes, however.

  // We do vectorized classification to validate the content.
  // We want to allow 0x30 to 0x39 (digits)
  // The hyphen 0x2d.
  // The dot 0x2e.
  // The lower-cased letters 0x61-0x6f (a-o), 0x70-0x7a (p-z)
  // The upper-cased letters 0x41-0x4f (A-O), 0x50-0x5a (P-Z)

  const char DIGIT = 0x01;
  const char HYPHENDOT = 0x02;
  const char LETTERAO = 0x04;
  const char LETTERPZ = 0x08;
  static int8_t zero_masks[32] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                  0,  0,  0,  0,  0,  -1, -1, -1, -1, -1, -1,
                                  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  __m128i highnibbles =
      _mm_setr_epi8(0, 0, HYPHENDOT, DIGIT, LETTERAO, LETTERPZ, LETTERAO,
                    LETTERPZ, 0, 0, 0, 0, 0, 0, 0, 0);
  __m128i lownibbles =
      _mm_setr_epi8(LETTERPZ | DIGIT, LETTERAO | LETTERPZ | DIGIT,
                    LETTERAO | LETTERPZ | DIGIT, LETTERAO | LETTERPZ | DIGIT,
                    LETTERAO | LETTERPZ | DIGIT, LETTERAO | LETTERPZ | DIGIT,
                    LETTERAO | LETTERPZ | DIGIT, LETTERAO | LETTERPZ | DIGIT,
                    LETTERAO | LETTERPZ | DIGIT, LETTERAO | LETTERPZ | DIGIT,
                    LETTERAO | LETTERPZ, LETTERAO, LETTERAO,
                    LETTERAO | HYPHENDOT, LETTERAO | HYPHENDOT, LETTERAO);
  // we always insert a fake '.' initially.
  __m128i input = _mm_loadu_si128((const __m128i *)src);
  input = _mm_alignr_epi8(input, _mm_set1_epi8('.'), 15);
  src -= 1; // we pretend that we are pointing at the virtual '.'.

  // We could possibly 'upper case everything' if we wanted to.
  //   __m128i inputlc = _mm_or_si128(input, _mm_set1_epi8(0x20));
  __m128i low = _mm_shuffle_epi8(
      lownibbles,
      input); // no need for _mm_and_si128(input,_mm_set1_epi8(0xf)) because
              // if high bit is set, there is no match
  __m128i high = _mm_shuffle_epi8(
      highnibbles, _mm_and_si128(_mm_srli_epi64(input, 4), _mm_set1_epi8(0xf)));
  __m128i classified =
      _mm_cmpeq_epi8(_mm_and_si128(low, high), _mm_setzero_si128());
  // m cannot be zero!!!
  unsigned m = (unsigned)_mm_movemask_epi8(
      classified); // should be 1 wherever we have a match.
  uint16_t length = (uint16_t)__builtin_ctz((unsigned int)m);
  src += length;
  __m128i zero_mask = _mm_loadu_si128((__m128i *)(zero_masks + 16 - length));
  // masking with '.'
  input = _mm_blendv_epi8(input, _mm_set1_epi8('.'), zero_mask);
  //
  __m128i dots = _mm_cmpeq_epi8(input, _mm_set1_epi8('.'));

  unsigned int mask = (unsigned)_mm_movemask_epi8(dots);
  __m128i sequential =
      _mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  const __m128i dotscounts = _mm_and_si128(dots, sequential);

  // We proceed to compute a shuffle mask that brings all counters/dots
  // together. We could do it with a single large table (2**16 * 128 bytes)
  // or 8 MB. Instead, we work from a 2 kB table and do more computation.
  mask = mask ^ 0xFFFF; // negate
  unsigned mask1 = (unsigned)(mask & 0xFF);
  int pop = 8 - __builtin_popcount(mask1);
  unsigned mask2 = (mask >> 8);
  __m128i m1 = _mm_loadl_epi64((const __m128i *)(thintable_epi8 + mask1));
  __m128i m2 = _mm_loadl_epi64((const __m128i *)(thintable_epi8 + mask2));
  __m128i m2add = _mm_add_epi8(m2, _mm_set1_epi8(8));
  __m128i m2shifted = left_shift_bytes(m2add, pop);
  __m128i shufmask = _mm_or_si128(m2shifted, m1);
  // The shuffle mask has been computed.
  // We also need the *reverse* mask which we compute with a prefix sum !
  __m128i dotones = _mm_and_si128(dots, _mm_set1_epi8(1));
  dotones = _mm_add_epi8(dotones,
                         _mm_alignr_epi8(dotones, _mm_setzero_si128(), 16 - 1));
  dotones = _mm_add_epi8(dotones,
                         _mm_alignr_epi8(dotones, _mm_setzero_si128(), 16 - 2));
  dotones = _mm_add_epi8(dotones,
                         _mm_alignr_epi8(dotones, _mm_setzero_si128(), 16 - 4));
  dotones = _mm_add_epi8(dotones,
                         _mm_alignr_epi8(dotones, _mm_setzero_si128(), 16 - 8));
  dotones = _mm_sub_epi8(dotones, _mm_set1_epi8(1));
  // Ok, dotones contains the reverse shuffle mask
  // Pheeewww... This was a lot of work.
  const __m128i packed_dotscounts = _mm_shuffle_epi8(dotscounts, shufmask);
  // Need to subtract the counters.
  // If there is an overflow, then we had two successive dots, we should error:
  // TODO.
  __m128i diffed_packed_dotscounts =
      _mm_sub_epi8(_mm_alignr_epi8(_mm_setzero_si128(), packed_dotscounts, 1),
                   packed_dotscounts);
  // need to subtract one to the counters.
  diffed_packed_dotscounts =
      _mm_sub_epi8(diffed_packed_dotscounts, _mm_set1_epi8(1));
  // send it back...
  __m128i magic = _mm_shuffle_epi8(diffed_packed_dotscounts, dotones);
  // shift it
  __m128i marked_input = _mm_blendv_epi8(input, magic, dots);
  _mm_storeu_si128((__m128i *)dst, marked_input);
  // dst += 16;
  return (size_t)(src - srcinit);
}

// This version processes at most 15 bytes from the input. A fallback would
// be necessary to use such code in production. TODO.
size_t name_to_dnswire_simd_fast(const char *src, uint8_t *dst) {
  const char *srcinit = src;
  // Each label may contain from 1 to 63 octets. The empty label is
  // reserved for the root node and when fully qualified is expressed
  // as the empty label terminated by a dot.
  // The full domain name may not exceed a total length of 253 ASCII characters
  // in its textual representation.
  //
  // It is likely that many name sfits under 16 bytes, however.

  // We do vectorized classification to validate the content.
  // We want to allow 0x30 to 0x39 (digits)
  // The hyphen 0x2d.
  // The dot 0x2e.
  // The lower-cased letters 0x61-0x6f (a-o), 0x70-0x7a (p-z)
  // The upper-cased letters 0x41-0x4f (A-O), 0x50-0x5a (P-Z)

  const char DIGIT = 0x01;
  const char HYPHENDOT = 0x02;
  const char LETTERAO = 0x04;
  const char LETTERPZ = 0x08;
  static int8_t zero_masks[32] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                  0,  0,  0,  0,  0,  -1, -1, -1, -1, -1, -1,
                                  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  __m128i highnibbles =
      _mm_setr_epi8(0, 0, HYPHENDOT, DIGIT, LETTERAO, LETTERPZ, LETTERAO,
                    LETTERPZ, 0, 0, 0, 0, 0, 0, 0, 0);
  __m128i lownibbles =
      _mm_setr_epi8(LETTERPZ | DIGIT, LETTERAO | LETTERPZ | DIGIT,
                    LETTERAO | LETTERPZ | DIGIT, LETTERAO | LETTERPZ | DIGIT,
                    LETTERAO | LETTERPZ | DIGIT, LETTERAO | LETTERPZ | DIGIT,
                    LETTERAO | LETTERPZ | DIGIT, LETTERAO | LETTERPZ | DIGIT,
                    LETTERAO | LETTERPZ | DIGIT, LETTERAO | LETTERPZ | DIGIT,
                    LETTERAO | LETTERPZ, LETTERAO, LETTERAO,
                    LETTERAO | HYPHENDOT, LETTERAO | HYPHENDOT, LETTERAO);
  // we always insert a fake '.' initially.
  __m128i input = _mm_loadu_si128((const __m128i *)src);
  input = _mm_alignr_epi8(input, _mm_set1_epi8('.'), 15);
  src -= 1; // we pretend that we are pointing at the virtual '.'.

  // We could possibly 'upper case everything' if we wanted to.
  //   __m128i inputlc = _mm_or_si128(input, _mm_set1_epi8(0x20));
  __m128i low = _mm_shuffle_epi8(
      lownibbles,
      input); // no need for _mm_and_si128(input,_mm_set1_epi8(0xf)) because
              // if high bit is set, there is no match
  __m128i high = _mm_shuffle_epi8(
      highnibbles, _mm_and_si128(_mm_srli_epi64(input, 4), _mm_set1_epi8(0xf)));
  __m128i classified =
      _mm_cmpeq_epi8(_mm_and_si128(low, high), _mm_setzero_si128());
  // m cannot be zero!!!
  unsigned m = (unsigned)_mm_movemask_epi8(
      classified); // should be 1 wherever we have a match.
  uint16_t length = (uint16_t)__builtin_ctz((unsigned int)m);
  src += length;
  __m128i zero_mask = _mm_loadu_si128((__m128i *)(zero_masks + 16 - length));
  // masking with '.'
  input = _mm_blendv_epi8(input, _mm_set1_epi8('.'), zero_mask);
  //
  __m128i dots = _mm_cmpeq_epi8(input, _mm_set1_epi8('.'));

  __m128i sequential =
      _mm_setr_epi8(-128, -127, -126, -125, -124, -123, -122, -121, -120, -119,
                    -118, -117, -116, -115, -114, -113);
  __m128i dotscounts = _mm_and_si128(dots, sequential);

  dotscounts =
      _mm_blendv_epi8(_mm_alignr_epi8(_mm_setzero_si128(), dotscounts, 1),
                      dotscounts, dotscounts);

  dotscounts =
      _mm_blendv_epi8(_mm_alignr_epi8(_mm_setzero_si128(), dotscounts, 2),
                      dotscounts, dotscounts);

  dotscounts =
      _mm_blendv_epi8(_mm_alignr_epi8(_mm_setzero_si128(), dotscounts, 4),
                      dotscounts, dotscounts);

  dotscounts =
      _mm_blendv_epi8(_mm_alignr_epi8(_mm_setzero_si128(), dotscounts, 8),
                      dotscounts, dotscounts);

  __m128i next = _mm_alignr_epi8(_mm_setzero_si128(), dotscounts, 1);
  dotscounts = _mm_sub_epi8(next, dotscounts);

  // need to subtract one to the counters.
  dotscounts = _mm_sub_epi8(dotscounts, _mm_set1_epi8(1));

  // shift it
  __m128i marked_input = _mm_blendv_epi8(input, dotscounts, dots);
  _mm_storeu_si128((__m128i *)dst, marked_input);
  // dst += 16;
  return (size_t)(src - srcinit);
}