#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "benchmark.h"

void count35(uint32_t N, uint32_t *count3, uint32_t *count5) {
  for (uint32_t i = 0; i < N; i++) {
    if ((i % 3) == 0)
      *count3 += 1;
    if ((i % 5) == 0)
      *count5 += 1;
  }
}

// M = ceil( (1<<64) / d ), d > 0
static inline uint64_t computeM_u32(uint32_t d) {
  return UINT64_C(0xFFFFFFFFFFFFFFFF) / d + 1;
}

// given precomputed M, checks whether n % d == 0
static inline bool is_divisible(uint32_t n, uint64_t M) {
  return n * M <= M - 1;
}

void fastcount35(uint32_t N, uint32_t *count3, uint32_t *count5) {
  uint64_t M3 = computeM_u32(3);
  uint64_t M5 = computeM_u32(5);
  for (uint32_t i = 0; i < N; i++) {
    if (is_divisible(i, M3))
      *count3 += 1;
    if (is_divisible(i, M5))
      *count5 += 1;
  }
}
void demo() {
  uint32_t N = 100000000;
  bool verbose = true;
  int repeat = 50;
  uint32_t count3 = 0;
  uint32_t count5 = 0;
  BEST_TIME_NOCHECK(count35(N, &count3, &count5), , repeat, N, verbose);
  printf("%u %u \n", count3, count5);
  count3 = 0;
  count5 = 0;
  BEST_TIME_NOCHECK(fastcount35(N, &count3, &count5), , repeat, N, verbose);
  printf("%u %u \n", count3, count5);
}

int main() {
  demo();
  return EXIT_SUCCESS;
}
