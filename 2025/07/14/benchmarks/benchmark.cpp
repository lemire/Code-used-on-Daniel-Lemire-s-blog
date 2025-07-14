#include "benchmarker.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#if defined(__ARM_NEON)
#include <arm_neon.h>
#endif
#if defined(__AVX2__)
#include <immintrin.h>
#endif

void pretty_print(const std::string &name, size_t num_values,
                  event_aggregate agg) {
  fmt::print("{:<50} : ", name);
  fmt::print(" {:5.2f} ns ", agg.fastest_elapsed_ns() / num_values);
  if (collector.has_events()) {
    fmt::print(" {:5.2f} GHz ",
               agg.fastest_cycles() / agg.fastest_elapsed_ns());
    fmt::print(" {:5.2f} c ", agg.fastest_cycles() / num_values);
    fmt::print(" {:5.2f} i ", agg.fastest_instructions() / num_values);
    fmt::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / agg.fastest_cycles());
  }
  fmt::print("\n");
}

int main(int argc, char **argv) {
  constexpr size_t num_values = 100'000;
  constexpr size_t alignment = 64;
  std::mt19937_64 rng(42);
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  // Aligned allocation
  float *a = (float *)aligned_alloc(alignment, (num_values + alignment) * sizeof(float));
  float *b = (float *)aligned_alloc(alignment, (num_values + alignment) * sizeof(float));
  for (size_t i = 0; i < num_values + alignment; i++) {
    a[i] = dist(rng);
    b[i] = dist(rng);
  }


  // AVX2 dot product
#if defined(__AVX2__)
  auto avx2_dot = [](const float *a, const float *b, size_t n) -> float {
    __m256 vsum = _mm256_setzero_ps();
    size_t i = 0;
    for (; i + 8 <= n; i += 8) {
      __m256 va = _mm256_loadu_ps(a + i);
      __m256 vb = _mm256_loadu_ps(b + i);
      vsum = _mm256_fmadd_ps(va, vb, vsum);
    }
    float sum[8];
    _mm256_storeu_ps(sum, vsum);
    float total = sum[0] + sum[1] + sum[2] + sum[3] + sum[4] + sum[5] + sum[6] + sum[7];
    for (; i < n; i++) {
      total += a[i] * b[i];
    }
    return total;
  };
#endif
#if defined(__ARM_NEON)

  // NEON dot product
  auto neon_dot = [](const float *a, const float *b, size_t n) -> float {
    float32x4_t vsum = vdupq_n_f32(0.0f);
    size_t i = 0;
    for (; i + 4 <= n; i += 4) {
      float32x4_t va = vld1q_f32(a + i);
      float32x4_t vb = vld1q_f32(b + i);
      vsum = vmlaq_f32(vsum, va, vb);
    }
    float sum = vgetq_lane_f32(vsum, 0) + vgetq_lane_f32(vsum, 1) + vgetq_lane_f32(vsum, 2) + vgetq_lane_f32(vsum, 3);
    for (; i < n; i++) {
      sum += a[i] * b[i];
    }
    return sum;
  };
#endif

  // Test offsets in bytes (from 0 to alignment-1)
  for (size_t byte_offset = 0; byte_offset < alignment; ++byte_offset) {
    const float* a_offset = reinterpret_cast<const float*>(reinterpret_cast<const uint8_t*>(a) + byte_offset);
    const float* b_offset = reinterpret_cast<const float*>(reinterpret_cast<const uint8_t*>(b) + byte_offset);
#if defined(__AVX2__)
    pretty_print(fmt::format("avx2_dot byte offset {}", byte_offset), num_values,
                 bench([&]() {
                   volatile float result = avx2_dot(a_offset, b_offset, num_values);
                   (void)result;
                 }));
#elif defined(__ARM_NEON__)
    pretty_print(fmt::format("neon_dot byte offset {}", byte_offset), num_values,
                 bench([&]() {
                   volatile float result = neon_dot(a_offset, b_offset, num_values);
                   (void)result;
                 }));
#endif
  }

  free(a);
  free(b);
}
