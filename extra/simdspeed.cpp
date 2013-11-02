// Daniel Lemire, Nov 2nd 2013
/***
$ g++-4.8.0 -O3 -mavx -o simdspeed simdspeed.cpp 
dlemire@fastpfor:~$ ./simdspeed 
39 17 10
2.56954e+08
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
#include <string>
#include <immintrin.h>
using namespace std;

 class WallClockTimer
  {
  public:
      struct timeval t1, t2;
  public:
      WallClockTimer() : t1(), t2(){ gettimeofday(&t1,0); t2 = t1; }
      void reset() {gettimeofday(&t1,0); t2 = t1;}
      int elapsed() { return ((t2.tv_sec - t1.tv_sec) * 1000) + ((t2.tv_usec - t1.tv_usec) / 1000); }
      int split() { gettimeofday(&t2,0); return elapsed(); }
  };

float scalar128(float * a, float *b, size_t length) {
	float sum = 0;
	for(size_t i = 0; i < length/4;i+=4 ) {
		__m128 veca= _mm_load_ps (a+4*i);
		__m128 vecb= _mm_load_ps (b+4*i);
		__m128 r1 = _mm_mul_ps(veca, vecb);
        __m128 r2 = _mm_hadd_ps(r1, r1);
        __m128 r3 = _mm_hadd_ps(r2, r2);
		sum +=  _mm_extract_ps(r3,0);
	}
	return sqrt(sum);
}

float scalar256(float * a, float *b, size_t length) {
	float sum = 0;
	for(size_t i = 0; i < length/8;i+=8 ) {
		__m256 veca= _mm256_loadu_ps (a+8*i);
		__m256 vecb= _mm256_loadu_ps (b+8*i);
		__m256 r1 = _mm256_mul_ps(veca, vecb);
        __m256 r2 = _mm256_hadd_ps(r1, r1);
        __m256 r3 = _mm256_hadd_ps(r2, r2);
        __m256 r4 = _mm256_hadd_ps(r2, r2);
        sum +=  _mm_extract_ps( _mm256_extractf128_ps (r4,0),0);
	}
	return sqrt(sum);
}
float scalar(float * a, float *b, size_t length) {
	float sum = 0;
	for(size_t i = 0; i < length;i++ ) {
		sum += a[i] * b[i];
	}
	return sqrt(sum);
}


int main() {
	size_t N = 1024*1024*4;
	vector<float> a(N,4);
	vector<float> b(N,5);
	WallClockTimer timer;
	float bogus = 0;
	timer.reset();
	for(int k = 0; k<10;++k)
	bogus += scalar(&a[0], &b[0],N);
	int time1 = timer.split();
	timer.reset();
    for(int k = 0; k<10;++k)
	bogus += scalar128(&a[0], &b[0],N);
    int time2 = timer.split();
	timer.reset();
    for(int k = 0; k<10;++k)
	bogus += scalar256(&a[0], &b[0],N);
    int time3 = timer.split();

    cout<<time1<<" "<<time2<<" "<<time3<<endl;
    cout<<bogus<<endl;
}
