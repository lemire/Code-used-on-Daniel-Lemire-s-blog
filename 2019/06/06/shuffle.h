#include "rangedrand.h"
#include <stdint.h>
#define BUFFERSIZE32 256
#define BUFFERSIZE64 256


// good old Fisher-Yates shuffle, shuffling an array of integers, uses java-like
// ranged rng
template <randfnc64 UniformRandomBitGenerator>
void shuffle_java64(uint32_t *storage, uint64_t size) {
  uint64_t i;
  for (i = size; i > 1; i--) {
    uint64_t nextpos = java_random_bounded64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}


// good old Fisher-Yates shuffle, shuffling an array of integers
template <randfnc64 UniformRandomBitGenerator>
void shuffle_floatmult64(uint32_t *storage, uint64_t size) {
  uint64_t i;
  for (i = size; i > 1; i--) {
    uint64_t nextpos = floatmult_random_bounded64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}


template <randfnc64 UniformRandomBitGenerator>
void shuffle_go64(uint32_t *storage, uint64_t size) {
  uint64_t i;
  for (i = size; i > 1; i--) {
    uint64_t nextpos = go_random_bounded64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

// good old Fisher-Yates shuffle, shuffling an array of integers, without
// division
template <randfnc64 UniformRandomBitGenerator>
void shuffle_nearlydivisionless64(uint32_t *storage, uint64_t size) {
  uint64_t i;
  for (i = size; i > 1; i--) {
    uint64_t nextpos =
        random_bounded_nearlydivisionless64<UniformRandomBitGenerator>(i);
    uint64_t tmp = storage[i - 1];   // likely in cache
    uint64_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

