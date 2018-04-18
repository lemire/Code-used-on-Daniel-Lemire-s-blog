// gcc -o maskcompress -march=native maskcompress.c
#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>

int main() {

  __m512i xx =
      _mm512_set_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  __m512i yy =
      _mm512_set_epi32(1, 1, 3, 3, 5, 5, 7, 7, 9, 9, 11, 11, 13, 13, 15, 15);
  __mmask16 zz = _mm512_cmpeq_epi32_mask(xx, yy);
  printf("%d\n", sizeof(zz));
  printf("%d\n", zz);
  __m512i ww = _mm512_mask_compress_epi32(_mm512_setzero_epi32(), zz, xx);
  uint32_t buf[16];
  _mm512_storeu_si512(buf, ww);
  for (int k = 0; k < 16; k++) {
    printf("%d ", buf[k]);
  }
  printf("\n");
}
