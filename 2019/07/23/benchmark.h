#include <chrono>
#include <stdint.h>

typedef std::chrono::high_resolution_clock Clock;
#define BEST_TIME_NS(test, pre, repeat, size, verbose)                         \
  do {                                                                         \
    if (verbose)                                                               \
      printf("%-60s: ", #test);                                                \
    fflush(NULL);                                                              \
    int64_t min_diff = INT64_MAX;                                              \
    for (int i = 0; i < repeat; i++) {                                         \
      pre;                                                                     \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      auto t0 = Clock::now();                                                  \
      test;                                                                    \
      auto t1 = Clock::now();                                                  \
      auto cycles_diff =                                                       \
          std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0)        \
              .count();                                                        \
      if (cycles_diff < min_diff)                                              \
        min_diff = cycles_diff;                                                \
    }                                                                          \
    uint64_t S = size;                                                         \
    if (verbose)                                                               \
      printf(" %d ns total, ", (int)min_diff);                                 \
    float cycle_per_op = (min_diff) / (double)S;                               \
    if (verbose)                                                               \
      printf(" %.2f ns per input key ", cycle_per_op);                         \
    if (verbose)                                                               \
      printf("\n");                                                            \
    if (!verbose)                                                              \
      printf("  %.2f ", cycle_per_op);                                         \
    fflush(NULL);                                                              \
  } while (0)
