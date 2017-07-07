// gcc -std=c99 -O3 -o despacebenchmark despacebenchmark.c
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "despacer.h"


const char * nameofcycles = "ns";


#include <time.h>

#define RDTSC_START(cycles) \
    do {                    \
       struct timeval tv; \
       gettimeofday(&tv, NULL); \
       cycles =  (uint64_t)tv.tv_sec * 1000000000 + tv.tv_usec*1000; \
    } while (0)

#define RDTSC_FINAL(cycles) \
    do {                    \
        RDTSC_START(cycles);   \
    } while (0)



#define BEST_TIME_NOCHECK(test, pre, repeat, size)                                  \
  do {                                                                         \
    printf("%-40s: ", #test);                                                     \
    fflush(NULL);                                                              \
    uint64_t cycles_start, cycles_final, cycles_diff;                          \
    uint64_t min_diff = (uint64_t)-1;                                          \
    for (int i = 0; i < repeat; i++) {                                         \
      pre;                                                                     \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      RDTSC_START(cycles_start);                                               \
      test;                                                       \
      RDTSC_FINAL(cycles_final);                                               \
      cycles_diff = (cycles_final - cycles_start);                             \
      if (cycles_diff < min_diff)                                              \
        min_diff = cycles_diff;                                                \
    }                                                                          \
    uint64_t S = (uint64_t)size;                                               \
    float cycle_per_op = (min_diff) / (float)S;                                \
    printf(" %.3f %s per operation", cycle_per_op, nameofcycles);                        \
    printf("\n");                                                              \
    fflush(NULL);                                                              \
  } while (0)

#define BEST_TIME_NOCHECK_NOPRE(test, repeat, size)                                  \
  do {                                                                         \
    printf("%-40s: ", #test);                                                     \
    fflush(NULL);                                                              \
    uint64_t cycles_start, cycles_final, cycles_diff;                          \
    uint64_t min_diff = (uint64_t)-1;                                          \
    RDTSC_START(cycles_start);                                               \
    for (int i = 0; i < repeat; i++) {                                         \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
       test;                                                       \
    }                                                                          \
    RDTSC_FINAL(cycles_final);                                               \
    cycles_diff = (cycles_final - cycles_start);                             \
    min_diff = cycles_diff;                                                \
    uint64_t S = (uint64_t)size;                                               \
    float cycle_per_op = (min_diff) / ((float)S * repeat);                                \
    printf(" %.3f %s per operation", cycle_per_op, nameofcycles);                        \
    printf("\n");                                                              \
    fflush(NULL);                                                              \
  } while (0)




#define BEST_TIME_NOPRE(test, answer,  repeat, size)                                  \
  do {                                                                         \
    printf("%-40s: ", #test);                                                     \
    fflush(NULL);                                                              \
    uint64_t cycles_start, cycles_final, cycles_diff;                          \
    uint64_t min_diff = (uint64_t)-1;                                          \
    int wrong_answer = 0;                                                      \
    RDTSC_START(cycles_start);                                               \
    for (int i = 0; i < repeat; i++) {                                         \
      if ((test != answer) && (wrong_answer == 0)) {                                                    \
        printf("ERROR: returned: %d, expected: %d\n", (int) test, (int)answer); \
        wrong_answer = 1;                                                      \
      }                                                                          \
    }                                                                          \
    RDTSC_FINAL(cycles_final);                                               \
    cycles_diff = (cycles_final - cycles_start);                             \
    min_diff = cycles_diff;                                                \
    uint64_t S = (uint64_t)size;                                               \
    float cycle_per_op = (min_diff) / ((float)S * repeat);                                \
    printf(" %.3f %s per operation", cycle_per_op, nameofcycles);                        \
    if (wrong_answer)                                                          \
      printf(" [ERROR]");                                                      \
    printf("\n");                                                              \
    fflush(NULL);                                                              \
  } while (0)



#define BEST_TIME(test, answer, pre, repeat, size)                                  \
  do {                                                                         \
    printf("%-40s: ", #test);                                                     \
    fflush(NULL);                                                              \
    uint64_t cycles_start, cycles_final, cycles_diff;                          \
    uint64_t min_diff = (uint64_t)-1;                                          \
    int wrong_answer = 0;                                                      \
    for (int i = 0; i < repeat; i++) {                                         \
       pre;                                                                     \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      RDTSC_START(cycles_start);                                               \
      if (test != answer)                                                      \
        wrong_answer = 1;                                                      \
      RDTSC_FINAL(cycles_final);                                               \
      cycles_diff = (cycles_final - cycles_start);                             \
      if (cycles_diff < min_diff)                                              \
        min_diff = cycles_diff;                                                \
    }                                                                          \
    uint64_t S = (uint64_t)size;                                               \
    float cycle_per_op = (min_diff) / (float)S;                                \
    printf(" %.2f %s per operation", cycle_per_op, nameofcycles);                        \
    if (wrong_answer)                                                          \
      printf(" [ERROR]");                                                      \
    printf("\n");                                                              \
    fflush(NULL);                                                              \
  } while (0)


// let us estimate that we have a 1% proba of hitting a white space
size_t fillwithtext(char *buffer, size_t size) {
  size_t howmany = 0;
  for (size_t i = 0; i < size; ++i) {
    double r = ((double)rand() / (RAND_MAX));
    if (r < 0.01) {
      buffer[i] = ' ';
      howmany++;
    } else if (r < 0.02) {
      buffer[i] = '\n';
      howmany++;
    } else if (r < 0.03) {
      buffer[i] = '\r';
      howmany++;
    } else {
      do {
        buffer[i] = (char)rand();
      } while (buffer[i] <= 32);
    }
  }
  return howmany;
}


int main(int argc, char ** argv) {
  const int N = 1024 * 32;
  int alignoffset = 0;
  if(argc>1)  {
    alignoffset = atoi(argv[1]);
    printf("alignment offset = %d \n", alignoffset);
  }
  char *origbuffer = malloc(N + alignoffset);
  char *origtmpbuffer = malloc(N + alignoffset);
  char *buffer = origbuffer + alignoffset;
  char *tmpbuffer = origtmpbuffer + alignoffset;
  printf("pointer alignment = %d bytes \n", 1<< __builtin_ctzll((uintptr_t)(const void *)(buffer)));

  int repeat = 100;
  size_t howmanywhite = fillwithtext(buffer, N);

  BEST_TIME_NOCHECK_NOPRE(memcpy(tmpbuffer,buffer,N),
                   repeat, N);
  printf("\n");
  BEST_TIME(despace(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME(neon_despace(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  BEST_TIME(neon_despace_branchless(buffer, N), N - howmanywhite,
                  howmanywhite = fillwithtext(buffer, N), repeat, N);
  free(origbuffer);
  free(origtmpbuffer);
}
