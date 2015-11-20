/***
* This tests the generation of ranged 16-bit random integers.
* gcc -O2 -o rangedrandint rangedrandint.c
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


int multapproach(uint16_t range) {
    printf("mult range %d \n",range);

    int counter[1<<16];
    for(int k = 0; k < 1<<16; ++k) {
        counter[k] = 0;
    }
    int threshold = (int) ((uint32_t)((1<<16)/range*range) -1);
    if(threshold+1 < (1<<16)-range+ (range & (~(range-1)))){
      printf("threshold = %d ",threshold);
      printf("crazy = %d ",(1<<16)-range+ (range & (~(range-1))));
      return -1;
    }

    for(int k = 0; k < 1<<16; ++k) {
        uint32_t r = k;
        uint16_t c =(r* range) >> 16;
        int leftover = (r* range) & 0xFFFF;
        if(leftover <= threshold) {
            counter[c]++;
        } else {

             if(leftover <= (1<<16)-range)
              return -1;

        }
    }

    int minc = UINT16_MAX+1;
    int maxc = 0;
    int fair = counter[0];
    for(int k = 0; k < range; ++k) {
        if(counter[k]<minc) minc = counter[k];
        if(counter[k]>maxc) maxc = counter[k];
    }
    printf("%d ** %d +++  %d -- %d +++ %d \n",-minc+maxc,minc,UINT16_MAX/range,(UINT16_MAX+range-1)/range,maxc);
    if(minc == maxc) printf("******fair\n");
    else {printf("======unfair \n"); return -1;}
    return 0;
}

int reversemultapproach(uint16_t range) {
    printf("reverse mult range %d \n",range);

    int counter[1<<16];
    for(int k = 0; k < 1<<16; ++k) {
        counter[k] = 0;
    }
    int threshold = (1<<16) % range;

    if((range &(range-1))!=0) {
        int fthreshold = ((1<<16)-1)%range + 1;
        if(threshold != fthreshold) {
          printf("bug %d %d %d ",range,threshold,fthreshold);
          abort();
        }
    }
    for(int k = 0; k < 1<<16; ++k) {
        uint32_t r = k;
        uint16_t c =(r* range) >> 16;
        int leftover = (r* range) & 0xFFFF;
        if(leftover >= threshold) {
            counter[c]++;
        } else {
        }
    }

    int minc = UINT16_MAX+1;
    int maxc = 0;
    int fair = counter[0];
    for(int k = 0; k < range; ++k) {
        if(counter[k]<minc) minc = counter[k];
        if(counter[k]>maxc) maxc = counter[k];
    }
    printf("%d ** %d +++  %d -- %d +++ %d \n",-minc+maxc,minc,UINT16_MAX/range,(UINT16_MAX+range-1)/range,maxc);
    if(minc == maxc) printf("******fair\n");
    else {printf("======unfair \n"); return -1;}
    return 0;
}

int fasterreversemultapproach(uint16_t range) {
    printf("faster reverse mult range %d \n",range);

    int counter[1<<16];
    for(int k = 0; k < 1<<16; ++k) {
        counter[k] = 0;
    }
    int threshold = (1<<16) % range;
    int cheap = range- (range & (~(range-1)));
    if(cheap < threshold ) {
      printf("bug");
      abort();
    }
    for(int k = 0; k < 1<<16; ++k) {
        uint32_t r = k;
        uint16_t c =(r* range) >> 16;
        int leftover = (r* range) & 0xFFFF;
        if(leftover >= threshold) {
            counter[c]++;
        } else {
        }
    }

    int minc = UINT16_MAX+1;
    int maxc = 0;
    int fair = counter[0];
    for(int k = 0; k < range; ++k) {
        if(counter[k]<minc) minc = counter[k];
        if(counter[k]>maxc) maxc = counter[k];
    }
    printf("%d ** %d +++  %d -- %d +++ %d \n",-minc+maxc,minc,UINT16_MAX/range,(UINT16_MAX+range-1)/range,maxc);
    if(minc == maxc) printf("******fair\n");
    else {printf("======unfair \n"); return -1;}
    return 0;
}


int fasterthanmultapproach(uint16_t range) {
    printf("fasterthanmult range %d \n",range);

    int counter[1<<16];
    for(int k = 0; k < 1<<16; ++k) {
        counter[k] = 0;
    }
    int fastthreshold = (1<<16)-range+ (range & (~(range-1)));
    int threshold = (int) ((uint32_t)((1<<16)/range*range) -1);
    if(threshold+1 < fastthreshold){
      printf("threshold = %d ",threshold);
      printf("crazy = %d ",fastthreshold);
      return -1;
    }

    for(int k = 0; k < 1<<16; ++k) {
        uint32_t r = k;
        uint16_t c =(r* range) >> 16;
        int leftover = (r* range) & 0xFFFF;
        if(leftover < fastthreshold) {
            counter[c]++;
        } else {
          if(leftover <= threshold)
            counter[c]++;
        }
    }

    int minc = UINT16_MAX+1;
    int maxc = 0;
    int fair = counter[0];
    for(int k = 0; k < range; ++k) {
        if(counter[k]<minc) minc = counter[k];
        if(counter[k]>maxc) maxc = counter[k];
    }
    printf("%d ** %d +++  %d -- %d +++ %d \n",-minc+maxc,minc,UINT16_MAX/range,(UINT16_MAX+range-1)/range,maxc);
    if(minc == maxc) printf("******fair\n");
    else {printf("======unfair \n"); return -1;}
    return 0;
}



int classicapproach(uint16_t range) {
    printf("classic range %d \n",range);

    int counter[1<<16];
    for(int k = 0; k < 1<<16; ++k) {
        counter[k] = 0;
    }
    int threshold = (int) ((uint32_t)((1<<8)/range*range) -1);
    int failure = 0;
    for(int k = 0; k < 1<<16; ++k) {
        uint32_t r = k;
        uint16_t c = r%range;
        if(r+ range -1 - c < (1<<16)) {
            counter[c]++;
        } else {
            ++failure;

        }
    }
    printf("failure rate  %f \n",failure * 1.0 / (1<<16));
    int minc = UINT16_MAX+1;
    int maxc = 0;
    int fair = counter[0];
    for(int k = 0; k < range; ++k) {
        if(counter[k]<minc) minc = counter[k];
        if(counter[k]>maxc) maxc = counter[k];
    }
    printf("%d ** %d +++  %d -- %d +++ %d \n",-minc+maxc,minc,UINT16_MAX/range,(UINT16_MAX+range-1)/range,maxc);
    if(minc == maxc) printf("******fair\n");
    else {printf("======unfair \n"); return -1;}
    return 0;
}

int fastapproach(uint16_t range) {
    printf("fast range %d \n",range);

    int counter[1<<16];
    for(int k = 0; k < 1<<16; ++k) {
        counter[k] = 0;
    }
    for(int k = 0; k < 1<<16; ++k) {
      if((range & (range - 1)) == 0) {
          counter[k & (range - 1)]++;
          continue;
      }
      uint32_t multiresult = k * range;
      uint32_t leftover = multiresult % (1<<16);
      if(leftover < range ) {
          uint32_t threshold = ((1<<16)-1) % range ;
          if (leftover > threshold) {
            counter[multiresult >> 16]++;
          }
      } else {
        counter[multiresult >> 16]++;
      }
    }
    int minc = UINT16_MAX+1;
    int maxc = 0;
    int fair = counter[0];
    for(int k = 0; k < range; ++k) {
        if(counter[k]<minc) minc = counter[k];
        if(counter[k]>maxc) maxc = counter[k];
    }
    printf("%d ** %d +++  %d -- %d +++ %d \n",-minc+maxc,minc,UINT16_MAX/range,(UINT16_MAX+range-1)/range,maxc);
    if(minc == maxc) printf("******fair\n");
    else {printf("======unfair \n"); return -1;}
    return 0;
}


int main() {

    for(int k = 1; k <60000; k++) {
        int r = multapproach(k);
        if(r<0) return r;
        r =  fastapproach(k);
        if(r<0) return r;
        r = reversemultapproach(k);
        if(r<0) return r;
        r = fasterreversemultapproach(k);
        if(r<0) return r;

        r = fasterthanmultapproach(k);
        if(r<0) return r;

        r = classicapproach(k);
        if(r<0) return r;
        printf("\n\n");



    }
    return 0;
}
