// gcc -std=c99 -msse4.2 -O2 -o card card.c
// icc -std=c99 -march=corei7 -O2 -o card card.c
// For IACA:
//$ gcc -std=c99 -march=corei7 -DIACA -O2 -o card card.c
//$ /opt/intel/iaca/bin/iaca -mark 2 -64 card
//$ /opt/intel/iaca/bin/iaca -mark 1 -64 card


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
    uint64_t * input1 = (uint64_t *) malloc(N*sizeof(input1));
    uint64_t * input2 = (uint64_t *) malloc(N*sizeof(input1));
    uint64_t * output = (uint64_t *) malloc(N*sizeof(input1));
    for(int k = 0; k<N; ++k) {
      input1[k] = k * k;
      input2[k] = k * k * k + 3;
    }
    uint64_t bogus = 0;
    int repeat = 2000;
    for(int k = 0; k<15;++k) {
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

      if(k>3) printf("%lu %lu  %lu  %llu \n", (S2-S1), (S3-S2), (S4-S3), bogus);
    }

    free(input1);
    free(input2);
    free(output);
}
