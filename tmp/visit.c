
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "benchmark.h"

static inline  int gcd(uint32_t u, uint32_t v) {
       int shift;
       if (u == 0) return v;
       if (v == 0) return u;
       for (shift = 0; ((u | v) & 1) == 0; ++shift) {
        u >>= 1;
        v >>= 1;
       }

       while ((u & 1) == 0)
        u >>= 1;

      do {
        while ((v & 1) == 0)
            v >>= 1;
        if (u > v) {
            int t = v;
            v = u;
            u = t;
        }
        v = v - u;
      } while (v != 0);
      return u << shift;
}

static inline bool coprime(uint32_t u, uint32_t v) {
       return gcd(u,v) == 1;
}


static uint32_t selectCoPrimeResev(uint32_t min, uint32_t target) {
      for(uint32_t val = min; val < target; ++val) {
        if(coprime(val,target)) {
          return val;
        };
      }
      return 0;
}

static uint32_t coPrime(uint32_t range) {
  //if(range < 2) // trouble
  return selectCoPrimeResev(range/2, range);
}

static inline int getMapped(uint32_t index, uint32_t prime, uint32_t offset, uint32_t maxrange) {
  return (index * prime + offset) % maxrange;
}

static void shuffle(int * source, int * target, uint32_t size) {
  uint32_t prime = coPrime(size);
  uint32_t offset = (rand() >> 1)% size;
  for(int  index = 0; index < size; index++)
    target[index] = source[getMapped(index, prime, offset, size)];
}

static void shuffleReallyFast(int * source, int * target, uint32_t size) {
  uint32_t prime = coPrime(size);
  assert(prime<size);
  uint32_t offset = (rand() >> 1)% size;
  //printf("orig offset = %d\n", offset);

  for(int  index = 0; index < size; index++) {
  //printf("offset at %d  offset = %d\n", index, offset);

    target[index] = source[offset];
    offset += prime;
    offset -= (((int32_t)size - (int32_t)size) >> 31) & size;
  }
}

void shuffleReallyFast2(int * source, int * target, uint32_t size) {
  uint32_t prime = coPrime(size);
  uint32_t offset = (rand() >> 1)% size;
  for(int  index = 0; index < size; index++) {
    target[index] = source[offset];
    uint32_t newoff = offset + prime;
    offset = (newoff < size) ? newoff : newoff - size;
  }
}
// M = ceil( (1<<64) / d ), d > 0
static inline uint64_t computeM_u32(uint32_t d) {
  return UINT64_C(0xFFFFFFFFFFFFFFFF) / d + 1;
}

// fastmod computes (a % d) given precomputed M
static inline uint32_t fastmod_u32(uint32_t a, uint64_t M, uint32_t d) {
  uint64_t lowbits = M * a;
  return ((__uint128_t)lowbits * d) >> 64;
}

static inline int getMappedFast(uint32_t index, uint32_t prime, uint32_t offset, uint32_t maxrange, uint64_t M) {
  return fastmod_u32(index * prime + offset,M, maxrange);
}

static void shuffleFast(int * source, int * target, uint32_t size) {
  uint32_t prime = coPrime(size);
  uint32_t offset = (rand() >> 1)% size;
  uint64_t M = computeM_u32(size);
  for(int  index = 0; index < size; index++)
    target[index] = source[getMappedFast(index, prime, offset, size, M)];
}


static bool IsPrime(uint32_t x) {
        if ((x & 1) != 0) { // odd case
          uint32_t limit = sqrt(x);
          for (int div = 3; div <= limit; div += 2) {
                  if ((x % div) == 0)
                      return false;
          }
          return true;
        }
        // x is even
        if (x == 2) {
          return true;
        }
        return false;
}


static uint32_t productOfAllPrimeDivisors(uint32_t val) {
  uint32_t pot = 2 ;
  uint32_t answer = 1;
  while((uint64_t) pot <= val/2) {
    if(IsPrime(pot)) {
      answer *= pot;
      while((val % pot)  == 0) {
        val /= pot;
      }
    }
  }
  return answer;
}

static inline uint32_t nextPower(uint32_t size) {
  uint32_t target = 1;
  while(target < size) {
    target *= 2;
  }
  return target;
}


static void shuffleLCG(int * source, int * target, uint32_t size) {
  uint32_t size2 = nextPower(size);
  uint32_t minb = productOfAllPrimeDivisors(size2);
  uint32_t b = minb;
  if((size2 % 4) == 0) {
    while((b % 4) != 0) {
      b *= 2;
    }
  }
  uint32_t a = b + 1;
  uint32_t lcg = (rand()>>1) % size;
  for(int  index = 0; index < size; index++) {
    target[index] = source[lcg];
    lcg = ( a * lcg + 1) & (size2 - 1);
    while(lcg >= size) {
      lcg = ( a * lcg + 1) & (size2 - 1);
    }
  }
}

#define REPEAT 50

void demo(size_t N, int * target) {
  srand(1234);
  uint32_t size = N;
  int repeat = REPEAT;
  int * source = malloc(N * sizeof(int));
  for(int k = 0; k < size; k++)source[k] = k;
  BEST_TIME_NOCHECK(shuffle(source,target,size),,repeat,size,true);
  free(source);
}

void demoReallyFast(size_t N, int * target) {
  srand(1234);
  uint32_t size = N;
  int repeat = REPEAT;
  int * source = malloc(N * sizeof(int));
  for(int k = 0; k < size; k++)source[k] = k;
  BEST_TIME_NOCHECK(shuffleReallyFast(source,target,size),,repeat,size,true);
  free(source);
}
void demoReallyFast2(size_t N, int * target) {
  srand(1234);
  uint32_t size = N;
  int repeat = REPEAT;
  int * source = malloc(N * sizeof(int));
  for(int k = 0; k < size; k++)source[k] = k;
  BEST_TIME_NOCHECK(shuffleReallyFast2(source,target,size),,repeat,size,true);
  free(source);
}
void demoFast(size_t N, int * target) {
  srand(time(NULL));
  uint32_t size = N;
  int repeat = REPEAT;
  int * source = malloc(N * sizeof(int));
  for(int k = 0; k < size; k++)source[k] = k;
  BEST_TIME_NOCHECK(shuffleFast(source,target,size),,repeat,size,true);
  free(source);
}
void demoLCG(size_t N, int * target) {
  srand(time(NULL));
  uint32_t size = N;
  int repeat = REPEAT;
  int * source = malloc(N * sizeof(int));
  for(int k = 0; k < size; k++)source[k] = k;
  BEST_TIME_NOCHECK(shuffleLCG(source,target,size),,repeat,size,true);
  free(source);
}

int main() {

  for(int N = 7 ; N <= 1000000; N *=7) {
    printf("N = %d \n", N);
    int * target = malloc(N * sizeof(int));

    demo(N, target);
    printf("\n");
    int * target2 = malloc(N * sizeof(int));
  //  demoReallyFast(N, target2);

  //  for(int k = 0; k < N; k++) if(target[k]!= target2[k]) {printf("bug\n");return -1;}

demoReallyFast2(N, target2);

for(int k = 0; k < N; k++) if(target[k]!= target2[k]) {printf("bug\n");return -1;}
    demoFast(N, target);
    demoLCG(N, target);
    free(target);
    free(target2);

  }
  return 0;
}
