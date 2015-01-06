/**
* cost of remainder in 64-bit multiplications
g++ -O2 -march=native -o costofremainder3 costofremainder3.cpp && ./costofremainder3

g++ -O2 -march=native -o costofremainder3 costofremainder3.cpp -DIACA 
/opt/intel/iaca/bin/iaca.sh -64 -mark 1 ./costofremainder3
/opt/intel/iaca/bin/iaca.sh -64 -mark 2 ./costofremainder3
/opt/intel/iaca/bin/iaca.sh -64 -mark 3 ./costofremainder3

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


#define COMPLETESUM_ALT__DECLARE \
	  uint64_t ctr0 = 0, ctr1 = 0, ctr2 = 0; \
	  uint64_t ctr1_0 = 0, ctr1_1 = 0, ctr1_2 = 0, ctr1_3 = 0; \
	  uint64_t ctr2_0 = 0, ctr2_1 = 0, ctr2_2 = 0, ctr2_3 = 0; \
	  uint64_t mulLow, mulHigh;

#define COMPLETESUM_ALT__T_BASE( ii ) { \
uint64_t rhi;  /*Dummy variable to tell the compiler that the register rax is input and clobbered but not actually output; see assembler code below. Better syntactic expression is very welcome.*/ \
__asm__( "mulq %5\n" \
        "addq %%rax, %0\n" \
        "adcq %%rdx, %1\n" \
        "adcq $0, %2\n" \
        : "+g" (ctr0), "+g" (ctr1), "+g" (ctr2), "=a" (rhi) \
        :"a"(x[ii]), "g"(coeff[ ii ]) : "rdx", "cc" ); \
}



#define COMPLETESUM_ALT__T1 COMPLETESUM_ALT__T_BASE

#define COMPLETESUM_ALT__T2( ii ) { \
uint64_t rhi;  /*Dummy variable to tell the compiler that the register rax is input and clobbered but not actually output; see assembler code below. Better syntactic expression is very welcome.*/ \
__asm__( "mulq %5\n" \
        "addq %%rax, %0\n" \
        "adcq %%rdx, %1\n" \
        "adcq $0, %2\n" \
        : "+g" (ctr2_0), "+g" (ctr2_1), "+g" (ctr2_2), "=a" (rhi) \
        :"a"(x[ii]), "g"(coeff[ ii ]) : "rdx", "cc" ); \
}

#define COMPLETESUM_ALT__FINALIZE { \
__asm__("addq %3, %0\n" \
        "adcq %4, %1\n" \
        "adcq %5, %2\n" \
        : "+g" (ctr0), "+g" (ctr1), "+g" (ctr2) \
        : "g"(ctr2_0), "g"(ctr2_1), "g"(ctr2_2) : "cc" ); \
}


