/* The Computer Language Benchmarks Game
* http://benchmarksgame.alioth.debian.org/

  contributed by Greg Buchholz

*/
#include <stdlib.h>
#include <stdio.h>
#ifdef USEAVX512
#include <x86intrin.h>
#endif

int main(int argc, char **argv) {
  int w, h, bit_num = 0;
  char byte_acc = 0;
  int i, iter = 50;
  double x, y, limit = 2.0;
  double Zr, Zi, Cr, Ci, Tr, Ti;

  w = h = argc > 1 ? atoi(argv[1]) : 32000;

  printf("P4\n%d %d\n", w, h);
#ifdef USEAVX512
  __m512i a = _mm512_set1_epi32(0);
  __m512i b = _mm512_set1_epi32(1);
  __m512i t;
#endif
  for (y = 0; y < h; ++y) {
#ifdef USEAVX512
    t = a;
    a = b;
#ifdef USEHEAVYAVX512
    b = _mm512_mul_epi32(b, t);
#else
    b = _mm512_add_epi32(b, t);
#endif
#endif
    for (x = 0; x < w; ++x) {
      Zr = Zi = Tr = Ti = 0.0;
      Cr = (2.0 * x / w - 1.5);
      Ci = (2.0 * y / h - 1.0);

      for (i = 0; i < iter && (Tr + Ti <= limit * limit); ++i) {
        Zi = 2.0 * Zr * Zi + Ci;
        Zr = Tr - Ti + Cr;
        Tr = Zr * Zr;
        Ti = Zi * Zi;
      }

      byte_acc <<= 1;
      if (Tr + Ti <= limit * limit)
        byte_acc |= 0x01;

      ++bit_num;

      if (bit_num == 8) {
        // putc(byte_acc,stdout);
        byte_acc = 0;
        bit_num = 0;
      } else if (x == w - 1) {
        byte_acc <<= (8 - w % 8);
        // putc(byte_acc,stdout);
        byte_acc = 0;
        bit_num = 0;
      }
    }
  }
#ifdef USEAVX512
  printf("we used avx512 %d \n", _mm256_extract_epi32(_mm512_extracti64x4_epi64(b, 1), 7));
#else
  printf("we did not use avx512\n");
#endif
  return 0;
}
