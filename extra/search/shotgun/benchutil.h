#ifndef BENCHUTIL_H
#define BENCHUTIL_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define RDTSC_START(cycles)                                                    \
  do {                                                                         \
    register unsigned cyc_high, cyc_low;                                       \
    __asm volatile("cpuid\n\t"                                                 \
                   "rdtsc\n\t"                                                 \
                   "mov %%edx, %0\n\t"                                         \
                   "mov %%eax, %1\n\t"                                         \
                   : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx",    \
                     "%rdx");                                                  \
    (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                           \
  } while (0)

#define RDTSC_FINAL(cycles)                                                    \
  do {                                                                         \
    register unsigned cyc_high, cyc_low;                                       \
    __asm volatile("rdtscp\n\t"                                                \
                   "mov %%edx, %0\n\t"                                         \
                   "mov %%eax, %1\n\t"                                         \
                   "cpuid\n\t"                                                 \
                   : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx",    \
                     "%rdx");                                                  \
    (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                           \
  } while (0)

/*
 * This is like BEST_TIME except that ... it runs functions "test" using the
 * first parameter "base" and various parameters from "testvalues" (there
 * are nbrtestvalues), calling pre on base between tests
 */
#define BEST_TIME_PRE_ARRAY(base, length, test, pre, testvalues,               \
                            nbrtestvalues, bogus)                              \
  do {                                                                         \
    fflush(NULL);                                                              \
    uint64_t cycles_start, cycles_final, cycles_diff;                          \
    uint64_t min_diff = (uint64_t)-1;                                          \
    int sum = 0;                                                               \
    printf("[%s %s] ", #test, #pre);                                           \
    for (size_t j = 0; j < nbrtestvalues; j++) {                               \
      pre(base, length);                                                       \
      pre(base, length);                                                       \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      RDTSC_START(cycles_start);                                               \
      bogus += test(base, length, testvalues[j]);                              \
      RDTSC_FINAL(cycles_final);                                               \
      cycles_diff = (cycles_final - cycles_start);                             \
      if (cycles_diff < min_diff)                                              \
        min_diff = cycles_diff;                                                \
      sum += cycles_diff;                                                      \
    }                                                                          \
    uint64_t S = nbrtestvalues;                                                \
    double cpo = sum / (double)S;                                              \
    printf("%f \n", cpo);                                                      \
  } while (0)

#define ASSERT_PRE_ARRAY(base, length, test, testvalues, nbrtestvalues)        \
  do {                                                                         \
    int error = 0;                                                             \
    for (size_t j = 0; j < nbrtestvalues; j++) {                               \
      int32_t re = test(base, length, testvalues[j]);                          \
      if (re >= 0) {                                                           \
        if (base[re] != testvalues[j]) {                                       \
          error = 1;                                                           \
          break;                                                               \
        }                                                                      \
      } else {                                                                 \
        int32_t ip = -re - 1;                                                  \
        if (ip < (int32_t)length) {                                            \
          if (base[ip] <= testvalues[j]) {                                     \
            printf(" Index returned is %d which points at %d for target %d. ", \
                   ip, base[ip], testvalues[j]);                               \
            error = 3;                                                         \
            break;                                                             \
          }                                                                    \
        }                                                                      \
        if (ip > 0) {                                                          \
          if (base[ip - 1] >= testvalues[j]) {                                 \
            printf(" Index returned is %d which points at %d for target %d. ", \
                   ip, base[ip], testvalues[j]);                               \
            printf(" Previous index is %d which points at %d for target %d. ", \
                   ip - 1, base[ip - 1], testvalues[j]);                       \
            error = 4;                                                         \
            break;                                                             \
          }                                                                    \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    if (error != 0)                                                            \
      printf("[%s error: %d]\n", #test, error);                                \
  } while (0)


#endif
