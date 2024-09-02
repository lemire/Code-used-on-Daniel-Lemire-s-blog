#ifndef AVX512COMPRESS_H
#define AVX512COMPRESS_H

void to_float16(uint16_t *dst, const double *src, size_t length) {
  for (size_t i = 0; i < length; i++) {
   // dst[i] = _cvtss_sh(src[i], 0);
  }
}

void from_float16(double *dst, const uint16_t *src, size_t length) {
  for (size_t i = 0; i < length; i++) {
   // dst[i] = _cvtsh_ss(src[i]);
  }
}

#endif
