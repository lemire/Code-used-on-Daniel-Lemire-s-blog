#ifndef SPLITMIX64_H
#define SPLITMIX64_H

/* Modified by D. Lemire, August 2017 */
/***
Fast Splittable Pseudorandom Number Generators
Steele Jr, Guy L., Doug Lea, and Christine H. Flood. "Fast splittable
pseudorandom number generators."
ACM SIGPLAN Notices 49.10 (2014): 453-472.
***/

/*  Written in 2015 by Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdint.h>

// original documentation by Vigna:
/* This is a fixed-increment version of Java 8's SplittableRandom generator
   See http://dx.doi.org/10.1145/2714064.2660195 and
   http://docs.oracle.com/javase/8/docs/api/java/util/SplittableRandom.html

   It is a very fast generator passing BigCrush, and it can be useful if
   for some reason you absolutely want 64 bits of state; otherwise, we
   rather suggest to use a xoroshiro128+ (for moderately parallel
   computations) or xorshift1024* (for massively parallel computations)
   generator. */

// state for splitmix64
uint64_t splitmix64_x; /* The state can be seeded with any value. */

// call this one before calling splitmix64
static inline void splitmix64_seed(uint64_t seed) { splitmix64_x = seed; }

// returns random number, modifies splitmix64_x
// compared with D. Lemire against
// http://grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/8-b132/java/util/SplittableRandom.java#SplittableRandom.0gamma
static inline uint64_t splitmix64(void) {
  uint64_t z = (splitmix64_x += UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

// returns the 32 least significant bits of a call to splitmix64
// this is a simple function call followed by a cast
static inline uint32_t splitmix64_cast32(void) {
  return (uint32_t)splitmix64();
}

// same as splitmix64, but does not change the state, designed by D. Lemire
static inline uint64_t splitmix64_stateless(uint64_t index) {
  uint64_t z = (index * UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

#endif // SPLITMIX64_H