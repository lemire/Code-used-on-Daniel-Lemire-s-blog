#ifndef SIMDASCIICHECK_H
#define SIMDASCIICHECK_H

#include <stddef.h> // size_t
#include <stdbool.h> // c99 bool
#include <emmintrin.h> // SSE2

// The function returns true (1) if all chars passed in src are 
// 7-bit values (0x00..0x7F). Otherwise, it returns false (0).
static bool validate_ascii_fast(const char *src, size_t len) {
  size_t i = 0;
  __m128i has_error = _mm_setzero_si128();
  if(len >= 16) {
    for (; i <= len - 16; i += 16) {
      __m128i current_bytes = _mm_loadu_si128((const __m128i *)(src + i));
      has_error = _mm_or_si128(has_error, current_bytes);
    }
  }
  int error_mask = _mm_movemask_epi8(has_error);

  char tail_has_error = 0;
  for (; i < len; i++) {
    tail_has_error |= src[i];
  }
  error_mask |= (tail_has_error & 0x80);

  return !error_mask;
}



// code contributed by Claude Roux (it is slower)
long clauderoux_validate_ascii(unsigned char *src, long len) {
  __m128i minusbyte = _mm_set1_epi8(0x80);
  __m128i current_bytes = _mm_setzero_si128();

  long i = 0;
  for (; i + 15 < len; i += 16) {
    // we load our section, the length should be larger than 16
    current_bytes = _mm_loadu_si128((const __m128i *)(src + i));
    if (!_mm_testz_si128(minusbyte, current_bytes))
      return i;
  }

  // last part
  if (i < len) {
    char buffer[16];
    memset(buffer, 0, 16);
    memcpy(buffer, src + i, len - i);
    current_bytes = _mm_loadu_si128((const __m128i *)buffer);
    if (!_mm_testz_si128(minusbyte, current_bytes))
      return i;
  }

  return -1;
}
#endif
