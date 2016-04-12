//g++ -std=c++11 -mavx2 -mbmi2 -msse4.1   -O2 -o ped ped.cpp
#include <x86intrin.h>
#include <bmi2intrin.h>
#include <smmintrin.h>
#include <cstdint>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>


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
    int elapsed() {
        return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000);
    }
    int split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};




#include <iostream>

using namespace std;


//#define IACA
#ifdef IACA
#include </opt/intel/iaca/include/iacaMarks.h>
#else
#define IACA_START
#define IACA_END
#endif



// we can pack 32 integers using 2 bits each in a 64-bit word
void bmidecode2(__int64_t w, uint32_t * output) {
    IACA_START;// says that the the throughput is 8 cycles or 1/4 of a cycle per integer

    __int64_t y;
    __m128i x;
    y = _pdep_u64(w, 0x0303030303030303); // decode 64/8=8 integers
    x =  _mm_cvtepu8_epi32(
         _mm_loadl_epi64((const __m128i *) & y ) );
    _mm_storeu_si128((__m128i*) output, x);
    y = _pdep_u64(w >> 16, 0x0303030303030303); // decode 64/8=8 integers
    x =  _mm_cvtepu8_epi32(
         _mm_loadl_epi64((const __m128i *) & y ) );
    _mm_storeu_si128((__m128i*) output + 1, x);
    y = _pdep_u64(w >> 32, 0x0303030303030303); // decode 64/8=8 integers
    x =  _mm_cvtepu8_epi32(
         _mm_loadl_epi64((const __m128i *) & y ) );
    _mm_storeu_si128((__m128i*) output + 2, x);
    y = _pdep_u64(w >> 48, 0x0303030303030303); // decode 64/8=8 integers
    x =  _mm_cvtepu8_epi32(
         _mm_loadl_epi64((const __m128i *) & y ) );
    _mm_storeu_si128((__m128i*) output + 3, x);
    IACA_END;
}


// we can pack 32 integers using 2 bits each in a 64-bit word
void bmidecode2avx(__int64_t w, uint32_t * output) {
    IACA_START;// says that the the throughput is 8 cycles or 1/4 of a cycle per integer

    __int64_t y[2];// hope for 128 alignment
    __m256i x;
    y[0] = _pdep_u64(w, 0x0303030303030303); // decode 64/8=8 integers
    y[1] = _pdep_u64(w >> 16, 0x0303030303030303); // decode 64/8=8 integers

    x =  _mm256_cvtepu8_epi32(_mm_load_si128(((const __m128i *) & y)));
    _mm256_storeu_si256((__m256i *) output, x);

    y[0] = _pdep_u64(w >> 32, 0x0303030303030303); // decode 64/8=8 integers
    y[1] = _pdep_u64(w >> 48, 0x0303030303030303); // decode 64/8=8 integers

    x =  _mm256_cvtepu8_epi32(_mm_load_si128(((const __m128i *) & y)));
    _mm256_storeu_si256((__m256i *) output + 1, x);

    IACA_END;
}

// we can pack 32 integers using 2 bits each in a 64-bit word
void bmidecode2alt(__int64_t w, uint32_t * output) {
    IACA_START;// says that the the throughput is 8 cycles or 1/4 of a cycle per integer

    __int64_t y[2];
    __m128i x, z;
    y[0] = _pdep_u64(w, 0x0303030303030303); // decode 64/8=8 integers
    y[1] = _pdep_u64(w >> 16, 0x0303030303030303); // decode 64/8=8 integers

    x =  _mm_load_si128((const __m128i *) & y );
    z = _mm_cvtepu8_epi32(x );
    _mm_storeu_si128((__m128i*) output, z);
    z = _mm_cvtepu8_epi32(_mm_srli_si128(x,8) );
    _mm_storeu_si128((__m128i*) output + 1, z);

    y[0] = _pdep_u64(w >> 32, 0x0303030303030303); // decode 64/8=8 integers
    y[1] = _pdep_u64(w >> 48, 0x0303030303030303); // decode 64/8=8 integers

    x =  _mm_load_si128((const __m128i *) & y );
    z = _mm_cvtepu8_epi32(x );
    _mm_storeu_si128((__m128i*) output + 2, z);
    z = _mm_cvtepu8_epi32(_mm_srli_si128(x,8) );
    _mm_storeu_si128((__m128i*) output + 3, z);

    IACA_END;
}

