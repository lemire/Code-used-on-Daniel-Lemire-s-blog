#include <algorithm>
#include <arm_neon.h>
#include <chrono>
#include <cstdint>
#include <format>
#include <iostream>
#include <print>
#include <random>
#include <string>
#include <vector>

#include "counters/bench.h"

void scalar_prefixsum(uint32_t *data, size_t length) {
  if (length == 0)
    return;
  for (size_t i = 1; i < length; i++) {
    data[i] += data[i - 1];
  }
}

void scalar_prefixsum_leftover(uint32_t *data, size_t length,
                               uint32_t blocksize) {
  size_t lengthlo = length % blocksize;
  size_t startpoint = length - lengthlo;
  if (startpoint == 0)
    startpoint++;
  for (size_t i = startpoint; i < length; i++) {
    data[i] += data[i - 1];
  }
}

inline uint32x4_t PrefixSum(uint32x4_t curr, uint32x4_t prev) {
  uint32x4_t zero = {0, 0, 0, 0};
  uint32x4_t add = vextq_u32(zero, curr, 3);
  const uint8x16_t BroadcastLast = {12, 13, 14, 15, 12, 13, 14, 15,
                                    12, 13, 14, 15, 12, 13, 14, 15};
  prev = vreinterpretq_u32_u8(
      vqtbl1q_u8(vreinterpretq_u8_u32(prev), BroadcastLast));
  curr = vaddq_u32(curr, add);
  add = vextq_u32(zero, curr, 2);
  curr = vaddq_u32(curr, prev);
  curr = vaddq_u32(curr, add);
  return curr;
}

void neon_prefixsum(uint32_t *data, size_t length) {
  uint32x4_t prev = {0, 0, 0, 0};
  uint32x4_t curr;
  for (size_t i = 0; i < length / 4; i++) {
    curr = vld1q_u32(data + 4 * i);
    uint32x4_t csum = PrefixSum(curr, prev);
    vst1q_u32(data + 4 * i, csum);
    prev = csum;
  }
  scalar_prefixsum_leftover(data, length, 4);
}

inline uint32x4_t SimplePrefixSum(uint32x4_t curr) {
  // ABCD
  uint32x4_t zero = {0, 0, 0, 0};
  uint32x4_t add = vextq_u32(zero, curr, 3); // 0ABCD
  curr = vaddq_u32(curr, add);               // A+0, B+A, C+B, D+C
  add = vextq_u32(zero, curr, 2);            // 0 0 A B+C
  curr = vaddq_u32(curr, add);               // A+0, B+A, C+B+A, D+C+B+A
  return curr;
}

void print(const char *name, uint32x4_t vec) {
  uint32_t arr[4];
  vst1q_u32(arr, vec);
  printf("%s: ", name);
  printf("%u %u %u %u\n", arr[0], arr[1], arr[2], arr[3]);
}

void neon_prefixsum2(uint32_t *data, size_t length) {
  const uint8x16_t BroadcastLast = {12, 13, 14, 15, 12, 13, 14, 15,
                                    12, 13, 14, 15, 12, 13, 14, 15};
  uint32x4_t prev = {0, 0, 0, 0};
  uint32x4_t curr;
  for (size_t i = 0; i < length / 16; i++) {
    curr = vld1q_u32(data + 16 * i);
    uint32x4_t csum1 = SimplePrefixSum(curr);
    csum1 = vaddq_u32(csum1, prev);
    vst1q_u32(data + 16 * i, csum1);
    prev = vreinterpretq_u32_u8(
        vqtbl1q_u8(vreinterpretq_u8_u32(csum1), BroadcastLast));
    curr = vld1q_u32(data + 16 * i + 4);
    uint32x4_t csum2 = SimplePrefixSum(curr);
    csum2 = vaddq_u32(csum2, prev);

    vst1q_u32(data + 16 * i + 4, csum2);
    prev = vreinterpretq_u32_u8(
        vqtbl1q_u8(vreinterpretq_u8_u32(csum2), BroadcastLast));
    curr = vld1q_u32(data + 16 * i + 8);
    uint32x4_t csum3 = SimplePrefixSum(curr);
    csum3 = vaddq_u32(csum3, prev);
    vst1q_u32(data + 16 * i + 8, csum3);
    prev = vreinterpretq_u32_u8(
        vqtbl1q_u8(vreinterpretq_u8_u32(csum3), BroadcastLast));
    curr = vld1q_u32(data + 16 * i + 12);
    uint32x4_t csum4 = SimplePrefixSum(curr);
    csum4 = vaddq_u32(csum4, prev);
    vst1q_u32(data + 16 * i + 12, csum4);
    prev = vreinterpretq_u32_u8(
        vqtbl1q_u8(vreinterpretq_u8_u32(csum4), BroadcastLast));
  }
  scalar_prefixsum_leftover(data, length, 16);
}

