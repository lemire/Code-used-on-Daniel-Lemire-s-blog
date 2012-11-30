//g++-fsf-4.7 -std=c++11  -O3 -funroll-loops -o leonidunpacking leonidunpacking.cpp
// C headers (sorted)
#include <immintrin.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

// C++ headers (sorted)
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


    const static __m128i mask1 =  _mm_set1_epi32(1U); 



class WallClockTimer {
public:
    struct timeval t1, t2;
    WallClockTimer() :
        t1(), t2() {
        gettimeofday(&t1, 0);
        t2 = t1;
    }
    void reset() {
        gettimeofday(&t1, 0);
        t2 = t1;
    }
    uint64_t elapsed() {
        return ((t2.tv_sec - t1.tv_sec) * 1000ULL * 1000ULL) + ((t2.tv_usec - t1. tv_usec));
    }
    uint64_t split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};


void maskfnc(vector<uint32_t> & out, const uint32_t L) {
    if (L == 32)
        return;
    for (auto i = out.begin(); i != out.end(); ++i) {
        *i = *i % (1U << L);
    }
}




static void __SIMD_fastpack1_32(const uint32_t *  __restrict__ _in, __m128i *  __restrict__  out) {
    const __m128i       *in = reinterpret_cast<const __m128i*>(_in);
    __m128i     OutReg;
      
    __m128i InReg = _mm_and_si128(_mm_load_si128(in), mask1);
    OutReg = InReg; 
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 1));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 2));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 3));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 4));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 5));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 6));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 7));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 8));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 9));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 10));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 11));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 12));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 13));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 14));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 15));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 16));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 17));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 18));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 19));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 20));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 21));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 22));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 23));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 24));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 25));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 26));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 27));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 28));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 29));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 30));
    InReg = _mm_and_si128(_mm_load_si128(++in), mask1);

    OutReg =  _mm_or_si128(OutReg,_mm_slli_epi32(InReg, 31));
    _mm_store_si128(out, OutReg);


}


void simdpack(const vector<uint32_t> & data,
        vector<uint32_t> & out) {
    const uint32_t N = data.size();
    for (uint32_t k = 0; k < N / 128; ++k) {
        __SIMD_fastpack1_32(&data[0] + 128 * k,
                reinterpret_cast<__m128i *> (&out[0] + 4 * 1 * k));
    }
}


static void unpackA(const  __m128i*  __restrict__ in, uint32_t *  __restrict__  _out) {
      
    __m128i*   out = reinterpret_cast<__m128i*>(_out);
    __m128i    InReg = _mm_load_si128(in);
    __m128i    OutReg;    
    
    OutReg = _mm_and_si128( InReg , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,1) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,2) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,3) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,4) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,5) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,6) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,7) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,8) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,9) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,10) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,11) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,12) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,13) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,14) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,15) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,16) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,17) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,18) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,19) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,20) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,21) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,22) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,23) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,24) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,25) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,26) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,27) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,28) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,29) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg = _mm_and_si128(  _mm_srli_epi32(InReg,30) , mask1);
    _mm_store_si128(out++, OutReg);

    OutReg =   _mm_srli_epi32(InReg,31) ;
    _mm_store_si128(out++, OutReg);


}



void unpackB(const  __m128i*  __restrict__ in, uint32_t *  __restrict__  _out) 
 {
      
    __m128i*   out = reinterpret_cast<__m128i*>(_out);
    __m128i    InReg1 = _mm_load_si128(in);
    __m128i    OutReg1, OutReg2;    

    OutReg1 = _mm_and_si128( InReg1 , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,1) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,2) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,3) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,4) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,5) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,6) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,7) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,8) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,9) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,10) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,11) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,12) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,13) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,14) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,15) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,16) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,17) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,18) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,19) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,20) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,21) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,22) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,23) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,24) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,25) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,26) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,27) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,28) , mask1);
    OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,29) , mask1);
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);

    OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,30) , mask1);
    OutReg2 =   _mm_srli_epi32(InReg1,31) ;
    _mm_store_si128(out++, OutReg1);
    _mm_store_si128(out++, OutReg2);
}

static void unpackC(const  __m128i*  __restrict__ in, uint32_t *  __restrict__  _out) {
      
    __m128i*   out = reinterpret_cast<__m128i*>(_out);
    __m128i    InReg1 = _mm_load_si128(in);
    __m128i    OutReg1, OutReg2;    
    
    unsigned shift = 0;

	for (unsigned i = 0; i < 32; ++i) {
    	OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,shift++) , mask1);
    	_mm_store_si128(out++, OutReg1);
	}

}
static void unpackD(const  __m128i*  __restrict__ in, uint32_t *  __restrict__  _out) {
    __m128i*   out = reinterpret_cast<__m128i*>(_out);
    __m128i    InReg1 = _mm_load_si128(in);
    __m128i    OutReg1, OutReg2;    

    unsigned shift = 0;

	for (unsigned i = 0; i < 16; ++i) {
    	OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,shift) , mask1);
    	OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg1,shift+1) , mask1);
    	_mm_store_si128(out++, OutReg1);
    	_mm_store_si128(out++, OutReg2);
    	shift += 2;
	}
}



