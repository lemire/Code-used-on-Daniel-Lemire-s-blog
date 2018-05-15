
#ifndef SIMDASCIICHECK_H
#define SIMDASCIICHECK_H
#include <stddef.h>
#include <stdint.h>
#include <x86intrin.h>

static bool validate_ascii_fast(const char *src, size_t len) {
  size_t i = 0;
  __m128i has_error = _mm_setzero_si128();
  __m128i zero = _mm_setzero_si128();
  for (; i + 15 < len; i += 16) {
    __m128i current_bytes = _mm_loadu_si128((const __m128i *)(src));
    has_error =
      _mm_or_si128(has_error, _mm_cmpgt_epi8(zero,current_bytes));
  }

  // last part
  if (i < len) {
    char buffer[16];
    memset(buffer, 0, 16);
    memcpy(buffer, src + i, len - i);
    __m128i current_bytes = _mm_loadu_si128((const __m128i *)(buffer));
    has_error =
      _mm_or_si128(has_error, _mm_cmpgt_epi8(zero,current_bytes));
  }

  return _mm_testz_si128(has_error, has_error);
}

#endif
