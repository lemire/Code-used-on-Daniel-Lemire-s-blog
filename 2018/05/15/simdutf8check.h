
#ifndef SIMDUTF8CHECK_H
#define SIMDUTF8CHECK_H
#include <stddef.h>
#include <stdint.h>
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

static inline void checkSmallerThan0xF4(__m128i current_bytes_unsigned,
                                        __m128i *has_error) {
  // the -128  is to compensate for the signed arithmetic (lack of
  // _mm_cmpgt_epu8)
  *has_error =
      _mm_or_si128(*has_error, _mm_cmpgt_epi8(current_bytes_unsigned,
                                              _mm_set1_epi8(0xF4 - 128)));
}

static inline void checkLargerThan0xC2(__m128i current_bytes_unsigned,
                                       __m128i high_nibbles,
                                       __m128i *has_error) {
  // the -128  is to compensate for the signed arithmetic (lack of
  // _mm_cmpgt_epu8)
  __m128i thismin = _mm_shuffle_epi8(
      _mm_setr_epi8(0 - 128, 0 - 128, 0 - 128, 0 - 128, 0 - 128, 0 - 128,
                    0 - 128, 0 - 128,                   // 0xxx (ASCII)
                    0 - 128, 0 - 128, 0 - 128, 0 - 128, // 10xx (continuation)
                    0xC2 - 128, 0 - 128,                // 110x
                    0 - 128,                            // 1110
                    0 - 128),                           // 1111,
      high_nibbles);
  *has_error =
      _mm_or_si128(*has_error, _mm_cmpgt_epi8(thismin, current_bytes_unsigned));
}

// Code contributed by Kendall Willets
static inline void checkContinuation(__m128i high_nibbles, __m128i counts,
                                     __m128i previous_counts,
                                     __m128i *has_error) {
  __m128i right1 = _mm_alignr_epi8(counts, previous_counts,
                                   16 - 1); //_mm_bslli_si128(counts, 1);
  __m128i right2 =
      _mm_subs_epu8(_mm_alignr_epi8(counts, previous_counts, 16 - 2),
                    _mm_set1_epi8(1)); // _mm_subs_epu8(  _mm_bslli_si128(
                                       // counts, 2), _mm_set1_epi8(1));
  __m128i right3 = _mm_subs_epu8(
      _mm_alignr_epi8(counts, previous_counts, 16 - 3),
      _mm_set1_epi8(
          2)); // _mm_subs_epu8( _mm_bslli_si128( counts, 3), _mm_set1_epi8(2));

  __m128i following = _mm_or_si128(_mm_or_si128(right1, right2), right3);

  __m128i continuations = _mm_cmpgt_epi8(following, _mm_set1_epi8(0));
  __m128i firsts = _mm_shuffle_epi8(_mm_setr_epi8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                  0xFF, 0xFF, 0xFF, // 0xxx
                                                  0, 0, 0, 0,       // 10xx
                                                  0xFF, 0xFF,       // 110x
                                                  0xFF,             // 1110
                                                  0xFF),            // 1111
                                    high_nibbles);
  *has_error = _mm_or_si128(*has_error, _mm_cmpeq_epi8(firsts, continuations));
}

static inline void checkFirstContinuationMax3(__m128i current_bytes_unsigned,
                                              __m128i off1_low_nibbles,
                                              __m128i off1_high_nibbles,
                                              __m128i *has_error) {
  // the next max only kicks in if the low nibble is d
  __m128i selector = _mm_and_si128(
      off1_high_nibbles, _mm_cmpeq_epi8(off1_low_nibbles, _mm_set1_epi8(0xD)));
  // the -128  is to compensate for the signed arithmetic (lack of
  // _mm_cmpgt_epu8)
  __m128i nextmax = _mm_shuffle_epi8(
      _mm_setr_epi8(0xFF - 128, 0xFF - 128, 0xFF - 128, 0xFF - 128, 0xFF - 128,
                    0xFF - 128, 0xFF - 128, 0xFF - 128, // 0xxx (ASCII)
                    0xFF - 128, 0xFF - 128, 0xFF - 128,
                    0xFF - 128,             // 10xx (continuation)
                    0xFF - 128, 0xFF - 128, // 110x
                    0x9F - 128,             // 1110
                    0xFF - 128),            // 1111, next should be 0
      selector);

  *has_error =
      _mm_or_si128(*has_error, _mm_cmpgt_epi8(current_bytes_unsigned, nextmax));
}

