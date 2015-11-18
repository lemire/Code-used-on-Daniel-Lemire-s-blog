/***
* This tests the generation of ranged 16-bit random integers.
* gcc -O2 -o rangedrandint rangedrandint.c
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


int fogapproach(uint16_t range) {
    printf("fog range %d \n",range);

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
    int failure = 0;
    int softfailure = 0;

    for(int k = 0; k < 1<<16; ++k) {
        uint32_t r = k;
        uint16_t c =(r* range) >> 16;
        int leftover = (r* range) & 0xFFFF;
        if(leftover > (1<<16)-range) ++softfailure;
        if(leftover <= threshold) {
            counter[c]++;
        } else {

             if(leftover <= (1<<16)-range)
              return -1;
            ++failure;

        }
    }
    printf("failure rate  %f \n",failure * 1.0 / (1<<16));
    printf("soft failure rate  %f \n",softfailure * 1.0 / (1<<16));

    int minc = UINT16_MAX+1;
    int maxc = 0;
    int fair = counter[0];
    for(int k = 0; k < range; ++k) {
        if(counter[k]<minc) minc = counter[k];
        if(counter[k]>maxc) maxc = counter[k];
    }
    printf("%d ** %d +++  %d -- %d +++ %d \n",-minc+maxc,minc,UINT16_MAX/range,(UINT16_MAX+range-1)/range,maxc);
    if(minc == maxc) printf("******fair\n");
    else printf("======unfair \n");
    return 0;
}
int nakedfogapproach(uint16_t range) {
    printf("naked fog range %d \n",range);

    int counter[1<<16];
    for(int k = 0; k < 1<<16; ++k) {
        counter[k] = 0;
    }
    int threshold = (int) ((uint32_t)((1<<16)/range*range) -1);
    if(threshold < (1<<16)-range) return -1;
    int failure = 0;
    int softfailure = 0;

    for(int k = 0; k < 1<<16; ++k) {
        uint32_t r = k;
        uint16_t c =(r* range) >> 16;
        counter[c]++;
    }
    printf("failure rate  %f \n",failure * 1.0 / (1<<16));
    printf("soft failure rate  %f \n",softfailure * 1.0 / (1<<16));

    int minc = UINT16_MAX+1;
    int maxc = 0;
    int fair = counter[0];
    for(int k = 0; k < range; ++k) {
        if(counter[k]<minc) minc = counter[k];
        if(counter[k]>maxc) maxc = counter[k];
    }
    printf("%d ** %d  %d \n",-minc+maxc,minc,maxc);
    if(minc == maxc) printf("******fair\n");
    else printf("======unfair \n");
    return 0;
}

int cheapfogapproach(uint16_t range) {
    printf("cheap fog range %d \n",range);

    int counter[1<<16];
    for(int k = 0; k < 1<<16; ++k) {
        counter[k] = 0;
    }
    int threshold = (int) ((uint32_t)((1<<16)/range*range) -1);
    if(threshold < (1<<16)-range) return -1;
    int failure = 0;
    int softfailure = 0;

    for(int k = 0; k < 1<<16; ++k) {
        uint32_t r = k;
        uint16_t c =(r* range) >> 16;
        int leftover = (r* range) & 0xFFFF;
        if(leftover <= (1<<16)-range){
          counter[c]++;
        }
    }
    printf("failure rate  %f \n",failure * 1.0 / (1<<16));
    printf("soft failure rate  %f \n",softfailure * 1.0 / (1<<16));

    int minc = UINT16_MAX+1;
    int maxc = 0;
    int fair = counter[0];
    for(int k = 0; k < range; ++k) {
        if(counter[k]<minc) minc = counter[k];
        if(counter[k]>maxc) maxc = counter[k];
    }
    printf("%d ** %d  %d \n",-minc+maxc,minc,maxc);
    if(minc == maxc) printf("******************************fair\n");
    else printf("======unfair \n");
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
    else printf("======unfair \n");
    return 0;
}



int main() {

    for(int k = 1; k <60000; k++) {
        int r = fogapproach(k);
        if(r<0) return r;
        r = nakedfogapproach(k);
        if(r<0) return r;
        r = cheapfogapproach(k);
        if(r<0) return r;

        r = classicapproach(k);
        if(r<0) return r;
        printf("\n\n");



    }
    return 0;
}
