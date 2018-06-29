// gcc  -std=c99 -O3 -o shotguntest shotguntest.c -Wall -Wextra -lm

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "random.h"
#include "benchutil.h"


// flushes the array from cache
void array_cache_flush(uint32_t *B, int32_t length) {
  const int32_t CACHELINESIZE = 64; // 64 bytes per cache line
  for (int32_t k = 0; k < length; k += CACHELINESIZE / sizeof(uint32_t)) {
    __builtin_ia32_clflush(B + k);
  }
}

// tries to put the array in cache
void array_cache_prefetch(uint32_t *B, int32_t length) {
  const int32_t CACHELINESIZE = 64; // 64 bytes per cache line
  for (int32_t k = 0; k < length; k += CACHELINESIZE / sizeof(uint32_t)) {
    __builtin_prefetch(B + k);
  }
}

// good old bin. search
int32_t __attribute__((noinline))
binary_search(uint32_t *array, int32_t lenarray, uint32_t ikey) {
  int32_t low = 0;
  int32_t high = lenarray - 1;
  while (low <= high) {
    int32_t middleIndex = (low + high) >> 1;
    uint32_t middleValue = array[middleIndex];
    if (middleValue < ikey) {
      low = middleIndex + 1;
    } else if (middleValue > ikey) {
      high = middleIndex - 1;
    } else {
      return middleIndex;
    }
  }
  return -(low + 1);
}

// experimental "shotgun" bin. search
int32_t __attribute__((noinline))
shotgun_binary_search(uint32_t *array, int32_t lenarray, uint32_t ikey) {
  int32_t low = 0;
  int32_t high = lenarray - 1;
  while (low <= high) {

    int32_t middleIndex = (low + high) >> 1;
    uint32_t middleValue = array[middleIndex];

    int32_t quarterIndex = (low + middleIndex) >> 1;
    uint32_t quarterValue = array[quarterIndex];

    int32_t threequarterIndex = (quarterIndex + high) >> 1;
    uint32_t threequarterValue = array[threequarterIndex];

    if (middleValue < ikey) {
      if (threequarterValue < ikey) {
        low = threequarterIndex + 1;
      } else if (threequarterValue > ikey) {
        high = threequarterIndex - 1;
        low = middleIndex + 1;
      } else
        return threequarterIndex;
    } else if (middleValue > ikey) {
      if (quarterValue < ikey) {
        low = quarterIndex + 1;
        high = middleIndex - 1;
      } else if (quarterValue > ikey) {
        high = quarterIndex - 1;
      } else
        return quarterIndex;
    } else {
      return middleIndex;
    }
  }
  return -(low + 1);
}

//  Paul-Virak Khuong and Pat Morin, http://arxiv.org/pdf/1509.05053.pdf
int32_t __attribute__((noinline))
branchless_binary_search(uint32_t *source, int32_t n, uint32_t target) {
  uint32_t *base = source;
  if (n == 0)
    return -1;
  if (target > source[n - 1])
    return -n - 1; // without this we have a buffer overrun
  while (n > 1) {
    int32_t half = n >> 1;
    base = (base[half] < target) ? &base[half] : base;
    n -= half;
  }
  base += *base < target;
  return *base == target ? base - source : source - base - 1;
}

void demo() {
  size_t nbrtestvalues = 5;
  uint32_t *testvalues = create_random_array(nbrtestvalues);
  int32_t bogus = 0;
  printf("# Objective: fast search in large arrays.\n");
  printf("# We report the average number of cycles per query.\n");
  size_t N = 100000000;
  printf("# Array size = %zu.\n", N);
  printf("# We do  %zu consecutive queries before flushing the cache.\n", nbrtestvalues);

  printf("# creating sorted random array (takes some time) ");
  fflush(NULL);
  uint32_t *source = create_sorted_array(N);
  printf(" Done! \n");
  printf("# Running sanity tests: ");
  fflush(NULL);
  ASSERT_PRE_ARRAY(source, N, binary_search, testvalues, nbrtestvalues);
  ASSERT_PRE_ARRAY(source, N, branchless_binary_search, testvalues, nbrtestvalues);
  ASSERT_PRE_ARRAY(source, N, shotgun_binary_search, testvalues, nbrtestvalues);
  printf(" Done! \n");

  BEST_TIME_PRE_ARRAY(source, N, binary_search, array_cache_flush, testvalues,
                      nbrtestvalues, bogus);
  BEST_TIME_PRE_ARRAY(source, N, branchless_binary_search, array_cache_flush,
                      testvalues, nbrtestvalues, bogus);
  BEST_TIME_PRE_ARRAY(source, N, shotgun_binary_search, array_cache_flush,
                      testvalues, nbrtestvalues, bogus);

  free(source);
  printf("\n");
  printf("bogus = %d \n", bogus);
  free(testvalues);
}
int main() {
  for(int t = 0; t < 3; ++t) {
  demo();
  printf("=====\n\n");
}
  return 0;
}
