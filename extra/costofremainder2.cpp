/**
* cost of remainder in 64-bit multiplications
g++ -O2 -march=native -o costofremainder2 costofremainder2.cpp && ./costofremainder2

g++ -O2 -march=native -o costofremainder2 costofremainder2.cpp -DIACA 
/opt/intel/iaca/bin/iaca.sh -64 -mark 1 ./costofremainder2
/opt/intel/iaca/bin/iaca.sh -64 -mark 2 ./costofremainder2
/opt/intel/iaca/bin/iaca.sh -64 -mark 3 ./costofremainder2

*/
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <stdint.h>
#include <x86intrin.h>
using namespace std;

#ifdef IACA
#include </opt/intel/iaca/include/iacaMarks.h>
#endif

#define MUL64(rh,rl,i1,i2) asm ("mulq %3" : "=a"(rl), "=d"(rh) : "a"(i1), "r"(i2) : "cc")
#define ADD128(rh,rl,ih,il)                                               \
    asm ("addq %3, %1 \n\t"                                               \
         "adcq %2, %0"                                                    \
    : "+r"(rh),"+r"(rl)                                                   \
    : "r"(ih),"r"(il) : "cc");



struct uint192 {
    uint64_t low;
    uint64_t high;
    uint64_t vhigh;
} ;

////// completesum_alt staff begin /////

#define COMPLETESUM_ALT_STAFF_TYPE 1
#define USE_SSE

#define COMPLETESUM_ALT_STAFF_DECLARE \
	  uint64_t ctr0 = 0, ctr1 = 0, ctr2 = 0; \
	  uint64_t ctr1_0 = 0, ctr1_1 = 0, ctr1_2 = 0, ctr1_3 = 0; \
	  uint64_t ctr2_0 = 0, ctr2_1 = 0, ctr2_2 = 0, ctr2_3 = 0; \
	  uint64_t mulLow, mulHigh;

#define COMPLETESUM_ALT_STAFF_T_BASE( ii ) { \
uint64_t rhi;  /*Dummy variable to tell the compiler that the register rax is input and clobbered but not actually output; see assembler code below. Better syntactic expression is very welcome.*/ \
__asm__( "mulq %5\n" \
        "addq %%rax, %0\n" \
        "adcq %%rdx, %1\n" \
        "adcq $0, %2\n" \
        : "+g" (ctr0), "+g" (ctr1), "+g" (ctr2), "=a" (rhi) \
        :"a"(x[ii]), "g"(coeff[ ii ]) : "rdx", "cc" ); \
}

#if COMPLETESUM_ALT_STAFF_TYPE == 0

#define COMPLETESUM_ALT_STAFF_T1 COMPLETESUM_ALT_STAFF_T_BASE
#define COMPLETESUM_ALT_STAFF_T2 COMPLETESUM_ALT_STAFF_T_BASE
#define COMPLETESUM_ALT_STAFF_FINALIZE

#elif COMPLETESUM_ALT_STAFF_TYPE == 1

#define COMPLETESUM_ALT_STAFF_T1 COMPLETESUM_ALT_STAFF_T_BASE

#define COMPLETESUM_ALT_STAFF_T2( ii ) { \
uint64_t rhi;  /*Dummy variable to tell the compiler that the register rax is input and clobbered but not actually output; see assembler code below. Better syntactic expression is very welcome.*/ \
__asm__( "mulq %6\n" \
        "addq %%rax, %0\n" \
        "addq %%rdx, %1\n" \
        "adcq $0, %2\n" \
        "shrq $32, %%rax\n" \
        "addq %%rax, %3\n" \
        : "+g" (ctr2_0), "+g" (ctr1), "+g" (ctr2), "+g" (ctr2_1), "=a" (rhi) \
        :"a"(x[ii]), "g"(coeff[ ii ]) : "rdx", "cc" ); \
}

#define COMPLETESUM_ALT_STAFF_FINALIZE { \
	uint32_t xx = ctr2_0 >> 32; \
	uint32_t yy = ctr2_1; \
	uint64_t zz = xx - yy; \
	ctr2_1 += zz; \
	ctr2_0 = (uint32_t)ctr2_0; \
	ctr2_0 += ((uint64_t)(uint32_t)ctr2_1) << 32; \
    uint64_t xyz = (ctr2_1)>>32; \
__asm__("addq %3, %0\n" \
        "adcq %4, %1\n" \
        "adcq $0, %2\n" \
        : "+g" (ctr0), "+g" (ctr1), "+g" (ctr2) \
        : "g" (ctr2_0), "g" (xyz) : "cc" ); \
}

#elif COMPLETESUM_ALT_STAFF_TYPE == 2

