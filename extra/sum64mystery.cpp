/**
* Scalar product over 64-bit integers
g++ -O3 -o sum64mystery sum64mystery.cpp && ./sum64mystery
*/
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
#include <stdint.h>
using namespace std;


__uint128_t carrylesscalarproduct(size_t length, const uint64_t * a, const uint64_t * x) {
	__uint128_t base = 0;
	for(size_t i = 0; i<length; ++i) {
	  base  += (__uint128_t) a[i] * (__uint128_t) x[i];
	}
	return base;
}


__uint128_t asmcarrylessscalarproduct(size_t length, const uint64_t * a, const uint64_t * x) {
	uint64_t part1 = 0;
	uint64_t part2 = 0;
	assert(length / 8 * 8 == length);
	for(int i = 0; i<length; i+= 8) {
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
                 :  [rh] "+r" (part2) , [rl] "+r" (part1)  : [u] "r" (a+i), [v] "r" (x+i)  :"rdx","rax","memory","cc");
	}
	return (((__uint128_t) part2)<<64)  + part1 ;
}

__uint128_t asmcarrylessscalarproduct(size_t length, const uint64_t * a, const uint64_t * x) {
	uint64_t part1 = 0;
	uint64_t part2 = 0;
	uint64_t part3 = 0;
	assert(length / 8 * 8 == length);
	for(int i = 0; i<length; i+= 8) {
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
                 :  [rh] "+r" (part2) , [rl] "+r" (part1)  : [u] "r" (a+i), [v] "r" (x+i)  :"rdx","rax","memory","cc");
	}
	return (((__uint128_t) part2)<<64)  + part1 ;
}
int main() {
	const size_t N = 100*128;
	uint64_t a[N];
	uint64_t x[N];
	for(size_t i = 0; i < N; ++i) {
	  a[i] = rand() + (((uint64_t)rand())<<32);
	  x[i] = rand() + (((uint64_t)rand())<<32);
	}
	__uint128_t s1, s2;
	const clock_t S1 = clock();
	for(int T=0; T<10000;++T) {
	  s1 = carrylesscalarproduct(N, a, x);
    }
	const clock_t S2 = clock();
	for(int T=0; T<10000;++T) {
	  s2 = asmcarrylessscalarproduct(N, a, x);
	}
    const clock_t S3 = clock();
	
    cout<<"uint128 time="<<(double)(S2-S1)/ CLOCKS_PER_SEC<<endl;
    cout<<"asm time="<<(double)(S3-S2)/ CLOCKS_PER_SEC<<endl;
 
	cout<<(uint64_t)s1<<" "<<(uint64_t)(s1>>64)<<endl;
	cout<<(uint64_t)s2<<" "<<(uint64_t)(s2>>64)<<endl;
}
