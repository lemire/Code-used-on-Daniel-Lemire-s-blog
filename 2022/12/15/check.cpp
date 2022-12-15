#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <x86intrin.h>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

bool is_present_naive(const char *in, size_t len, const char *needle,
                size_t needle_len) {
  for (size_t i = 0; i + needle_len - 1 < len; i ++) {
    bool matches = (needle[0] == in[i]);
    for(size_t char_index = 1; matches && char_index < needle_len; char_index++) {
        matches &= (needle[char_index] == in[i + char_index]);
    }
    if(matches) { return true; }
  }
  return false;
}


// no such thing as avx128
bool is_present_avx128(const char *in, size_t len, const char *needle,
                size_t needle_len) {
  if (needle_len == 0) {
    return true;
  }
  size_t i = 0;
  for (; i + 64 + needle_len - 1 < len; i += 16) {
    __m128i comparator = _mm_set1_epi8(needle[0]);
    __m128i input = _mm_loadu_si128((const __m128i *)(in + i));
    __mmask64 matches = _mm_cmpeq_epi8_mask(comparator, input);
    for (size_t char_index = 1; matches && char_index < needle_len; char_index++) {
      comparator = _mm_set1_epi8(needle[char_index]);
      input = _mm_loadu_si128((const __m128i *)(in + i + char_index));
      matches =
          _kand_mask64(matches, _mm_cmpeq_epi8_mask(comparator, input));
    }
    if(matches) { return true; }
  }
  if (i + needle_len - 1 < len) {
    size_t char_index = 0;
    __m128i comparator = _mm_set1_epi8(needle[char_index]);
    __mmask64 mask = len - i >= 64 ? -1 : (uint64_t(1) << (len - i)) - 1;
    __m128i input = _mm_maskz_loadu_epi8(mask, in + i + char_index);
    __mmask64 matches = _mm_mask_cmpeq_epi8_mask(mask, comparator, input);
    for (size_t char_index = 1; matches && char_index < needle_len; char_index++) {
      comparator = _mm_set1_epi8(needle[char_index]);
      mask = len - i - char_index >= 64
                 ? -1
                 : (uint64_t(1) << (len - i - char_index)) - 1;
      input = _mm_maskz_loadu_epi8(mask, in + i + char_index);
      matches = _kand_mask64(
          matches, _mm_mask_cmpeq_epi8_mask(mask, comparator, input));
    }
    if(matches) { return true; }
  }
  return false;
}

// no such thing as avx256
bool is_present_avx256(const char *in, size_t len, const char *needle,
                size_t needle_len) {
  if (needle_len == 0) {
    return true;
  }
  size_t i = 0;
  for (; i + 64 + needle_len - 1 < len; i += 32) {
    size_t char_index = 0;
    __m256i comparator = _mm256_set1_epi8(needle[char_index]);
    __m256i input = _mm256_loadu_si256((const __m256i *)(in + i + char_index));
    __mmask64 matches = _mm256_cmpeq_epi8_mask(comparator, input);
    for (size_t char_index = 1; matches && char_index < needle_len; char_index++) {
      comparator = _mm256_set1_epi8(needle[char_index]);
      input = _mm256_loadu_si256((const __m256i *)(in + i + char_index));
      matches =
          _kand_mask64(matches, _mm256_cmpeq_epi8_mask(comparator, input));
    }
    if(matches) { return true; }
  }
  if (i + needle_len - 1 < len) {
    size_t char_index = 0;
    __m256i comparator = _mm256_set1_epi8(needle[char_index]);
    __mmask64 mask = len - i >= 64 ? -1 : (uint64_t(1) << (len - i)) - 1;
    __m256i input = _mm256_maskz_loadu_epi8(mask, in + i + char_index);
    __mmask64 matches = _mm256_mask_cmpeq_epi8_mask(mask, comparator, input);
    for (size_t char_index = 1; matches && char_index < needle_len; char_index++) {
      comparator = _mm256_set1_epi8(needle[char_index]);
      mask = len - i - char_index >= 64
                 ? -1
                 : (uint64_t(1) << (len - i - char_index)) - 1;
      input = _mm256_maskz_loadu_epi8(mask, in + i + char_index);
      matches = _kand_mask64(
          matches, _mm256_mask_cmpeq_epi8_mask(mask, comparator, input));
    }
    if(matches) { return true; }
  }
  return false;
}

