// gcc -std=c99 -mavx2 -msse4.2 -O2 -o card card.c
// icc -std=c99 -march=corei7 -O2 -o card card.c
// For IACA:
//$ gcc -std=c99 -march=corei7 -DIACA -O2 -o card card.c
//$ /opt/intel/iaca/bin/iaca -mark 2 -64 card
//$ /opt/intel/iaca/bin/iaca -mark 1 -64 card


#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>


//#define IACA
#ifdef IACA
#include </opt/intel/iaca/include/iacaMarks.h>
#else
#define IACA_START
#define IACA_END
#endif

void bitwiseor(uint64_t * input1, uint64_t * input2, uint64_t * output, size_t length) {
    for(int k = 0; k < length; ++k) {
        IACA_START;
        output[k] = input1[k] | input2[k];
        IACA_END;
    }
}


int card(uint64_t * input, size_t length) {
    int card = 0;
    for(int k = 0; k < length; k+=4) {
        IACA_START;
        card += _mm_popcnt_u64(input[k]);
        card += _mm_popcnt_u64(input[k+1]);
        card += _mm_popcnt_u64(input[k+2]);
        card += _mm_popcnt_u64(input[k+3]);
        IACA_END;
    }
    return card;
}




int AVXcard(uint64_t * input, size_t length) {
    // these are precomputed hamming weights (weight(0), weight(1)...)
    const __m256i shuf = _mm256_setr_epi8(0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
       0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4);
    const __m256i  mask = _mm256_set1_epi8(0x0f); // low 4 bits of each byte
    __m256i total = _mm256_setzero_si256();
    __m256i zero = _mm256_setzero_si256();
    for(int k = 0; k < length; k+=4) {
        IACA_START;
        __m256i ymm1 = _mm256_lddqu_si256((const __m256i *)(input + k));
        __m256i ymm2 = _mm256_srli_epi32(ymm1,4); // shift right, shiftingin zeroes
        ymm1 = _mm256_and_si256(ymm1,mask); // contains even 4 bits
        ymm2 = _mm256_and_si256(ymm2,mask); // contains odd 4 bits
        ymm1 = _mm256_shuffle_epi8(shuf,ymm1);// use table look-up to sum the 4 bits
        ymm2 = _mm256_shuffle_epi8(shuf,ymm2);
        ymm1 = _mm256_add_epi8(ymm1,ymm2);// each byte contains weight of input byte (values are in [0,8))
        ymm1 = _mm256_sad_epu8(zero,ymm1);// produces 4 64-bit counters (having values in [0,32))
        total= _mm256_add_epi64(total,ymm1); // add the 4 64-bit counters to previous counter
        IACA_END;
    }
    return _mm256_extract_epi64(total,0)+_mm256_extract_epi64(total,1)+_mm256_extract_epi64(total,2)+_mm256_extract_epi64(total,3);
}

