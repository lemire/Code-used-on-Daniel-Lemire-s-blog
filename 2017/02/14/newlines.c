// gcc -O3 -o newlines newlines.c -march=native
// inspired by https://github.com/llogiq/newlinebench



#include "benchmark.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "x86intrin.h"


char * get_realistic(size_t size) {
  char * answer = (char*) malloc(size);
  for(size_t i = 0; i < size; i++) {
    if(rand() % 100 == 57)
      answer[i] = '\n';
    else
      answer[i] = rand();
  }
  return answer;
}


size_t basiccount(char * buffer, size_t size) {
  size_t cnt = 0;
  for(size_t i = 0; i < size; i++)
    if(buffer[i] == '\n') cnt ++;
  return cnt;
}


size_t avxcount(char * buffer, size_t size) {
  size_t answer = 0;
  __m256i cnt = _mm256_setzero_si256();
  __m256i newline = _mm256_set1_epi8('\n');
  __m256i mask1 = _mm256_set1_epi8(1);
  size_t i = 0;
  uint8_t tmpbuffer[sizeof(__m256i)];
  while( i + 32 <= size ) {
    size_t remaining = size - i;
    size_t howmanytimes =  remaining / 32;
    if(howmanytimes > 256) howmanytimes = 256;
    const __m256i * buf = (const __m256i *) (buffer + i);
    for (size_t j = 0; j <  howmanytimes; j++) {
      __m256i newdata = _mm256_lddqu_si256(buf + j);
      __m256i cmp = _mm256_cmpeq_epi8(newline,newdata);
      cmp = _mm256_and_si256(mask1, cmp);
      cnt = _mm256_add_epi8(cnt,cmp);
    }
    i += howmanytimes * 32;
    _mm256_storeu_si256((__m256i *) tmpbuffer,cnt);
    for(int k = 0; k < sizeof(__m256i); ++k) answer += tmpbuffer[k];
    cnt = _mm256_setzero_si256();
  }
  for(; i < size; i++)
    if(buffer[i] == '\n') answer ++;
  return answer;
}


int main() {
   size_t N = 10000;
   char * buffer = get_realistic(N);
   size_t tc = basiccount(buffer,N);
   const int repeat = 5;
   BEST_TIME(basiccount(buffer,N),tc, , repeat, N, true);
   BEST_TIME(avxcount(buffer,N),tc, , repeat, N, true);
   free(buffer);
}
