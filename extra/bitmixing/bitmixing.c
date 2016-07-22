/**
* We verify the quality of bit mixing functions.
* You must have a recent x64 processor.
*/

#include <x86intrin.h> // assumes recent x64 machine
#include <emmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <math.h>
#include <inttypes.h>

size_t length = 1000000;
uint32_t start = 0;
uint32_t gap = 16;

typedef uint32_t (*mixer32)(uint32_t);

/**
* See https://github.com/aappleby/smhasher/wiki/MurmurHash3
*/
uint32_t murmurhash_mixer(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

uint32_t identity(uint32_t h) {
    return h;
}

uint64_t CLMS_CONSTANT =  UINT64_C(9787182197298948716) ; //9786759984796133996);

uint32_t clmultiplyshift_mixer(uint32_t h) {
    return _mm_cvtsi128_si64(_mm_srli_si128(_mm_clmulepi64_si128(_mm_cvtsi64_si128(CLMS_CONSTANT),_mm_cvtsi64_si128(h),0x00),4));
}

uint64_t MS_CONSTANT = UINT64_C(16914929135595462722);//12303806032761757698);

uint32_t multiplyshift_mixer(uint32_t h) {
    return (MS_CONSTANT * h ) >> 32;
}


uint32_t crc32_mixer(uint32_t h) {
    uint32_t answer =  _mm_crc32_u32(0xFFFFFFFF,h);
    return answer;
}


typedef struct zobrist32_s {
    uint32_t hashtab[sizeof(uint32_t)][1 << 8];
} zobrist32_t;

void zobrist32_init(zobrist32_t *k) {
    for (uint32_t i = 0; i < sizeof(uint32_t); i++) {
        for (uint32_t j = 0; j < (1 << 8); j++) {
            _rdrand32_step( & k->hashtab[i][j]);
        }
    }
}

zobrist32_t k;

uint32_t zobrist32(uint32_t val) {
    uint32_t h = 0;
    const unsigned char *s = (const unsigned char *)&val;
    h ^= k.hashtab[0][s[0]];
    h ^= k.hashtab[1][s[1]];
    h ^= k.hashtab[2][s[2]];
    h ^= k.hashtab[3][s[3]];
    return h;
}


int hamming_diff(uint32_t h1, uint32_t h2) {
    return _mm_popcnt_u32(h1 ^ h2);
}


/**
* Given a bit mixing function f, this function computes the standard error of
* the hamming distance between successive values, compared to an expected mean of 16 (assuming 32-bit values)
*/
double stderr_arithmetic(mixer32 f, uint32_t start, uint32_t number, uint32_t gap) {
    int diffs[33];
    for(int k = 0; k <= 32 ; ++k) diffs[k] = 0;
    uint32_t end = start + number * gap;
    size_t counter = 0;
    for(uint32_t x = start ; x + gap <= end; x += gap) {
        for(int y = 1; y <= gap; ++y) {
          counter++;
          int d = hamming_diff(f(x),f(x+y));
          diffs[d]++;
        }
    }
    int width = 32;
    // expected value of (x-16)^2
    double  expect = 0;
    for(int k = 0; k <= width ; ++k) {
        expect += diffs[k] * (width/2 - k) * (width/2 - k);
    }
    double stderror = sqrt(expect / counter);
    return stderror;
}



bool clmultiplyshift_tuning() {
    uint64_t bestconstant = CLMS_CONSTANT;
    double bestv = stderr_arithmetic(clmultiplyshift_mixer,start,length,gap);
    printf("CLMS_CONSTANT = %" PRIu64 " gives me %f \n",CLMS_CONSTANT,bestv);
    for(int k = 0; k < 64; ++k) {
        uint64_t oldc = CLMS_CONSTANT;
        CLMS_CONSTANT |= (UINT64_C(1)<<k);
        double tv = stderr_arithmetic(clmultiplyshift_mixer,start,length,gap);
        if(bestv > tv ) {
            printf("%f is better than %f\n",tv,bestv);
            bestv = tv;
            bestconstant = CLMS_CONSTANT;
            printf("clmultiplyshift_tuning... found better constant %" PRIu64 "\n",CLMS_CONSTANT);
        }
        CLMS_CONSTANT = oldc;
    }
    for(int k = 0; k < 64; ++k) {
        uint64_t oldc = CLMS_CONSTANT;
        CLMS_CONSTANT &= ~(UINT64_C(1)<<k);
        double tv = stderr_arithmetic(clmultiplyshift_mixer,start,length,gap);
        if(bestv > tv ) {
            printf("%f is better than %f\n",tv,bestv);
            bestv = tv;
            bestconstant = CLMS_CONSTANT;
            printf("clmultiplyshift_tuning... found better constant %" PRIu64 "\n",CLMS_CONSTANT);
        }
        CLMS_CONSTANT = oldc;
    }
    for(int k = 0; k < 64; ++k) {
        CLMS_CONSTANT ^= (UINT64_C(1)<<k);
        for(int kk = 0; kk < 64; ++kk) {
            CLMS_CONSTANT ^= (UINT64_C(1)<<kk);
            double tv = stderr_arithmetic(clmultiplyshift_mixer,start,length,gap);
            if(bestv > tv ) {
                printf("%f is better than %f\n",tv,bestv);
                bestv = tv;
                bestconstant = CLMS_CONSTANT;
                printf("clmultiplyshift_tuning... found better constant %" PRIu64 "\n",CLMS_CONSTANT);
            }
            CLMS_CONSTANT ^= (UINT64_C(1)<<kk);
        }
        CLMS_CONSTANT ^= (UINT64_C(1)<<k);
    }
    if(CLMS_CONSTANT != bestconstant) {
        CLMS_CONSTANT = bestconstant;
        return true;
    }
    return false;
}

bool multiplyshift_tuning() {
    uint64_t bestconstant = MS_CONSTANT;
    double bestv = stderr_arithmetic(multiplyshift_mixer,start,length,gap);
    printf("MS_CONSTANT = %" PRIu64 " gives me %f \n",MS_CONSTANT,bestv);
    for(int k = 0; k < 64; ++k) {
        uint64_t oldc = MS_CONSTANT;
        MS_CONSTANT |= (UINT64_C(1)<<k);
        double tv = stderr_arithmetic(multiplyshift_mixer,start,length,gap);
        if(bestv > tv ) {
            printf("%f is better than %f\n",tv,bestv);
            bestv = tv;
            bestconstant = MS_CONSTANT;
            printf("multiplyshift_tuning... found better constant %" PRIu64 "\n",MS_CONSTANT);
        }
        MS_CONSTANT = oldc;
    }
    for(int k = 0; k < 64; ++k) {
        uint64_t oldc = MS_CONSTANT;
        MS_CONSTANT &= ~(UINT64_C(1)<<k);
        double tv = stderr_arithmetic(multiplyshift_mixer,start,length,gap);
        if(bestv > tv ) {
            printf("%f is better than %f\n",tv,bestv);
            bestv = tv;
            bestconstant = MS_CONSTANT;
            printf("multiplyshift_tuning... found better constant %" PRIu64 "\n",MS_CONSTANT);
        }
        MS_CONSTANT = oldc;
    }
    for(int k = 0; k < 64; ++k) {
        MS_CONSTANT ^= (UINT64_C(1)<<k);
        for(int kk = 0; kk < 64; ++kk) {
            MS_CONSTANT ^= (UINT64_C(1)<<kk);
            double tv = stderr_arithmetic(multiplyshift_mixer,start,length,gap);
            if(bestv > tv ) {
                printf("%f is better than %f\n",tv,bestv);
                bestv = tv;
                bestconstant = MS_CONSTANT;
                printf("multiplyshift_tuning...found better constant %" PRIu64 "\n",MS_CONSTANT);
            }
            MS_CONSTANT ^= (UINT64_C(1)<<kk);
        }
        MS_CONSTANT ^= (UINT64_C(1)<<k);
    }
    if(MS_CONSTANT != bestconstant) {
        MS_CONSTANT = bestconstant;
        return true;
    }
    return false;
}


void demo() {
    zobrist32_init(&k);
    if(false) {
      while(clmultiplyshift_tuning()) {};
        while(multiplyshift_tuning()) {};
    }
    printf("MS_CONSTANT = %" PRIu64 "\n",MS_CONSTANT);
    printf("CLMS_CONSTANT = %" PRIu64 "\n",CLMS_CONSTANT);

    for(int g = 1; g <= gap; ++g) {
        printf("gap = %5d id= %f murmur = %f crc = %f zobrist = %f clms = %f ms = %f \n",
               g,

               stderr_arithmetic(identity,start,length,g),
               stderr_arithmetic(murmurhash_mixer,start,length,g),
               stderr_arithmetic(crc32_mixer,start,length,g),
               stderr_arithmetic(zobrist32,start,length,g),
               stderr_arithmetic(clmultiplyshift_mixer,start,length,g),
               stderr_arithmetic(multiplyshift_mixer,start,length,g));
    }
}

int main() {
    demo();
}
