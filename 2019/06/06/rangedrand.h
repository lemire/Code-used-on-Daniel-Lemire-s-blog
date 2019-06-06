#ifndef RANGEDRAND_H
#define RANGEDRAND_H
#include <random>
#include <stdint.h>

#include "lehmer64.h"
#include "splitmix64.h"

static void setseed(uint64_t seed) {
  splitmix64_seed(seed);
  lehmer64_seed(seed);
}

static inline uint32_t lehmer64_32(void) { return (uint32_t)lehmer64(); }


std::mt19937 mersenne;

static inline uint32_t twister32(void) { return (uint32_t)mersenne(); }

typedef uint64_t (*randfnc64)(void);

typedef uint32_t (*randfnc32)(void);

// wrapper to satisfy fancy C++
template <randfnc32 RandomBitGenerator>
struct UniformRandomBitGenerator32Struct {
  typedef uint32_t result_type;
  static constexpr result_type min() { return 0; }
  static constexpr result_type max() { return UINT32_MAX; }
  result_type operator()() { return RandomBitGenerator(); }
};

// wrapper to satisfy fancy C++
template <randfnc64 RandomBitGenerator>
struct UniformRandomBitGenerator64Struct {
  typedef uint64_t result_type;
  static constexpr result_type min() { return 0; }
  static constexpr result_type max() { return UINT64_MAX; }
  result_type operator()() { return RandomBitGenerator(); }
};

// return value in [0,bound)
template <randfnc32 RandomBitGenerator>
static inline uint32_t naive_random_bounded32(uint32_t bound) {
  return RandomBitGenerator() % bound;
}

template <randfnc64 RandomBitGenerator>
static inline uint64_t naive_random_bounded64(uint64_t bound) {
  return RandomBitGenerator() % bound;
}

// return value in [0,bound)
// as per the PCG implementation, uses two 32-bit divisions
template <randfnc32 RandomBitGenerator>
static inline uint32_t random_bounded32(uint32_t bound) {
  uint32_t threshold = (~bound + 1) % bound; // -bound % bound
  for (;;) {
    uint32_t r = RandomBitGenerator();
    if (r >= threshold)
      return r % bound;
  }
}

template <randfnc64 RandomBitGenerator>
static inline uint64_t random_bounded64(uint64_t bound) {
  uint64_t threshold = (~bound + 1) % bound; // -bound % bound
  for (;;) {
    uint64_t r = RandomBitGenerator();
    if (r >= threshold)
      return r % bound;
  }
}

template <randfnc32 RandomBitGenerator>
static inline uint32_t floatmult_random_bounded32(uint32_t bound) {
  return (uint32_t)((float)(RandomBitGenerator() & 0xffffff) /
                    (float)(1 << 24) * bound);
}

template <randfnc64 RandomBitGenerator>
static inline uint64_t floatmult_random_bounded64(uint64_t bound) {
  return (uint64_t)(
      (double)(RandomBitGenerator() & UINT64_C(0x1fffffffffffff)) /
      (double)(UINT64_C(1) << 53) * bound);
}

// return value in [0,bound)
// as per the Java implementation, uses one or more 32-bit divisions
template <randfnc32 RandomBitGenerator>
static inline uint32_t java_random_bounded32(uint32_t bound) {
  uint32_t rkey = RandomBitGenerator();
  uint32_t candidate = rkey % bound;
  while (rkey - candidate >
         UINT32_MAX - bound + 1) { // will be predicted as false
    rkey = RandomBitGenerator();
    candidate = rkey % bound;
  }
  return candidate;
}

template <randfnc64 RandomBitGenerator>
static inline uint64_t java_random_bounded64(uint64_t bound) {
  uint64_t rkey = RandomBitGenerator();
  uint64_t candidate = rkey % bound;
  while (rkey - candidate >
         UINT64_MAX - bound + 1) { // will be predicted as false
    rkey = RandomBitGenerator();
    candidate = rkey % bound;
  }
  return candidate;
}

// return value in [0,bound)
// similar to the Go implementation and arc4random_uniform
template <randfnc32 RandomBitGenerator>
static inline uint32_t go_random_bounded32(uint32_t bound) {
  uint32_t bits;
  // optimizing for powers of two is harmful
  uint32_t t = (-bound) % bound; // this is 2^32 % bound
  do {
    bits = RandomBitGenerator();
  } while (bits < t);
  return bits % bound;
}

// return value in [0,bound)
// as per the Go implementation
template <randfnc64 RandomBitGenerator>
static inline uint64_t go_random_bounded64(uint64_t bound) {
  uint64_t bits;
  // optimizing for powers of two is harmful
  uint64_t t = (-bound) % bound;
  do {
    bits = RandomBitGenerator();
  } while (bits < t);
  return bits % bound;
}

// map random value to [0,range), redraws to avoid bias if
// needed
template <randfnc32 RandomBitGenerator>
static inline uint32_t random_bounded_nearlydivisionless32(uint32_t range) {
  uint64_t random32bit, multiresult;
  uint32_t leftover;
  uint32_t threshold;
  random32bit = RandomBitGenerator();
  multiresult = random32bit * range;
  leftover = (uint32_t)multiresult;
  if (leftover < range) {
    threshold = -range % range;
    while (leftover < threshold) {
      random32bit = RandomBitGenerator();
      multiresult = random32bit * range;
      leftover = (uint32_t)multiresult;
    }
  }
  return multiresult >> 32; // [0, range)
}

// map random value to [0,range), redraws to avoid bias if
// needed
template <randfnc64 RandomBitGenerator>
static inline uint64_t random_bounded_nearlydivisionless64(uint64_t range) {
  __uint128_t random64bit, multiresult;
  uint64_t leftover;
  uint64_t threshold;
  random64bit = RandomBitGenerator();
  multiresult = random64bit * range;
  leftover = (uint64_t)multiresult;
  if (leftover < range) {
    threshold = -range % range;
    while (leftover < threshold) {
      random64bit = RandomBitGenerator();
      multiresult = random64bit * range;
      leftover = (uint64_t)multiresult;
    }
  }
  return multiresult >> 64; // [0, range)
}
#endif