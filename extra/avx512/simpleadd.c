#include <x86intrin.h>

__m256i vectorFibo(__m256i a, __m256i b, size_t number) {
  for(size_t i = 0; i < number ; i++) {
    __m256i newb = _mm256_add_epi32(a,b);
    a = b;
    b = newb;
  }
  return b;
}


int main() {
  __m256i a =_mm256_set1_epi32(1);
  __m256i b =_mm256_set1_epi32(1);
  __m256i c = vectorFibo(a,b,2048);
  return _mm512_extracti64x4_epi64(_mm256_extract_epi32(_mm256_extractf128_si256(c),7));
}