#define COMPLETESUM_ALT_STAFF_T1 COMPLETESUM_ALT_STAFF_T_BASE

#define COMPLETESUM_ALT_STAFF_T2( ii ) { \
        MUL64( mulHigh, mulLow, x[ii], coeff[ ii ] ); \
        ctr2_0 += mulLow; \
        mulLow >>= 32; \
        ctr2_1 += mulLow; \
        ctr2_2 += mulHigh; \
        mulHigh >>= 32; \
        ctr2_3 += mulHigh; \
}

#define COMPLETESUM_ALT_STAFF_FINALIZE { \
	uint32_t xx = ctr2_0 >> 32; \
	uint32_t yy = ctr2_1; \
	uint64_t zz = xx - yy; \
	ctr2_1 += zz; \
	ctr2_0 = (uint32_t)ctr2_1; \
	ctr2_0 += ((uint64_t)(uint32_t)ctr2_1) << 32; \
    uint64_t xyz = (ctr2_1)>>32; \
__asm__("addq %3, %0\n" \
        "adcq %4, %1\n" \
        "adcq $0, %2\n" \
        : "+g" (ctr0), "+g" (ctr1), "+g" (ctr2) \
        : "g" (ctr2_0), "g" (xyz) : "cc" ); \
	xx = ctr2_2 >> 32; \
	yy = ctr2_3; \
	zz = xx - yy; \
	ctr2_3 += zz; \
	ctr2_2 = (uint32_t)ctr2_3; \
	ctr2_2 += ((uint64_t)(uint32_t)ctr2_3) << 32; \
    xyz = ctr2_3>>32; \
__asm__("addq %2, %0\n" \
        "adcq %3, %1\n" \
        : "+g" (ctr1), "+g" (ctr2) \
        : "g" (ctr2_2), "g" (xyz) : "cc" ); \
}

#elif COMPLETESUM_ALT_STAFF_TYPE == 3

#define COMPLETESUM_ALT_STAFF_T1 COMPLETESUM_ALT_STAFF_T_BASE

#define COMPLETESUM_ALT_STAFF_T2( ii ) { \
uint64_t rhi;  /*Dummy variable to tell the compiler that the register rax is input and clobbered but not actually output; see assembler code below. Better syntactic expression is very welcome.*/ \
__asm__( "mulq %6\n" \
        "addq %%rdx, %1\n" \
        "addq %%rax, %0\n" \
        "shrq $32, %%rdx\n" \
        "addq %%rdx, %2\n" \
        "shrq $32, %%rax\n" \
        "addq %%rax, %3\n" \
        : "+g" (ctr2_0), "+g" (ctr2_2), "+g" (ctr2_3), "+g" (ctr2_1), "=a" (rhi) \
        :"a"(x[ii]), "g"(coeff[ ii ]) : "rdx", "cc" ); \
}

#define COMPLETESUM_ALT_STAFF_FINALIZE { \
	uint32_t xx = ctr2_0 >> 32; \
	uint32_t yy = ctr2_1; \
	uint64_t zz = xx - yy; \
	ctr2_1 += zz; \
	ctr2_0 = (uint32_t)ctr2_1; \
	ctr2_0 += ((uint64_t)(uint32_t)ctr2_1) << 32; \
    uint64_t xyz = (ctr2_1)>>32; \
__asm__("addq %3, %0\n" \
        "adcq %4, %1\n" \
        "adcq $0, %2\n" \
        : "+g" (ctr0), "+g" (ctr1), "+g" (ctr2) \
        : "g" (ctr2_0), "g" (xyz) : "cc" ); \
	xx = ctr2_2 >> 32; \
	yy = ctr2_3; \
	zz = xx - yy; \
	ctr2_3 += zz; \
	ctr2_2 = (uint32_t)ctr2_3; \
	ctr2_2 += ((uint64_t)(uint32_t)ctr2_3) << 32; \
    xyz = ctr2_3>>32; \
__asm__("addq %2, %0\n" \
        "adcq %3, %1\n" \
        : "+g" (ctr1), "+g" (ctr2) \
        : "g" (ctr2_2), "g" (xyz) : "cc" ); \
}

#elif COMPLETESUM_ALT_STAFF_TYPE == 4

#define COMPLETESUM_ALT_STAFF_T1( ii ) { \
uint64_t rhi;  /*Dummy variable to tell the compiler that the register rax is input and clobbered but not actually output; see assembler code below. Better syntactic expression is very welcome.*/ \
__asm__( "mulq %6\n" \
        "addq %%rdx, %1\n" \
        "addq %%rax, %0\n" \
        "shrq $32, %%rdx\n" \
        "shrq $32, %%rax\n" \
        "addq %%rdx, %2\n" \
        "addq %%rax, %3\n" \
        : "+g" (ctr1_0), "+g" (ctr1_2), "+g" (ctr1_3), "+g" (ctr1_1), "=a" (rhi) \
        :"a"(x[ii]), "g"(coeff[ ii ]) : "rdx", "cc" ); \
}

