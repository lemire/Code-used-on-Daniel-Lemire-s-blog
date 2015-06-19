/**
* D. Lemire, June 2015
*
* We insert two bits in a stream.

$ g++  -o shifting shifting.cpp

$ ./shifting
*/




#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <stdexcept>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>

using namespace std;


// the naive approach is used as a reference
//
// length is the number of 2-bits.
// we assume that bitstoinsert is an integer in [0,4)
void naiveinsert(uint8_t * data, size_t length, size_t insert_pos, int bitstoinsert) {
    size_t bytelocation = insert_pos / 4; // 4 2-bit per byte
    int bitlocation = (2 * insert_pos) % 8;
    uint8_t origbyte = data[bytelocation];
    uint8_t mask = ((1<<bitlocation)-1);
    uint8_t newbyte = (origbyte & mask) | (bitstoinsert << bitlocation) | ((origbyte & ~mask)<<2);
    data[bytelocation] = newbyte;
    origbyte >>= 6;
    for(size_t k = bytelocation + 1 ; k < length / 4  ; ++k) {
        newbyte = (data[k] << 2) | origbyte;
        origbyte = data[k] >> 6;
        data[k] = newbyte;
    }
    if(bytelocation < length/4) {
        if(length % 4 == 0) {
            data[length/4] = origbyte;
        } else {
            data[length/4] = (data[length/4] << 2) | origbyte;
        }
    }
}



// a less-naive approach
//
// length is the number of 2-bits.
// we assume that bitstoinsert is an integer in [0,4)
void scalarinsert(uint8_t * data8, size_t length, size_t insert_pos, int bitstoinsert) {
    uint64_t * data = (uint64_t *) data8;
    size_t wordlocation = insert_pos / 32; // 32 2-bit per byte
    int bitlocation = (2 * insert_pos) % 64;
    uint64_t origword = data[wordlocation];
    uint64_t mask = ((((uint64_t)1)<<bitlocation)-1);
    uint64_t newword = (origword & mask) | (((uint64_t)bitstoinsert) << bitlocation) | ((origword & ~mask)<<2);
    data[wordlocation] = newword;
    origword >>= 62;
    for(size_t k = wordlocation + 1 ; k < (length / 32) ; ++k) {
        newword = (data[k] << 2) | origword;
        origword = data[k] >> 62;
        data[k] = newword;
    }

    if(wordlocation < length / 32) {
        if(length % 32 == 0) {
            data[length/32] = origword;
        } else {
            data[length/32] = origword | (data[length/32] << 2);
        }
    }
}

// a less-naive approach
//
// length is the number of 2-bits.
// we assume that bitstoinsert is an integer in [0,4)
void simdinsert(uint8_t * data8, size_t length, size_t insert_pos, int bitstoinsert) {
    uint64_t * data = (uint64_t *) data8;
    size_t wordlocation = insert_pos / 32; // 32 2-bit per byte
    int bitlocation = (2 * insert_pos) % 64;
    uint64_t origword = data[wordlocation];
    uint64_t mask = ((((uint64_t)1)<<bitlocation)-1);
    uint64_t newword = (origword & mask) | (((uint64_t)bitstoinsert) << bitlocation) | ((origword & ~mask)<<2);
    size_t k = wordlocation + 1;
    if(k+2 < length / 32) {
        __m128i dold =  _mm_srli_epi64(_mm_loadu_si128((__m128i *) (data + wordlocation)),62);
        data[wordlocation] = newword;
        for( ; k + 2 < (length / 32) ; k+=2) {
            __m128i d = _mm_loadu_si128((__m128i *) (data + k));
            __m128i dnext = _mm_loadu_si128((__m128i *) (data + k + 1));

            _mm_storeu_si128((__m128i *) (data + k),_mm_or_si128(dold,_mm_slli_epi64(d,2)));
            dold = _mm_srli_epi64(dnext,62);
        }
        origword = _mm_cvtsi128_si64(dold);
    } else {
        data[wordlocation] = newword;
        origword >>= 62;
    }
    for(; k < (length / 32) ; ++k) {
        newword = (data[k] << 2) | origword;
        origword = data[k] >> 62;
        data[k] = newword;
    }

    if(wordlocation < length / 32) {
        if(length % 32 == 0) {
            data[length/32] = origword;
        } else {
            data[length/32] = origword | (data[length/32] << 2);
        }
    }
}

void enumerate(vector<uint8_t> vec,size_t l) {
    for(size_t i = 0; i < l ; ++i) {
        cout <<"["<<i<<"] "<< (vec[i] & 3) << " "  << ( ( vec[i] >> 2 ) & 3) << " "  << ( ( vec[i] >> 4 ) & 3) << " "  << ( ( vec[i] >> 6 ) & 3) << "--";
    }
    cout<<endl;
}


int main() {
    size_t Max = 1024 * 32;
    vector<uint8_t> repos1(Max); // plenty of memory
    vector<uint8_t> repos2(Max); // plenty of memory
    vector<uint8_t> repos3(Max); // plenty of memory

    const size_t N = 2048;
    vector<uint32_t> a(N);
    vector<uint32_t> abits(N);

    for(size_t i = 0; i < N; ++i) {
        a[i] = rand() % (i + 1);
        abits[i] = 1+(rand() % 3);
    }
    int T = 100;
    const clock_t S1 = clock();

    for(int t = 0; t < T; ++t) for(int T=0; T<N; ++T) {
        naiveinsert(repos1.data(), T ,a[T], abits[T]);
    }
    const clock_t S2 = clock();
    for(int t = 0; t < T; ++t) for(int T=0; T<N; ++T) {
        scalarinsert(repos2.data(), T ,a[T], abits[T]);
    }
    const clock_t S3 = clock();
    for(int t = 0; t < T; ++t) for(int T=0; T<N; ++T) {
        simdinsert(repos3.data(), T ,a[T], abits[T]);
    }
    const clock_t S4 = clock();
    repos1.resize((N+7)*2/8);
    repos2.resize((N+7)*2/8);
    repos3.resize((N+7)*2/8);
    cout<<"Naive time="<<(double)(S2-S1)/ CLOCKS_PER_SEC<<endl;
    cout<<"scalar time="<<(double)(S3-S2)/ CLOCKS_PER_SEC<<endl;
    cout<<"simd time="<<(double)(S4-S3)/ CLOCKS_PER_SEC<<endl;
    if(repos1 != repos2) throw runtime_error("bug");
    if(repos1 != repos3) throw runtime_error("bug");

}
