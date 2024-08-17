
/**
 * This header contains C++ templates that shuffle the elements in the range
 * [first, last) using the random number generator g. They are meant to emulate
 * the standard std::shuffle function and can often act as drop-in replacement.
 */
#ifndef TEMPLATE_SHUFFLE_H
#define TEMPLATE_SHUFFLE_H

#include <cstdint>
namespace batched_random {
/** 
 * Nevin Brackett-Rozinsky, Daniel Lemire, Batched Ranged Random Integer Generation, Software: Practice and Experience (to appear) 
 * Daniel Lemire, Fast Random Integer Generation in an Interval, ACM Transactions on Modeling and Computer Simulation, Volume 29 Issue 1, February 2019 
 */
template <class URBG> uint64_t random_bounded(uint64_t range, URBG &&rng) {
  __uint128_t random64bit, multiresult;
  uint64_t leftover;
  uint64_t threshold;
  random64bit = rng();
  multiresult = random64bit * range;
  leftover = (uint64_t)multiresult;
  if (leftover < range) {
    threshold = -range % range;
    while (leftover < threshold) {
      random64bit = rng();
      multiresult = random64bit * range;
      leftover = (uint64_t)multiresult;
    }
  }
  return (uint64_t)(multiresult >> 64); // [0, range)
}


// product_bound can be any integer >= range1*range2
// it may be updated to become range1*range2
template <class URBG>
std::pair<uint64_t, uint64_t> random_bounded_2(uint64_t range1, uint64_t range2,
                                               uint64_t &product_bound,
                                               URBG &&rng) {
  __uint128_t random64bit, multiresult;
  uint64_t leftover;
  uint64_t threshold;
  random64bit = rng();
  multiresult = random64bit * range1;
  leftover = (uint64_t)multiresult;
  uint64_t result1 = (uint64_t)(multiresult >> 64); // [0, range1)
  multiresult = leftover * range2;
  leftover = (uint64_t)multiresult;
  uint64_t result2 = (uint64_t)(multiresult >> 64); // [0, range2)
  if (leftover < product_bound) {
    uint64_t product_bound = range2 * range1;
    if (leftover < product_bound) {
      threshold = -product_bound % product_bound;
      while (leftover < threshold) {
        random64bit = rng();
        multiresult = random64bit * range1;
        leftover = (uint64_t)multiresult;
        result1 = (uint64_t)(multiresult >> 64); // [0, range1)
        multiresult = leftover * range2;
        leftover = (uint64_t)multiresult;
        result2 = (uint64_t)(multiresult >> 64); // [0, range2)
      }
    }
  }
  return std::make_pair(result1, result2);
}

// This is a template function that shuffles the elements in the range [first,
// last).
//
// It is similar to std::shuffle, but it uses a different algorithm.
template <class RandomIt, class URBG>
extern void shuffle_2(RandomIt first, RandomIt last, URBG &&g) {
  uint64_t i = std::distance(first, last);
  for (; i > 1 << 30; i--) {
    uint64_t index = random_bounded(i, g); // index is in [0, i-1]
    std::iter_swap(first + i - 1, first + index);
  }

  // Batches of 2 for sizes up to 2^30 elements
  uint64_t product_bound = i * (i - 1);
  for (; i > 1; i -= 2) {
    auto [index1, index2] = random_bounded_2(i, i - 1, product_bound, g);
    // index1 is in [0, i-1]
    // index2 is in [0, i-2]
    std::iter_swap(first + i - 1, first + index1);
    std::iter_swap(first + i - 2, first + index2);
  }
}

} // namespace batched_random

#endif // TEMPLATE_SHUFFLE_H