#define COMPLETESUM_ALT_STAFF_T2( ii ) { \
uint64_t rhi;  /*Dummy variable to tell the compiler that the register rax is input and clobbered but not actually output; see assembler code below. Better syntactic expression is very welcome.*/ \
__asm__( "mulq %6\n" \
        "addq %%rdx, %1\n" \
        "addq %%rax, %0\n" \
        "shrq $32, %%rdx\n" \
        "shrq $32, %%rax\n" \
        "addq %%rdx, %2\n" \
        "addq %%rax, %3\n" \
        : "+g" (ctr2_0), "+g" (ctr2_2), "+g" (ctr2_3), "+g" (ctr2_1), "=a" (rhi) \
        :"a"(x[ii]), "g"(coeff[ ii ]) : "rdx", "cc" ); \
}

#define COMPLETESUM_ALT_STAFF_FINALIZE { \
	uint32_t xx = ctr1_0 >> 32; \
	uint32_t yy = ctr1_1; \
	uint64_t zz = xx - yy; \
	ctr1_1 += zz; \
	ctr1_0 = (uint32_t)ctr1_1; \
	ctr1_0 += ((uint64_t)(uint32_t)ctr1_1) << 32; \
    uint64_t xyz = (ctr1_1)>>32; \
__asm__("addq %3, %0\n" \
        "adcq %4, %1\n" \
        "adcq $0, %2\n" \
        : "+g" (ctr0), "+g" (ctr1), "+g" (ctr2) \
        : "g" (ctr1_0), "g" (xyz) : "cc" ); \
	xx = ctr1_2 >> 32; \
	yy = ctr1_3; \
	zz = xx - yy; \
	ctr1_3 += zz; \
	ctr1_2 = (uint32_t)ctr1_3; \
	ctr1_2 += ((uint64_t)(uint32_t)ctr1_3) << 32; \
    xyz = ctr1_3>>32; \
__asm__("addq %2, %0\n" \
        "adcq %3, %1\n" \
        : "+g" (ctr1), "+g" (ctr2) \
        : "g" (ctr1_2), "g" (xyz) : "cc" ); \
	xx = ctr2_0 >> 32; \
	yy = ctr2_1; \
	zz = xx - yy; \
	ctr2_1 += zz; \
	ctr2_0 = (uint32_t)ctr2_1; \
	ctr2_0 += ((uint64_t)(uint32_t)ctr2_1) << 32; \
    xyz = (ctr2_1)>>32; \
__asm__("addq %3, %0\n" \
        "adcq %4, %1\n" \
        "adcq $0, %2\n" \
        : "+g" (ctr0), "+g" (ctr1), "+g" (ctr2) \
        : "g" (ctr2_0), "g" (xyz) : "cc" ); \
	xx = ctr2_2 >> 32; \
	yy = ctr2_3; \
	zz = xx - yy; \
	ctr2_3 += zz; \
	ctr2_2 = (uint32_t)ctr2_3; \
	ctr2_2 += ((uint64_t)(uint32_t)ctr2_3) << 32; \
    xyz = ctr2_3>>32; \
__asm__("addq %2, %0\n" \
        "adcq %3, %1\n" \
        : "+g" (ctr1), "+g" (ctr2) \
        : "g" (ctr2_2), "g" (xyz) : "cc" ); \
}

#endif

