// this C code is just meant to benchmark speed. The function selectCoPrimeResev is not randomized. TODO: randomize it.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "benchmark.h"

static inline int gcd(uint32_t u, uint32_t v) {
  int shift;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
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

static inline bool coprime(uint32_t u, uint32_t v) { return gcd(u, v) == 1; }

static uint32_t selectCoPrimeResev(uint32_t min, uint32_t target) {
  for (uint32_t val = min; val < target; ++val) {
    if (coprime(val, target)) {
      return val;
    };
  }
  return 0;
}

static uint32_t coPrime(uint32_t range) {
  // if(range < 2) // trouble
  return selectCoPrimeResev(range / 2, range);
}

static inline int getMapped(uint32_t index, uint32_t prime, uint32_t offset,
                            uint32_t maxrange) {
  return ((uint64_t)index * prime + offset) % maxrange;
}

static void shuffle(int *source, int *target, uint32_t size) {
  uint32_t prime = coPrime(size);
  uint32_t offset = (rand() >> 1) % size;
  for (int index = 0; index < size; index++)
    target[index] = source[getMapped(index, prime, offset, size)];
}

void shuffleReallyFast(int *source, int *target, uint32_t size) {
  uint32_t prime = coPrime(size);
  uint32_t initoffset = (rand() >> 1) % size;
  uint32_t offset = initoffset;
  assert(offset == getMapped(0, prime, offset, size));

  for (int index = 0; index < size; index++) {
    target[index] = source[offset];
    uint32_t newoff = offset + prime;
    offset = (newoff < size) ? newoff : newoff - size;
  }
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
  uint32_t pot = 2;
  uint32_t answer = 1;
  while ((uint64_t)pot <= val / 2) {
    if (IsPrime(pot)) {
      answer *= pot;
      while ((val % pot) == 0) {
        val /= pot;
      }
    }
  }
  return answer;
}

static inline uint32_t nextPower(uint32_t size) {
  uint32_t target = 1;
  while (target < size) {
    target *= 2;
  }
  return target;
}

static void shuffleLCG(int *source, int *target, uint32_t size) {
  uint32_t size2 = nextPower(size);
  uint32_t minb = productOfAllPrimeDivisors(size2);
  uint32_t b = minb;
  if ((size2 % 4) == 0) {
    while ((b % 4) != 0) {
      b *= 2;
    }
  }
  uint32_t a = b + 1;
  uint32_t lcg = (rand() >> 1) % size;
  for (int index = 0; index < size; index++) {
    target[index] = source[lcg];
    lcg = (a * lcg + 1) & (size2 - 1);
    while (lcg >= size) {
      lcg = (a * lcg + 1) & (size2 - 1);
    }
  }
}

#define REPEAT 50

void demo(size_t N, int *target) {
  srand(1234);
  uint32_t size = N;
  int repeat = REPEAT;
  int *source = malloc(N * sizeof(int));
  for (int k = 0; k < size; k++)
    source[k] = k;
  BEST_TIME_NOCHECK(shuffle(source, target, size), , repeat, size, false);
  free(source);
}

void demoReallyFast(size_t N, int *target) {
  srand(1234);
  uint32_t size = N;
  int repeat = REPEAT;
  int *source = malloc(N * sizeof(int));
  for (int k = 0; k < size; k++)
    source[k] = k;
  BEST_TIME_NOCHECK(shuffleReallyFast(source, target, size), , repeat, size,
                    true);
  free(source);
}

void demoLCG(size_t N, int *target) {
  srand(time(NULL));
  uint32_t size = N;
  int repeat = REPEAT;
  int *source = malloc(N * sizeof(int));
  for (int k = 0; k < size; k++)
    source[k] = k;
  BEST_TIME_NOCHECK(shuffleLCG(source, target, size), , repeat, size, true);
  free(source);
}

int main() {
  for (int N = 500; N <= 10000000; N *= 7) {
    printf("N = %d \n", N);
    int *target = malloc(N * sizeof(int));

    demo(N, target);
    int *target2 = malloc(N * sizeof(int));
    demoReallyFast(N, target2);
    for (int k = 0; k < N; k++)
      if (target[k] != target2[k]) {
        printf("bug\n");
        return -1;
      }
    demoLCG(N, target);
    free(target);
    free(target2);
  }
  return 0;
}
