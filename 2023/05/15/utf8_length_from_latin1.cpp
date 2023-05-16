#include <arm_neon.h>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}



__attribute__((noinline))
size_t scalar_utf8_length(const uint8_t *c, size_t len) {
  size_t answer = 0;
  for (size_t i = 0; i < len; i++) {
    if ((c[i] >> 7)) {
      answer++;
    }
  }
  return answer + len;
}

// from https://github.com/nodejs/node/pull/48009#issuecomment-1547344694
__attribute__((noinline))
uint64_t utf8_length_kvakil(const uint8_t *data, uint32_t length) {
  uint64_t result = 0;
  const int lanes = sizeof(uint8x16_t);
  uint8_t rem = length % lanes;
  const uint8_t *simd_end = data + (length / lanes) * lanes;
  const uint8x16_t threshold = vdupq_n_u8(0x80);
  for (; data < simd_end; data += lanes) {
    // load 16 bytes
    uint8x16_t input = vld1q_u8(data);
    // compare to threshold (0x80)
    uint8x16_t withhighbit = vcgeq_u8(input, threshold);
    // shift and narrow
    uint8x8_t highbits = vshrn_n_u16(vreinterpretq_u16_u8(withhighbit), 4);
    // we have 0, 4 or 8 bits per byte
    uint8x8_t bitsperbyte = vcnt_u8(highbits);
    // sum the bytes vertically to uint16_t
    result += vaddlv_u8(bitsperbyte);
  }
  result /= 4; // we overcounted by a factor of 4
  // scalar tail
  for (uint8_t j = 0; j < rem; j++) {
    result += (simd_end[j] >> 7);
  }
  return result + length;
}

__attribute__((noinline))
uint64_t utf8_length_faster(const uint8_t *data, uint32_t length) {
  uint64_t result = 0;
  const int lanes = sizeof(uint8x16_t);
  uint8_t rem = length % lanes;
  const uint8_t *simd_end = data + (length / lanes) * lanes;
  const uint8x16_t threshold = vdupq_n_u8(0x80);
  for (; data < simd_end; data += lanes) {
    // load 16 bytes
    uint8x16_t input = vld1q_u8(data);
    // compare to threshold (0x80)
    uint8x16_t withhighbit = vcgeq_u8(input, threshold);
    // vertical addition
    result -= vaddvq_s8(withhighbit);
  }
  // scalar tail
  for (uint8_t j = 0; j < rem; j++) {
    result += (simd_end[j] >> 7);
  }
  return result + length;
}

__attribute__((noinline))
uint64_t utf8_length_shift(const uint8_t *data, uint32_t length) {
  uint64_t result = length;
  
  while(length >= 64) {
    uint8x16_t acc1, acc2, acc3, acc4;
    acc1 = acc2 = acc3 = acc4 = vdupq_n_u8(0);
    unsigned iters = length / 64;
    if(iters > 255) iters = 255;
    length -= 64 * iters;
    while(iters--) {
      acc1 = vsraq_n_u8(acc1, vld1q_u8(data), 7);
      acc2 = vsraq_n_u8(acc2, vld1q_u8(data + 16), 7);
      acc3 = vsraq_n_u8(acc3, vld1q_u8(data + 32), 7);
      acc4 = vsraq_n_u8(acc4, vld1q_u8(data + 48), 7);
      data += 64;
    }
    uint16x8_t sum1 = vpaddlq_u8(acc1), sum2 = vpaddlq_u8(acc2);
    sum1 = vpadalq_u8(sum1, acc3);
    sum2 = vpadalq_u8(sum2, acc4);
    sum1 = vaddq_u16(sum1, sum2);
    result += vaddvq_u16(sum1);
  }
  
  // scalar tail
  for (uint8_t j = 0; j < length; j++) {
    result += (data[j] >> 7);
  }
  return result;
}

__attribute__((noinline))
uint64_t utf8_length_strager(const uint8_t *data, uint32_t length) {
   uint64_t result = 0;

   const int simd_lanes = sizeof(uint8x16_t);

   const uint8_t *simd_end = data + (length / simd_lanes) * simd_lanes;
   uint32_t length_after_simd = length % simd_lanes;
   for (; data < simd_end; data += simd_lanes) {
     uint8x16_t chunk = vld1q_u8(data);
     uint8x16_t high_bits = vshrq_n_u8(chunk, 7);
     result += vaddvq_u8(high_bits);
   }

   const uint8_t *scalar_end = data + length_after_simd;
   for (; data < scalar_end; data += 1) {
     result += *data >> 7;
   }

   return result + length;
 }

int main() {
  size_t trials = 3;
  size_t warm_trials = 3;

  size_t N = 8000*2;
  uint8_t *input = new uint8_t[N];
  for (size_t i = 0; i < N; i++) {
    input[i] = rand();
  }
  size_t expected = scalar_utf8_length(input, N);

  volatile size_t len{0};

  std::cout << "scalar (with autovec)" << std::endl;
  for (size_t t = 0; t < trials + warm_trials; t++) {
    uint64_t before = nano();
    len = scalar_utf8_length(input, N);
    uint64_t after = nano();
    if (t >= warm_trials) {
      std::cout << "ns/bytes " << double(after - before) / (len) << std::endl;
      std::cout << "GB/s " << (len)/double(after - before) << std::endl;
    }
  }
  if (len != expected) {
    printf("%zu %zu \n", len, expected);
    abort();
  }

  std::cout << std::endl;

  std::cout << "kvakil" << std::endl;
  for (size_t t = 0; t < trials + warm_trials; t++) {
    uint64_t before = nano();
    len = utf8_length_kvakil(input, N);
    uint64_t after = nano();
    if (t >= warm_trials) {
      std::cout << "ns/bytes " << double(after - before) / (len) << std::endl;
      std::cout << "GB/s " << (len)/double(after - before) << std::endl;

    }
  }
  if (len != expected) {
    printf("%zu %zu \n", len, expected);
    abort();
  }
  std::cout << std::endl;


  std::cout << "faster" << std::endl;
  for (size_t t = 0; t < trials + warm_trials; t++) {
    uint64_t before = nano();
    len = utf8_length_faster(input, N);
    uint64_t after = nano();
    if (t >= warm_trials) {
      std::cout << "ns/bytes " << double(after - before) / (len) << std::endl;
      std::cout << "GB/s " << (len)/double(after - before) << std::endl;

    }
  }
  if (len != expected) {
    printf("%zu %zu \n", len, expected);
    abort();
  }
  std::cout << std::endl;



  std::cout << "shift" << std::endl;
  for (size_t t = 0; t < trials + warm_trials; t++) {
    uint64_t before = nano();
    len = utf8_length_shift(input, N);
    uint64_t after = nano();
    if (t >= warm_trials) {
      std::cout << "ns/bytes " << double(after - before) / (len) << std::endl;
      std::cout << "GB/s " << (len)/double(after - before) << std::endl;

    }
  }
  if (len != expected) {
    printf("%zu %zu \n", len, expected);
    abort();
  }
  std::cout << std::endl;


  std::cout << "strager" << std::endl;
  for (size_t t = 0; t < trials + warm_trials; t++) {
    uint64_t before = nano();
    len = utf8_length_strager(input, N);
    uint64_t after = nano();
    if (t >= warm_trials) {
      std::cout << "ns/bytes " << double(after - before) / (len) << std::endl;
      std::cout << "GB/s " << (len)/double(after - before) << std::endl;

    }
  }
  if (len != expected) {
    printf("%zu %zu \n", len, expected);
    abort();
  }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
