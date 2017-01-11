// gcc -o reduce reduce.c -O3 -mpclmul

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include <x86intrin.h>

#define RDTSC_START(cycles)                                             \
    do {                                                                \
        uint32_t cyc_high, cyc_low;                                     \
        __asm volatile("cpuid\n"                                        \
                       "rdtsc\n"                                        \
                       "mov %%edx, %0\n"                                \
                       "mov %%eax, %1" :                                \
                       "=r" (cyc_high),                                 \
                       "=r"(cyc_low) :                                  \
                       : /* no read only */                             \
                       "%rax", "%rbx", "%rcx", "%rdx" /* clobbers */    \
                       );                                               \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                \
    } while (0)

#define RDTSC_STOP(cycles)                                              \
    do {                                                                \
        uint32_t cyc_high, cyc_low;                                     \
        __asm volatile("rdtscp\n"                                       \
                       "mov %%edx, %0\n"                                \
                       "mov %%eax, %1\n"                                \
                       "cpuid" :                                        \
                       "=r"(cyc_high),                                  \
                       "=r"(cyc_low) :                                  \
                       /* no read only registers */ :                   \
                       "%rax", "%rbx", "%rcx", "%rdx" /* clobbers */    \
                       );                                               \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                \
    } while (0)

static __attribute__ ((noinline))
uint64_t rdtsc_overhead_func(uint64_t dummy) {
    return dummy;
}

uint64_t global_rdtsc_overhead = (uint64_t) UINT64_MAX;

#define RDTSC_SET_OVERHEAD(test, repeat)			      \
  do {								      \
    uint64_t cycles_start, cycles_final, cycles_diff;		      \
    uint64_t min_diff = UINT64_MAX;				      \
    for (int i = 0; i < repeat; i++) {			      \
      __asm volatile("" ::: /* pretend to clobber */ "memory");	      \
      RDTSC_START(cycles_start);				      \
      test;							      \
      RDTSC_STOP(cycles_final);                                       \
      cycles_diff = (cycles_final - cycles_start);		      \
      if (cycles_diff < min_diff) min_diff = cycles_diff;	      \
    }								      \
    global_rdtsc_overhead = min_diff;				      \
    printf("rdtsc_overhead set to %d\n", (int)global_rdtsc_overhead);     \
  } while (0)							      \


#define BEST_TIME_NOCHECK(test,  repeat, size, verbose)                         \
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
                __asm volatile("" ::: /* pretend to clobber */ "memory"); \
                RDTSC_START(cycles_start);                                \
                test;                     \
                RDTSC_STOP(cycles_final);                                \
                cycles_diff = (cycles_final - cycles_start - global_rdtsc_overhead);           \
                if (cycles_diff < min_diff) min_diff = cycles_diff;       \
                sum_diff += cycles_diff;                                  \
            }                                                             \
            uint64_t S = size;                                            \
            float cycle_per_op = (min_diff) / (double)S;                  \
            float avg_cycle_per_op = (sum_diff) / ((double)S * repeat);   \
            if(verbose) printf(" %.2f cycles per operation (best) ", cycle_per_op);   \
            if(verbose) printf("\t%.2f cycles per operation (avg) ", avg_cycle_per_op);   \
            if(verbose) printf("\n");                                                 \
            if(!verbose) printf(" %.2f ",cycle_per_op);                   \
            fflush(NULL);                                                 \
 } while (0)



void print(__m128i x) {
  printf("high: %zu low: %zu \n", (size_t) _mm_extract_epi64(x,1),(size_t) _mm_extract_epi64(x,0));
}

//////////////
// So  x^128 + x^7 + x^2 + x + 1 is an irreducible polynomial
// We want to reduce 256-bit values with  x^128 + x^7 + x^2 + x + 1.
// Let us represent a 256-bit word as  64-bit words in little endian mode
// IH1 IH0 IL1 IL0
//
// The values IL1 IL0 are already part of the modulo reduction, without
// effort. So we can XOR them at the end.
//
// To reduce IH0, we simply need to multiply it by x^7 + x^2 + x + 1, the 64-bit
// result is reduced.
// For IH1, we can proceed in two steps... if we multiply it by x^7 + x^2 + x + 1,
// getting mul2 (written as mul2_2 mul2_1)
// then we effectively shift down by 128 bits...
// so we have 0 mul2_2 mul2_1 0
// so we can add mul2_2 to IH0 and reduce them together.
/////////////
__m128i Reduce(const __m128i IH, const __m128i IL) {
  const __m128i p = _mm_set1_epi64x(0x87U);  // x^7 + x^2 + x + 1
  const __m128i IHshifted = _mm_slli_si128(IH,8);// we preshift so that when H1mul, we can remultiply early
  const __m128i H1mul = _mm_clmulepi64_si128(IH, p, 0x11); // multiply the most significant 64 bits of IH by (x^7 + x^2 + x + 1)
  const __m128i mul2 = _mm_clmulepi64_si128(_mm_xor_si128(H1mul, IHshifted), p, 0x11);
  __m128i answer = _mm_xor_si128(_mm_srli_si128(H1mul,8) , IL);
  answer = _mm_xor_si128(mul2 ,answer );
  return answer;
}


__m128i ReduceOrig(const __m128i IH, const __m128i IL) {
  const __m128i p = _mm_set1_epi64x(0x87U);  // x^7 + x^2 + x + 1
  const __m128i H1mul = _mm_clmulepi64_si128(IH, p, 0x11); // multiply the most significant 64 bits of IH by (x^7 + x^2 + x + 1)
  const __m128i H1mul_L0 = _mm_unpacklo_epi64(_mm_setzero_si128(), H1mul); // grab the lowest 64 bits of H1mul and move it up
  const __m128i H1mul_0H = _mm_unpackhi_epi64(H1mul, _mm_setzero_si128()); // grab the highest 64 bits of H1mul and move it down
  const __m128i mul2 = _mm_clmulepi64_si128(_mm_xor_si128(H1mul_L0, IH), p, 0x00);
  __m128i answer = _mm_xor_si128(mul2 , _mm_xor_si128(H1mul_0H , IL));
  return answer;
}

__m128i chain(__m128i ih, __m128i il, size_t number) {
  for(size_t i = 0; i < number; ++i) {
    __m128i r = Reduce(ih,il);
    il = ih;
    ih = r;
  }
  return il;
}


__m128i chainorig(__m128i ih, __m128i il, size_t number) {
  for(size_t i = 0; i < number; ++i) {
    __m128i r = ReduceOrig(ih,il);
    il = ih;
    ih = r;
  }
  return il;
}
int main() {
  __m128i il = _mm_set_epi64x(2,1);
  __m128i ih = _mm_set_epi64x(8,4);
  print(Reduce(ih,il));
  const int repeat = 50;
  const int N = 1000;

  il = _mm_set_epi64x(2,1);
  ih = _mm_set_epi64x(8,4);
  BEST_TIME_NOCHECK(il = chain(ih,il,N),  repeat, N, true);
  print(il);
  print(ih);

  printf("%d \n",(int)il[0]);

  il = _mm_set_epi64x(2,1);
  ih = _mm_set_epi64x(8,4);
  BEST_TIME_NOCHECK(il = chainorig(ih,il,N),  repeat, N, true);
  print(il);
  print(ih);

  return 0;

}