void completesum_alt(const uint64_t* coeff, const uint64_t *  x, const size_t length, uint64_t * out) 
{
	COMPLETESUM_ALT_STAFF_DECLARE
	
    size_t i = 0;
    for(; i<length*32/32; i+= 32) {
//    for(; i<length*16/16; i+= 16) {
//    for(; i<length*8/8; i+= 8) {
#ifdef IACA
        IACA_START;
#endif

	COMPLETESUM_ALT_STAFF_T2( i + 0 )
	COMPLETESUM_ALT_STAFF_T1( i + 1 )
	COMPLETESUM_ALT_STAFF_T2( i + 2 )
	COMPLETESUM_ALT_STAFF_T1( i + 3 )
	COMPLETESUM_ALT_STAFF_T2( i + 4 )
	COMPLETESUM_ALT_STAFF_T1( i + 5 )
	COMPLETESUM_ALT_STAFF_T2( i + 6 )
	COMPLETESUM_ALT_STAFF_T1( i + 7 )
	COMPLETESUM_ALT_STAFF_T2( i + 8 )
	COMPLETESUM_ALT_STAFF_T1( i + 9 )
	COMPLETESUM_ALT_STAFF_T2( i + 10 )
	COMPLETESUM_ALT_STAFF_T1( i + 11 )
	COMPLETESUM_ALT_STAFF_T2( i + 12 )
	COMPLETESUM_ALT_STAFF_T1( i + 13 )
	COMPLETESUM_ALT_STAFF_T2( i + 14 )
	COMPLETESUM_ALT_STAFF_T1( i + 15 )
	COMPLETESUM_ALT_STAFF_T2( i + 16 )
	COMPLETESUM_ALT_STAFF_T1( i + 17 )
	COMPLETESUM_ALT_STAFF_T2( i + 18 )
	COMPLETESUM_ALT_STAFF_T1( i + 19 )
	COMPLETESUM_ALT_STAFF_T2( i + 20 )
	COMPLETESUM_ALT_STAFF_T1( i + 21 )
	COMPLETESUM_ALT_STAFF_T2( i + 22 )
	COMPLETESUM_ALT_STAFF_T1( i + 23 )
	COMPLETESUM_ALT_STAFF_T2( i + 24 )
	COMPLETESUM_ALT_STAFF_T1( i + 25 )
	COMPLETESUM_ALT_STAFF_T2( i + 26 )
	COMPLETESUM_ALT_STAFF_T1( i + 27 )
	COMPLETESUM_ALT_STAFF_T2( i + 28 )
	COMPLETESUM_ALT_STAFF_T1( i + 29 )
	COMPLETESUM_ALT_STAFF_T2( i + 30 )
	COMPLETESUM_ALT_STAFF_T1( i + 31 )

#ifdef IACA
        IACA_END;
#endif
    }
	
    for(; i<length; ++i) 
	{
	COMPLETESUM_ALT_STAFF_T1( i )
    }
	
	COMPLETESUM_ALT_STAFF_FINALIZE
	
    out[0] = ctr0;
    out[1] = ctr1;
    out[2] = ctr2;
}

#ifdef USE_SSE

