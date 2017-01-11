#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <x86intrin.h>
#include <string.h>
#include <math.h>

/**
* Idea:
* take a 256-bit input word, flip a single bit. Compute the Hamming
* difference between them. If the avalanche effect holds, then, on average
* the Hamming weight should be 128.
*/


typedef __m256i (*mixer256)(__m256i);


/****
* Begin Zobrist
****/

/**
* As a reference, let us consider Zobrist hashing
* which should be very close to optimal in bit mixing
* quality
*/
typedef struct zobrist256_s {
    __m256i hashtab[sizeof(__m256i)][1 << 8];
} zobrist256_t;

static void zobrist256_init(zobrist256_t *k) {
    uint64_t rand1, rand2, rand3, rand4;
    for (uint32_t i = 0; i < sizeof(__m256i); i++) {
        for (uint32_t j = 0; j < (1 << 8); j++) {
          _rdrand64_step(&rand1);
          _rdrand64_step(&rand2);
          _rdrand64_step(&rand3);
          _rdrand64_step(&rand4);
          k->hashtab[i][j] = _mm256_set_epi64x(rand1,rand2,rand3,rand4);
        }
    }
}

zobrist256_t zobrist256_global;

__m256i zobrist256(__m256i val) {
    // this will not be fast, don't use this in the real world!
    uint8_t buffer[sizeof(__m256i)];
    _mm256_storeu_si256((__m256i*)buffer,val);
    __m256i answer = _mm256_xor_si256(zobrist256_global.hashtab[0][buffer[0]], zobrist256_global.hashtab[1][buffer[1]]);
    for(int i = 2; i < 32; ++i)
      answer = _mm256_xor_si256(answer, zobrist256_global.hashtab[i][buffer[i]]);
    return answer;
}

/****
* End Zobrist
****/




/**
* Experimental mixer (Pure random garbage)
*/
void print(__m256i val) {
      const int numberof32 = sizeof(__m256i) / sizeof(uint32_t);
      uint32_t buffer[numberof32];
          _mm256_storeu_si256((__m256i*)buffer,val);
    for( int k = 0; k < numberof32; k++)
      printf("%x ", buffer[k]);
    printf("\n");
}

inline __m256i reverse(__m256i x){
    x = _mm256_permute2f128_si256(x,x,1);
    x = _mm256_permute_pd(x,5);
    return x;
}

__m256i threeshift(__m256i val) {
  // first mixing step

  val = _mm256_xor_si256(val,
    _mm256_alignr_epi8(_mm256_setzero_si256(),val,sizeof(__m256i)/2));
//  val = _mm256_xor_si256(val,
  //  _mm256_alignr_epi8(val,_mm256_setzero_si256(),sizeof(__m256i)/2));


  // second mixing step
  val = _mm256_xor_si256(val,
    _mm256_srli_si256(val,sizeof(__m256i)/4));
  val = _mm256_xor_si256(val,
    _mm256_slli_si256(val,sizeof(__m256i)/4));


  // third mixing
  val = _mm256_xor_si256(val,
    _mm256_srli_epi64(val,32));

  return val;
}


__m256i expmixer256(__m256i val) {
  __m256i multipliers1 = _mm256_set_epi32(0x5d2cf611 ,
0xac4352d5 ,
0x730d9b21 ,
0x2772221b ,
0x4d0fb039 ,
0x703b66c9 ,
0x79ebb5f9 ,
0xed12447b );//
  __m256i multipliers2 = _mm256_set_epi32(0xeaa617dd ,
0xcd651e17 ,
0xc7e5408f ,
0xec81805 ,
0x85a2092d ,
0xedf287ab ,
0xe84f989f ,
0xf2021371 );//
__m256i multipliers3 = _mm256_set_epi32(0xc07634f ,
0xef66d0d3 ,
0xcbe3f1db ,
0xbaab54e3 ,
0x458febd9 ,
0x4a9235af ,
0x757d1a9b ,
0xac1f668d);
printf("orig val : "); print(val);
  val = threeshift(val);
printf("threeshift val : "); print(val);

  val = _mm256_mul_epu32(val,multipliers1);
  val = reverse(val);
  val = threeshift(val);
  val = _mm256_mul_epu32(val,multipliers2);
  val = reverse(val);
  val = threeshift(val);
  val = _mm256_mul_epu32(val,multipliers3);
  val = reverse(val);
  val = threeshift(val);

  return val;
}


/**
* End of experimental mixer
*/

static int hamming_diff(__m256i h1, __m256i h2) {
    __m256i diff = _mm256_xor_si256(h1,h2);
    uint64_t buffer[sizeof(__m256i) / sizeof(uint64_t)];
    _mm256_storeu_si256((__m256i*)buffer,diff);
    return _mm_popcnt_u64(buffer[0]) + _mm_popcnt_u64(buffer[1])
             + _mm_popcnt_u64(buffer[2]) + _mm_popcnt_u64(buffer[3]);
}

// mean sqr avg
double avg_hamming(mixer256 f, uint32_t start, uint32_t number, uint32_t wordindex, bool verbose) {
    const int numberof32 = sizeof(__m256i) / sizeof(uint32_t);
    uint32_t buffer[numberof32];
    memset(buffer,0,sizeof(uint32_t) * numberof32);
    __m256i init = _mm256_loadu_si256((__m256i *) buffer);
    const __m256i initmixed = f(init);
    double expectedavg = sizeof(__m256i) * 8.0 / 2;
    double sumofdiffs = 0;
    double sumofsquares = 0;

    for(int k = 0; k < number; ++k) {
       buffer[wordindex] = start + k;
       __m256i newvec = _mm256_loadu_si256((__m256i *) buffer);
       __m256i newvecmixed = f(newvec);
       int thisdiff = hamming_diff(newvecmixed,initmixed);
       sumofdiffs +=  thisdiff;
       sumofsquares +=  (thisdiff - expectedavg) * (thisdiff - expectedavg);
   }
   if (verbose) printf("avg diff = %f with error of %f \n", sumofdiffs  / number, sqrt(sumofsquares / number));
   return sumofdiffs  / number;
}

int main() {
  zobrist256_init(&zobrist256_global);
  for(int k = 0; k < sizeof(__m256i) / sizeof(uint32_t); k++) {
    printf("word index = %d \n", k);
    printf("zobrist\n");
    avg_hamming(zobrist256, 10, 1000,k, true);
    avg_hamming(expmixer256, 10, 1000,k, true);


  }
}
