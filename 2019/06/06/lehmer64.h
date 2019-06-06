#ifndef LEHMER64_H
#define LEHMER64_H

#include "splitmix64.h"

static __uint128_t g_lehmer64_state;

/**
* D. H. Lehmer, Mathematical methods in large-scale computing units.
* Proceedings of a Second Symposium on Large Scale Digital Calculating
* Machinery;
* Annals of the Computation Laboratory, Harvard Univ. 26 (1951), pp. 141-146.
*/

static inline void lehmer64_seed(uint64_t seed) {
  g_lehmer64_state = (((__uint128_t)splitmix64_stateless(seed)) << 64) +
                     splitmix64_stateless(seed + 1);
}

static inline uint64_t lehmer64() {
  g_lehmer64_state *= UINT64_C(0xda942042e4dd58b5);
  ;
  return g_lehmer64_state >> 64;
}

#endif
