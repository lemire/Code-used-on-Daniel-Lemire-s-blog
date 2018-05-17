
#ifndef SIMDASCIICHECK_H
#define SIMDASCIICHECK_H
#include <stddef.h>
#include <stdint.h>
#include <x86intrin.h>

static bool validate_ascii_fast(const char *src, size_t len) {
  size_t i = 0;
  __m128i has_error = _mm_setzero_si128();
  for (; i + 15 < len; i += 16) {
    __m128i current_bytes = _mm_loadu_si128((const __m128i *)(src + i));
    has_error = _mm_or_si128(has_error, current_bytes);
  }

  // last part
  if (i < len) {
    char buffer[16];
    memset(buffer, 0, 16);
    memcpy(buffer, src + i, len - i);
    __m128i current_bytes = _mm_loadu_si128((const __m128i *)(buffer));
    has_error = _mm_or_si128(has_error, current_bytes);
  }

  __m128i signbitmask = _mm_set1_epi8(0b10000000);
  return _mm_testz_si128(has_error, signbitmask);
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
    memcpy(buffer, src + i, len – i);
    current_bytes = _mm_loadu_si128((const __m128i *)buffer);
    if (!_mm_testz_si128(minusbyte, current_bytes))
      return i;
  }

  return -1;
}

#endif
