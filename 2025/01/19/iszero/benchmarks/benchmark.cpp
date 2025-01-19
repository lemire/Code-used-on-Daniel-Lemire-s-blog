
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <arm_neon.h>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <vector>

enum number_types {
  MFP_NORMAL_NON_ZERO = 0,
  MFP_SUBNORMAL = 1,
  MFP_ZERO = 2,
  MFP_INFINITE = 3,
  MFP_QNAN = 4,
  MFP_SNAN = 5
};

const char *number_type_names[] = {"Normal non-zero", "Subnormal", "Zero",
                                   "Infinite",        "QNaN",      "SNaN"};

int classify_double(double x) {
  // Check if the number is NaN
  if (isnan(x)) {
    // Determine if it's a Quiet NaN or Signaling NaN
    // On most systems, QNaN has the MSB of the fraction set, SNaN does not
    uint64_t u;
    memcpy(&u, &x, sizeof(double));
    if (u & (1ULL << 51)) { // Checking the MSB of the fraction part
      return MFP_QNAN;
    } else {
      return MFP_SNAN;
    }
  }

  // Check for infinity
  if (isinf(x)) {
    return MFP_INFINITE;
  }

  // Check if it's subnormal (denormalized)
  if (fpclassify(x) == FP_SUBNORMAL) {
    return MFP_SUBNORMAL;
  }

  // Check if it's zero
  if (x == 0.0) {
    return MFP_ZERO;
  }

  // If none of the above, it must be normal
  return MFP_NORMAL_NON_ZERO;
}

uint64_t expand_bits(uint16_t input) {
  uint64_t result = 0;
  for (int i = 0; i < 16; ++i) {
    // Check each bit of the input
    if (input & (1U << i)) {
      // If the bit is set, set the corresponding 4 bits in result
      result |= 0xFULL << (4 * i);
    }
    // If the bit is not set, we don't need to do anything as the result starts
    // at 0
  }
  return result;
}

void stats() {
  size_t counts[6] = {0};
  for (size_t i = 0; i < 65536; i++) {
    uint16_t input = i;
    uint64_t result = expand_bits(input);
    double f = 0;
    memcpy(&f, &result, sizeof(f));
    int c = classify_double(f);
    counts[c]++;
  }
  for (int i = 0; i < 6; i++) {
    printf("%s: %zu\n", number_type_names[i], counts[i]);
  }
}

int veq_non_zero_max(uint8x16_t v) {
  return vmaxvq_u32(vreinterpretq_u32_u8(v)) != 0;
}

int veq_non_zero_mov(uint8x16_t v) {
  return vget_lane_u64(vqmovn_u64(vreinterpretq_u64_u8(v)), 0) != 0;
}

int veq_non_zero_narrow(uint8x16_t v) {
  return vget_lane_u64(vshrn_n_u16(vreinterpretq_u16_u8(v), 4), 0) != 0;
}

int veq_non_zero_float(uint8x16_t v) {
  uint8x8_t narrowed = vshrn_n_u16(vreinterpretq_u16_u8(v), 4);
  return (vdupd_lane_f64(vreinterpret_f64_u16(narrowed), 0) != 0.0);
}

int veq_non_zero_float_safe(uint8x16_t v) {
  uint8x8_t result = vdup_n_u8(0x10);
  uint8x8_t narrowed = vshrn_n_u16(vreinterpretq_u16_u8(v), 4);
  // This is either 0 or 0xFFFFFFFFFFFFFFFF
  uint64x1_t narrowed64 = vceqz_u64(vreinterpret_u64_u8(narrowed));
  // We either get +0 or a quiet NaN
  return (vdupd_lane_f64(vreinterpret_f64_u64(narrowed64), 0) == 0.0);
}

