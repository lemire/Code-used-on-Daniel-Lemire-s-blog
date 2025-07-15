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


  // AVX2 dot product
#if defined(__AVX2__)
float __attribute__ ((noinline)) avx2_dot(const float *a, const float *b, size_t n) {
  __m256 vsum1 = _mm256_setzero_ps();
  __m256 vsum2 = _mm256_setzero_ps();
  __m256 vsum3 = _mm256_setzero_ps();
  __m256 vsum4 = _mm256_setzero_ps();

  size_t i = 0;
  for (; i + 32 <= n; i += 32) {
    __m256 va = _mm256_loadu_ps(a + i);
    __m256 vb = _mm256_loadu_ps(b + i);
    vsum1 = _mm256_fmadd_ps(va, vb, vsum1);

    va = _mm256_loadu_ps(a + i + 8);
    vb = _mm256_loadu_ps(b + i + 8);
    vsum2 = _mm256_fmadd_ps(va, vb, vsum2);

    va = _mm256_loadu_ps(a + i + 16);
    vb = _mm256_loadu_ps(b + i + 16);
    vsum3 = _mm256_fmadd_ps(va, vb, vsum3);

    va = _mm256_loadu_ps(a + i + 24);
    vb = _mm256_loadu_ps(b + i + 24);
    vsum4 = _mm256_fmadd_ps(va, vb, vsum4);
  }
  vsum1 = _mm256_add_ps(vsum1, vsum2);
  vsum2 = _mm256_add_ps(vsum3, vsum4);

  for (; i + 16 <= n; i += 16) {
    __m256 va = _mm256_loadu_ps(a + i);
    __m256 vb = _mm256_loadu_ps(b + i);
    vsum1 = _mm256_fmadd_ps(va, vb, vsum1);

    va = _mm256_loadu_ps(a + i + 8);
    vb = _mm256_loadu_ps(b + i + 8);
    vsum2 = _mm256_fmadd_ps(va, vb, vsum2);
  }
  __m256 vsum = _mm256_add_ps(vsum1, vsum2);
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
float __attribute__ ((noinline)) neon_dot(const float *a, const float *b, size_t n) {
  float32x4_t vsum1 = vdupq_n_f32(0.0f);
  float32x4_t vsum2 = vdupq_n_f32(0.0f);
  float32x4_t vsum3 = vdupq_n_f32(0.0f);
  float32x4_t vsum4 = vdupq_n_f32(0.0f);

  size_t i = 0;
  for (; i + 16 <= n; i += 16) {
    float32x4_t va = vld1q_f32(a + i);
    float32x4_t vb = vld1q_f32(b + i);
    vsum1 = vmlaq_f32(vsum1, va, vb);

    va = vld1q_f32(a + i + 4);
    vb = vld1q_f32(b + i + 4);
    vsum2 = vmlaq_f32(vsum2, va, vb);

    va = vld1q_f32(a + i + 8);
    vb = vld1q_f32(b + i + 8);
    vsum3 = vmlaq_f32(vsum3, va, vb);

    va = vld1q_f32(a + i + 12);
    vb = vld1q_f32(b + i + 12);
    vsum4 = vmlaq_f32(vsum4, va, vb);
  }

  vsum1 = vaddq_f32(vsum1, vsum2);
  vsum3 = vaddq_f32(vsum3, vsum4);
  float32x4_t vsum = vaddq_f32(vsum1, vsum3);

  float sum = vgetq_lane_f32(vsum, 0) + vgetq_lane_f32(vsum, 1) + vgetq_lane_f32(vsum, 2) + vgetq_lane_f32(vsum, 3);
  for (; i < n; i++) {
    sum += a[i] * b[i];
  }
  return sum;
};
#endif
#if defined(__AVX512F__)
// AVX-512 dot product
float __attribute__ ((noinline)) avx512_dot(const float *a, const float *b, size_t n) {
  __m512 vsum1 = _mm512_setzero_ps();
  __m512 vsum2 = _mm512_setzero_ps();
  __m512 vsum3 = _mm512_setzero_ps();
  __m512 vsum4 = _mm512_setzero_ps();

  size_t i = 0;
  for (; i + 16*4 <= n; i += 16*4) {
    __m512 va = _mm512_loadu_ps(a + i);
    __m512 vb = _mm512_loadu_ps(b + i);
    vsum1 = _mm512_fmadd_ps(va, vb, vsum1);
    va = _mm512_loadu_ps(a + i + 16);
    vb = _mm512_loadu_ps(b + i + 16);
    vsum2 = _mm512_fmadd_ps(va, vb, vsum2);
    va = _mm512_loadu_ps(a + i + 32);
    vb = _mm512_loadu_ps(b + i + 32);
    vsum3 = _mm512_fmadd_ps(va, vb, vsum3);
    va = _mm512_loadu_ps(a + i + 48);
    vb = _mm512_loadu_ps(b + i + 48);
    vsum4 = _mm512_fmadd_ps(va, vb, vsum4);
  };
  for (; i + 16 <= n; i += 16) {
    __m512 va = _mm512_loadu_ps(a + i);
    __m512 vb = _mm512_loadu_ps(b + i);
    vsum1 = _mm512_fmadd_ps(va, vb, vsum1);
  };
  __m512 vsum = _mm512_add_ps(vsum1, vsum2);
  vsum = _mm512_add_ps(vsum, vsum3);
  vsum = _mm512_add_ps(vsum, vsum4);
  // Handle tail (remaining elements < 16) with masked load
  if (i < n) {
      // Create mask for remaining elements
      __mmask16 mask = (1U << (n - i)) - 1;
      // Load remaining elements with zero-masking for unused lanes
      __m512 va = _mm512_maskz_loadu_ps(mask, a + i);
      __m512 vb = _mm512_maskz_loadu_ps(mask, b + i);
      // Accumulate into vsum
      vsum = _mm512_fmadd_ps(va, vb, vsum);
  }
  float total = _mm512_reduce_add_ps(vsum);
  return total;
};
#endif

int main(int argc, char **argv) {
  constexpr size_t num_values = 100'000;
  constexpr size_t alignment = 8;

  // Aligned allocation
  float *a = (float *)aligned_alloc(alignment, (num_values + alignment) * sizeof(float));
  float *b = (float *)aligned_alloc(alignment, (num_values + alignment) * sizeof(float));
  for (size_t i = 0; i < num_values + alignment; i++) {
    a[i] = 0.0f; // Initialize to zero
    b[i] = 0.0f; // Initialize to zero
  }
  std::printf("Benchmarking dot product with %zu values, total memory usage is %.1f MB\n", num_values, 2 * num_values * sizeof(float) / 1024.0 / 1024.0);



  struct Row {
    size_t byte_offset;
    double ns_per_float;
    double ins_per_float;
    double ins_per_cycle;
  };
  std::vector<Row> results_avx512;
  std::vector<Row> results_avx2;
  std::vector<Row> results_neon;

  for (size_t byte_offset = 0; byte_offset < alignment; ++byte_offset) {
    const float* a_offset = reinterpret_cast<const float*>(reinterpret_cast<const uint8_t*>(a) + byte_offset);
    const float* b_offset = reinterpret_cast<const float*>(reinterpret_cast<const uint8_t*>(b) + byte_offset);

#if defined(__AVX512F__)
    event_aggregate agg_avx512 = bench([&]() {
      volatile float result = avx512_dot(a_offset, b_offset, num_values);
      (void)result;
    });
    double ns_per_float_avx512 = agg_avx512.fastest_elapsed_ns() / static_cast<double>(num_values);
    double ins_per_float_avx512 = agg_avx512.fastest_instructions() / static_cast<double>(num_values);
    double ins_per_cycle_avx512 = agg_avx512.fastest_cycles() > 0 ? agg_avx512.fastest_instructions() / agg_avx512.fastest_cycles() : 0.0;
    results_avx512.push_back(Row{byte_offset, ns_per_float_avx512, ins_per_float_avx512, ins_per_cycle_avx512});
#endif

#if defined(__AVX2__)
    event_aggregate agg_avx2 = bench([&]() {
      volatile float result = avx2_dot(a_offset, b_offset, num_values);
      (void)result;
    });
    double ns_per_float_avx2 = agg_avx2.fastest_elapsed_ns() / static_cast<double>(num_values);
    double ins_per_float_avx2 = agg_avx2.fastest_instructions() / static_cast<double>(num_values);
    double ins_per_cycle_avx2 = agg_avx2.fastest_cycles() > 0 ? agg_avx2.fastest_instructions() / agg_avx2.fastest_cycles() : 0.0;
    results_avx2.push_back(Row{byte_offset, ns_per_float_avx2, ins_per_float_avx2, ins_per_cycle_avx2});
#endif

#if defined(__ARM_NEON)
    event_aggregate agg_neon = bench([&]() {
      volatile float result = neon_dot(a_offset, b_offset, num_values);
      (void)result;
    });
    double ns_per_float_neon = agg_neon.fastest_elapsed_ns() / static_cast<double>(num_values);
    double ins_per_float_neon = agg_neon.fastest_instructions() / static_cast<double>(num_values);
    double ins_per_cycle_neon = agg_neon.fastest_cycles() > 0 ? agg_neon.fastest_instructions() / agg_neon.fastest_cycles() : 0.0;
    results_neon.push_back(Row{byte_offset, ns_per_float_neon, ins_per_float_neon, ins_per_cycle_neon});
#endif
}

#if defined(__ARM_NEON)
fmt::print("NEON Results:\n");
fmt::print("| Byte Offset | ns/float | ins/float | instruction/cycle |\n");
fmt::print("|------------:|---------:|----------:|------------------:|\n");
for (const auto& row : results_neon) {
  fmt::print("| {:12} | {:9.2f} | {:10.2f} | {:17.2f} |\n", row.byte_offset, row.ns_per_float, row.ins_per_float, row.ins_per_cycle);
}
#elif defined(__AVX512F__) && defined(__AVX2__)
fmt::print("AVX-512 Results:\n");
fmt::print("| Byte Offset | ns/float | ins/float | instruction/cycle |\n");
fmt::print("|------------:|---------:|----------:|------------------:|\n");
for (const auto& row : results_avx512) {
  fmt::print("| {:12} | {:9.2f} | {:10.2f} | {:17.2f} |\n", row.byte_offset, row.ns_per_float, row.ins_per_float, row.ins_per_cycle);
}

fmt::print("AVX2 Results:\n");
fmt::print("| Byte Offset | ns/float | ins/float | instruction/cycle |\n");
fmt::print("|------------:|---------:|----------:|------------------:|\n");
for (const auto& row : results_avx2) {
  fmt::print("| {:12} | {:9.2f} | {:10.2f} | {:17.2f} |\n", row.byte_offset, row.ns_per_float, row.ins_per_float, row.ins_per_cycle);
}
#elif defined(__AVX2__)
fmt::print("AVX2 Results:\n");
fmt::print("| Byte Offset | ns/float | ins/float | instruction/cycle |\n");
fmt::print("|------------:|---------:|----------:|------------------:|\n");
for (const auto& row : results_avx2) {
  fmt::print("| {:12} | {:9.2f} | {:10.2f} | {:17.2f} |\n", row.byte_offset, row.ns_per_float, row.ins_per_float, row.ins_per_cycle);
}
#endif

  free(a);
  free(b);
}