// we can pack 32 integers using 2 bits each in a 64-bit word
void scalardecode2(__int64_t w, uint32_t * output) {
    IACA_START;
    for(int k = 0; k < 32; ++k)
      output[k] = (w >> (k*2)) & 3;
    IACA_END;
}
// we can pack 32 integers using 2 bits each in a 64-bit word
void scalardecodeunrolled2(__int64_t w, uint32_t * output) {
    IACA_START; // says that this has a throughput of 32 cycles or 1 cycle per integer

    output[0] = w & 3;
    output[1] = (w >> (1*2)) & 3;
    output[2] = (w >> (2*2)) & 3;
    output[3] = (w >> (3*2)) & 3;
    output[4] = (w >> (4*2)) & 3;
    output[5] = (w >> (5*2)) & 3;
    output[6] = (w >> (6*2)) & 3;
    output[7] = (w >> (7*2)) & 3;
    output[8] = (w >> (8*2)) & 3;
    output[9] = (w >> (9*2)) & 3;
    output[10] = (w >> (10*2)) & 3;
    output[11] = (w >> (11*2)) & 3;
    output[12] = (w >> (12*2)) & 3;
    output[13] = (w >> (13*2)) & 3;
    output[14] = (w >> (14*2)) & 3;
    output[15] = (w >> (15*2)) & 3;
    output[16] = (w >> (16*2)) & 3;
    output[17] = (w >> (17*2)) & 3;
    output[18] = (w >> (18*2)) & 3;
    output[19] = (w >> (19*2)) & 3;
    output[20] = (w >> (20*2)) & 3;
    output[21] = (w >> (21*2)) & 3;
    output[22] = (w >> (22*2)) & 3;
    output[23] = (w >> (23*2)) & 3;
    output[24] = (w >> (24*2)) & 3;
    output[25] = (w >> (25*2)) & 3;
    output[26] = (w >> (26*2)) & 3;
    output[27] = (w >> (27*2)) & 3;
    output[28] = (w >> (28*2)) & 3;
    output[29] = (w >> (29*2)) & 3;
    output[30] = (w >> (30*2)) & 3;
    output[31] = (w >> (31*2)) & 3;
    IACA_END;

}

// we can pack 32 integers using 2 bits each in a 64-bit word
uint64_t  scalarencode2(uint32_t * output) {
    uint64_t w = 0;
    for(int k = 0; k < 32; ++k)
      w |= ((uint64_t) output[k] << (k*2));
    return w;
}


int main() {
    WallClockTimer time;
    int N = 32*1024;
    int REPEAT = 10000;
    uint32_t * data = (uint32_t *) malloc(N * sizeof(uint32_t));
    for(int k = 0; k < N; ++k) {
        data[k] = k & 3;
    }
    uint64_t * codeddata = (uint64_t *) malloc(N * sizeof(uint32_t) / 16);
    for(int k = 0; k < N; k += 32) {
        codeddata[k/32] = scalarencode2(data + k);
    }
    uint32_t * outdata = (uint32_t *) malloc(N * sizeof(uint32_t));
    time.reset();
    for(int T = 0; T < REPEAT; ++T)
    for(int k = 0; k < N; k += 32) {
        scalardecode2(codeddata[k/32],outdata+k);
    }
    cout<<"scalar speed "<<N*REPEAT/(1000.0*time.split())<<endl;


    for(int k = 0; k < N; ++k) {
        if(data[k] != outdata[k]) {
            cout << " bug  " <<endl;
            return -1;
        }
    }
    time.reset();
    for(int T = 0; T < REPEAT; ++T)
    for(int k = 0; k < N; k += 32) {
        scalardecodeunrolled2(codeddata[k/32],outdata+k);
    }
    cout<<"unrolled scalar speed "<<N*REPEAT/(1000.0*time.split())<<endl;


    for(int k = 0; k < N; ++k) {
        if(data[k] != outdata[k]) {
            cout << " bug  " <<endl;
            return -1;
        }
    }
    time.reset();

    for(int T = 0; T < REPEAT; ++T)
    for(int k = 0; k < N; k += 32) {
        bmidecode2(codeddata[k/32],outdata+k);
    }
    cout<<"bmi2 speed "<<N*REPEAT/(1000.0*time.split())<<endl;

    for(int k = 0; k < N; ++k) {
        if(data[k] != outdata[k]) {
            cout << " bug  " <<endl;
            return -1;
        }
    }

    time.reset();

    for(int T = 0; T < REPEAT; ++T)
    for(int k = 0; k < N; k += 32) {
        bmidecode2avx(codeddata[k/32],outdata+k);
    }
    cout<<"bmi2avx speed "<<N*REPEAT/(1000.0*time.split())<<endl;

    for(int k = 0; k < N; ++k) {
        if(data[k] != outdata[k]) {
            cout << " bug  " <<endl;
            return -1;
        }
    }

    for(int T = 0; T < REPEAT; ++T)
    for(int k = 0; k < N; k += 32) {
        bmidecode2alt(codeddata[k/32],outdata+k);
    }
    cout<<"bmi2alt speed "<<N*REPEAT/(1000.0*time.split())<<endl;

    for(int k = 0; k < N; ++k) {
        if(data[k] != outdata[k]) {
            cout << " bug  " <<endl;
            return -1;
        }
    }


    return 0;
}
