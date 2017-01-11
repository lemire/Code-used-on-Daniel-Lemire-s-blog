// gcc -o reduce reduce.c -O3 -mpclmul -msse4.2

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
static inline __m128i Reduce(const __m128i IH, const __m128i IL) {
  // Designed by D. Lemire in January 2017
  const __m128i p = _mm_set1_epi64x(0x87U);  // x^7 + x^2 + x + 1
  const __m128i IHshifted = _mm_slli_si128(IH,8);// we preshift so that when H1mul, we can remultiply early
  const __m128i H1mul = _mm_clmulepi64_si128(IH, p, 0x11); // multiply the most significant 64 bits of IH by (x^7 + x^2 + x + 1)
  const __m128i mul2 = _mm_clmulepi64_si128(_mm_xor_si128(H1mul, IHshifted), p, 0x11);
  __m128i answer = _mm_xor_si128(_mm_srli_si128(H1mul,8) , IL);
  answer = _mm_xor_si128(mul2 ,answer );
  return answer;
}


// source unknown
static inline __m128i ReduceOrig(const __m128i IH, const __m128i IL) {
  const __m128i p = _mm_set1_epi64x(0x87U);  // x^7 + x^2 + x + 1
  const __m128i H1mul = _mm_clmulepi64_si128(IH, p, 0x11); // multiply the most significant 64 bits of IH by (x^7 + x^2 + x + 1)
  const __m128i H1mul_L0 = _mm_unpacklo_epi64(_mm_setzero_si128(), H1mul); // grab the lowest 64 bits of H1mul and move it up
  const __m128i H1mul_0H = _mm_unpackhi_epi64(H1mul, _mm_setzero_si128()); // grab the highest 64 bits of H1mul and move it down
  const __m128i mul2 = _mm_clmulepi64_si128(_mm_xor_si128(H1mul_L0, IH), p, 0x00);
  __m128i answer = _mm_xor_si128(mul2 , _mm_xor_si128(H1mul_0H , IL));
  return answer;
}



static inline __m128i lazymod127(__m128i Alow, __m128i Ahigh) {
    ///////////////////////////////////////////////////
    // CHECKING THE PRECONDITION:
    // Important: we are assuming that the two highest bits of Ahigh
    // are zero. This could be checked by adding a line such as this one:
    // if(_mm_extract_epi64(Ahigh,1) >= (1ULL<<62)){printf("bug\n");abort();}
    //                       (this assumes SSE4.1 support)
    ///////////////////////////////////////////////////
    // The answer is Alow XOR  (  Ahigh <<1 ) XOR (  Ahigh <<2 )
    // This is correct because the two highest bits of Ahigh are
    // assumed to be zero.
    ///////////////////////////////////////////////////
    // We want to take Ahigh and compute       (  Ahigh <<1 ) XOR (  Ahigh <<2 )
    // Except that there is no way to shift an entire XMM register by 1 or 2 bits  using a single instruction.
    // So how do you compute Ahigh <<1 using as few instructions as possible?
    //
    // First you do _mm_slli_epi64(Ahigh,1). This is *almost* correct... except that
    // the 64th bit is not shifted in 65th position.
    // Well, ok, but we can compute Ahigh >> 8, this given by _mm_srli_si128(Ahigh,1)
    // _mm_srli_si128(Ahigh,1) has the same bits as Ahigh (except that we lose the lowest 8)
    // but at different positions.
    // So let us shift left the result again...
    //  _mm_slli_epi64(_mm_srli_si128(Ahigh,1),1)
    // If you keep track, this is "almost" equivalent to A >> 7, except that the 72th bit
    // from A is lost.
    // From something that is almost A >>7, we can get back something that is almost A << 1
    // by shifting left by 8 bits...
    // _mm_slli_si128(_mm_slli_epi64(_mm_srli_si128(Ahigh,1),1),1)
    // So this is a shift left by 1, except that the 72th bit is lost along with the lowest 8 bits.
    // We have that  _mm_slli_epi64(Ahigh,1) is a shift let by 1 except that the 64th bit
    // is lost. We can combine the two to get the desired result (just OR them).
    // The algorithm below is just an optimized version of this where we do both shifts (by 1 and 2)
    // at the same time and XOR the result.
    //
    __m128i shifteddownAhigh = _mm_srli_si128(Ahigh,1);
    __m128i s1 = _mm_slli_epi64(Ahigh,1);
    __m128i s2 = _mm_slli_epi64(Ahigh,2);
    __m128i sd1 = _mm_slli_si128(_mm_slli_epi64(shifteddownAhigh,1),1);
    __m128i sd2 = _mm_slli_si128(_mm_slli_epi64(shifteddownAhigh,2),1);
    s1 = _mm_or_si128(s1,sd1);
    s2 = _mm_or_si128(s2,sd2);
    __m128i reduced = _mm_xor_si128(s1,s2);
    // combining results
    __m128i final = _mm_xor_si128(Alow,reduced);
    return final;
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

__m128i chainlazy(__m128i ih, __m128i il, size_t number) {
  for(size_t i = 0; i < number; ++i) {
    __m128i r = lazymod127(il,ih);
    il = ih;
    ih = r;
  }
  return il;
}
int main() {
  __m128i il = _mm_set_epi64x(2,1);
  __m128i ih = _mm_set_epi64x(8,4);
  const int repeat = 50;
  const int N = 1000;
  printf("This program attempts to measure the latency of modulo reduction functions.\n");

  il = _mm_set_epi64x(2,1);
  ih = _mm_set_epi64x(8,4);
  BEST_TIME_NOCHECK(il = chain(ih,il,N),  repeat, N, true);
  printf("bogus line (ignore me): "); print(il);
  printf("bogus line (ignore me): "); print(ih);


  il = _mm_set_epi64x(2,1);
  ih = _mm_set_epi64x(8,4);
  BEST_TIME_NOCHECK(il = chainorig(ih,il,N),  repeat, N, true);
  printf("bogus line (ignore me): ") ;print(il);
  printf("bogus line (ignore me): "); print(ih);


  il = _mm_set_epi64x(2,1);
  ih = _mm_set_epi64x(8,4);
  BEST_TIME_NOCHECK(il = chainlazy(ih,il,N),  repeat, N, true);
  printf("bogus line (ignore me): "); print(il);
  printf("bogus line (ignore me): "); print(ih);

  return 0;

}
