/**
g++ -O2 -march=native -o scalarproduct scalarproduct.cpp && ./scalarproduct

*/
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
#include <mmintrin.h>
#include <stdint.h>
#include <x86intrin.h>
using namespace std;


float product(float * v1, float * v2, size_t length) {
    __m128 res;
    for(size_t i = 0; i < length / 4; ++i) {
       __m128 mv1 = _mm_loadu_ps(v1 +4*i);
       __m128 mv2 = _mm_loadu_ps(v2 +4*i); 
       res = _mm_add_ps(res,_mm_dp_ps(mv1,mv2,0x7f));    
    }
    float sofar = _mm_extract_ps(res,0);
    for(size_t i = length / 4 * 4; i<4; ++i) {
        sofar += v1[i] * v2[i];
    }
    return sofar;
}


int main() {
    const size_t N = 128;
    const size_t repeat = 100000;
    float a[N];
    float b[N];
    for(size_t i = 0; i < N; ++i) {
        a[i] = (float) rand() ;
        b[i] = (float) rand() ;
    }
 
    float dummy = 0;
    const clock_t S0 = clock();
    for(int T=0; T<repeat; ++T) {
        dummy += product( a, b,N);
    }
    const clock_t S1 = clock();
    cout<<"we report the speed."<<endl;
    double oneoveronemillion = 1 / (1000 * 1000.0);
    double normalizedvolume = N * repeat * oneoveronemillion; 
    cout<<"SIMD ="<<normalizedvolume/((double)(S1-S0)/ CLOCKS_PER_SEC)<<endl;
    return (int) dummy;
}
