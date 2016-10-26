#include <stdio.h>
#include <stdint.h>

#define RDTSC_START(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "cpuid\n\t"                                                       \
            "rdtsc\n\t"                                                       \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)

#define RDTSC_FINAL(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "rdtscp\n\t"                                                      \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            "cpuid\n\t"                                                       \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)



#define BEST_TIME(test, answer, repeat, size)                         \
    do {                                                              \
        printf("%s: ", #test);                                        \
        fflush(NULL);                                                 \
        uint64_t cycles_start, cycles_final, cycles_diff;             \
        uint64_t min_diff = (uint64_t)-1;                             \
        int wrong_answer = 0;                                         \
        for (int i = 0; i < repeat; i++) {                            \
            __asm volatile("" ::: /* pretend to clobber */ "memory"); \
            RDTSC_START(cycles_start);                                \
            if (test != answer) wrong_answer = 1;                     \
            RDTSC_FINAL(cycles_final);                                \
            cycles_diff = (cycles_final - cycles_start);              \
            if (cycles_diff < min_diff) min_diff = cycles_diff;       \
        }                                                             \
        uint64_t S = (uint64_t)size;                                  \
        float cycle_per_op = (min_diff) / (float)S;                   \
        printf(" %.2f cycles per operation", cycle_per_op);           \
        if (wrong_answer) printf(" [ERROR]");                         \
        printf("\n");                                                 \
        fflush(NULL);                                                 \
    } while (0)


uint32_t mod7(uint32_t a) {
  return a % 7;
}

uint32_t fastmod7(uint32_t a) {
    return (((4908534053*a) & ((1ULL<<35)-1)) * 7) >> 35;
}

uint32_t sumofmod7(uint32_t maxval) {
  uint32_t sumofmods = 0;
  for(uint32_t k = 0; k < maxval; ++k) sumofmods += mod7(k);
  return sumofmods;
}


uint32_t fastsumofmod7(uint32_t maxval) {
  uint32_t sumofmods = 0;
  for(uint32_t k = 0; k < maxval; ++k) sumofmods += fastmod7(k);
  return sumofmods;
}

int main() {
  uint32_t sumofmods = 0;
  const uint32_t maxval = 1000000;
  for(uint32_t k = 0; k < maxval; ++k) sumofmods += k % 7;
  const int repeat = 5;
  BEST_TIME(sumofmod7(maxval), sumofmods, repeat, maxval) ;
  BEST_TIME(fastsumofmod7(maxval), sumofmods, repeat, maxval) ;



  for(uint32_t x = 1; x !=0; x++) {
    if(mod7(x) != fastmod7(x)) printf("%x\n",x);
  }
}
