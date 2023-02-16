#include "linux-perf-events.h"
#include <x86intrin.h>
#include <iostream>
#include <random>
#include <vector>

size_t scalar_utf8_length(const uint8_t *c, size_t len) {
  size_t answer = 0;
  for (size_t i = 0; i < len; i++) {
    if ((c[i] >> 7)) {
      answer++;
    }
  }
  return answer + len;
}

size_t avx2_utf8_length_basic(const uint8_t *str, size_t len) {
  size_t answer = len / sizeof(__m256i) * sizeof(__m256i);
  size_t i;
  for (i = 0; i + sizeof(__m256i) <= len; i += 32) {
    __m256i input = _mm256_loadu_si256((const __m256i *)(str + i));
    answer += __builtin_popcount(_mm256_movemask_epi8(input));
  }
  return answer + scalar_utf8_length(str + i, len - i);
}

/**
 * Similar to :
 * Wojciech Muła, Nathan Kurz, Daniel Lemire
 * Faster Population Counts Using AVX2 Instructions
 * Computer Journal 61 (1), 2018
 **/
size_t avx2_utf8_length_mkl(const uint8_t *str, size_t len) {
  size_t answer = len / sizeof(__m256i) * sizeof(__m256i);
  size_t i = 0;
  __m256i four_64bits = _mm256_setzero_si256();
  while (i + sizeof(__m256i) <= len) {
    __m256i runner = _mm256_setzero_si256();
    // We can do up to 255 loops without overflow.
    size_t iterations = (len - i) / sizeof(__m256i);
    if (iterations > 255) {
      iterations = 255;
    }
    size_t max_i = i + iterations * sizeof(__m256i) - sizeof(__m256i);
    for (; i <= max_i; i += sizeof(__m256i)) {
      __m256i input = _mm256_loadu_si256((const __m256i *)(str + i));
      runner = _mm256_sub_epi8(
          runner, _mm256_cmpgt_epi8(_mm256_setzero_si256(), input));
    }
    four_64bits = _mm256_add_epi64(
        four_64bits, _mm256_sad_epu8(runner, _mm256_setzero_si256()));
  }
  answer += _mm256_extract_epi64(four_64bits, 0) +
            _mm256_extract_epi64(four_64bits, 1) +
            _mm256_extract_epi64(four_64bits, 2) +
            _mm256_extract_epi64(four_64bits, 3);
  return answer + scalar_utf8_length(str + i, len - i);
}


size_t avx2_utf8_length_mkl2(const uint8_t *str, size_t len) {
  size_t answer = len / sizeof(__m256i) * sizeof(__m256i);
  size_t i = 0;
  __m256i four_64bits = _mm256_setzero_si256();
  while (i + sizeof(__m256i) <= len) {
    __m256i runner = _mm256_setzero_si256();
    // We can do up to 255 loops without overflow.
    size_t iterations = (len - i) / sizeof(__m256i);
    if (iterations > 255) {
      iterations = 255;
    }
    size_t max_i = i + iterations * sizeof(__m256i) - sizeof(__m256i);
    for (; i + 4*sizeof(__m256i) <= max_i; i += 4*sizeof(__m256i)) {
      __m256i input1 = _mm256_loadu_si256((const __m256i *)(str + i));
      __m256i input2 = _mm256_loadu_si256((const __m256i *)(str + i + sizeof(__m256i)));
      __m256i input3 = _mm256_loadu_si256((const __m256i *)(str + i + 2*sizeof(__m256i)));
      __m256i input4 = _mm256_loadu_si256((const __m256i *)(str + i + 3*sizeof(__m256i)));
      __m256i input12 = _mm256_add_epi8(_mm256_cmpgt_epi8(_mm256_setzero_si256(), input1),
              _mm256_cmpgt_epi8(_mm256_setzero_si256(), input2));
      __m256i input23 = _mm256_add_epi8(_mm256_cmpgt_epi8(_mm256_setzero_si256(), input3),
              _mm256_cmpgt_epi8(_mm256_setzero_si256(), input4));
      __m256i input1234 = _mm256_add_epi8(input12, input23);
      runner = _mm256_sub_epi8(
          runner, input1234);
    }
    for (; i <= max_i; i += sizeof(__m256i)) {
      __m256i input = _mm256_loadu_si256((const __m256i *)(str + i));
      runner = _mm256_sub_epi8(
          runner, _mm256_cmpgt_epi8(_mm256_setzero_si256(), input));
    }
    four_64bits = _mm256_add_epi64(
        four_64bits, _mm256_sad_epu8(runner, _mm256_setzero_si256()));
  }
  answer += _mm256_extract_epi64(four_64bits, 0) +
            _mm256_extract_epi64(four_64bits, 1) +
            _mm256_extract_epi64(four_64bits, 2) +
            _mm256_extract_epi64(four_64bits, 3);
  return answer + scalar_utf8_length(str + i, len - i);
}

int main() {
  size_t trials = 3;
  size_t warm_trials = 3;

  size_t N = 8000;
  uint8_t *input = new uint8_t[N];
  for (size_t i = 0; i < N; i++) {
    input[i] = rand();
  }
  size_t expected = scalar_utf8_length(input, N);
  LinuxEvents<PERF_TYPE_HARDWARE> linux_events(
      std::vector<int>{ PERF_COUNT_HW_CPU_CYCLES,
                        PERF_COUNT_HW_INSTRUCTIONS, });
  volatile size_t len{ 0 };
  std::cout << "scalar" << std::endl;
  std::vector<unsigned long long> results(2);
  for (size_t t = 0; t < trials + warm_trials; t++) {
    linux_events.start();
    len = scalar_utf8_length(input, N);
    linux_events.end(results);
    if (t >= warm_trials) {

      std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
      std::cout << "instructions/bytes " << double(results[1]) / (len) << " ";
      std::cout << "instructions/cycle " << double(results[1]) / results[0]
                << std::endl;
    }
  }
  if(len != expected) { abort(); }

  std::cout << std::endl;

  std::cout << "avx2 (basic)" << std::endl;
  for (size_t t = 0; t < trials + warm_trials; t++) {

    linux_events.start();
    len = avx2_utf8_length_basic(input, N);
    linux_events.end(results);
    if (t >= warm_trials) {
      std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
      std::cout << "instructions/bytes " << double(results[1]) / (len) << " ";
      std::cout << "instructions/cycle " << double(results[1]) / results[0]
                << std::endl;
    }
  }
  if(len != expected) { abort(); }

  std::cout << std::endl;

  std::cout << "avx2 (mkl)" << std::endl;
  for (size_t t = 0; t < trials + warm_trials; t++) {

    linux_events.start();
    len = avx2_utf8_length_mkl(input, N);
    linux_events.end(results);
    if (t >= warm_trials) {

      std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
      std::cout << "instructions/bytes " << double(results[1]) / (len) << " ";
      std::cout << "instructions/cycle " << double(results[1]) / results[0]
                << std::endl;
    }
  }
  if(len != expected) { abort(); }

  std::cout << std::endl;

  std::cout << "avx2 (mkl 2)" << std::endl;
  for (size_t t = 0; t < trials + warm_trials; t++) {

    linux_events.start();
    len = avx2_utf8_length_mkl2(input, N);
    linux_events.end(results);
    if (t >= warm_trials) {

      std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
      std::cout << "instructions/bytes " << double(results[1]) / (len) << " ";
      std::cout << "instructions/cycle " << double(results[1]) / results[0]
                << std::endl;
    }
  }
  if(len != expected) { abort(); }

  std::cout << std::endl;
  return EXIT_SUCCESS;
}
