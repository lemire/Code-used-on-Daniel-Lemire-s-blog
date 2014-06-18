/**
* Scalar product over 64-bit integers
g++ -O3 -msse4.1 -o sum32 sum32.cpp && ./sum32
*/
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <stdint.h>
#include <cassert>
#include <x86intrin.h>
using namespace std;

__uint128_t scalarproduct(size_t len, const uint32_t * a, const uint32_t * x) {
	__uint128_t s = 0;
	for(int k = 0; k<len; ++k)
	    s+= (__uint128_t) a[k] * x[k];
	return s;
}


void printme32(__m128i v1) {
    printf(" %lu %lu %lu %lu  ", _mm_extract_epi32(v1,0), _mm_extract_epi32(v1,1), _mm_extract_epi32(v1,2), _mm_extract_epi32(v1,3));
}


void printme64(__m128i v1) {
    printf(" %llu %llu  ", _mm_extract_epi64(v1,0), _mm_extract_epi64(v1,1));
}

__uint128_t ssescalarproduct(size_t len, const uint32_t * a, const uint32_t * x) {
	__m128i c0 = _mm_setzero_si128 (); // Sets the 128-bit value to zero.
	__m128i c1 = _mm_setzero_si128 ();
	__m128i M = _mm_set_epi32 ( 0, -1, 0 , -1 );

	for(int k = 0; k<len; k+=4) {
	   __m128i A = _mm_loadu_si128 ((__m128i *)(a+k));
	   __m128i X = _mm_loadu_si128 ((__m128i *)(x+k));
	   __m128i p = _mm_mul_epu32 (A,X);
	   __m128i t = _mm_srli_epi64( p, 32 );
	   c1 = _mm_add_epi64 ( c1, t );
		t = _mm_and_si128 ( M, p );
		c0 = _mm_add_epi64 ( c0, t );
		A = _mm_shuffle_epi32( A, 0xb1 );
		X = _mm_shuffle_epi32( X, 0xb1 );
		p = _mm_mul_epu32 ( A, X);
		t = _mm_srli_epi64( p, 32 );
		c1 = _mm_add_epi64 ( c1, t );
		t = _mm_and_si128 ( M, p );
		c0 = _mm_add_epi64 ( c0, t );
	}
	__m128i t = _mm_unpackhi_epi64 ( c0, c1 ); 
	__m128i d = _mm_unpacklo_epi64 ( c0, c1 );
	c1 = _mm_add_epi64 ( d, t );
	uint32_t b[4];
	_mm_storeu_si128((__m128i *)b,c1);
	uint32_t low = b[0];
	uint64_t  high =  b[1] + b[2] + ((uint64_t) b[3] <<32);
	return low + ((__uint128_t) high << 32);
	
//	return ((__uint128_t)_mm_extract_epi64(c1,0)) + ((__uint128_t)_mm_extract_epi64(c1,1)<<32);
}

int main() {
 	const size_t N = 1000*128;
	uint32_t a[N];
	uint32_t x[N];
	for(size_t i = 0; i < N; ++i) {
	  a[i] = rand();
	  x[i] = rand();
	}
	__uint128_t s1,s2;
	int t1 = 0; 
	int t2 = 0;
	int times = 100;
	const clock_t S1 = clock();

	for(int T=0; T<times;++T) {
	  s1 = scalarproduct(N, a, x);
        }
	const clock_t S2 = clock();

	for(int T=0; T<times;++T) {
	  s2 = ssescalarproduct(N, a, x);
        }
	const clock_t S3 = clock();

    cout<<"GCC time="<<(double)(S2-S1)/ CLOCKS_PER_SEC<<endl;
    cout<<"SSE2 time="<<(double)(S3-S2)/ CLOCKS_PER_SEC<<endl;

	cout<<(uint64_t)s1<<" "<<(uint64_t)(s1>>64)<<endl;
	cout<<(uint64_t)s2<<" "<<(uint64_t)(s2>>64)<<endl;

}