void completesum_alt_with_sse(const uint64_t* coeff, const uint64_t *  x, const size_t length, uint64_t * out) 
{
	COMPLETESUM_ALT_STAFF_DECLARE

	__m256i sse_ctr0, sse_ctr1, sse_ctr2, sse_ctr3, a, a_shifted, a_low, data, data_low, product, temp, mask_low;
	sse_ctr0 = _mm256_setzero_si256 (); // Sets the 128-bit value to zero.
	sse_ctr1 = _mm256_setzero_si256 ();
	sse_ctr2 = _mm256_setzero_si256 ();
	sse_ctr3 = _mm256_setzero_si256 ();
	mask_low = _mm256_set_epi32 ( 0, -1, 0 , -1, 0, -1, 0 , -1 );
	
    size_t i = 0;
    for(; i<length*32/32; i+= 16) {
#ifdef IACA
        IACA_START;
#endif

		a = _mm256_load_si256 ((__m256i *)(coeff+i)); // Loads 128-bit value. Address p must be 16-byte aligned.
		data = _mm256_loadu_si256 ((__m256i *)(x+i)); // Loads 128-bit value. Address p does not need be 16-byte aligned.

		// lower 32 bits
		a_low = _mm256_and_si256 ( mask_low, a );
		data_low = _mm256_and_si256 ( mask_low, data );
		product = _mm256_mul_epu32 ( data_low, a_low); // A 128-bit value that contains two 64-bit unsigned integers. The result can be expressed by the following equations. r0 := a0 * b0; r1 := a2 * b2
		temp = _mm256_srli_epi64( product, 32 ); // Shifts the 2 signed or unsigned 64-bit integers in a right by count bits while shifting in zeros.
		sse_ctr1 = _mm256_add_epi64 ( sse_ctr1, temp );
		temp = _mm256_and_si256 ( mask_low, product );
		sse_ctr0 = _mm256_add_epi64 ( sse_ctr0, temp );

		COMPLETESUM_ALT_STAFF_T1( 4 + i )
		// first cross
		a_shifted = _mm256_srli_epi64( a, 32 );
		product = _mm256_mul_epu32 ( data_low, a_shifted ); // A 128-bit value that contains two 64-bit unsigned integers. The result can be expressed by the following equations. r0 := a0 * b0; r1 := a2 * b2
		temp = _mm256_srli_epi64( product, 32 ); // Shifts the 2 signed or unsigned 64-bit integers in a right by count bits while shifting in zeros.
		sse_ctr2 = _mm256_add_epi64 ( sse_ctr2, temp );
		temp = _mm256_and_si256 ( mask_low, product );
		sse_ctr1 = _mm256_add_epi64 ( sse_ctr1, temp );

		COMPLETESUM_ALT_STAFF_T2( 5 + i )
		// second cross
		data = _mm256_srli_epi64( data, 32 );
		product = _mm256_mul_epu32 ( data, a_low ); // A 128-bit value that contains two 64-bit unsigned integers. The result can be expressed by the following equations. r0 := a0 * b0; r1 := a2 * b2
		temp = _mm256_srli_epi64( product, 32 ); // Shifts the 2 signed or unsigned 64-bit integers in a right by count bits while shifting in zeros.
		sse_ctr2 = _mm256_add_epi64 ( sse_ctr2, temp );
		temp = _mm256_and_si256 ( mask_low, product );
		sse_ctr1 = _mm256_add_epi64 ( sse_ctr1, temp );

		COMPLETESUM_ALT_STAFF_T1( 6 + i )
		// upper 32 bits
		product = _mm256_mul_epu32 ( data, a_shifted ); // A 128-bit value that contains two 64-bit unsigned integers. The result can be expressed by the following equations. r0 := a0 * b0; r1 := a2 * b2
		temp = _mm256_srli_epi64( product, 32 ); // Shifts the 2 signed or unsigned 64-bit integers in a right by count bits while shifting in zeros.
		sse_ctr3 = _mm256_add_epi64 ( sse_ctr3, temp );
		temp = _mm256_and_si256 ( mask_low, product );
		sse_ctr2 = _mm256_add_epi64 ( sse_ctr2, temp );
		COMPLETESUM_ALT_STAFF_T2( 7 + i )		



		a = _mm256_load_si256 ((__m256i *)(coeff+i+8)); // Loads 128-bit value. Address p must be 16-byte aligned.
		data = _mm256_loadu_si256 ((__m256i *)(x+i+8)); // Loads 128-bit value. Address p does not need be 16-byte aligned.

		// lower 32 bits
		a_low = _mm256_and_si256 ( mask_low, a );
		data_low = _mm256_and_si256 ( mask_low, data );
		product = _mm256_mul_epu32 ( data_low, a_low); // A 128-bit value that contains two 64-bit unsigned integers. The result can be expressed by the following equations. r0 := a0 * b0; r1 := a2 * b2
		temp = _mm256_srli_epi64( product, 32 ); // Shifts the 2 signed or unsigned 64-bit integers in a right by count bits while shifting in zeros.
		sse_ctr1 = _mm256_add_epi64 ( sse_ctr1, temp );
		temp = _mm256_and_si256 ( mask_low, product );
		sse_ctr0 = _mm256_add_epi64 ( sse_ctr0, temp );

		COMPLETESUM_ALT_STAFF_T1( 12 + i )

		// first cross
		a_shifted = _mm256_srli_epi64( a, 32 );
		product = _mm256_mul_epu32 ( data_low, a_shifted ); // A 128-bit value that contains two 64-bit unsigned integers. The result can be expressed by the following equations. r0 := a0 * b0; r1 := a2 * b2
		temp = _mm256_srli_epi64( product, 32 ); // Shifts the 2 signed or unsigned 64-bit integers in a right by count bits while shifting in zeros.
		sse_ctr2 = _mm256_add_epi64 ( sse_ctr2, temp );
		temp = _mm256_and_si256 ( mask_low, product );
		sse_ctr1 = _mm256_add_epi64 ( sse_ctr1, temp );

		COMPLETESUM_ALT_STAFF_T2( 13 + i )

		// second cross
		data = _mm256_srli_epi64( data, 32 );
		product = _mm256_mul_epu32 ( data, a_low ); // A 128-bit value that contains two 64-bit unsigned integers. The result can be expressed by the following equations. r0 := a0 * b0; r1 := a2 * b2
		temp = _mm256_srli_epi64( product, 32 ); // Shifts the 2 signed or unsigned 64-bit integers in a right by count bits while shifting in zeros.
		sse_ctr2 = _mm256_add_epi64 ( sse_ctr2, temp );
		temp = _mm256_and_si256 ( mask_low, product );
		sse_ctr1 = _mm256_add_epi64 ( sse_ctr1, temp );

		COMPLETESUM_ALT_STAFF_T1( 14 + i )
		// upper 32 bits
		product = _mm256_mul_epu32 ( data, a_shifted ); // A 128-bit value that contains two 64-bit unsigned integers. The result can be expressed by the following equations. r0 := a0 * b0; r1 := a2 * b2
		temp = _mm256_srli_epi64( product, 32 ); // Shifts the 2 signed or unsigned 64-bit integers in a right by count bits while shifting in zeros.
		sse_ctr3 = _mm256_add_epi64 ( sse_ctr3, temp );
		temp = _mm256_and_si256 ( mask_low, product );
		sse_ctr2 = _mm256_add_epi64 ( sse_ctr2, temp );
		COMPLETESUM_ALT_STAFF_T2( 15 + i )		

#ifdef IACA
        IACA_END;
#endif
    }
	
    for(; i<length; ++i) 
	{
	COMPLETESUM_ALT_STAFF_T1( i )
    }
	
	uint64_t t0, t1, t2, t3;
	t0 = ((uint64_t*)(&sse_ctr0))[0] + ((uint64_t*)(&sse_ctr0))[1] + ((uint64_t*)(&sse_ctr0))[2] + ((uint64_t*)(&sse_ctr0))[3];
	t1 = ((uint64_t*)(&sse_ctr1))[0] + ((uint64_t*)(&sse_ctr1))[1] + ((uint64_t*)(&sse_ctr1))[2] + ((uint64_t*)(&sse_ctr1))[3];
	t2 = ((uint64_t*)(&sse_ctr2))[0] + ((uint64_t*)(&sse_ctr2))[1] + ((uint64_t*)(&sse_ctr2))[2] + ((uint64_t*)(&sse_ctr2))[3];
	t3 = ((uint64_t*)(&sse_ctr3))[0] + ((uint64_t*)(&sse_ctr3))[1] + ((uint64_t*)(&sse_ctr3))[2] + ((uint64_t*)(&sse_ctr3))[3];

	t1 += t0 >> 32;
	t0 = (uint64_t)(uint32_t)t0 + (t1 << 32);

	t2 += t1 >> 32;

	t3 += t2 >> 32;
	t2 = (uint64_t)(uint32_t)t2 + (t3 << 32);

	ctr0 += t0;
	uint64_t copy = ctr1;
	ctr1 += t2 + (ctr0 < t0);
	ctr2 += ctr1 < copy;
	ctr2 += t3 >> 32;


	COMPLETESUM_ALT_STAFF_FINALIZE
	
    out[0] = ctr0;
    out[1] = ctr1;
    out[2] = ctr2;
}

