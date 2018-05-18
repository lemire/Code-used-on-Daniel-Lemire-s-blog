
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

// all byte values must be no larger than 0xF4
static inline void checkSmallerThan0xF4(__m128i current_bytes_unsigned,
                                        __m128i *has_error) {
  // the -128  is to compensate for the signed arithmetic (lack of
  // _mm_cmpgt_epu8)
  *has_error =
      _mm_or_si128(*has_error, _mm_cmpgt_epi8(current_bytes_unsigned,
                                              _mm_set1_epi8(0xF4 - 128)));
}

// except for ASCII, the first byte in a character must be at least 0xC2
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
static inline void checkContinuation(__m128i *counts, __m128i previous_counts,
                                     __m128i *has_error) {
  __m128i right1 = _mm_subs_epu8(
      _mm_alignr_epi8(*counts, previous_counts, 16 - 1), _mm_set1_epi8(1));
  __m128i sum = _mm_add_epi8(*counts, right1);
  __m128i right2 = _mm_subs_epu8(_mm_alignr_epi8(sum, previous_counts, 16 - 2),
                                 _mm_set1_epi8(2));
  sum = _mm_add_epi8(sum, right2);
  // overlap || underlap
  // sum > count && count > 0 || !(sum > count) && !(count > 0)
  // (sum > count) == (count > 0)
  __m128i overunder = _mm_cmpeq_epi8(
      _mm_cmpgt_epi8(sum, *counts), _mm_cmpgt_epi8(*counts, _mm_setzero_si128()));
  *has_error = _mm_or_si128(*has_error, overunder);

  *counts = sum;
}

// check that the string is allowed to terminate
static inline void checkFinalContinuation(__m128i counts,
                                     __m128i *has_error) {
  __m128i right1 = _mm_subs_epu8(
      _mm_alignr_epi8( _mm_setzero_si128(), counts, 16 - 1), _mm_set1_epi8(1));
  __m128i sum = right1;
  __m128i right2 = _mm_subs_epu8(_mm_alignr_epi8(sum, counts, 16 - 2),
                                 _mm_set1_epi8(2));
  sum = _mm_add_epi8(sum, right2);
  *has_error = _mm_or_si128(*has_error, sum);
}

// when 0xED is found, next byte must be no larger than 0x9F
// when 0xF4 is found, next byte must be no larger than 0x8F
static inline void checkFirstContinuationMax(__m128i current_bytes_unsigned,
                                             __m128i off1_current_bytes,
                                             __m128i *has_error) {
  __m128i maskED = _mm_cmpeq_epi8(off1_current_bytes, _mm_set1_epi8(0xED));
  __m128i maskF4 = _mm_cmpeq_epi8(off1_current_bytes, _mm_set1_epi8(0xF4));

  __m128i followED = _mm_and_si128(
      current_bytes_unsigned, maskED); // these should be no larger than 0x9F
  __m128i badfollowED = _mm_cmpgt_epi8(followED, _mm_set1_epi8(0x9F - 128));
  __m128i followF4 = _mm_and_si128(
      current_bytes_unsigned, maskF4); // these should be no larger than 0x8F
  __m128i badfollowF4 = _mm_cmpgt_epi8(followF4, _mm_set1_epi8(0x8F - 128));
  *has_error = _mm_or_si128(*has_error, _mm_or_si128(badfollowED, badfollowF4));
}

// we have that E0 must be followed by something no smaller than A0
// we have that F0 must be followed by something no smaller than 90
static inline void checkFirstContinuationMin(__m128i current_bytes_unsigned,
                                             __m128i off1_current_bytes,
                                             __m128i *has_error) {
  // could be done by looking for uint16_t instead
  __m128i maskE0 = _mm_cmpeq_epi8(off1_current_bytes, _mm_set1_epi8(0xE0));
  __m128i maskF0 = _mm_cmpeq_epi8(off1_current_bytes, _mm_set1_epi8(0xF0));
  __m128i smallerthanA0 = _mm_cmpgt_epi8(_mm_set1_epi8(0xA0 - 128), current_bytes_unsigned);
  __m128i smallerthan90 = _mm_cmpgt_epi8(_mm_set1_epi8(0x90 - 128), current_bytes_unsigned);
  __m128i badfollowE0 = _mm_and_si128(maskE0, smallerthanA0);
  __m128i badfollowF0 = _mm_and_si128(maskF0, smallerthan90);
  *has_error = _mm_or_si128(*has_error, _mm_or_si128(badfollowE0, badfollowF0));
}

struct processed_utf_bytes {
  __m128i rawbytes;
  __m128i high_nibbles;
  __m128i counts;
};

static inline void count_nibbles(__m128i bytes,
                                 struct processed_utf_bytes *answer) {
  answer->rawbytes = bytes;
  __m128i nibble_mask = _mm_set1_epi8(0x0F);

  answer->high_nibbles = _mm_and_si128(_mm_srli_epi16(bytes, 4), nibble_mask);
  answer->counts = _mm_shuffle_epi8(
      _mm_setr_epi8(1, 1, 1, 1, 1, 1, 1, 1, // 0xxx (ASCII)
                    0, 0, 0, 0,             // 10xx (continuation)
                    2, 2,                   // 110x
                    3,                      // 1110
                    4), // 1111, next should be 0 (not checked here)
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
  checkLargerThan0xC2(current_bytes_unsigned, pb.high_nibbles, has_error);
  checkContinuation(&pb.counts, previous->counts, has_error);
  __m128i off1_current_bytes =
      _mm_alignr_epi8(pb.rawbytes, previous->rawbytes, 16 - 1);
  checkFirstContinuationMax(current_bytes_unsigned, off1_current_bytes,
                            has_error);
  checkFirstContinuationMin(current_bytes_unsigned, off1_current_bytes,
                            has_error);
  return pb;
}

static bool validate_utf8_fast(const char *src, size_t len) {
  size_t i = 0;
  __m128i has_error = _mm_setzero_si128();
  struct processed_utf_bytes previous = {.rawbytes = _mm_setzero_si128(),
                                         .high_nibbles = _mm_setzero_si128(),
                                         .counts = _mm_setzero_si128()};
  for (; i + 15 < len; i += 16) {
    __m128i current_bytes = _mm_loadu_si128((const __m128i *)(src + i));
    previous = checkUTF8Bytes(current_bytes, &previous, &has_error);
  }

  // last part
  if (i < len) {
    char buffer[16];
    memset(buffer, 0, 16);
    memcpy(buffer, src + i, len - i);
    __m128i current_bytes = _mm_loadu_si128((const __m128i *)(buffer));
    previous = checkUTF8Bytes(current_bytes, &previous, &has_error);
  } else {
    checkFinalContinuation(previous.counts,
                                         &has_error);
  }

  return _mm_testz_si128(has_error, has_error);
}



#endif