uint32_t neon_reduction(uint32_t *data, size_t length) {
  uint32_t sum = 0;

  for (size_t i = 0; i < length / 16; i++) {
    auto vec1 = vld1q_u32(data + 16 * i);
    auto vec2 = vld1q_u32(data + 16 * i + 4);
    auto vec3 = vld1q_u32(data + 16 * i + 8);
    auto vec4 = vld1q_u32(data + 16 * i + 12);
    auto vec = vaddq_u32(vaddq_u32(vec1, vec2), vaddq_u32(vec3, vec4));
    sum += vaddvq_u32(vec);
  }
  for (size_t i = (length / 16) * 16; i < length; i++) {
    sum += data[i];
  }
  return sum;
}
void neon_prefixsum3(uint32_t *data, size_t length) {
  const uint8x16_t BroadcastLast = {12, 13, 14, 15, 12, 13, 14, 15,
                                    12, 13, 14, 15, 12, 13, 14, 15};
  uint32x4_t prev = {0, 0, 0, 0};
  uint32x4_t zero = {0, 0, 0, 0};

  for (size_t i = 0; i < length / 16; i++) {
    uint32x4x4_t vals = vld4q_u32(data + 16 * i);
    vals.val[1] = vaddq_u32(vals.val[1], vals.val[0]);
    vals.val[2] = vaddq_u32(vals.val[2], vals.val[1]);
    vals.val[3] = vaddq_u32(vals.val[3], vals.val[2]);
    uint32x4_t ps = SimplePrefixSum(vals.val[3]);
    ps = vaddq_u32(ps, prev);
    uint32x4_t add = vextq_u32(prev, ps, 3);
    prev = vdupq_laneq_u32(ps, 3);
    vals.val[0] = vaddq_u32(vals.val[0], add);
    vals.val[1] = vaddq_u32(vals.val[1], add);
    vals.val[2] = vaddq_u32(vals.val[2], add);
    vals.val[3] = vaddq_u32(vals.val[3], add);
    vst4q_u32(data + 16 * i, vals);
  }
  scalar_prefixsum_leftover(data, length, 16);
}

void neon_prefixsum_two_steps(uint32_t *data, size_t length) {
  size_t first_half = (length / 16) / 2 * 16;
  uint32_t sum_half = neon_reduction(data, first_half);
  // We are going to process both halves independently
  uint32x4_t prev_first = {0, 0, 0, 0};
  uint32x4_t prev_second = {sum_half, sum_half, sum_half, sum_half};

  uint32x4_t zero = {0, 0, 0, 0};

  for (size_t i = 0; i < first_half / 16; i++) {
    uint32x4x4_t vals_first_half = vld4q_u32(data + 16 * i);
    vals_first_half.val[1] = vaddq_u32(vals_first_half.val[1], vals_first_half.val[0]);
    vals_first_half.val[2] = vaddq_u32(vals_first_half.val[2], vals_first_half.val[1]);
    vals_first_half.val[3] = vaddq_u32(vals_first_half.val[3], vals_first_half.val[2]);
    uint32x4_t ps = SimplePrefixSum(vals_first_half.val[3]);
    ps = vaddq_u32(ps, prev_first);
    uint32x4_t add = vextq_u32(prev_first, ps, 3);
    prev_first = vdupq_laneq_u32(ps, 3);
    vals_first_half.val[0] = vaddq_u32(vals_first_half.val[0], add);
    vals_first_half.val[1] = vaddq_u32(vals_first_half.val[1], add);
    vals_first_half.val[2] = vaddq_u32(vals_first_half.val[2], add);
    vals_first_half.val[3] = vaddq_u32(vals_first_half.val[3], add);
    vst4q_u32(data + 16 * i, vals_first_half);
      uint32x4x4_t vals_second_half = vld4q_u32(data + 16 * i + first_half);
    vals_second_half.val[1] = vaddq_u32(vals_second_half.val[1], vals_second_half.val[0]);
    vals_second_half.val[2] = vaddq_u32(vals_second_half.val[2], vals_second_half.val[1]);
    vals_second_half.val[3] = vaddq_u32(vals_second_half.val[3], vals_second_half.val[2]);
    ps =  SimplePrefixSum(vals_second_half.val[3]);
    ps = vaddq_u32(ps, prev_second);
    add = vextq_u32(prev_second, ps, 3);
    prev_second = vdupq_laneq_u32(ps, 3);
    vals_second_half.val[0] = vaddq_u32(vals_second_half.val[0], add);
    vals_second_half.val[1] = vaddq_u32(vals_second_half.val[1], add);
    vals_second_half.val[2] = vaddq_u32(vals_second_half.val[2], add);
    vals_second_half.val[3] = vaddq_u32(vals_second_half.val[3], add);
    vst4q_u32(data + 16 * i + first_half, vals_second_half);
  }

  uint32_t start_value = 0;
  if(first_half != 0) {
    start_value = prev_second[3];
  }
  for (size_t i = first_half*2; i < length; i++) {
    data[i] += data[i - 1];
  }
}

