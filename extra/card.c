// gcc -std=c99 -msse4.2 -O3 -o card card.c
// icc -std=c99 -march=corei7 -O3 -o card card.c
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>


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
    for(int k = 0; k < length; ++k) {
        card += _mm_popcnt_u64(input[k]);
    }
    return card;
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
    const int N = 1024*1024;
    uint64_t * input1 = (uint64_t *) malloc(N*sizeof(input1));
    uint64_t * input2 = (uint64_t *) malloc(N*sizeof(input1));
    uint64_t * output = (uint64_t *) malloc(N*sizeof(input1));
    for(int k = 0; k<N; ++k) {
      input1[k] = k * k;
      input2[k] = k * k * k + 3;
    }
    uint64_t bogus = 0;
    int repeat = 200;
    for(int k = 0; k<10;++k) {
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
      if(k>3) printf("%lu %lu %llu \n", (S2-S1), (S3-S2), bogus);
    }

    free(input1);
    free(input2);    
    free(output);
}
