/**
* We verify the quality of bit mixing functions.
* You must have a recent x64 processor.
*/

#include <x86intrin.h> // assumes recent x64 machine
#include <emmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>

size_t howmanygaps = 1000;
#ifdef IACA
#include </opt/intel/iaca/include/iacaMarks.h>
#else
#define IACA_START
#define IACA_END
#endif

typedef uint32_t (*mixer32)(uint32_t);

/**
* See https://github.com/aappleby/smhasher/wiki/MurmurHash3
*/
uint32_t murmurhashold_mixer(uint32_t h) {
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;
  return h;
}
uint32_t murmurhash_mixer(uint32_t hh) {
  uint64_t h = hh;
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

uint32_t odd_mixer(uint32_t h) {
  return h * UINT32_C(774932967);
}
uint64_t TEST_CONSTANT =  UINT64_C(9786759967625374317);//UINT64_C(9786759969235987053);
uint32_t testmultiplyshift_mixer(uint32_t h) {
    return _mm_cvtsi128_si64(_mm_srli_si128(_mm_clmulepi64_si128(_mm_cvtsi64_si128(TEST_CONSTANT),_mm_cvtsi64_si128(h),0x00),4));
}

uint32_t testmultiplyshift2_mixer(uint32_t h) {
  return testmultiplyshift_mixer(testmultiplyshift_mixer(h));
}


uint64_t MS_CONSTANT = UINT64_C(4321529154185334784);
uint32_t multiplyshift_mixer(uint32_t h) {
  return (MS_CONSTANT * h ) >> 32;
}
uint64_t MS_CONSTANT1 = UINT64_C(9615667904973576846);
uint64_t MS_CONSTANT2 = UINT64_C(2881622810808819561);

uint32_t multiplyshift2_mixer(uint32_t h) {
  h =  (MS_CONSTANT1 * h ) >> 32;
  h =  (MS_CONSTANT2 * h ) >> 32;
  return h;
}
uint64_t MS_CONSTANT3 = UINT64_C(4321529154185334784);
uint64_t MS_CONSTANT4 = UINT64_C(15571291822141779727);
uint64_t MS_CONSTANT5 = UINT64_C(15571291822141119727);


uint32_t multiplyshift4_mixer(uint32_t h) {
  uint32_t h1 =  (MS_CONSTANT1 * h ) >> 32;
  uint32_t h2 =  (MS_CONSTANT2 * h ) >> 32;
  uint32_t h3 =  (MS_CONSTANT3 * h ) >> 32;
  uint32_t answer =  h1 ^ h2 ^ h3;
  return answer;
}

uint32_t multiplyshift5_mixer(uint32_t h) {
  uint32_t h1 =  (MS_CONSTANT1 * h ) >> 32;
  uint32_t h2 =  (MS_CONSTANT2 * h ) >> 32;
  return h1 ^ h2;
}

uint32_t multiplyshift6_mixer(uint32_t h) {
  uint32_t h1 =  (MS_CONSTANT1 * h ) >> 32;
  uint32_t h2 =  (MS_CONSTANT2 * h ) >> 32;
  uint32_t h3 =  (MS_CONSTANT3 * h ) >> 32;
  uint32_t h4 =  (MS_CONSTANT4 * h ) >> 32;
  return h1 ^ h2 ^ h3 ^ h4;
}
uint32_t multiplyshift7_mixer(uint32_t h) {
  uint32_t h1 =  (MS_CONSTANT1 * h ) >> 32;
  uint32_t h2 =  (MS_CONSTANT2 * h ) >> 32;
  uint32_t h3 =  (MS_CONSTANT3 * h ) >> 32;
  uint32_t h4 =  (MS_CONSTANT4 * h ) >> 32;
  uint32_t h5 =  (MS_CONSTANT5 * h ) >> 32;

  return h1 ^ h2 ^ h3 ^ h4 ^ h5;
}

uint32_t xorshift32_mixer(uint32_t h) {
  h ^= h << 13;
  h ^= h >> 17;
  h ^= h << 5;
  return h;
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


double test_arithmetic(mixer32 f, uint32_t start, uint32_t number, uint32_t gap, double * maxbias, double * totalbias) {
  int diffs[33];
  for(int k = 0; k <= 32 ; ++k) diffs[k] = 0;
  uint32_t end = start + number * gap;
  for(uint32_t x = start ; x + gap <= end; x += gap) {
    int d = hamming_diff(f(x),f(x+gap));
    diffs[d]++;
  }
  int width = 32;// try 8

  // expected value of (x-16)^2
  double  expect = 0;
  for(int k = 0; k <= width ; ++k) {
    expect += diffs[k] * (width/2 - k) * (width/2 - k);
  }
  double bias = sqrt(expect / number);
  if(*maxbias < bias) *maxbias = bias;
  *totalbias += bias;
  return *maxbias;
}


void simpletest_ms_tuning() {
  double maxv = 0;
  double avgv = 0;
  double bestv = 10000000;
  uint64_t bestconstant = UINT64_C(17845529765768110319);//TEST_CONSTANT;
      for(int gap = 1; gap < 2; gap ++) {

      test_arithmetic(testmultiplyshift_mixer,13,1000000,gap,&maxv,&avgv);
    }
    if(bestv > maxv) {//11765803696038574438
      bestv = maxv;
      bestconstant = TEST_CONSTANT;
        printf("%" PRIu64 " ",TEST_CONSTANT);

    }

  for(int k = 0; k < 1000; ++k) {
    _rdrand64_step(&TEST_CONSTANT);
    maxv = 0;
    for(int gap = 1; gap < 2; gap ++) {
      test_arithmetic(testmultiplyshift_mixer,13,1000000,gap,&maxv,&avgv);
    }
  printf("%f %f \n", bestv, maxv);

    if(bestv > maxv) {//11765803696038574438
      bestv = maxv;
      bestconstant = TEST_CONSTANT;
        printf("%" PRIu64 " ",TEST_CONSTANT);

    }


  }
  TEST_CONSTANT = bestconstant;
  printf("%" PRIu64 " ",TEST_CONSTANT);

  printf("%f \n", bestv);
}

void test_ms_tuning() {
  double maxv = 0;
  double avgv = 0;
  double bestv = 10000000;
  uint64_t bestconstant = UINT64_C(9786759967625374317);//TEST_CONSTANT;
  size_t length = 1000000;

      test_arithmetic(testmultiplyshift_mixer,13,length,1,&maxv,&avgv);

    if(bestv > maxv) {//11765803696038574438
      bestv = maxv;
      bestconstant = TEST_CONSTANT;
        printf("%" PRIu64 " ",TEST_CONSTANT);

    }

  for(int k = 0; k < 64; ++k) {
    TEST_CONSTANT ^= (UINT64_C(1)<<k);

  for(int kk = 0; kk < 64; ++kk) {
  TEST_CONSTANT ^= (UINT64_C(1)<<kk);
  //for(int kkk = 0; kkk < 64; ++kkk) {
  //TEST_CONSTANT ^= (UINT64_C(1)<<kkk);

    maxv = 0;
      test_arithmetic(testmultiplyshift_mixer,13,length,1,&maxv,&avgv);

//  printf("%f %f \n", bestv, maxv);

    if(bestv > maxv) {//11765803696038574438
      bestv = maxv;
      bestconstant = TEST_CONSTANT;
        printf("%" PRIu64 " ",TEST_CONSTANT);
    }
    //TEST_CONSTANT ^= (UINT64_C(1)<<kkk);

  //}
  TEST_CONSTANT ^= (UINT64_C(1)<<kk);

  }
  TEST_CONSTANT ^= (UINT64_C(1)<<k);


  }
  TEST_CONSTANT = bestconstant;
  printf("%" PRIu64 " ",TEST_CONSTANT);

  printf("%f \n", bestv);
}


void simple_ms_tuning() {
  double maxv = 0;
  double avgv = 0;
  double bestv = 10000000;
  for(int k = 0; k < 1000; ++k) {
    _rdrand64_step(&MS_CONSTANT);
    maxv = 0;
    for(int gap = 1; gap < 2; gap ++) {
      test_arithmetic(multiplyshift_mixer,13,1000000,gap,&maxv,&avgv);
    }
    if(bestv > maxv) bestv = maxv;

  }

  printf("%f \n", bestv);
}

void simple_ms2_tuning() {
  double maxv = 0;
  double avgv = 0;
  double bestv = 10000000;
  for(int k = 0; k < 1000; ++k) {
    _rdrand64_step(&MS_CONSTANT1);
    _rdrand64_step(&MS_CONSTANT2);
    MS_CONSTANT1 |= 1;
    MS_CONSTANT2 |= 1;
    maxv = 0;
    for(int gap = 1; gap < howmanygaps; gap ++) {
      test_arithmetic(multiplyshift2_mixer,13,1000,gap,&maxv,&avgv);
    }
    if(bestv > maxv) bestv = maxv;

  }
  printf("%f \n", bestv);
}

void oldsimple_ms4_tuning() {
  double maxv = 0;
  double avgv = 0;
  double bestv = 10000000;
  for(int k = 0; k < 1000; ++k) {
    _rdrand64_step(&MS_CONSTANT1);
    _rdrand64_step(&MS_CONSTANT2);
    _rdrand64_step(&MS_CONSTANT3);

    MS_CONSTANT1 |= 1;
    MS_CONSTANT2 |= 1;
    MS_CONSTANT3 |= 1;
    maxv = 0;
    for(int gap = 1; gap < howmanygaps; gap ++) {
      test_arithmetic(multiplyshift4_mixer,13,1000,gap,&maxv,&avgv);
    }
    if(bestv > maxv) bestv = maxv;

  }
  printf("%f \n", bestv);
}

void simple_ms5_tuning() {
  double maxv = 0;
  double avgv = 0;
  double bestv = 10000000;
  MS_CONSTANT1 = UINT64_C(3005201888959602881); //UINT64_C(15100527840056609390);
  MS_CONSTANT2 = UINT64_C(1926437807953379218);//UINT64_C(15144388073579178144);
  double maxmu = 0;
  double avgmu = 0;

  uint64_t best1, best2;
  for(int gap = 1; gap < howmanygaps*100; gap ++) {
      test_arithmetic(multiplyshift5_mixer,13,10,gap,&maxv,&avgv);
      test_arithmetic(murmurhash_mixer,13,10,gap, &maxmu, &avgmu);
  }
    if(bestv > maxv) {
      best1 = MS_CONSTANT1;
      best2 = MS_CONSTANT2;
      bestv = maxv;
    }
  printf("%" PRIu64 " %" PRIu64 " ",best1,best2);

  printf("init: %f %f \n", bestv, maxmu);

  for(int k = 0; k < 1000; k++) {
    _rdrand64_step(&MS_CONSTANT1);
    _rdrand64_step(&MS_CONSTANT2);
    maxv = 0;
    for(int gap = 1; gap < howmanygaps*100; gap ++) {
      test_arithmetic(multiplyshift5_mixer,13,10,gap,&maxv,&avgv);
    }
    if(bestv > maxv) {
      best1 = MS_CONSTANT1;
      best2 = MS_CONSTANT2;
      bestv = maxv;
    }

  }
  printf("%" PRIu64 " %" PRIu64 " ",best1,best2);
  printf("%f \n", bestv);
}

void simple_ms4_tuning() {
  double maxv = 0;
  double avgv = 0;
  double bestv = 10000000;
  MS_CONSTANT1 = UINT64_C(6447882596858021560); //UINT64_C(15100527840056609390);
  MS_CONSTANT2 = UINT64_C(4343140493376601914);//UINT64_C(15144388073579178144);
  MS_CONSTANT3 = UINT64_C(18034287934215492240);//UINT64_C(15144388073579178144);

  double maxmu = 0;
  double avgmu = 0;

  uint64_t best1, best2, best3;
  for(int gap = 1; gap < howmanygaps; gap++) {
      test_arithmetic(multiplyshift4_mixer,13,1000,gap,&maxv,&avgv);
      test_arithmetic(murmurhash_mixer,13,1000,gap, &maxmu, &avgmu);
  }
    if(bestv > maxv) {
      best1 = MS_CONSTANT1;
      best2 = MS_CONSTANT2;
      best3 = MS_CONSTANT3;

      bestv = maxv;
    }
  printf("%" PRIu64 " %" PRIu64 "  %" PRIu64 " ",best1,best2,best3);

  printf("init: %f %f \n", bestv, maxmu);

  for(int k = 0; k < 1000; ++k) {
    _rdrand64_step(&MS_CONSTANT1);
    _rdrand64_step(&MS_CONSTANT2);
    _rdrand64_step(&MS_CONSTANT3);
    maxv = 0;
    for(int gap = 1; gap < howmanygaps; gap++) {
      test_arithmetic(multiplyshift4_mixer,13,1000,gap,&maxv,&avgv);
    }
    if(bestv > maxv) {
      best1 = MS_CONSTANT1;
      best2 = MS_CONSTANT2;
            best3 = MS_CONSTANT3;

      bestv = maxv;
    }

  }
  printf("%" PRIu64 " %" PRIu64 "  %" PRIu64 " ",best1,best2,best3);
  printf("%f \n", bestv);
  MS_CONSTANT1 = UINT64_C(6447882596858021560); //UINT64_C(15100527840056609390);
  MS_CONSTANT2 = UINT64_C(4343140493376601914);//UINT64_C(15144388073579178144);
  MS_CONSTANT3 = UINT64_C(18034287934215492240);//UINT64_C(15144388073579178144);

}

void simple_ms6_tuning() {
  double maxv = 0;
  double avgv = 0;
  double bestv = 10000000;
  uint64_t best4;
  MS_CONSTANT1 = UINT64_C(6447882596858021560); //UINT64_C(15100527840056609390);
  MS_CONSTANT2 = UINT64_C(4343140493376601914);//UINT64_C(15144388073579178144);
  MS_CONSTANT3 = UINT64_C(18034287934215492240);//UINT64_C(15144388073579178144);

  for(int k = 0; k < 1000; ++k) {
//    _rdrand64_step(&MS_CONSTANT1);
  //  _rdrand64_step(&MS_CONSTANT2);
    //_rdrand64_step(&MS_CONSTANT3);
    _rdrand64_step(&MS_CONSTANT4);

    maxv = 0;
    for(int gap = 1; gap < howmanygaps; gap ++) {
      test_arithmetic(multiplyshift6_mixer,13,1000,gap,&maxv,&avgv);
    }
    if(bestv > maxv) {
      bestv = maxv;
      best4 = MS_CONSTANT4;
    }

  }
  MS_CONSTANT4 = best4;
  printf( "  %" PRIu64 " ",best4);

  printf("%f \n", bestv);
}

void simple_ms7_tuning() {
  double maxv = 0;
  double avgv = 0;
  double bestv = 10000000;
  for(int k = 0; k < 1000; ++k) {
    _rdrand64_step(&MS_CONSTANT1);
    _rdrand64_step(&MS_CONSTANT2);
    _rdrand64_step(&MS_CONSTANT3);
    _rdrand64_step(&MS_CONSTANT4);
    _rdrand64_step(&MS_CONSTANT5);

    maxv = 0;
    for(int gap = 1; gap < howmanygaps; gap ++) {
      test_arithmetic(multiplyshift7_mixer,13,1000,gap,&maxv,&avgv);
    }
    if(bestv > maxv) bestv = maxv;

  }
  printf("%f \n", bestv);
}

void demo() {
  zobrist32_init(&k);
  double maxi = 0;
  double maxo = 0;
  double maxms = 0;
  double maxxor = 0;
  double maxmu = 0;
  double maxcrc = 0;
  double maxz = 0;
  double maxms2 = 0;
  double maxms4 = 0;
  double maxms6 = 0;
  double maxtest = 0;

  double avgi = 0;
  double avgo = 0;
  double avgms = 0;
  double avgxor = 0;
  double avgmu = 0;
  double avgcrc = 0;
  double avgz = 0;
  double avgms2 = 0;
  double avgms4 = 0;
  double avgms6 = 0;
  double avgtest = 0;
  size_t counter = 0;
    size_t lengthofruns = 10000000;

  for(int gap = 1; gap < 2; gap ++) {
    ++counter;
    test_arithmetic(identity,13,lengthofruns,gap, &maxi, &avgi);
    test_arithmetic(odd_mixer,13,lengthofruns,gap,&maxo,&avgo);
    test_arithmetic(multiplyshift_mixer,13,lengthofruns,gap,&maxms,&avgms);
    test_arithmetic(xorshift32_mixer,13,lengthofruns,gap,&maxxor,&avgxor);
    test_arithmetic(murmurhash_mixer,13,lengthofruns,gap, &maxmu, &avgmu);
    test_arithmetic(crc32_mixer,13,lengthofruns,gap,&maxcrc,&avgcrc);
    test_arithmetic(zobrist32,13,lengthofruns,gap,&maxz,&avgz);
    test_arithmetic(multiplyshift4_mixer,13,lengthofruns,gap,&maxms4,&avgms4);
    test_arithmetic(multiplyshift6_mixer,13,lengthofruns,gap,&maxms6,&avgms6);
    test_arithmetic(testmultiplyshift_mixer,13,lengthofruns,gap,&maxtest,&avgtest);


  }
  printf("identity odd ms xor murmur crc zobrist ms4 ms6 test\n");
  printf("max bias %f %f %f %f %f %f %f  %f %f %f \n",maxi,maxo,maxms,maxxor,maxmu,maxcrc,maxz,maxms4,maxms6,maxtest);
  printf("avg bias %f %f %f %f %f %f %f %f %f %f \n",avgi/counter,avgo/counter,avgms/counter,
     avgxor/counter,avgmu/counter,avgcrc/counter,avgz/counter,avgms4/counter,avgms6/counter,avgtest/counter);

}

int main() {
/*    simple_ms5_tuning();

  simple_ms7_tuning();
  simple_ms6_tuning();*/


  //simple_ms4_tuning();
  //simple_ms6_tuning();
  //simple_ms_tuning();
//simpletest_ms_tuning();
//test_ms_tuning();
  demo();
}