void neon_prefixsum4(uint32_t *data, size_t length) {
  const uint8x16_t BroadcastLast = {12, 13, 14, 15, 12, 13, 14, 15,
                                    12, 13, 14, 15, 12, 13, 14, 15};
  uint32x4_t prev = {0, 0, 0, 0};
  uint32x4_t zero = {0, 0, 0, 0};

  for (size_t i = 0; i < length / 32; i++) {
    uint32x4x4_t vals1 = vld4q_u32(data + 32 * i);
    vals1.val[1] = vaddq_u32(vals1.val[1], vals1.val[0]);
    vals1.val[2] = vaddq_u32(vals1.val[2], vals1.val[1]);
    vals1.val[3] = vaddq_u32(vals1.val[3], vals1.val[2]);
    uint32x4x4_t vals2 = vld4q_u32(data + 32 * i + 16);
    vals2.val[1] = vaddq_u32(vals2.val[1], vals2.val[0]);
    vals2.val[2] = vaddq_u32(vals2.val[2], vals2.val[1]);
    vals2.val[3] = vaddq_u32(vals2.val[3], vals2.val[2]);
    uint32x4_t nextprev =
        vdupq_n_u32(vaddvq_u32(vaddq_u32(vals2.val[3], vals1.val[3])));
    nextprev = vaddq_u32(nextprev, prev);

    uint32x4_t ps1 = SimplePrefixSum(vals1.val[3]);
    uint32x4_t ps2 = SimplePrefixSum(vals2.val[3]);

    ps1 = vaddq_u32(ps1, prev);
    // = vdupq_laneq_u32(vaddq_u32(ps1, ps2), 3);

    uint32x4_t add1 = vextq_u32(prev, ps1, 3);

    prev = vdupq_laneq_u32(ps1, 3);
    ps2 = vaddq_u32(ps2, prev);
    uint32x4_t add2 = vextq_u32(prev, ps2, 3);
    prev = nextprev;

    vals1.val[0] = vaddq_u32(vals1.val[0], add1);
    vals1.val[1] = vaddq_u32(vals1.val[1], add1);
    vals1.val[2] = vaddq_u32(vals1.val[2], add1);
    vals1.val[3] = vaddq_u32(vals1.val[3], add1);
    vst4q_u32(data + 32 * i, vals1);
    vals2.val[0] = vaddq_u32(vals2.val[0], add2);
    vals2.val[1] = vaddq_u32(vals2.val[1], add2);
    vals2.val[2] = vaddq_u32(vals2.val[2], add2);
    vals2.val[3] = vaddq_u32(vals2.val[3], add2);
    vst4q_u32(data + 32 * i + 16, vals2);
  }
  scalar_prefixsum_leftover(data, length, 32);
}