static inline void checkFirstContinuationMax4(__m128i current_bytes_unsigned,
                                              __m128i off1_low_nibbles,
                                              __m128i off1_high_nibbles,
                                              __m128i *has_error) {
  // the next max only kicks in if the low nibble is 4

  __m128i selector = _mm_and_si128(
      off1_high_nibbles, _mm_cmpeq_epi8(off1_low_nibbles, _mm_set1_epi8(0x4)));
  // the -128  is to compensate for the signed arithmetic (lack of
  // _mm_cmpgt_epu8)
  __m128i nextmax = _mm_shuffle_epi8(
      _mm_setr_epi8(0xFF - 128, 0xFF - 128, 0xFF - 128, 0xFF - 128, 0xFF - 128,
                    0xFF - 128, 0xFF - 128, 0xFF - 128, // 0xxx (ASCII)
                    0xFF - 128, 0xFF - 128, 0xFF - 128,
                    0xFF - 128,             // 10xx (continuation)
                    0xFF - 128, 0xFF - 128, // 110x
                    0xFF - 128,             // 1110
                    0x8F - 128),            // 1111, next should be 0
      selector);
  *has_error =
      _mm_or_si128(*has_error, _mm_cmpgt_epi8(current_bytes_unsigned, nextmax));
}
static inline void checkFirstContinuationMin(__m128i current_bytes_unsigned,
                                             __m128i off1_low_nibbles,
                                             __m128i off1_high_nibbles,
                                             __m128i *has_error) {
  __m128i nextmin =
      _mm_shuffle_epi8(_mm_setr_epi8(0, 0, 0, 0, 0, 0, 0, 0, // 0xxx (ASCII)
                                     0, 0, 0, 0, // 10xx (continuation)
                                     0, 0,       // 110x
                                     0xA0,       // 1110
                                     0x90),      // 1111,
                       off1_high_nibbles);

  // the  mins only kicks in if the low nibble is zero

  nextmin = _mm_and_si128(
      nextmin, _mm_cmpeq_epi8(off1_low_nibbles, _mm_setzero_si128()));

  __m128i nextmin_unsigned = _mm_sub_epi8(nextmin, _mm_set1_epi8(-128));

  *has_error = _mm_or_si128(
      *has_error, _mm_cmpgt_epi8(nextmin_unsigned, current_bytes_unsigned));
}

struct processed_utf_bytes {
  __m128i rawbytes;
  __m128i low_nibbles;
  __m128i high_nibbles;
  __m128i counts;
};

static inline void count_nibbles(__m128i bytes,
                                 struct processed_utf_bytes *answer) {
  __m128i high = _mm_and_si128(_mm_srli_epi16(bytes, 4), _mm_set1_epi8(0x0F));
  answer->rawbytes = bytes;
  __m128i nibble_mask = _mm_set1_epi8(0x0F);
  answer->low_nibbles = _mm_and_si128(bytes, nibble_mask);

  answer->high_nibbles = _mm_and_si128(_mm_srli_epi16(bytes, 4), nibble_mask);
  answer->counts = _mm_shuffle_epi8(
      _mm_setr_epi8(0, 0, 0, 0, 0, 0, 0, 0, // 0xxx (ASCII)
                    0, 0, 0, 0,             // 10xx (continuation)
                    1, 1,                   // 110x
                    2,                      // 1110
                    3), // 1111, next should be 0 (not checked here)
      answer->high_nibbles);
}

// check whether the current bytes are valid UTF-8
// at the end of the function, previous gets updated
static struct processed_utf_bytes
checkUTF8Bytes(__m128i current_bytes, struct processed_utf_bytes *previous,
               __m128i *has_error) {
  struct processed_utf_bytes pb;
  count_nibbles(current_bytes, &pb);

  __m128i current_bytes_unsigned =
      _mm_sub_epi8(current_bytes, _mm_set1_epi8(-128));
  checkSmallerThan0xF4(current_bytes_unsigned, has_error);
  /*if( ! _mm_testz_si128(*has_error,*has_error)){
    printf("fuck!1\n");
    abort();
  }*/
  checkLargerThan0xC2(current_bytes_unsigned, pb.high_nibbles, has_error);
  /*  if( ! _mm_testz_si128(*has_error,*has_error)){
      printf("fuck!2\n");
                  abort();

    }*/
  checkContinuation(pb.high_nibbles, pb.counts, previous->counts, has_error);
  __m128i off1_low_nibbles =
      _mm_alignr_epi8(pb.low_nibbles, previous->low_nibbles, 16 - 1);
  __m128i off1_high_nibbles =
      _mm_alignr_epi8(pb.high_nibbles, previous->high_nibbles, 16 - 1);
  checkFirstContinuationMax3(current_bytes_unsigned, off1_low_nibbles,
                             off1_high_nibbles, has_error);
  checkFirstContinuationMax4(current_bytes_unsigned, off1_low_nibbles,
                             off1_high_nibbles, has_error);
  checkFirstContinuationMin(current_bytes_unsigned, off1_low_nibbles,
                            off1_high_nibbles, has_error);
  return pb;
}

static bool validate_utf8_fast(const char *src, size_t len) {
  size_t i = 0;
  __m128i has_error = _mm_setzero_si128();
  struct processed_utf_bytes previous = {.rawbytes = _mm_setzero_si128(),
                                         .high_nibbles = _mm_setzero_si128(),
                                         .counts = _mm_setzero_si128()};
  for (; i + 15 < len; i += 16) {
    __m128i current_bytes = _mm_loadu_si128((const __m128i *)(src));
    previous = checkUTF8Bytes(current_bytes, &previous, &has_error);
  }

  // last part
  if (i < len) {
    char buffer[16];
    memset(buffer, 0, 16);
    memcpy(buffer, src + i, len - i);
    __m128i current_bytes = _mm_loadu_si128((const __m128i *)(buffer));
    previous = checkUTF8Bytes(current_bytes, &previous, &has_error);
  }

  return _mm_testz_si128(has_error, has_error);
}
#endif