bool is_float_safe() {
  uint8x16_t v = {0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8x8_t narrowed = vshrn_n_u16(vreinterpretq_u16_u8(v), 4);
  return vdupd_lane_f64(vreinterpret_f64_u16(narrowed), 0) != 0;
}

template <typename F> int scan(uint8_t *input, size_t length, F f) {
  int result = 0;
  for (size_t i = 0; i + 16 <= length; i += 16) {
    uint8x16_t v = vld1q_u8(input + i);
    result++;
    if (f(v)) {
      i += 3;
    }
  }
  return result;
}

uint32_t sum_uint8x16(uint8x16_t v) {
  // Pairwise addition from uint8 to uint16
  uint16x8_t sum16 = vpaddlq_u8(v);

  // Further reduce from uint16 to uint32
  uint32x4_t sum32 = vpaddlq_u16(sum16);

  // Reduce from uint32x4 to uint32x2
  uint32x2_t sum32x2 = vpadd_u32(vget_low_u32(sum32), vget_high_u32(sum32));

  // Final horizontal sum to get a single uint32_t result
  return vaddv_u32(sum32x2);
}

template <typename F> int branchyscan(uint8_t *input, size_t length, F f) {
  uint8x16_t v{};
  uint8x16_t sum{};
  for (size_t i = 0; i + 16 + 1 <= length; i += 16) {
    sum = vaddq_u8(sum, v);
    if (f(v)) {
      v = vld1q_u8(input + i + 1);
    } else {
      v = vld1q_u8(input + i);
    }
  }
  return sum_uint8x16(sum);
}

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.2f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

int main(int argc, char **argv) {
  stats();
  printf("is_float_safe: %d\n", is_float_safe());
  printf("veq_non_zero_float_safe: %d\n",
         veq_non_zero_float_safe(uint8x16_t()));
  std::vector<uint8_t> data(1000000);
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint64_t> dis(
      0, (std::numeric_limits<uint8_t>::max)());
  std::ranges::generate(data, [&]() { return dis(gen); });
  size_t count = scan(data.data(), data.size(), veq_non_zero_max);
  size_t volume = count * 16;
  volatile size_t counter = 0;
  printf("# check: %d %d %d %d %d\n",
         scan(data.data(), data.size(), veq_non_zero_max),
         scan(data.data(), data.size(), veq_non_zero_mov),
         scan(data.data(), data.size(), veq_non_zero_narrow),
         scan(data.data(), data.size(), veq_non_zero_float),
         scan(data.data(), data.size(), veq_non_zero_float_safe));
  for (size_t trial = 0; trial < 3; trial++) {
    printf("Trial %zu\n", trial + 1);

    pretty_print(count, volume, "veq_non_zero_max", bench([&data, &counter]() {
                   counter = counter +
                             scan(data.data(), data.size(), veq_non_zero_max);
                 }));
    pretty_print(count, volume, "veq_non_zero_mov", bench([&data, &counter]() {
                   counter = counter +
                             scan(data.data(), data.size(), veq_non_zero_mov);
                 }));
    pretty_print(
        count, volume, "veq_non_zero_narrow", bench([&data, &counter]() {
          counter = counter + scan(data.data(), data.size(), veq_non_zero_mov);
        }));
    pretty_print(count, volume, "veq_non_zero_float",
                 bench([&data, &counter]() {
                   counter = counter +
                             scan(data.data(), data.size(), veq_non_zero_float);
                 }));
    pretty_print(count, volume, "veq_non_zero_float_safe",
                 bench([&data, &counter]() {
                   counter = counter + scan(data.data(), data.size(),
                                            veq_non_zero_float_safe);
                 }));
  }
  printf("branchy\n");

  for (size_t trial = 0; trial < 3; trial++) {
    printf("Trial %zu\n", trial + 1);

    pretty_print(count, volume, "veq_non_zero_max", bench([&data, &counter]() {
                   counter = counter + branchyscan(data.data(), data.size(),
                                                   veq_non_zero_max);
                 }));
    pretty_print(count, volume, "veq_non_zero_mov", bench([&data, &counter]() {
                   counter = counter + branchyscan(data.data(), data.size(),
                                                   veq_non_zero_mov);
                 }));
    pretty_print(
        count, volume, "veq_non_zero_narrow", bench([&data, &counter]() {
          counter =
              counter + branchyscan(data.data(), data.size(), veq_non_zero_mov);
        }));
    pretty_print(count, volume, "veq_non_zero_float",
                 bench([&data, &counter]() {
                   counter = counter + branchyscan(data.data(), data.size(),
                                                   veq_non_zero_float);
                 }));
    pretty_print(count, volume, "veq_non_zero_float_safe",
                 bench([&data, &counter]() {
                   counter = counter + branchyscan(data.data(), data.size(),
                                                   veq_non_zero_float_safe);
                 }));
  }
  std::fill(data.begin(), data.end(), 0);

  printf("branchy zeros\n");

  for (size_t trial = 0; trial < 3; trial++) {
    printf("Trial %zu\n", trial + 1);

    pretty_print(count, volume, "veq_non_zero_max", bench([&data, &counter]() {
                   counter = counter + branchyscan(data.data(), data.size(),
                                                   veq_non_zero_max);
                 }));
    pretty_print(count, volume, "veq_non_zero_mov", bench([&data, &counter]() {
                   counter = counter + branchyscan(data.data(), data.size(),
                                                   veq_non_zero_mov);
                 }));
    pretty_print(
        count, volume, "veq_non_zero_narrow", bench([&data, &counter]() {
          counter =
              counter + branchyscan(data.data(), data.size(), veq_non_zero_mov);
        }));
    pretty_print(count, volume, "veq_non_zero_float",
                 bench([&data, &counter]() {
                   counter = counter + branchyscan(data.data(), data.size(),
                                                   veq_non_zero_float);
                 }));
    pretty_print(count, volume, "veq_non_zero_float_safe",
                 bench([&data, &counter]() {
                   counter = counter + branchyscan(data.data(), data.size(),
                                                   veq_non_zero_float_safe);
                 }));
  }
}