#endif // USE_SSE 

////// completesum_alt staff end /////


void completesum(const uint64_t* a, const uint64_t *  x, const size_t length, uint64_t * out) {
    uint192 s;
    s.low = 0;
    s.high = 0;
    s.vhigh = 0;
    size_t i = 0;
    for(; i<length*8/8; i+= 8) {
#ifdef IACA
        IACA_START;
#endif
        __asm__ (
            "movq (%[u]),%%rax\n"
            "mulq (%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 8(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 8(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 16(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 16(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 24(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 24(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 32(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 32(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 40(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 40(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 48(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 48(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 56(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 56(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "r" (a+i), [v] "r" (x+i)  :"rdx","rax","memory","cc");
#ifdef IACA
        IACA_END;
#endif
    }
    for(; i<length; ++i) {
        __asm__ ("mulq %[v]\n"
                 "addq %%rax,  %[rl]\n"
                 "adcq %%rdx,  %[rh]\n"
                 "adcq $0,  %[rhh]\n"
                 :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "a" (a[i]), [v] "r" (x[i])  :"rdx","cc");
    }
    out[0] = s.low;
    out[1] = s.high;
    out[2] = s.vhigh;
}


void completesum32blocks(const uint64_t* a, const uint64_t *  x, const size_t length, uint64_t * out) {
    uint192 s;
    s.low = 0;
    s.high = 0;
    s.vhigh = 0;
    size_t i = 0;
    for(; i<length*32/32; i+= 32) {
#ifdef IACA
        IACA_START;
#endif
        __asm__ (
            "movq (%[u]),%%rax\n"
            "mulq (%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 8(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 8(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 16(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 16(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 24(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 24(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 32(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 32(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 40(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 40(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 48(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 48(%[v])\n"
            "addq %%rax,  %[rl]\n"			
            "movq 56(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 56(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 64(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 64(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 72(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 72(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 80(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 80(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 88(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 88(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 96(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 96(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 104(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 104(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 112(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 112(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 120(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 120(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 128(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 128(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 136(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 136(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 144(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 144(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 152(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 152(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 160(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 160(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 168(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 168(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 176(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 176(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 184(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 184(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 192(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 192(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 200(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 200(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 208(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 208(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 216(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 216(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 224(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 224(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 232(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 232(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 240(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 240(%[v])\n"
            "addq %%rax,  %[rl]\n"			
            "movq 248(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 248(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "r" (a+i), [v] "r" (x+i)  :"rdx","rax","memory","cc");
#ifdef IACA
        IACA_END;
#endif
    }
    for(; i<length; ++i) {
        __asm__ ("mulq %[v]\n"
                 "addq %%rax,  %[rl]\n"
                 "adcq %%rdx,  %[rh]\n"
                 "adcq $0,  %[rhh]\n"
                 :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "a" (a[i]), [v] "r" (x[i])  :"rdx","cc");
    }
    out[0] = s.low;
    out[1] = s.high;
    out[2] = s.vhigh;
}


// like MHH, this is essentially multilinear with 64bit multiplication
// summed up over a 128-bit counter
void MMHsum(const uint64_t* randomsource, const uint64_t *  string, const size_t length, uint64_t * out) {

    uint64_t low = 0;
    uint64_t high = 0;
    size_t i = 0;
    for(; i<length/8*8; i+=8) {
#ifdef IACA
        IACA_START;
#endif
        __asm__ (
            "movq (%[u]),%%rax\n"
            "mulq (%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 8(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 8(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 16(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 16(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 24(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 24(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 32(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 32(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 40(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 40(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 48(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 48(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 56(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 56(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            :  [rh] "+r" (high) , [rl] "+r" (low)  : [u] "r" (randomsource+i), [v] "r" (string+i)  :"rdx","rax","memory","cc");
#ifdef IACA
        IACA_END;
#endif
    }

    for(; i<length; ++i) {
        __asm__ ("mulq %[v]\n"
                 "addq %%rax,  %[rl]\n"
                 "adcq %%rdx,  %[rh]\n"
                 :  [rh] "+r" (high), [rl] "+r" (low)  : [u] "a" (randomsource[i]), [v] "r" (string[i])  :"rdx","cc");
    }
    out[0] = low;
    out[1] = high;
}


// a sum with half the number of multiplications
void NHsum(const uint64_t* kp, const uint64_t *  mp, const size_t length, uint64_t * out) {
    uint64_t th, tl;
    uint64_t rh = 0;
    uint64_t rl = 0;
    uint64_t low = 0;
    uint64_t high = 0;
    size_t i = 0;
    for(; i<length/8*8; i+=8) {
#ifdef IACA
        IACA_START;
#endif
        MUL64(th,tl,mp[i  ]+(kp)[i  ],mp[i +1 ]+(kp)[i+1]);
        ADD128(rh,rl,th,tl);
        MUL64(th,tl,mp[i+2]+(kp)[i+2],mp[i +3 ]+(kp)[i+3]);
        ADD128(rh,rl,th,tl);
        MUL64(th,tl,mp[i +4 ]+(kp)[i+4],mp[i +5 ]+(kp)[i+5]);
        ADD128(rh,rl,th,tl);
        MUL64(th,tl,mp[i +6 ]+(kp)[i+6],mp[i +7 ]+(kp)[i+7]);
        ADD128(rh,rl,th,tl);
#ifdef IACA
        IACA_END;
#endif
    }
    if(i!=length) cout<<"please use a multiple of 8"<<endl;
    out[0] = rl;
    out[1] = rh;
}


void completesum2(const uint64_t* a, const uint64_t *  x, const size_t length, uint64_t * out) {
    uint192 s;
    s.low = 0;
    s.high = 0;
    s.vhigh = 0;
    size_t i = 0;
    for(; i<length*8/8; i+= 8) {
#ifdef IACA
        IACA_START;
#endif
        __asm__ (
            "movq (%[u]),%%rax\n"
            "mulq (%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 8(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 8(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 16(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 16(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 24(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 24(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 32(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 32(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 40(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 40(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 48(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 48(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 56(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 56(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
                        :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "r" (a+i), [v] "r" (x+i)  :"rdx","rax","memory","cc");
#ifdef IACA
        IACA_END;
#endif
    }
    for(; i<length; ++i) {
        __asm__ ("mulq %[v]\n"
                 "addq %%rax,  %[rl]\n"
                 "adcq %%rdx,  %[rh]\n"
                 "adcq $0,  %[rhh]\n"
                 :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "a" (a[i]), [v] "r" (x[i])  :"rdx","cc");
    }
    out[0] = s.low;
    out[1] = s.high;
    out[2] = s.vhigh;
}



void completesumi(const uint64_t* a, const uint64_t *  x, const size_t length, uint64_t * out) {
    uint192 s;
    s.low = 0;
    s.high = 0;
    s.vhigh = 0;
    uint192 ss;
    ss.low = 0;
    ss.high = 0;
    ss.vhigh = 0;

    size_t i = 0;
    for(; i<length*8/8; i+= 8) {
#ifdef IACA
        IACA_START;
#endif
        __asm__ (
            "movq (%[u]),%%rax\n"
            "mulq (%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 8(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 8(%[v])\n"
            "addq %%rax,  %[rrl]\n"
            "movq 16(%[u]),%%rax\n"
            "adcq %%rdx,  %[rrh]\n"
            "adcq $0,  %[rrhh]\n"
            "mulq 16(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 24(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 24(%[v])\n"
            "addq %%rax,  %[rrl]\n"
            "movq 32(%[u]),%%rax\n"
            "adcq %%rdx,  %[rrh]\n"
            "adcq $0,  %[rrhh]\n"
            "mulq 32(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 40(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 40(%[v])\n"
            "addq %%rax,  %[rrl]\n"
            "movq 48(%[u]),%%rax\n"
            "adcq %%rdx,  %[rrh]\n"
            "adcq $0,  %[rrhh]\n"
            "mulq 48(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 56(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 56(%[v])\n"
            "addq %%rax,  %[rrl]\n"
            "adcq %%rdx,  %[rrh]\n"
            "adcq $0,  %[rrhh]\n"
            :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low) ,[rrh] "+r" (ss.high), [rrhh] "+r" (ss.vhigh) , [rrl] "+r" (ss.low)  : [u] "r" (a+i), [v] "r" (x+i)  :"rdx","rax","memory","cc");
#ifdef IACA
        IACA_END;
#endif
    }
    
    for(; i<length; ++i) {
        __asm__ ("mulq %[v]\n"
                 "addq %%rax,  %[rl]\n"
                 "adcq %%rdx,  %[rh]\n"
                 "adcq $0,  %[rhh]\n"
                 :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "a" (a[i]), [v] "r" (x[i])  :"rdx","cc");
    }
    out[0] = s.low;
    out[1] = s.high;
    out[2] = s.vhigh;
}

int main() {
    const size_t N = 1000*128;
    const size_t repeat = 10000;
#ifdef USE_SSE
	// for SSE at least one of args should be 16-byte aligned; this is OK for predefined sets of coefficients
    uint64_t _a_[N+4];
	uint64_t *a = (uint64_t*)( ( (((uint64_t)(_a_)) >> 5 ) << 5 ) + 32 );
#else
    uint64_t a[N];
#endif
    uint64_t x[N];
    uint64_t out[3];
    for(size_t i = 0; i < N; ++i) {
        a[i] = rand() + (((uint64_t)rand())<<32);
        x[i] = rand() + (((uint64_t)rand())<<32);
    }
    uint192 s1, s2;
	
    const clock_t S0 = clock();
    out[0]=0; out[1]=0; out[2]=0;   
    for(int T=0; T<repeat; ++T) {
        completesum_alt( a, x, N,out);
    }
    cout<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
	
    const clock_t S1 = clock();
    out[0]=0; out[1]=0; out[2]=0;   
    for(int T=0; T<repeat; ++T) {
        completesum32blocks( a, x, N,out);
    }
    cout<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
    out[0]=0; out[1]=0; out[2]=0;  
    const clock_t S2 = clock();
    for(int T=0; T<repeat; ++T) {
        MMHsum( a, x, N,out);
    }
    cout<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
    out[0]=0; out[1]=0; out[2]=0;  

    const clock_t S3 = clock();
    for(int T=0; T<repeat; ++T) {
        NHsum( a, x, N,out);
    }
    cout<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
    out[0]=0; out[1]=0; out[2]=0;  

    const clock_t S4 = clock();
    for(int T=0; T<repeat; ++T) {
        completesumi( a, x, N,out);
    }
    const clock_t S5 = clock();
    cout<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
	
#ifdef USE_SSE
    out[0]=0; out[1]=0; out[2]=0;   
    for(int T=0; T<repeat; ++T) {
        completesum_alt_with_sse( a, x, N,out);
    }
    const clock_t S6 = clock();
    cout<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
#endif
     

    cout<<"complete_alt sum time="<<(double)(S1-S0)/ CLOCKS_PER_SEC<<endl;
    cout<<"complete sum time="<<(double)(S2-S1)/ CLOCKS_PER_SEC<<endl;
    cout<<"MMH sum ="<<(double)(S3-S2)/ CLOCKS_PER_SEC<<endl;
    cout<<"NH sum ="<<(double)(S4-S3)/ CLOCKS_PER_SEC<<endl;
    cout<<"complete sum time="<<(double)(S5-S4)/ CLOCKS_PER_SEC<<endl;
#ifdef USE_SSE
    cout<<"complete_alt_w_sse sum time="<<(double)(S6-S5)/ CLOCKS_PER_SEC<<endl;
#endif
}