double pretty_print(const std::string &name, size_t num_values,
                    counters::event_aggregate agg) {
  std::print("{:<50} : ", name);
  std::printf(" %10zu values ", num_values);
  std::print(" {:5.3f} ms ", agg.fastest_elapsed_ns() / 1e6);
  std::print(" {:5.3f} ns ", agg.fastest_elapsed_ns() / double(num_values));
  std::print(" {:5.2f} Gv/s ", double(num_values) / agg.fastest_elapsed_ns());
  std::print(" {:5.2f} GB/s ",
             double(num_values) * sizeof(uint32_t) / agg.fastest_elapsed_ns());
  if (counters::has_performance_counters()) {
    std::print(" {:5.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
    std::print(" {:5.2f} c ", agg.fastest_cycles() / double(num_values));
    std::print(" {:5.2f} i ", agg.fastest_instructions() / double(num_values));
    std::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / double(agg.fastest_cycles()));
  }
  std::print("\n");
  return double(num_values) / agg.fastest_elapsed_ns();
}

void collect_benchmark_results(size_t input_size) {

  std::vector<uint32_t> input(input_size);
  for (size_t i = 0; i < input_size; ++i) {
    input[i] = 1;
  }

  std::vector<uint32_t> input_copy = input;
  neon_prefixsum2(input_copy.data(), input_copy.size());
  std::vector<uint32_t> input_copy2 = input;
  scalar_prefixsum(input_copy2.data(), input_copy2.size());
  if (input_copy != input_copy2) {
    std::cerr << "Error: neon_prefixsum2 results do not match!\n";
    for (size_t i = 0; i < input_size; ++i) {
      std::cerr << "neon: " << input_copy[i] << " scalar: " << input_copy2[i]
                << "\n";
    }
    return;
  }
  std::vector<uint32_t> input_copy3 = input;
  neon_prefixsum3(input_copy3.data(), input_copy3.size());
  if (input_copy3 != input_copy2) {
    std::cerr << "Error: neon_prefixsum3 results do not match!\n";
    for (size_t i = 0; i < input_size; ++i) {
      std::cerr << "neon3: " << input_copy3[i] << " scalar: " << input_copy2[i]
                << "\n";
    }
    return;
  }
  std::vector<uint32_t> input_copy4 = input;
  neon_prefixsum4(input_copy4.data(), input_copy4.size());
  if (input_copy4 != input_copy2) {
    std::cerr << "Error: neon_prefixsum4 results do not match!\n";
    for (size_t i = 0; i < input_size; ++i) {
      std::cerr << "neon4: " << input_copy4[i] << " scalar: " << input_copy2[i]
                << "\n";
    }
    return;
  }
  std::vector<uint32_t> sink_buffer = input;

  volatile uint32_t sink;
  pretty_print("memcpy", input_size, counters::bench([&input, &sink_buffer]() {
                 std::memcpy(sink_buffer.data(), input.data(),
                             input.size() * sizeof(uint32_t));
               }));
  pretty_print("neon_reduction", input_size, counters::bench([&input, &sink]() {
                 sink = neon_reduction(input.data(), input.size());
               }));
  pretty_print("neon_prefixsum_two_steps", input_size,
               counters::bench([&input, &sink]() {
                 neon_prefixsum_two_steps(input.data(), input.size());
                 sink = input.back();
               }));
  pretty_print("neon_prefixsum4", input_size,
               counters::bench([&input, &sink]() {
                 neon_prefixsum4(input.data(), input.size());
                 sink = input.back();
               }));
  pretty_print("neon_prefixsum3", input_size,
               counters::bench([&input, &sink]() {
                 neon_prefixsum3(input.data(), input.size());
                 sink = input.back();
               }));
  pretty_print("neon_prefixsum2", input_size,
               counters::bench([&input, &sink]() {
                 neon_prefixsum2(input.data(), input.size());
                 sink = input.back();
               }));
  pretty_print("neon_prefixsum", input_size, counters::bench([&input, &sink]() {
                 neon_prefixsum(input.data(), input.size());
                 sink = input.back();
               }));
  pretty_print("scalar_prefixsum", input_size,
               counters::bench([&input, &sink]() {
                 scalar_prefixsum(input.data(), input.size());
               }));
}

int main(int argc, char **argv) { collect_benchmark_results(1000); }
