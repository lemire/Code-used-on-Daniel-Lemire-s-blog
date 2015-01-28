/**
searching values over small blocks of integers.
g++ -O2 -march=native -o findfast findfast.cpp && ./findfast
*/
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <x86intrin.h>
using namespace std;

// returns -1 if value is not found, otherwise returns the index
int find(uint32_t * begin, uint32_t * end, uint32_t val) {
    for(uint32_t * i = begin; i != end; ++i) {
        if(*i == val) return i - begin;
    }
    return -1;
}

int findbs(uint32_t * begin, uint32_t * end, uint32_t val) {
    uint32_t * i = lower_bound (begin,end, val);
    if(i != end) {
      if(*i == val) return i - begin;
    }
    return -1;
}

int findSIMD(uint32_t * begin, uint32_t * end, uint32_t val) {
    size_t length = end-begin;
    __m128i valvec = _mm_set1_epi32(val);
    __m128i * vi = (__m128i *) begin;
    for(size_t i = 0; i < length / 4; ++i) {
        __m128i v = _mm_lddqu_si128(vi +i);
        __m128i cm = _mm_cmpeq_epi32(v,valvec);
        if(!_mm_testz_si128(cm,cm)) {// we have a match
          	uint32_t * hits = (uint32_t *) &cm;
          	for(int j = 0; ; ++j) if(hits[j] != 0) return i*4 +j;
        }
    }
    for(uint32_t * i = begin + length / 4  * 4; i != end; ++i) {
        if(*i == val) return i - begin;
    }
    return -1;
}




int main() {
    const size_t repeat = 50000000;
    srand(12);
    size_t dummy = 0;
    for(size_t N = 16; N <= 128; N*=2) {
    cout<<"block size = "<<N<<endl;
    vector<uint32_t> a(N);
    for(size_t i = 0; i < N; ++i) {
        a[i] = rand() ;
    }
    sort( &a[0], &a[0]+N);
    for(int T=0; T<N; ++T) {
        int x = find( &a[0], &a[0]+N,a[T%N]);
        int y = findSIMD( &a[0], &a[0]+N,a[T%N]);
        int z = findbs( &a[0], &a[0]+N,a[T%N]);
        assert(x == y);
        assert(x == z);
    }
 
    const clock_t S0 = clock();
    for(int T=0; T<repeat; ++T) {
        dummy += find( &a[0], &a[0]+N,a[T%N]);
    }
    const clock_t S1 = clock();
    for(int T=0; T<repeat; ++T) {
        dummy += findSIMD( &a[0], &a[0]+N,a[T%N]);
    }
    const clock_t S2 = clock();
    for(int T=0; T<repeat; ++T) {
        dummy += findbs( &a[0], &a[0]+N,a[T%N]);
    }
    const clock_t S3 = clock();
     cout<<"We report the speed in millions of values checked per second."<<endl;
    double oneoveronemillion = 1 / (1000 * 1000.0);
    double normalizedvolume = N * repeat * oneoveronemillion; 
    cout<<"scalar sequential search = "<<normalizedvolume/((double)(S1-S0)/ CLOCKS_PER_SEC)<<endl;
    cout<<"SIMD sequential search = "<<normalizedvolume/((double)(S2-S1)/ CLOCKS_PER_SEC)<<endl;
    cout<<"binary search = "<<normalizedvolume/((double)(S3-S2)/ CLOCKS_PER_SEC)<<endl;
    cout<<endl;
    }
    return dummy;
}
