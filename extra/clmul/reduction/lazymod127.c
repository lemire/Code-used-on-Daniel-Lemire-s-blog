/**
* Fastest way to compute lazymod127
 gcc -O3 -o lazymod127 lazymod127.c
*/

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <x86intrin.h>
#include "benchmark.h"

#ifdef IACA
#include </opt/intel/iaca/include/iacaMarks.h>
#else
#define IACA_START
#define IACA_END
#endif

/***
* Original approach :
**/

__m128i lazymod127(__m128i Alow, __m128i Ahigh) {
  IACA_START;
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
IACA_END;
    return final;
}

/*
Throughput Analysis Report
--------------------------
Block Throughput: 3.90 Cycles       Throughput Bottleneck: Port5

Port Binding In Cycles Per Iteration:
---------------------------------------------------------------------------------------
|  Port  |  0   -  DV  |  1   |  2   -  D   |  3   -  D   |  4   |  5   |  6   |  7   |
---------------------------------------------------------------------------------------
| Cycles | 3.6    0.0  | 3.6  | 0.0    0.0  | 0.0    0.0  | 0.0  | 3.8  | 0.0  | 0.0  |
---------------------------------------------------------------------------------------

N - port number or number of cycles resource conflict caused delay, DV - Divider pipe (on port 0)
D - Data fetch pipe (on ports 2 and 3), CP - on a critical path
F - Macro Fusion with the previous instruction occurred
* - instruction micro-ops not bound to a port
^ - Micro Fusion happened
# - ESP Tracking sync uop was issued
@ - SSE instruction followed an AVX256 instruction, dozens of cycles penalty is expected
! - instruction not supported, was not accounted in Analysis

| Num Of |                    Ports pressure in cycles                     |    |
|  Uops  |  0  - DV  |  1  |  2  -  D  |  3  -  D  |  4  |  5  |  6  |  7  |    |
---------------------------------------------------------------------------------
|   0*   |           |     |           |           |     |     |     |     |    | movdqa xmm2, xmm1
|   1    |           |     |           |           |     | 1.0 |     |     | CP | psrldq xmm2, 0x1
|   0*   |           |     |           |           |     |     |     |     |    | movdqa xmm3, xmm1
|   1    |           | 1.0 |           |           |     |     |     |     |    | paddq xmm3, xmm3
|   1    | 1.0       |     |           |           |     |     |     |     |    | psllq xmm1, 0x2
|   0*   |           |     |           |           |     |     |     |     |    | movdqa xmm4, xmm2
|   1    |           | 0.2 |           |           |     | 0.8 |     |     | CP | paddq xmm4, xmm4
|   1    |           |     |           |           |     | 1.0 |     |     | CP | pslldq xmm4, 0x1
|   1    | 1.0       |     |           |           |     |     |     |     |    | psllq xmm2, 0x2
|   1    |           |     |           |           |     | 1.0 |     |     | CP | pslldq xmm2, 0x1
|   1    |           | 1.0 |           |           |     |     |     |     |    | por xmm4, xmm3
|   1    | 0.6       | 0.4 |           |           |     |     |     |     |    | por xmm2, xmm1
|   1    | 0.4       | 0.6 |           |           |     |     |     |     |    | pxor xmm2, xmm4
|   1    | 0.6       | 0.3 |           |           |     | 0.1 |     |     | CP | pxor xmm0, xmm2
Total Num Of Uops: 11

*/



/***
* Alternative approach :
**/


// computes a << 1
static inline __m128i leftshift1(__m128i a) {
    const int x = 1;
    __m128i u64shift =  _mm_slli_epi64(a,x);
    __m128i topbits =  _mm_slli_si128(_mm_srli_epi64(a,64 - x),sizeof(uint64_t));
    return _mm_or_si128(u64shift, topbits);
}

// computes a << 2
static inline __m128i leftshift2(__m128i a) {
    const int x = 2;
    __m128i u64shift =  _mm_slli_epi64(a,x);
    __m128i topbits =  _mm_slli_si128(_mm_srli_epi64(a,64 - x),sizeof(uint64_t));
    return _mm_or_si128(u64shift, topbits);
}


