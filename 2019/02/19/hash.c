#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "benchmark.h"

#define D 21
#define M ((uint64_t)(UINT64_C(0xFFFFFFFFFFFFFFFF) / (D) + 1))

#define INITSEED 1234
#define MULTIPLIER 31
#define CONSTANTADD 27961
#define REPEAT 1000000

static inline uint32_t compilermod32(uint32_t a) {
  return a % D;
}

static inline uint32_t directmod64(uint32_t a) {
  uint64_t lowbits = M * a;
  return ((__uint128_t)lowbits * D) >> 64;
}

static inline uint32_t indirectmod64(uint32_t a) {
  uint64_t quotient = ( (__uint128_t) M * a ) >> 64;// this is the division
  return a - quotient * D;
}


uint32_t compiler() {
  uint32_t seed = INITSEED;
  for(size_t i = 0; i < REPEAT; i++) {
    seed = compilermod32(seed * MULTIPLIER + CONSTANTADD) ;
  }
  return seed;
}

uint32_t direct() {
  uint32_t seed = INITSEED;
  for(size_t i = 0; i < REPEAT; i++) {
    seed = directmod64(seed * MULTIPLIER + CONSTANTADD) ;
  }
  return seed;
}

uint32_t indirect() {
  uint32_t seed = INITSEED;
  for(size_t i = 0; i < REPEAT; i++) {
    seed = indirectmod64(seed * MULTIPLIER + CONSTANTADD) ;
  }
  return seed;
}

void demo() {
  bool verbose = true;
  int repeat = 50;
  uint32_t expected = indirect();
  BEST_TIME(compiler(), expected, , repeat, REPEAT, verbose);
  BEST_TIME(direct(), expected, , repeat, REPEAT, verbose);
  BEST_TIME(indirect(), expected, , repeat, REPEAT, verbose);
}

int main() {
  demo();

}
