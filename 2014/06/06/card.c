// gcc -std=c99 -msse4.2 -O2 -o card card.c
// icc -std=c99 -march=corei7 -O2 -o card card.c


#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>



void bitwiseor(uint64_t * input1, uint64_t * input2, uint64_t * output, size_t length) {
    for(int k = 0; k < length; ++k) {
        output[k] = input1[k] | input2[k];
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
    for(int k = 0; k < length; ++k) {
        output[k] = input1[k] | input2[k];
        card += __builtin_popcountl(output[k]);
    }
    return card;
} 



int main() {
    const int N = 1024*1024;
    uint64_t * input1 = (uint64_t *) malloc(N*sizeof(uint64_t));
    uint64_t * input2 = (uint64_t *) malloc(N*sizeof(uint64_t));
    uint64_t * output = (uint64_t *) malloc(N*sizeof(uint64_t));
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
      if(k>3) printf("%f %f %f %llu \n", (double)repeat*N*CLOCKS_PER_SEC/(1000.0*1000.0 *(S2-S1)), (double)repeat*N*CLOCKS_PER_SEC/(1000.0*1000.0 *(S3-S2)), (S2-S1)*1.0/(S3-S2), bogus);
    }

    free(input1);
    free(input2);    
    free(output);
}