bool is_present_avx512(const char *in, size_t len, const char *needle,
                size_t needle_len) {
  if (needle_len == 0) {
    return true;
  }
  size_t i = 0;
  for (; i + 64 + needle_len - 1 < len; i += 64) {
    size_t char_index = 0;
    __m512i comparator = _mm512_set1_epi8(needle[char_index]);
    __m512i input = _mm512_loadu_si512(in + i + char_index);
    __mmask64 matches = _mm512_cmpeq_epi8_mask(comparator, input);
    for (size_t char_index = 1; matches && char_index < needle_len; char_index++) {
      comparator = _mm512_set1_epi8(needle[char_index]);
      input = _mm512_loadu_si512(in + i + char_index);
      matches =
          _kand_mask64(matches, _mm512_cmpeq_epi8_mask(comparator, input));
    }
    if(matches) { return true; }
  }
  if (i + needle_len - 1 < len) {
    size_t char_index = 0;
    __m512i comparator = _mm512_set1_epi8(needle[char_index]);
    __mmask64 mask = len - i >= 64 ? -1 : (uint64_t(1) << (len - i)) - 1;
    __m512i input = _mm512_maskz_loadu_epi8(mask, in + i + char_index);
    __mmask64 matches = _mm512_mask_cmpeq_epi8_mask(mask, comparator, input);
    for (size_t char_index = 1; matches && char_index < needle_len; char_index++) {
      comparator = _mm512_set1_epi8(needle[char_index]);
      mask = len - i - char_index >= 64
                 ? -1
                 : (uint64_t(1) << (len - i - char_index)) - 1;
      input = _mm512_maskz_loadu_epi8(mask, in + i + char_index);
      matches = _kand_mask64(
          matches, _mm512_mask_cmpeq_epi8_mask(mask, comparator, input));
    }
    if(matches) { return true; }
  }
  return false;
}

int main() {
  size_t N = 1000000;
  char *buffer = new char[N + 1];
  for (size_t i = 0; i < N; i++) {
    buffer[i] = i;
    if (buffer[i] == 0)
      buffer[i] += 1;
  }
  buffer[N] = '\0';
  size_t M = 15;
  char *needle = new char[M + 1];
  memset(needle, 0, M);
  needle[0] = '1';
  needle[1] = '2';

  for (size_t needle_len = 2; needle_len < M; needle_len++) {
    for(size_t k = 0; k < needle_len; k++) {
        needle[k] = '0' + k;
    }
    needle[needle_len-1] = 'a';
    needle[needle_len] = '\0';
    printf("\nneedle_len = %ld : %s \n", needle_len, needle);

    {
      uint64_t start = nano();
      uint64_t finish = start;
      size_t count{0};
      uint64_t threshold = 500'000'000;
      volatile int bogus = 0;
      for (; finish - start < threshold;) {
        count++;
        bogus += is_present_naive(buffer, N, needle, strlen(needle));
        finish = nano();
      }
      double t = (N * count) / double(finish - start);

      printf("naive %f GB/s\n", t);
    }
    {
      uint64_t start = nano();
      uint64_t finish = start;
      size_t count{0};
      uint64_t threshold = 500'000'000;
      volatile int bogus = 0;
      for (; finish - start < threshold;) {
        count++;
        bogus += is_present_avx128(buffer, N, needle, strlen(needle));
        finish = nano();
      }
      double t = (N * count) / double(finish - start);

      printf("avx-128 %f GB/s\n", t);
    }
    {
      uint64_t start = nano();
      uint64_t finish = start;
      size_t count{0};
      uint64_t threshold = 500'000'000;
      volatile int bogus = 0;
      for (; finish - start < threshold;) {
        count++;
        bogus += is_present_avx256(buffer, N, needle, strlen(needle));
        finish = nano();
      }
      double t = (N * count) / double(finish - start);

      printf("avx-256 %f GB/s\n", t);
    }

    {
      uint64_t start = nano();
      uint64_t finish = start;
      size_t count{0};
      uint64_t threshold = 500'000'000;
      volatile int bogus = 0;
      for (; finish - start < threshold;) {
        count++;
        bogus += is_present_avx512(buffer, N, needle, strlen(needle));
        finish = nano();
      }
      double t = (N * count) / double(finish - start);

      printf("avx-512 %f GB/s\n", t);
    }

    {
      uint64_t start = nano();
      uint64_t finish = start;
      size_t count{0};
      uint64_t threshold = 500'000'000;
      volatile int bogus = 0;
      for (; finish - start < threshold;) {
        count++;
        bogus += (strstr(buffer, needle) != NULL);
        finish = nano();
      }
      double t = (N * count) / double(finish - start);

      printf("strstr %f GB/s\n", t);
    }
  }

  delete[] needle;

  delete[] buffer;
  return EXIT_SUCCESS;
}