// gcc -std=c99 -msse4.2 -O2 -o card card.c
// icc -std=c99 -march=corei7 -O2 -o card card.c


#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>


void bitwiseor(uint64_t * input1, uint64_t * input2, uint64_t * output, size_t length) {
    for(int j = 0; j < length; j+=4) {
        for(int k = 0; k<4; ++k)
        output[k+j] = input1[k+j] | input2[k+j];
    }
} 


int card(uint64_t * input, size_t length) {
    int card = 0;
    for(int k = 0; k < length; ++k) {
        card += __builtin_popcountl(input[k]);
    }
    return card;
} 


int bitwiseorcard(uint64_t * input1, uint64_t * input2, uint64_t * output, size_t length) {
    int card = 0;
    assert(length / 8 * 8 == length);
    for(int k = 0; k < length; k+=8) {
        output[k] = input1[k] | input2[k];
        card += __builtin_popcountl(output[k]);
        output[k+1] = input1[k+1] | input2[k+1];
        card += __builtin_popcountl(output[k+1]);
        output[k+2] = input1[k+2] | input2[k+2];
        card += __builtin_popcountl(output[k+2]);
        output[k+3] = input1[k+3] | input2[k+3];
        card += __builtin_popcountl(output[k+3]);
        output[k+4] = input1[k+4] | input2[k+4];
        card += __builtin_popcountl(output[k+4]);
        output[k+5] = input1[k+5] | input2[k+5];
        card += __builtin_popcountl(output[k+5]);
        output[k+6] = input1[k+6] | input2[k+6];
        card += __builtin_popcountl(output[k+6]);
        output[k+7] = input1[k+7] | input2[k+7];
        card += __builtin_popcountl(output[k+7]);
    }
    return card;
} 
void test(const int N) {
    printf("array sizes = %i kB \n",N*sizeof(uint64_t)/1024);
    uint64_t * input1 = (uint64_t *) malloc(N*sizeof(input1));
    uint64_t * input2 = (uint64_t *) malloc(N*sizeof(input1));
    uint64_t * output = (uint64_t *) malloc(N*sizeof(input1));
    for(int k = 0; k<N; ++k) {
      input1[k] = k * k;
      input2[k] = k * k * k + 3;
    }
    uint64_t bogus = 0;
    int repeat = 200*1024*1024.0/N;
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
      if(k>3) printf("%f %f %f %llu \n", (double)repeat*N*CLOCKS_PER_SEC/(1000.0*1000.0 *(S2-S1)), (double)repeat*N*CLOCKS_PER_SEC/(1000.0*1000.0 *(S3-S2)), (S2-S1)*1.0/(S3-S2), bogus);
    }

    free(input1);
    free(input2);    
    free(output);
}


int main() {
    test(1024*1024);
    test(512*1024);
    test(128*1024);
    test(64*1024);
    test(32*1024);
    test(1024);
}
