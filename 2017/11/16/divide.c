// gcc -O3 -o divide divide.c && ./divide
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "benchmark.h"

uint64_t  int_div64(uint64_t a, uint64_t b) {
  return a / b;
}

double float_div64(double a, double b) {
  return a / b;
}


uint32_t  int_div32(uint32_t a, uint32_t b) {
  return a / b;
}

double float_div32(float a, float b) {
  return a / b;
}

 __attribute__((noinline))
void populate_int64(uint64_t num, uint64_t * array, uint64_t size) {
  for(uint64_t i = 1; i <= size; i++) {
    array[i] = int_div64(num, i);
  }
}

 __attribute__((noinline))
void populate_float64(uint64_t num, double * array, uint64_t size) {
  for(uint64_t i = 1; i <= size; i++) {
    array[i] = float_div64(num, i);
  }
}


 __attribute__((noinline))
void populate_int32(uint32_t num, uint32_t * array, uint32_t size) {
  for(uint32_t i = 1; i <= size; i++) {
    array[i] = int_div32(num, i);
  }
}

 __attribute__((noinline))
void populate_int32viafloat64(uint32_t num, uint32_t * array, uint32_t size) {
  for(uint32_t i = 1; i <= size; i++) {
    array[i] = (uint32_t) float_div64(num, i);
  }
}


 __attribute__((noinline))
void populate_float32(uint32_t num, float * array, uint32_t size) {
  for(uint32_t i = 1; i <= size; i++) {
    array[i] = float_div32(num, i);
  }
}

void demo(size_t N) {
  double * farray64 = (double *) malloc(N * sizeof(double));
  uint64_t * iarray64 = (uint64_t *) malloc(N * sizeof(uint64_t));
  float * farray32 = (float *) malloc(N * sizeof(float));
  uint32_t * iarray32 = (uint32_t *) malloc(N * sizeof(uint32_t));

  int repeat = 5;
  uint64_t inum64 = 1000000;
  BEST_TIME_NOCHECK(populate_int64(inum64,iarray64,N),memset(iarray64,0,N * sizeof(uint64_t)) , repeat, N, true);
  BEST_TIME_NOCHECK(populate_float64(inum64,farray64,N),memset(farray64,0,N * sizeof(double)) , repeat, N, true);
  BEST_TIME_NOCHECK(populate_int32(inum64,iarray32,N),memset(iarray32,0,N * sizeof(uint32_t)) , repeat, N, true);
  BEST_TIME_NOCHECK(populate_int32viafloat64(inum64,iarray32,N),memset(iarray32,0,N * sizeof(uint32_t)) , repeat, N, true);
  BEST_TIME_NOCHECK(populate_float32(inum64,farray32,N),memset(farray32,0,N * sizeof(float)) , repeat, N, true);
}


int main() {
  printf("clocks per s = %d \n", CLOCKS_PER_SEC);
  demo(1000);
  return 0;
}
