/**
* D. Lemire, June 2014
*
*

$ g++  -o stepbysteppacking stepbysteppacking.cpp

$ ./stepbysteppacking
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


void simdpack_length(uint32_t initvalue,  const uint32_t *   in, size_t length, __m128i *    out, const uint32_t bit) {
    if(bit == 0) return;// nothing to do
    if(bit == 32) {
        memcpy(out,in,length*sizeof(uint32_t));
        return;
    }
    int inwordpointer = 0;
    __m128i P = _mm_setzero_si128();
    for(size_t k = 0; k < length / 4 ; ++k) {
        __m128i value = _mm_loadu_si128(((const __m128i * ) in + k));
        P = _mm_or_si128(P,_mm_slli_epi32(value, inwordpointer));
        const int firstpass = sizeof(uint32_t) * 8 - inwordpointer;
        if(bit<firstpass) {
            inwordpointer+=bit;
        } else {
            _mm_storeu_si128(out++, P);
            P = _mm_srli_epi32(value, firstpass);
            inwordpointer=bit-firstpass;
        }
    }
    if(length % 4 != 0) {
        uint32_t buffer[4];
        for(int k = 0; k < (length % 4); ++k) {
            buffer[k] = in[length/4*4+k];
        }
        for(int k = (length % 4); k < 4 ; ++k) {
            buffer[k] = 0;
        }
        __m128i value = _mm_loadu_si128((__m128i * ) buffer);
        P = _mm_or_si128(P,_mm_slli_epi32(value, inwordpointer));
        const int firstpass = sizeof(uint32_t) * 8 - inwordpointer;
        if(bit<firstpass) {
            inwordpointer+=bit;
        } else {
            _mm_storeu_si128(out++, P);
            P = _mm_srli_epi32(value, firstpass);
            inwordpointer=bit-firstpass;
        }
    }
    if(inwordpointer != 0) {
        _mm_storeu_si128(out++, P);
    }
}


void simdunpack_length(uint32_t initvalue, const __m128i *   in, size_t length, uint32_t * out, const uint32_t bit) {
    if(bit == 0) {
        for(size_t k = 0; k < length; ++k) {
            out[k] = initvalue;
        }
    }
    if(bit == 32) {
        memcpy(out,in,length*sizeof(uint32_t));
        return;
    }
    const __m128i offset = _mm_set1_epi32(initvalue);
    const __m128i maskbits = _mm_set1_epi32((1<<bit)-1);
    int inwordpointer = 0;
    __m128i P = _mm_sub_epi32(_mm_loadu_si128((__m128i * ) in));
    ++in;
    for(size_t k = 0; k < length  / 4; ++k) {
        __m128i answer = _mm_srli_epi32(P, inwordpointer);
        const int firstpass = sizeof(uint32_t) * 8 - inwordpointer;
        if( bit < firstpass) {
            inwordpointer += bit;
        } else {
            P = _mm_loadu_si128((__m128i * ) in);
            ++in;
            answer = _mm_or_si128(_mm_slli_epi32(P, firstpass),answer);
            inwordpointer = bit - firstpass;
        }
        answer = _mm_and_si128(maskbits,answer);
        _mm_storeu_si128((__m128i *)out, answer);
        out += 4;
    }
    if(length % 4 != 0) {
        __m128i answer = _mm_srli_epi32(P, inwordpointer);
        const int firstpass = sizeof(uint32_t) * 8 - inwordpointer;
        if( bit < firstpass) {
            inwordpointer += bit;
        } else {
            P = _mm_loadu_si128((__m128i * ) in);
            ++in;
            answer = _mm_or_si128(_mm_slli_epi32(P, firstpass),answer);
            inwordpointer = bit - firstpass;
        }
        uint32_t buffer[4];
        answer = _mm_and_si128(maskbits,answer);
        _mm_storeu_si128((__m128i *)buffer, answer);
        for(int k = 0; k < (length % 4); ++k) {
            *out = buffer[k];
            ++out;
        }
    }
}

void test(uint32_t bit) {
    assert(bit < 32);
    cout<<"bit = "<<bit<<endl;
    const size_t N = 128;
    vector<uint32_t> data(N);
    vector<uint32_t> backdata(N);
    vector<uint32_t> buffer(2*N + 1024);

    for(size_t i = 0 ; i < N; ++i) {
        data[i] = rand() & ((1<<bit)-1);
    }
    for(size_t length = 0 ; length <= N; ++length) {
        for(size_t i = 0 ; i < N; ++i) {
            backdata[i] = 0;
        }
        simdpack_length(0, data.data(), length, (__m128i *) buffer.data(), bit);
        simdunpack_length(0, (__m128i *) buffer.data(), length, backdata.data(), bit);
        for(size_t i = 0 ; i < length; ++i) {
            if(data[i] != backdata[i]) {
              cout<<"backdata["<<i<<"]="<<backdata[i]<<endl;
              cout<<"data["<<i<<"]="<<data[i]<<endl;  
            }
            assert(data[i] == backdata[i]);
        }
    }
}


int main() {
    for(uint32_t i = 0; i < 32; ++i) test(i);
}