uint64_t intrinsics_count4(uint64_t * bitset, size_t length) {
    // these are precomputed hamming weights (weight(0), weight(1)...)
    const __m256i shuf = _mm256_setr_epi8(0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
                                          0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4);
    const __m256i  mask = _mm256_set1_epi8(0x0f); // low 4 bits of each byte
    __m256i total = _mm256_setzero_si256();
    __m256i zero = _mm256_setzero_si256();
    const int inner = 4;
    for(unsigned  k = 0; k < length*sizeof(uint64_t)/(sizeof(__m256i)*inner); k++) {
        __m256i innertotal = _mm256_setzero_si256();
        {
            __m256i ymm1 = _mm256_lddqu_si256((const __m256i *)bitset + k*inner + 0);
            __m256i ymm2 = _mm256_srli_epi32(ymm1,4); // shift right, shiftingin zeroes
            ymm1 = _mm256_and_si256(ymm1,mask); // contains even 4 bits
            ymm2 = _mm256_and_si256(ymm2,mask); // contains odd 4 bits
            ymm1 = _mm256_shuffle_epi8(shuf,ymm1);// use table look-up to sum the 4 bits
            ymm2 = _mm256_shuffle_epi8(shuf,ymm2);
            innertotal = _mm256_add_epi8(innertotal,ymm1);// inner total values in each byte are bounded by 8 * inner
            innertotal = _mm256_add_epi8(innertotal,ymm2);// inner total values in each byte are bounded by 8 * inner
        }
        {
            __m256i ymm1 = _mm256_lddqu_si256((const __m256i *)bitset + k*inner + 1);
            __m256i ymm2 = _mm256_srli_epi32(ymm1,4); // shift right, shiftingin zeroes
            ymm1 = _mm256_and_si256(ymm1,mask); // contains even 4 bits
            ymm2 = _mm256_and_si256(ymm2,mask); // contains odd 4 bits
            ymm1 = _mm256_shuffle_epi8(shuf,ymm1);// use table look-up to sum the 4 bits
            ymm2 = _mm256_shuffle_epi8(shuf,ymm2);
            innertotal = _mm256_add_epi8(innertotal,ymm1);// inner total values in each byte are bounded by 8 * inner
            innertotal = _mm256_add_epi8(innertotal,ymm2);// inner total values in each byte are bounded by 8 * inner
        }
        {
            __m256i ymm1 = _mm256_lddqu_si256((const __m256i *)bitset + k*inner + 2);
            __m256i ymm2 = _mm256_srli_epi32(ymm1,4); // shift right, shiftingin zeroes
            ymm1 = _mm256_and_si256(ymm1,mask); // contains even 4 bits
            ymm2 = _mm256_and_si256(ymm2,mask); // contains odd 4 bits
            ymm1 = _mm256_shuffle_epi8(shuf,ymm1);// use table look-up to sum the 4 bits
            ymm2 = _mm256_shuffle_epi8(shuf,ymm2);
            innertotal = _mm256_add_epi8(innertotal,ymm1);// inner total values in each byte are bounded by 8 * inner
            innertotal = _mm256_add_epi8(innertotal,ymm2);// inner total values in each byte are bounded by 8 * inner
        }
        {
            __m256i ymm1 = _mm256_lddqu_si256((const __m256i *)bitset + k*inner + 3);
            __m256i ymm2 = _mm256_srli_epi32(ymm1,4); // shift right, shiftingin zeroes
            ymm1 = _mm256_and_si256(ymm1,mask); // contains even 4 bits
            ymm2 = _mm256_and_si256(ymm2,mask); // contains odd 4 bits
            ymm1 = _mm256_shuffle_epi8(shuf,ymm1);// use table look-up to sum the 4 bits
            ymm2 = _mm256_shuffle_epi8(shuf,ymm2);
            innertotal = _mm256_add_epi8(innertotal,ymm1);// inner total values in each byte are bounded by 8 * inner
            innertotal = _mm256_add_epi8(innertotal,ymm2);// inner total values in each byte are bounded by 8 * inner
        }
        innertotal = _mm256_sad_epu8(zero,innertotal);// produces 4 64-bit counters (having values in [0,8 * inner * 4])
        total= _mm256_add_epi64(total,innertotal); // add the 4 64-bit counters to previous counter
    }
    return _mm256_extract_epi64(total,0)+_mm256_extract_epi64(total,1)+_mm256_extract_epi64(total,2)+_mm256_extract_epi64(total,3);
}

int bitwiseorcard(uint64_t * input1, uint64_t * input2, uint64_t * output, size_t length) {
    int card = 0;
    for(int k = 0; k < length; ++k) {
        IACA_START;
        output[k] = input1[k] | input2[k];
        card += _mm_popcnt_u64(output[k]);
        IACA_END;
    }
    return card;
}



int main() {
    const int N = 4096;
    uint64_t * input1 = (uint64_t *) malloc(N*sizeof(uint64_t));
    uint64_t * input2 = (uint64_t *) malloc(N*sizeof(uint64_t));
    uint64_t * output = (uint64_t *) malloc(N*sizeof(uint64_t));
    for(int k = 0; k<N; ++k) {
        input1[k] = k * k;
        input2[k] = k * k * k + 3;
    }
    uint64_t bogus = 0;
    int repeat = 2000;
    if( card(input2,N)!=AVXcard(input2,N)) printf("bug");
    if( card(input1,N)!=AVXcard(input1,N)) printf("bug");
    if( card(input2,N)!=intrinsics_count4(input2,N)) printf("bug");
    if( card(input1,N)!=intrinsics_count4(input1,N)) printf("bug");
     for(int k = 0; k<15; ++k) {
        const clock_t S1 = clock();
        for(int z = 0; z < repeat; ++z) {
            bitwiseor(input1,input2,output,N);
            bogus += output[10];
            input1[10] = z;
        }
        const clock_t S2 = clock();
        for(int z = 0; z < repeat; ++z) {
            bogus += bitwiseorcard(input1,input2,output,N);
            input1[10] = z;
        }
        const clock_t S3 = clock();
        for(int z = 0; z < repeat; ++z) {
            bogus += card(output,N);
            input1[10] = z;
        }
        const clock_t S4 = clock();
        for(int z = 0; z < repeat; ++z) {
            bogus += AVXcard(output,N);
            input1[10] = z;
        }
        const clock_t S5 = clock();
        for(int z = 0; z < repeat; ++z) {
            bogus += intrinsics_count4(output,N);
            input1[10] = z;
        }
        const clock_t S6 = clock();


        if(k>3) printf("%lu %lu  %lu  %lu  %llu \n", (S2-S1), (S3-S2), (S4-S3), (S5-S4), (S6-S5), bogus);
    }

    free(input1);
    free(input2);
    free(output);
}
