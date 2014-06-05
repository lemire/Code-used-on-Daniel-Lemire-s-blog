#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <popcntintrin.h>

void bitwiseor(uint64_t * input1, uint64_t * input2, uint64_t * output, size_t length) {
    for(int k = 0; k < length; ++k) {
        output[k] = input1[k] | input2[k];
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
        output[k] = input1[k] | input2[k];
        card += _mm_popcnt_u64(output[k]);
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
    for(int k = 0; k<10;++k) {
      const clock_t S1 = clock();
      bitwiseor(input1,input2,output,N);
      bogus += output[10];
      const clock_t S2 = clock();
      bogus += bitwiseorcard(input1,input2,output,N); 
      const clock_t S3 = clock();    
      if(k>3) printf("%lu %lu %llu \n", (S2-S1), (S3-S2), bogus);
    }

    free(input1);
    free(input2);    
    free(output);
}