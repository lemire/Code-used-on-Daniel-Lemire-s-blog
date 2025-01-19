// clang -O3 -o iszero iszero.c && ./iszero

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <arm_neon.h>
#include <time.h>

const char * unitname = " (clock units) ";

#define RDTSC_START(cycles) \
    do {                    \
        cycles = clock();   \
    } while (0)

#define RDTSC_STOP(cycles) \
    do {                    \
        cycles = clock();   \
    } while (0)

static __attribute__ ((noinline))
uint64_t rdtsc_overhead_func(uint64_t dummy) {
    return dummy;
}

uint64_t global_rdtsc_overhead = (uint64_t) UINT64_MAX;

#define RDTSC_SET_OVERHEAD(test, repeat)                  \
  do {                                      \
    uint64_t cycles_start, cycles_final, cycles_diff;              \
    uint64_t min_diff = UINT64_MAX;                      \
    for (int i = 0; i < repeat; i++) {                  \
      __asm volatile("" ::: /* pretend to clobber */ "memory");          \
      RDTSC_START(cycles_start);                      \
      test;                                  \
      RDTSC_STOP(cycles_final);                                       \
      cycles_diff = (cycles_final - cycles_start);              \
      if (cycles_diff < min_diff) min_diff = cycles_diff;          \
    }                                      \
    global_rdtsc_overhead = min_diff;                      \
    printf("rdtsc_overhead set to %d\n", (int)global_rdtsc_overhead);     \
  } while (0)                                  \

#define BEST_TIME_NOCHECK(test, pre, repeat, size, verbose, bogus)                         \
        do {                                                              \
            if (global_rdtsc_overhead == UINT64_MAX) {                    \
               RDTSC_SET_OVERHEAD(rdtsc_overhead_func(1), repeat);        \
            }                                                             \
            if(verbose) printf("%-60s\t: ", #test);                                        \
            fflush(NULL);                                                 \
            uint64_t cycles_start, cycles_final, cycles_diff;             \
            uint64_t min_diff = (uint64_t)-1;                             \
            uint64_t sum_diff = 0;                                        \
            for (int i = 0; i < repeat; i++) {                            \
                pre;                                                      \
                __asm volatile("" ::: /* pretend to clobber */ "memory"); \
                RDTSC_START(cycles_start);                                \
                bogus += test;                     \
                RDTSC_STOP(cycles_final);                                \
                cycles_diff = (cycles_final - cycles_start - global_rdtsc_overhead);           \
                if (cycles_diff < min_diff) min_diff = cycles_diff;       \
                sum_diff += cycles_diff;                                  \
            }                                                             \
            uint64_t S = size;                                            \
            float cycle_per_op = (min_diff) ;                  \
            float avg_cycle_per_op = (sum_diff) / (  repeat);   \
            if(verbose) printf(" %.5f %s per operation (best) ", cycle_per_op, unitname);   \
            if(verbose) printf("\t%.5f %s per operation (avg) ", avg_cycle_per_op,unitname);   \
            if(verbose) printf("\n");                                                 \
            if(!verbose) printf(" %.3f ",cycle_per_op);                   \
            fflush(NULL);                                                 \
 } while (0)


int is_zero(uint32x4_t mask) {
     uint64x2_t v64 = vreinterpretq_u64_u32(mask);
     uint32x2_t v32 = vqmovn_u64(v64);
     uint64x1_t result = vreinterpret_u64_u32(v32);
     return vget_lane_u64(result,0) == 0;
 }

int run_is_zero(uint32_t * array, size_t length) {
  int count = 0;
  for(size_t i = 0 ; i + 3 < length; i += 4) {
    uint32x4_t val = vld1q_u32(array + i);
    count += is_zero(val);
  }
  return count;
}


int is_zero_long(uint32x4_t mask) {
    return (vgetq_lane_u32(mask, 0) == 0 &&
                     vgetq_lane_u32(mask, 1) == 0 &&
                     vgetq_lane_u32(mask, 2) == 0 &&
                     vgetq_lane_u32(mask, 3) == 0);
}


int run_is_zero_long(uint32_t * array, size_t length) {
  int count = 0;
  for(size_t i = 0 ; i + 3 < length; i += 4) {
    uint32x4_t val = vld1q_u32(array + i);
    count += is_zero_long(val);
  }
  return count;
}

// count how many sequences of 4 words are zero
void demo() {
  const int N = 1024 * 1024;// ensure it is a power of two
  int verbose = 1;
  int repeat = 5;
  int bogus = 0;
  uint32_t * buffer =  (uint32_t *) malloc(N * sizeof(uint32_t));
  for(int density = 1; density <= N; density *= 2) {
    memset(buffer,0, N *sizeof(uint32_t));
    printf("density = %f \n", (double) density / (double)N);
    for(int k = 0 ; k < density; k++) {
      buffer[rand() & (N-1)] = 0xFF;
    }
    BEST_TIME_NOCHECK(run_is_zero(buffer,N),, repeat,
                        N, verbose, bogus);
    BEST_TIME_NOCHECK(run_is_zero_long(buffer,N),, repeat,
                          N, verbose, bogus);
    printf("\n");
  }
  printf("bogus %d \n", bogus);
}


int main() {
  demo();
  return EXIT_SUCCESS;
}
