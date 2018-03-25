// gcc -o cachemissbench cachemissbench.c  -mbmi2 -mavx2 -O3 && ./cachemissbench
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "simdprune.h"

__attribute__ ((noinline))
void randomize(int * bitmasks, int N, int mask) {
  for (int k = 0; k < N; k++) {
    bitmasks[k] = rand() & mask;
  }
}

__attribute__ ((noinline))
__m128i runprune_epi8(int * bitmasks, int N, __m128i *x) {
  for (int k = 0; k < N; k++) {
    *x = prune_epi8(*x, bitmasks[k]);
  }
  return *x;
}

int main() {
  const int N = 1024 * 1024;
  int * bitmasks = malloc(sizeof(int) * N);
  randomize(bitmasks, N, (1<<16)-1);
  __m128i x = _mm_set_epi8(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
  runprune_epi8(bitmasks, N, &x);
  free(bitmasks);
  return _mm_extract_epi8(x,0) ;
}