static void unpackE(const  __m128i*  __restrict__ in, uint32_t *  __restrict__  _out) {
    __m128i*   out = reinterpret_cast<__m128i*>(_out);
    __m128i    InReg1 = _mm_load_si128(in);
    __m128i    InReg2 = InReg1;
    __m128i    OutReg1, OutReg2, OutReg3, OutReg4;    

    unsigned shift = 0;

    for (unsigned i = 0; i < 8; ++i) {
        OutReg1 = _mm_and_si128(  _mm_srli_epi32(InReg1,shift++) , mask1);
        OutReg2 = _mm_and_si128(  _mm_srli_epi32(InReg2,shift++) , mask1);
        OutReg3 = _mm_and_si128(  _mm_srli_epi32(InReg1,shift++) , mask1);
        OutReg4 = _mm_and_si128(  _mm_srli_epi32(InReg2,shift++) , mask1);
        _mm_store_si128(out++, OutReg1);
        _mm_store_si128(out++, OutReg2);
        _mm_store_si128(out++, OutReg3);
        _mm_store_si128(out++, OutReg4);
    }
}


void simdunpackA(const vector<uint32_t> & data,
        vector<uint32_t> & out) {
    const uint32_t N = out.size();
    for (uint32_t k = 0; k < N / 128; ++k) {
        unpackA(
                reinterpret_cast<const __m128i *> (&data[0] + 4 * 1 * k),
                &out[0] + 128 * k);
    }
}

void simdunpackB(const vector<uint32_t> & data,
        vector<uint32_t> & out) {
    const uint32_t N = out.size();
    for (uint32_t k = 0; k < N / 128; ++k) {
        unpackA(
                reinterpret_cast<const __m128i *> (&data[0] + 4 * 1 * k),
                &out[0] + 128 * k);
    }
}

void simdunpackC(const vector<uint32_t> & data,
        vector<uint32_t> & out) {
    const uint32_t N = out.size();
    for (uint32_t k = 0; k < N / 128; ++k) {
        unpackA(
                reinterpret_cast<const __m128i *> (&data[0] + 4 * 1 * k),
                &out[0] + 128 * k);
    }
}

void simdunpackD(const vector<uint32_t> & data,
        vector<uint32_t> & out) {
    const uint32_t N = out.size();
    for (uint32_t k = 0; k < N / 128; ++k) {
        unpackA(
                reinterpret_cast<const __m128i *> (&data[0] + 4 * 1 * k),
                &out[0] + 128 * k);
    }
}

void simdunpackE(const vector<uint32_t> & data,
        vector<uint32_t> & out) {
    const uint32_t N = out.size();
    for (uint32_t k = 0; k < N / 128; ++k) {
        unpackA(
                reinterpret_cast<const __m128i *> (&data[0] + 4 * 1 * k),
                &out[0] + 128 * k);
    }
}

vector<uint32_t> generateArray32(uint32_t N, const uint32_t mask = 0xFFFFFFFFU) {
    vector < uint32_t> ans(N);
    for (size_t k = 0; k < N; ++k)
        ans[k] = rand() & mask;
    return ans;
}


void simplebenchmark(uint32_t N = 1U << 16, uint32_t T = 1U << 9) {
    T = T + 1; // we have a warming up pass
    vector<uint32_t> data = generateArray32(N);
    vector<uint32_t> compressed(N, 0);
    vector<uint32_t> recovered(N, 0);
    WallClockTimer z;
    double unpacktimeA, unpacktimeB, unpacktimeC, unpacktimeD, unpacktimeE;

    cout << "#million of integers per second: higher is better" << endl;
    uint32_t bitindex = 31;
    {
        uint32_t bit = 32 - bitindex;
        maskfnc(data, 1);
        for (uint32_t repeat = 0; repeat < 1; ++repeat) {
            unpacktimeA = 0;
            unpacktimeB = 0;
            unpacktimeC = 0;
            unpacktimeD = 0;
            unpacktimeE = 0;
            
            for (uint32_t t = 0; t < T; ++t) {
                compressed.clear();
                compressed.resize(N * 1 / 32, 0);
                recovered.clear();
                recovered.resize(N, 0);
                simdpack(data, compressed);
                z.reset();
                simdunpackA(compressed, recovered);
                if (t > 0)
                    unpacktimeA += z.split();
                z.reset();
                simdunpackB(compressed, recovered);
                if (t > 0)
                    unpacktimeB += z.split();
                z.reset();
                simdunpackC(compressed, recovered);
                if (t > 0)
                    unpacktimeC += z.split();
                z.reset();
                simdunpackD(compressed, recovered);
                if (t > 0)
                    unpacktimeD += z.split();
                z.reset();
                simdunpackE(compressed, recovered);
                if (t > 0)
                    unpacktimeE += z.split();

            }

            cout << std::setprecision(4) << "\t\t" << N * (T - 1) / (unpacktimeA);
            cout << std::setprecision(4) << "\t\t" << N * (T - 1) / (unpacktimeB);
            cout << std::setprecision(4) << "\t\t" << N * (T - 1) / (unpacktimeC);
            cout << std::setprecision(4) << "\t\t" << N * (T - 1) / (unpacktimeD);
            cout << std::setprecision(4) << "\t\t" << N * (T - 1) / (unpacktimeE);

            cout << endl;
        }

    }

}



using namespace std;
int main() {
    cout<<"# cache-to-cache"<<endl; 
    simplebenchmark(1U << 16, 1U << 9);
    cout<<endl;
    cout<<"# ram-to-ram"<<endl;
    simplebenchmark(1U << 25, 1U << 0);

    return 0;
}
