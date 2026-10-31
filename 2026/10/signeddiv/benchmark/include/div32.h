// Signed 32-bit division by a constant d > 1 (not a power of two), rounding
// toward zero like x / d, using a 64-bit multiplication.
//
// See Lemire, Bartlett, Kaser, Integer division by constants: optimal bounds,
// Heliyon 7 (6), 2021. https://arxiv.org/abs/2012.12369
#pragma once
#include <bit>
#include <cstdint>

template <int32_t d> int32_t div32(int32_t x) {
  static_assert(d > 1 && (d & (d - 1)) != 0, "d > 1 must not be a power of two");
  // We have |x| <= 2^31. With 2^s >= d * 2^31, the multiplier c = ceiling(2^s / d)
  // gives floor(c * |x| / 2^s) = floor(|x| / d) (Theorem 1). The multiplier
  // has 33 bits, and the product fits in 64 bits.
  constexpr int s = 31 + std::bit_width(uint32_t(d - 1)); // 31 + ceiling(log2(d))
  constexpr int64_t c = (int64_t(1) << s) / d + 1;        // ceiling(2^s / d)
  // floor(c * x / 2^s), plus one when x is negative
  return int32_t((int64_t(x) * c) >> s) - (x >> 31);
}