__m128i alt_lazymod127(__m128i Alow, __m128i Ahigh) {
IACA_START;
    __m128i shift1 = leftshift1(Ahigh);
    __m128i shift2 = leftshift2(Ahigh);
    __m128i final =  _mm_or_si128(_mm_or_si128(Alow, shift1),shift2);
IACA_END;
    return final;
}
/*
Throughput Analysis Report
--------------------------
Block Throughput: 5.40 Cycles       Throughput Bottleneck: InterIteration

Port Binding In Cycles Per Iteration:
---------------------------------------------------------------------------------------
|  Port  |  0   -  DV  |  1   |  2   -  D   |  3   -  D   |  4   |  5   |  6   |  7   |
---------------------------------------------------------------------------------------
| Cycles | 3.3    0.0  | 3.3  | 0.0    0.0  | 0.0    0.0  | 0.0  | 3.4  | 0.0  | 0.0  |
---------------------------------------------------------------------------------------

N - port number or number of cycles resource conflict caused delay, DV - Divider pipe (on port 0)
D - Data fetch pipe (on ports 2 and 3), CP - on a critical path
F - Macro Fusion with the previous instruction occurred
* - instruction micro-ops not bound to a port
^ - Micro Fusion happened
# - ESP Tracking sync uop was issued
@ - SSE instruction followed an AVX256 instruction, dozens of cycles penalty is expected
! - instruction not supported, was not accounted in Analysis

| Num Of |                    Ports pressure in cycles                     |    |
|  Uops  |  0  - DV  |  1  |  2  -  D  |  3  -  D  |  4  |  5  |  6  |  7  |    |
---------------------------------------------------------------------------------
|   0*   |           |     |           |           |     |     |     |     |    | movdqa xmm2, xmm1
|   1    |           | 0.6 |           |           |     | 0.4 |     |     |    | paddq xmm2, xmm2
|   0*   |           |     |           |           |     |     |     |     | CP | movdqa xmm3, xmm1
|   1    | 1.0       |     |           |           |     |     |     |     | CP | psrlq xmm3, 0x3f
|   1    |           |     |           |           |     | 1.0 |     |     | CP | pslldq xmm3, 0x8
|   0*   |           |     |           |           |     |     |     |     |    | movdqa xmm4, xmm1
|   1    | 1.0       |     |           |           |     |     |     |     |    | psllq xmm4, 0x2
|   1    | 1.0       |     |           |           |     |     |     |     | CP | psrlq xmm1, 0x3e
|   1    |           |     |           |           |     | 1.0 |     |     | CP | pslldq xmm1, 0x8
|   1    |           | 0.9 |           |           |     | 0.1 |     |     |    | por xmm0, xmm4
|   1    |           | 0.9 |           |           |     | 0.1 |     |     |    | por xmm0, xmm2
|   1    |           | 0.4 |           |           |     | 0.6 |     |     | CP | por xmm1, xmm3
|   1    | 0.3       | 0.6 |           |           |     | 0.2 |     |     | CP | por xmm0, xmm1
Total Num Of Uops: 10
*/


void init(__m128i * X, __m128i * Y) {
  *X = _mm_set1_epi64x(1);
  *Y = _mm_set1_epi64x(3);
}

bool equals(__m128i v0, __m128i v1) {
   __m128i vcmp = _mm_xor_si128(v0, v1);        // PXOR
  return _mm_testz_si128(vcmp, vcmp);
}

int main() {
  uint64_t r1, r2;
  for(int z = 0; z < 10000; ++z) {
  _rdrand64_step(&r1);
  _rdrand64_step(&r2);
  __m128i x = _mm_set_epi64x(r1,r2);
  _rdrand64_step(&r1);
  _rdrand64_step(&r2);

  __m128i y = _mm_set_epi64x(r1,r2);
  assert(equals(lazymod127(x,y),alt_lazymod127(x,y)));
}

  const int repeat = 1000;


   BEST_TIME_NOCHECK(lazymod127(x,y), lazymod127(y,x), repeat, 1, true);

   BEST_TIME_NOCHECK(alt_lazymod127(x,y), lazymod127(y,x), repeat, 1, true);
   return _mm_cvtsd_si32(x);
}
