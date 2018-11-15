// gcc -O2 -o twomods twomods.c -fno-vectorize

#include <memory.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define d -7
//int32_t d = -7;;// = ...;// your non-zero divisor in [-2147483647,2147483647]
int32_t pd =  d < 0 ? -d : d; // absolute value, abs(d)
uint32_t upd =  d < 0 ? -d : d; // absolute value, abs(d)

// c = floor( (1<<64) / pd ) + 1; Take L = N + 1
uint64_t c;// = UINT64_C(0xFFFFFFFFFFFFFFFF) / pd
             //     + 1 + ((pd & (pd-1))==0 ? 1 : 0);

int32_t fastmodu(int32_t n /* , uint64_t c, int32_t pd */) {
  uint64_t lowbits = c * n;
  int32_t highbits = ((__uint128_t) lowbits * pd) >> 64;
  // answer is equivalent to (n<0) ? highbits - 1 + d : highbits
  return highbits - ((upd - 1) & (n >> 31));
}
// fastmod computes (n mod d) given precomputed c
int32_t fastmod(int32_t n /* , uint64_t c, int32_t pd */) {
  uint64_t lowbits = c * n;
  int32_t highbits = ((__uint128_t) lowbits * pd) >> 64;
  // answer is equivalent to (n<0) ? highbits - 1 + d : highbits
  return highbits - ((pd - 1) & (n >> 31));
}

int main() {
  c = UINT64_C(0xFFFFFFFFFFFFFFFF) / pd
                  + 1 + ((pd & (pd-1))==0 ? 1 : 0);
  for(int32_t x = -1000; x < 1000; x++) {
    int32_t mod1 = fastmodu(x);
    int32_t mod2 = fastmod(x);
    int32_t mod3 = x % d;
    if((mod1 != mod2) || (mod2 != mod3)) {
      printf("bug");
      abort();
    }
  }
  clock_t begin_time, end_time;
  begin_time = clock();

  int32_t mod1  = 0;
  for(int32_t x = -10000000; x < 10000000; x++) {
     mod1 += fastmodu(x);
  }
  end_time = clock();
  printf("%zu \n", end_time-begin_time);

  begin_time = clock();
  int32_t mod2  = 0;
  for(int32_t x = -10000000; x < 10000000; x++) {
     mod2 += fastmod(x);
  }
  end_time = clock();
  printf("%zu \n", end_time-begin_time);
  if(mod1 != mod2) printf("bug\n");

  begin_time = clock();
  int32_t mod3  = 0;
  for(int32_t x = -10000000; x < 10000000; x++) {
     mod3 += x % d;
  }
  end_time = clock();
  printf("%zu \n", end_time-begin_time);
  if(mod1 != mod3) printf("bug\n");
}
