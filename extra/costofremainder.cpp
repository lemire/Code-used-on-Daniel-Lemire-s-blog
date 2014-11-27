/**
* cost of remainder in 64-bit multiplications
g++ -O3 -march=native -o costofremainder costofremainder.cpp && ./costofremainder
*/
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <stdint.h>
#include <x86intrin.h>
using namespace std;


struct uint192 {
	uint64_t low;
	uint64_t high;
	uint64_t vhigh;
} ;

void completesum(const uint64_t* a, const uint64_t *  x, const size_t length, uint64_t * out) {
	uint192 s;
	s.low = 0;
	s.high = 0;
	s.vhigh = 0;
	size_t i = 0;
	for(; i<length/8; i+= 8) {
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

int main() {
    const size_t N = 100*128;
	uint64_t a[N];
	uint64_t x[N];
	uint64_t out[3];
	for(size_t i = 0; i < N; ++i) {
	  a[i] = rand() + (((uint64_t)rand())<<32);
	  x[i] = rand() + (((uint64_t)rand())<<32);
	}
	uint192 s1, s2;
	const clock_t S1 = clock();

	for(int T=0; T<10000;++T) {
	  completesum( a, x, N,out);
    }
	const clock_t S2 = clock();
	for(int T=0; T<10000;++T) {
	  MMHsum( a, x, N,out);
    }
    const clock_t S3 = clock();
	
    cout<<"complete sum time="<<(double)(S2-S1)/ CLOCKS_PER_SEC<<endl;
    cout<<"MMH sum ="<<(double)(S3-S2)/ CLOCKS_PER_SEC<<endl;
   
}
