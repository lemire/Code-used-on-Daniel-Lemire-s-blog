// gcc -O3 -o timings timings.c -march=native  && ./timings
#include "benchmark.h"
#include <assert.h>
#include <immintrin.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stddef.h>
#include <stdint.h>

static inline uint64_t interleave_uint32_with_zeros(uint32_t input)  {
    uint64_t word = input;
    word = (word ^ (word << 16)) & 0x0000ffff0000ffff;
    word = (word ^ (word << 8 )) & 0x00ff00ff00ff00ff;
    word = (word ^ (word << 4 )) & 0x0f0f0f0f0f0f0f0f;
    word = (word ^ (word << 2 )) & 0x3333333333333333;
    word = (word ^ (word << 1 )) & 0x5555555555555555;
    return word;
}



static inline uint32_t deinterleave_lowuint32(uint64_t word) {
    word &= 0x5555555555555555;
    word = (word ^ (word >> 1 )) & 0x3333333333333333;
    word = (word ^ (word >> 2 )) & 0x0f0f0f0f0f0f0f0f;
    word = (word ^ (word >> 4 )) & 0x00ff00ff00ff00ff;
    word = (word ^ (word >> 8 )) & 0x0000ffff0000ffff;
    word = (word ^ (word >> 16)) & 0x00000000ffffffff;
    return (uint32_t)word;
}

typedef struct  {
  uint32_t x;
  uint32_t y;
} uint32_2;


uint64_t interleave(uint32_2 input)  {
    return interleave_uint32_with_zeros(input.x) | (interleave_uint32_with_zeros(input.y) << 1);
}

void interleave_array(uint32_2 *input, size_t length, uint64_t * out)  {
  for(size_t i = 0; i < length; i++) {
    out[i] = interleave(input[i]);
  }
}

uint32_2 deinterleave(uint64_t input)  {
  uint32_2 answer;
  answer.x = deinterleave_lowuint32(input);
  answer.y = deinterleave_lowuint32(input>>1);
  return answer;
}

void deinterleave_array(uint64_t *input, size_t length, uint32_2 * out)  {
  for(size_t i = 0; i < length; i++) {
    out[i] = deinterleave(input[i]);
  }
}


void demo(size_t N) {
  printf("N = %zu \n", N);
  uint32_2* array =  (uint32_2 *)malloc(N * sizeof(uint32_2));
  uint64_t* storedarray =  (uint64_t *)malloc(N * sizeof(uint64_t));
  for(uint32_t k = 0; k < N; k++) {
    array[k].x = k;
    array[k].y = k+1;
  }
  int repeat = 2048;
  uint64_t * timings1 = (uint64_t*) malloc(repeat * sizeof(uint64_t));
  uint64_t * timings2 = (uint64_t*) malloc(repeat * sizeof(uint64_t));
  BEST_TIME_NOCHECK(interleave_array(array,N,storedarray),, repeat, N, timings1, true);
  BEST_TIME_NOCHECK(deinterleave_array(storedarray,N,array),, repeat, N, timings2, true);
  for(uint32_t k = 0; k < N; k++) {
    assert(array[k].x == k);
    assert(array[k].y == k+1);
  }
  for(int i = 0; i < repeat; i++) {
    printf("%d %f %f \n", i, (double) timings1[i] / N, (double) timings2[i] / N);
  }
  free(timings1);
  free(timings2);

  free(array);
  free(storedarray);

}
int main() {
  demo(1000);
  return 0;
}