void completesum_alt(const uint64_t* coeff, const uint64_t *  x, const size_t length, uint64_t * out) 
{
	COMPLETESUM_ALT__DECLARE
	
    size_t i = 0;
    for(; i<length*32/32; i+= 32) {
//    for(; i<length*16/16; i+= 16) {
//    for(; i<length*8/8; i+= 8) {
#ifdef IACA
        IACA_START;
#endif

	COMPLETESUM_ALT__T2( i + 0 )
	COMPLETESUM_ALT__T1( i + 1 )
	COMPLETESUM_ALT__T2( i + 2 )
	COMPLETESUM_ALT__T1( i + 3 )
	COMPLETESUM_ALT__T2( i + 4 )
	COMPLETESUM_ALT__T1( i + 5 )
	COMPLETESUM_ALT__T2( i + 6 )
	COMPLETESUM_ALT__T1( i + 7 )
	COMPLETESUM_ALT__T2( i + 8 )
	COMPLETESUM_ALT__T1( i + 9 )
	COMPLETESUM_ALT__T2( i + 10 )
	COMPLETESUM_ALT__T1( i + 11 )
	COMPLETESUM_ALT__T2( i + 12 )
	COMPLETESUM_ALT__T1( i + 13 )
	COMPLETESUM_ALT__T2( i + 14 )
	COMPLETESUM_ALT__T1( i + 15 )
	COMPLETESUM_ALT__T2( i + 16 )
	COMPLETESUM_ALT__T1( i + 17 )
	COMPLETESUM_ALT__T2( i + 18 )
	COMPLETESUM_ALT__T1( i + 19 )
	COMPLETESUM_ALT__T2( i + 20 )
	COMPLETESUM_ALT__T1( i + 21 )
	COMPLETESUM_ALT__T2( i + 22 )
	COMPLETESUM_ALT__T1( i + 23 )
	COMPLETESUM_ALT__T2( i + 24 )
	COMPLETESUM_ALT__T1( i + 25 )
	COMPLETESUM_ALT__T2( i + 26 )
	COMPLETESUM_ALT__T1( i + 27 )
	COMPLETESUM_ALT__T2( i + 28 )
	COMPLETESUM_ALT__T1( i + 29 )
	COMPLETESUM_ALT__T2( i + 30 )
	COMPLETESUM_ALT__T1( i + 31 )

#ifdef IACA
        IACA_END;
#endif
    }
	
    for(; i<length; ++i) 
	{
	COMPLETESUM_ALT__T1( i )
    }
	
	COMPLETESUM_ALT__FINALIZE
	
    out[0] = ctr0;
    out[1] = ctr1;
    out[2] = ctr2;
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
    for(; i<length/32*32; i+=32) {
#ifdef IACA
        IACA_START;
#endif
        __asm__ (
            "movq (%[u]),%%rax\n"
            "mulq (%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 8(%[u]),%%rax\n"
            "mulq 8(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 16(%[u]),%%rax\n"
            "mulq 16(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 24(%[u]),%%rax\n"
            "mulq 24(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 32(%[u]),%%rax\n"
            "mulq 32(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 40(%[u]),%%rax\n"
            "mulq 40(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 48(%[u]),%%rax\n"
            "mulq 48(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 56(%[u]),%%rax\n"
            "mulq 56(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 64(%[u]),%%rax\n"
            "mulq 64(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 72(%[u]),%%rax\n"
            "mulq 72(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 80(%[u]),%%rax\n"
            "mulq 80(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 88(%[u]),%%rax\n"
            "mulq 88(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 96(%[u]),%%rax\n"
            "mulq 96(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 104(%[u]),%%rax\n"
            "mulq 104(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 112(%[u]),%%rax\n"
            "mulq 112(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 120(%[u]),%%rax\n"
            "mulq 120(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 128(%[u]),%%rax\n"
            "mulq 128(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 136(%[u]),%%rax\n"
            "mulq 136(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 144(%[u]),%%rax\n"
            "mulq 144(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 152(%[u]),%%rax\n"
            "mulq 152(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 160(%[u]),%%rax\n"
            "mulq 160(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 168(%[u]),%%rax\n"
            "mulq 168(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 176(%[u]),%%rax\n"
            "mulq 176(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 184(%[u]),%%rax\n"
            "mulq 184(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 192(%[u]),%%rax\n"
            "mulq 192(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 200(%[u]),%%rax\n"
            "mulq 200(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 208(%[u]),%%rax\n"
            "mulq 208(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 216(%[u]),%%rax\n"
            "mulq 216(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 224(%[u]),%%rax\n"
            "mulq 224(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 232(%[u]),%%rax\n"
            "mulq 232(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 240(%[u]),%%rax\n"
            "mulq 240(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "movq 248(%[u]),%%rax\n"
            "mulq 248(%[v])\n"
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


int main() {
    const size_t N = 100*128;
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
    for(int T=0; T<10000; ++T) {
        completesum_alt( a, x, N,out);
    }
    cout<<"completesum_alt "<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
	
    const clock_t S1 = clock();
    out[0]=0; out[1]=0; out[2]=0;   
    for(int T=0; T<10000; ++T) {
        completesum32blocks( a, x, N,out);
    }
    cout<<"completesum32blocks "<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
    out[0]=0; out[1]=0; out[2]=0;  
    const clock_t S2 = clock();
    for(int T=0; T<10000; ++T) {
        MMHsum( a, x, N,out);
    }
    cout<<"MMHsum "<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
    out[0]=0; out[1]=0; out[2]=0;  

    const clock_t S3 = clock();
    for(int T=0; T<10000; ++T) {
        NHsum( a, x, N,out);
    }
    cout<<"NHsum "<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
    out[0]=0; out[1]=0; out[2]=0;  

    const clock_t S4 = clock();
 
    cout<<"complete_alt sum time="<<(double)(S1-S0)/ CLOCKS_PER_SEC<<endl;
	cout<<"completesum32blocks time="<<(double)(S2-S1)/ CLOCKS_PER_SEC<<endl;
    cout<<"MMH sum ="<<(double)(S3-S2)/ CLOCKS_PER_SEC<<endl;
    cout<<"NH sum ="<<(double)(S4-S3)/ CLOCKS_PER_SEC<<endl;
}

