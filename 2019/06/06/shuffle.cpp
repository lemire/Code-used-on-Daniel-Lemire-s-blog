#include "rangedrand.h"
#include "shuffle.h"
#include "benchmark.h"
#include <algorithm>
#include <math.h>
#include <random>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Sorting stuff
*/
#define INCLUDESTDSHUFFLE
int qsort_compare_uint32_t(const void *a, const void *b) {
  return (*(uint32_t *)a - *(uint32_t *)b);
}

uint32_t *create_sorted_array(size_t length) {
  uint32_t *array = (uint32_t *)malloc(length * sizeof(uint32_t));
  for (size_t i = 0; i < length; i++)
    array[i] = (uint32_t)lehmer64();
  qsort(array, length, sizeof(*array), qsort_compare_uint32_t);
  return array;
}

uint32_t *create_random_array(size_t count) {
  uint32_t *targets = (uint32_t *)malloc(count * sizeof(uint32_t));
  for (size_t i = 0; i < count; i++)
    targets[i] = (uint32_t)(lehmer64() & 0x7FFFFFF);
  return targets;
}

// tries to put the array in cache
void array_cache_prefetch(uint32_t *B, int32_t length) {
  const int32_t CACHELINESIZE = 64; // 64 bytes per cache line
  for (int32_t k = 0; k < length; k += CACHELINESIZE / sizeof(uint32_t)) {
    __builtin_prefetch(B + k);
  }
}

#if defined(__x86_64) || defined(__i386__)
void array_cache_flush(uint32_t *B, int32_t length) {
  const int32_t CACHELINESIZE = 64; // 64 bytes per cache line
  for (int32_t k = 0; k < length; k += CACHELINESIZE / sizeof(uint32_t)) {
    __builtin_ia32_clflush(B + k);
  }
}
#else
void array_cache_flush(uint32_t *B, int32_t length) { (void) B; }

#endif

int sortAndCompare(uint32_t *shuf, uint32_t *orig, uint32_t size) {
  qsort(shuf, size, sizeof(uint32_t), qsort_compare_uint32_t);
  qsort(orig, size, sizeof(uint32_t), qsort_compare_uint32_t);
  for (uint32_t k = 0; k < size; ++k)
    if (shuf[k] != orig[k]) {
      printf("[bug]\n");
      return -1;
    }
  return 0;
}
#define INCLUDEFLOAT

template <randfnc64 rfnc64> void ShuffleBenchmark64(size_t size) {
    bool verbose = true;
    bool prefetch = true;
    printf(" %s\n", __PRETTY_FUNCTION__);
    printf("Shuffling arrays of size %zu \n", size);
    printf("Time reported in number of ns per array element.\n");
    printf("Tests assume that array is in cache as much as possible.\n");
  int repeat = 10;
  if (size < 1000000)
    repeat = 50;
  if (size > 10000000)
    repeat = 5;
  bool sortandcompare = size < 1000000;
  uint32_t *testvalues = create_random_array(size);
  uint32_t *pristinecopy = (uint32_t *)malloc(size * sizeof(uint32_t));
  memcpy(pristinecopy, testvalues, sizeof(uint32_t) * size);
  uint32_t *precomputed = (uint32_t *) malloc((size + 1) * sizeof(uint32_t));
  for(size_t i = 1; i <= size; i++) precomputed[i] = (uint32_t)java_random_bounded64<rfnc64>(i);


  BEST_TIME_NS(shuffle_go64<rfnc64>(testvalues, size),
               prefetch ? array_cache_prefetch(testvalues, size) : array_cache_flush(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;

  BEST_TIME_NS(shuffle_java64<rfnc64>(testvalues, size),
               prefetch ? array_cache_prefetch(testvalues, size) : array_cache_flush(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;
  BEST_TIME_NS(shuffle_nearlydivisionless64<rfnc64>(testvalues, size),
               prefetch ? array_cache_prefetch(testvalues, size) : array_cache_flush(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;
  BEST_TIME_NS(shuffle_floatmult64<rfnc64>(testvalues, size),
               prefetch ? array_cache_prefetch(testvalues, size) : array_cache_flush(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;
  UniformRandomBitGenerator64Struct<rfnc64> gen;
  BEST_TIME_NS(std::shuffle(testvalues, testvalues + size, gen),
               prefetch ? array_cache_prefetch(testvalues, size) : array_cache_flush(testvalues, size), repeat, size, verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;

  free(testvalues);
  free(pristinecopy);
  printf("\n");
}

int main(int argc, char** argv) {

  setseed(12345);
  size_t N = 1000;

  ShuffleBenchmark64<lehmer64>(N);
  return 0;
}