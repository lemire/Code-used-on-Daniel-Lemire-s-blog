// Daniel Lemire, Nov 2nd 2013
/***
$ g++-4.8.0 -O3 -mavx -o simdspeed simdspeed.cpp 
91 29 28
1 3.14 3.25
#ignore=724.07733 724.07733 724.07733
**/
#include <cmath>
#include <vector>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <immintrin.h>
using namespace std;

 class WallClockTimer {
  public:
      struct timeval t1, t2;
      WallClockTimer() : t1(), t2(){ gettimeofday(&t1,0); t2 = t1; }
      void reset() {gettimeofday(&t1,0); t2 = t1;}
      int elapsed() { return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000); }
      int split() { gettimeofday(&t2,0); return elapsed(); }
  };


__attribute__((__noinline__,__target__("no-avx")))
float scalar(const float * a, const float *b, size_t length) {
	float sum = 0;
	for(size_t i = 0; i < length;i++ ) {
		sum += a[i] * b[i];
	}
	return sqrt(sum);
}

#ifdef __SSE3__
__attribute__((__noinline__,__target__("no-avx")))
float scalar128(const float * a, const float *b, size_t length) {
    __m128 sum = _mm_set1_ps(0);
    assert(length/4*4==length);
	for(size_t i = 0; i < length/4; ++i ) {
		const __m128 veca = _mm_loadu_ps (a+4*i);
		const __m128 vecb = _mm_loadu_ps (b+4*i);
		const __m128 r1 = _mm_mul_ps(veca, vecb);
        sum = _mm_add_ps(sum,r1);
	}
    __m128 r2 = _mm_hadd_ps(sum, sum);
    __m128 r3 = _mm_hadd_ps(r2, r2);
	return sqrt( _mm_cvtss_f32 (r3));
}
#endif

#ifdef  __AVX__
__attribute__((__noinline__))
float scalar256(const float * a, const float *b, size_t length) {
    __m256 sum =  _mm256_set1_ps (0);
    assert(length/8*8==length);
	for(size_t i = 0; i < length/8; ++i ) {
		const __m256 veca = _mm256_loadu_ps (a+8*i);
		const __m256 vecb = _mm256_loadu_ps (b+8*i);
		const __m256 r1 = _mm256_mul_ps(veca, vecb);
        sum =  _mm256_add_ps (sum,r1);
	}
    __m256 r2 = _mm256_hadd_ps(sum,sum);
    __m256 r3 = _mm256_hadd_ps(r2, r2);
    __m256 r4 = _mm256_hadd_ps(r3, r3);
	return sqrt( _mm_cvtss_f32 ( _mm256_extractf128_ps (r4,0)) );
}
#endif

int main() {
    int time1, time2, time3;
	size_t N =  1024*1024;
    int T=100;
	vector<float> a(N,0.1);
	vector<float> b(N,5);
	WallClockTimer timer;
	float bogus1 = 0;
    float bogus2 = 0;
    float bogus3 = 0;
	timer.reset();
	for(int k = 0; k<T;++k)
	bogus1 = scalar(&a[0], &b[0],N);
	time1 = timer.split();
#ifdef __SSE3__
	timer.reset();
    for(int k = 0; k<T;++k)
	bogus2 = scalar128(&a[0], &b[0],N);
    time2 = timer.split();
#endif
#ifdef __AVX__
	timer.reset();
    for(int k = 0; k<T;++k)
	bogus3 = scalar256(&a[0], &b[0],N);
    time3 = timer.split();
#endif
    cout<<time1<<" "<<time2<<" "<<time3<<endl;
    cout<< std::setprecision(3)<<time1*1.0/time1<<" "<<time1*1.0/time2<<" "<<time1*1.0/time3<<endl;
     cout<< std::setprecision(8)<<"#ignore="<<bogus1<<" "<<bogus2<<" "<<bogus3<<endl;
}
