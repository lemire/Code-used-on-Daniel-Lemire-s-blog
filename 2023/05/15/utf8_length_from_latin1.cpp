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

size_t pure_scalar_utf8_length(const uint8_t *c, size_t len) {
  size_t answer = 0;
  for (size_t i = 0; i < len; i++) {
    if ((c[i] >> 7)) {
      answer++;
    }
  }
  return answer + len;
}

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
    result -= vaddvq_s8(vreinterpretq_s8_u8(withhighbit));
  }
  // scalar tail
  for (uint8_t j = 0; j < rem; j++) {
    result += (simd_end[j] >> 7);
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

  return EXIT_SUCCESS;
}
