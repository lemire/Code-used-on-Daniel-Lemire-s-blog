#include <x86intrin.h>
#include <stdint.h>
#include <stdio.h>

/* returns the integer logarithm of v (bit width) */
uint32_t bits(const uint32_t v) {
#ifdef _MSC_VER
  unsigned long answer;
  if (v == 0) {
    return 0;
  }
  _BitScanReverse(&answer, v);
  return answer + 1;
#else
  return v == 0 ? 0
                : 32 - __builtin_clz(
                           v); /* assume GCC-like compiler if not microsoft */
#endif
}

static uint32_t maxbitas32int(const __m256i accumulator) {
  const __m256i _tmp1 =
      _mm256_or_si256(_mm256_srli_si256(accumulator, 8), accumulator);
  const __m256i _tmp2 = _mm256_or_si256(_mm256_srli_si256(_tmp1, 4), _tmp1);
  uint32_t ans1 = _mm256_extract_epi32(_tmp2, 0);
  uint32_t ans2 = _mm256_extract_epi32(_tmp2, 4);
  uint32_t ans = ans1 > ans2 ? ans1 : ans2;
  return ans;
}

static uint32_t avx512maxbitas32int(const __m512i accumulator) {
  uint32_t ans1 = maxbitas32int(_mm512_castsi512_si256(accumulator));
  uint32_t ans2 = maxbitas32int(_mm512_extracti64x4_epi64(accumulator, 1));
  printf("ans1 = %u ans2 = % u\n", ans1, ans2);
  uint32_t ans = ans1 > ans2 ? ans1 : ans2;
  return bits(ans);
}

int main() {
  __m512i x =
      _mm512_set_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  printf("max %u \n", avx512maxbitas32int(x));
}
