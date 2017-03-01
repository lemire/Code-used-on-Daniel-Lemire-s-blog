#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

void demo() {
  float oldy = -1;// arbitrary
  float y;
  size_t counter = 0;
  size_t currentcount = 0;
  size_t largestcount = 0;
  size_t smallestcount = 0xFFFFFFFF;
  float mode = -1;
  float minmode = -1;
  for(size_t i = 0; i <= UINT32_C(0xFFFFFFFF); i++) {
    uint32_t x = (uint32_t) i;
    y = x / ((float)(UINT64_C(1) << 32)) ;
    assert(y >= 0);
    assert(y <= 1);
    currentcount++;
    if(y == 0.5) printf("%zu \n", i);
    if(y > oldy) {
       counter++;
       if(currentcount > largestcount) {
          largestcount = currentcount;
          mode = y;
       }
       if(smallestcount > currentcount) {
          smallestcount = currentcount;
          minmode = y;
       }
       currentcount = 0;
    }
    oldy = y;
  }
  printf("%zu -- %zu %f -- %zu %f --  %f \n", counter, largestcount, mode, smallestcount, minmode,  y);
}
void demo24() {
  float oldy = -1;// arbitrary
  float y;
  size_t counter = 0;
  size_t currentcount = 0;
  size_t largestcount = 0;
  size_t smallestcount = 0xFFFFFFFF;
  float mode = -1;
  float minmode = -1;
  for(size_t i = 0; i < (1<<24); i++) {
    uint32_t x = (uint32_t) i;
    y = x / ((float)(1 << 24)) ;
    assert((uint32_t)(y * (float) (1 << 24)) == x);
    assert(y >= 0);
    assert(y <= 1);
    currentcount++;
    if(y > oldy) {
       counter++;
       if(currentcount > largestcount) {
          largestcount = currentcount;
          mode = y;
       }
       if(smallestcount > currentcount) {
          smallestcount = currentcount;
          minmode = y;
       }
       currentcount = 0;
    }
    oldy = y;
  }
  printf("%zu -- %zu %f -- %zu %f --  %f \n", counter, largestcount, mode, smallestcount, minmode,  y);
}

void demo25() {
  float oldy = -1;// arbitrary
  float y;
  size_t counter = 0;
  size_t currentcount = 0;
  size_t largestcount = 0;
  size_t smallestcount = 0xFFFFFFFF;
  float mode = -1;
  float minmode = -1;
  for(size_t i = 0; i < (1<<25); i++) {
    uint32_t x = (uint32_t) i;
    y = x / ((float)(1 << 25)) ;
    assert(y >= 0);
    assert(y <= 1);
    currentcount++;
    if(y > oldy) {
       counter++;
       if(currentcount > largestcount) {
          largestcount = currentcount;
          mode = y;
       }
       if(smallestcount > currentcount) {
          smallestcount = currentcount;
          minmode = y;
       }
       currentcount = 0;
    }
    oldy = y;
  }
  printf("%zu -- %zu %f -- %zu %f --  %f \n", counter, largestcount, mode, smallestcount, minmode,  y);
}



int main() {
  size_t counter = 0;
  assert(!isnormal(0.0));
  size_t byexponent[512];
  for(size_t i = 0; i < 512; ++i) byexponent[i] = 0;
  for(size_t i = 0; i <= UINT32_C(0xFFFFFFFF); i++) {
    uint32_t x = (uint32_t) i;
    float y;
    memcpy(&y, &x, sizeof(float));
    if((y >= 0) && ( y <= 1) && isnormal(y)) {
       counter++;
       byexponent[x>>23]++;
    }
  }
  printf("%zu \n", counter);
  for(size_t i = 0; i < 512; ++i) printf("%zu %zu \n", i, byexponent[i]);

  printf("====\n");
  demo();
  demo24();
  demo25();
 }
