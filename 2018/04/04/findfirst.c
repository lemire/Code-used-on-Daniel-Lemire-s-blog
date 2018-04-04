#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include <x86intrin.h>

#include "benchmark.h"

// return index of first occurence of a character or -1
int findfirst(const char * stream, size_t startindex, size_t length, const char target) {
  for(size_t i = startindex; i < length; i++) {
    if(stream[i] == target) return i;
  }
  return -1;
}

int strchr_findfirst(const char * stream, size_t startindex, size_t length, const char target) {
  const char * currentpos = strchr(stream + startindex, target);
  if(currentpos != NULL) {
    return currentpos - stream;
  }
  return -1;
}

int sse_findfirst(const char * stream, size_t startindex, size_t length, const char target) {
  size_t i = startindex;
  __m128i targetvec = _mm_set1_epi8(target);
  for(; i + sizeof(__m128i) <=  length; i += sizeof(__m128i)) {
      __m128i streamvec = _mm_loadu_si128((const __m128i*)(stream + i));
      __m128i mask = _mm_cmpeq_epi8(targetvec, streamvec);
      if(!_mm_testz_si128(mask,mask)) {
        int m = _mm_movemask_epi8(mask);
        return _tzcnt_u32(m) + i;
      }
  }
  for(; i < length; i++) {
    if(stream[i] == target) return i;
  }
  return -1;
}

int sse_movemask_findfirst(const char * stream, size_t startindex, size_t length, const char target) {
  size_t i = startindex;
  int m;
  __m128i targetvec = _mm_set1_epi8(target);
  for(; i + sizeof(__m128i) <=  length; i += sizeof(__m128i)) {
      __m128i streamvec = _mm_loadu_si128((const __m128i*)(stream + i));
      __m128i mask = _mm_cmpeq_epi8(targetvec, streamvec);
      if((m = _mm_movemask_epi8(mask))!=0) {
        return _tzcnt_u32(m) + i;
      }
  }
  for(; i < length; i++) {
    if(stream[i] == target) return i;
  }
  return -1;
}

int avx_findfirst(const char * stream, size_t startindex, size_t length, const char target) {
  size_t i = startindex;
  __m256i targetvec = _mm256_set1_epi8(target);
  for(; i + sizeof(__m256i) <=  length; i += sizeof(__m256i)) {
      __m256i streamvec = _mm256_loadu_si256((const __m256i*)(stream + i));
      __m256i mask = _mm256_cmpeq_epi8(targetvec, streamvec);
      if(!_mm256_testz_si256(mask,mask)) {
        int m = _mm256_movemask_epi8(mask);
        return _tzcnt_u32(m) + i;
      }
  }
  for(; i < length; i++) {
    if(stream[i] == target) return i;
  }
  return -1;
}

int avx_movemask_findfirst(const char * stream, size_t startindex, size_t length, const char target) {
  size_t i = startindex;
  int m;
  __m256i targetvec = _mm256_set1_epi8(target);
  for(; i + sizeof(__m256i) <=  length; i += sizeof(__m256i)) {
      __m256i streamvec = _mm256_loadu_si256((const __m256i*)(stream + i));
      __m256i mask = _mm256_cmpeq_epi8(targetvec, streamvec);
      if((m = _mm256_movemask_epi8(mask))!=0) {
        return _tzcnt_u32(m) + i;
      }
  }
  for(; i < length; i++) {
    if(stream[i] == target) return i;
  }
  return -1;
}

size_t findall(const char * stream, size_t length, const char target) {
  size_t sum = 0;
  int t = findfirst(stream, 0, length, target);
  while(t != -1) {
    sum ++;
    t = findfirst(stream, t + 1, length, target);
  }
  return sum;
}

size_t strchr_findall(const char * stream, size_t length, const char target) {
  size_t sum = 0;
  int t = strchr_findfirst(stream, 0, length, target);
  while(t != -1) {
    sum ++;
    t = strchr_findfirst(stream, t + 1, length, target);
  }
  return sum;
}

size_t sse_findall(const char * stream, size_t length, const char target) {
  size_t sum = 0;
  int t = avx_findfirst(stream, 0, length, target);
  while(t != -1) {
    sum ++;
    t = avx_findfirst(stream, t + 1, length, target);
  }
  return sum;
}


size_t sse_movemask_findall(const char * stream, size_t length, const char target) {
  size_t sum = 0;
  int t = avx_movemask_findfirst(stream, 0, length, target);
  while(t != -1) {
    sum ++;
    t = avx_movemask_findfirst(stream, t + 1, length, target);
  }
  return sum;
}

size_t avx_findall(const char * stream, size_t length, const char target) {
  size_t sum = 0;
  int t = avx_findfirst(stream, 0, length, target);
  while(t != -1) {
    sum ++;
    t = avx_findfirst(stream, t + 1, length, target);
  }
  return sum;
}


size_t avx_movemask_findall(const char * stream, size_t length, const char target) {
  size_t sum = 0;
  int t = avx_movemask_findfirst(stream, 0, length, target);
  while(t != -1) {
    sum ++;
    t = avx_movemask_findfirst(stream, t + 1, length, target);
  }
  return sum;
}

void demo() {
  printf("[demo]\n");
  const char target = 'x';
  size_t N = 1024;
  const int repeat = 5;
  char *array = malloc((N + 1) * sizeof(char));
  array[N] = 0;
  for(size_t matches = 1; matches <= N; matches *= 2) {
        printf("memset \n");

    memset(array,' ',N * sizeof(char));
            printf("rand \n");

    for(size_t k = 0; k < matches; k++) {
      array[rand() % N] = target;
    }
    size_t answer = findall(array, N, target);

    printf("N = %zu, matches = %zu, density %f \n", N, answer,(float) answer / (float) N);


    BEST_TIME(findall(array, N, target), answer, , repeat,  N, true);
    BEST_TIME(strchr_findall(array, N, target), answer, , repeat,  N, true);
    BEST_TIME(sse_findall(array, N, target), answer, , repeat,  N, true);
    BEST_TIME(sse_movemask_findall(array, N, target), answer, , repeat,  N, true);
    BEST_TIME(avx_findall(array, N, target), answer, , repeat,  N, true);
    BEST_TIME(avx_movemask_findall(array, N, target), answer, , repeat,  N, true);
    printf("===\n");

  }
  free(array);
}
int main() {
  demo();
  return EXIT_SUCCESS;
}
