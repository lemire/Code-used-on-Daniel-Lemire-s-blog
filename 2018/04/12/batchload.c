// gcc -O3 -o batchload batchload.c  -march=native
#include "benchmark.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t murmur32(uint32_t h) {
  h ^= h >> 16;
  h *= UINT32_C(0x85ebca6b);
  h ^= h >> 13;
  h *= UINT32_C(0xc2b2ae35);
  h ^= h >> 16;
  return h;
}

void populaterandom(uint32_t *indexes, size_t size) {
  assert((size & (size - 1)) == 0); // power of two
  for (size_t k = 0; k < size; ++k) {
    indexes[k] = murmur32(k) & (size - 1);
  }
}

uint64_t sumrandom(uint64_t *values, size_t size) {
  uint64_t sum = 0;
  for (size_t k = 0; k < size; ++k) {
    sum += values[murmur32(k) & (size - 1)];
  }
  return sum;
}

uint64_t sumrandomandindexes(uint64_t *values, uint32_t *indexes, size_t size) {
  uint64_t sum = 0;
  assert((size & (size - 1)) == 0); // power of two
  for (size_t k = 0; k < size; ++k) {
    indexes[k] = murmur32(k) & (size - 1);
  }
  for (size_t k = 0; k < size; ++k) {
    sum += values[indexes[k]];
  }
  return sum;
}

uint64_t sumrandomfromindexes(uint64_t *values, uint32_t *indexes,
                              size_t size) {
  uint64_t sum = 0;
  for (size_t k = 0; k < size; ++k) {
    sum += values[indexes[k]];
  }
  return sum;
}

void demo() {
  size_t N = 1024 * 1024 * 64;

  printf("[demo] N= %zu \n", N);
  assert((N & (N - 1)) == 0); // power of two

  uint64_t *values = malloc(N * sizeof(uint64_t));
  uint32_t *indexes = malloc(N * sizeof(uint32_t));
  for (size_t i = 0; i < N; i++) {
    values[i] = i * 3 - 2;
  }
  uint64_t answer = sumrandom(values, N);

#define repeat 5
  BEST_TIME_NOCHECK(populaterandom(indexes, N), , repeat, N, true);
  BEST_TIME(sumrandom(values, N), answer, , repeat, N, true);
  BEST_TIME(sumrandomandindexes(values, indexes, N), answer, , repeat, N, true);
  BEST_TIME(sumrandomfromindexes(values, indexes, N), answer, , repeat, N,
            true);

  free(values);

  free(indexes);
}

int main() {
  demo();
  return EXIT_SUCCESS;
}
