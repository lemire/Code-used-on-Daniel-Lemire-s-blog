#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <x86intrin.h>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

void print(__m512i x) {
  char buffer[65];
  _mm512_storeu_epi8(buffer, x);
  buffer[64] = '\0';
  for (size_t i = 0; i < 64; i++) {
    if (buffer[i] < 32)
      printf("%x ", int(buffer[i]));
    else
      printf("%c ", buffer[i]);
  }

  printf("\n");
}
size_t escape(const char *in, size_t len, char *out) {
  const char *const finalin = in + len;
  const char *const initout = out;
  for (; in < finalin; in++) {
    if ((*in == '\\') || (*in == '"')) {
      *out = '\\';
      out++;
    }
    *out = *in;
    out++;
  }
  return out - initout;
}

size_t avx512_escape(const char *in, size_t len, char *out) {
  const char *const finalin = in + len;
  const char *const initout = out;
  __m512i solidus = _mm512_set1_epi8('\\');
  __m512i quote = _mm512_set1_epi8('"');
  for (; in + 32 <= finalin; in += 32) {
    __m256i input = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(in));
    __m512i input1 = _mm512_cvtepu8_epi16(input);
    __m512i shifted_input1 = _mm512_bslli_epi128(input1, 1);

    __mmask64 is_solidus = _mm512_cmpeq_epi8_mask(input1, solidus);
    __mmask64 is_quote = _mm512_cmpeq_epi8_mask(input1, quote);
    __mmask64 is_quote_or_solidus = _kor_mask64(is_solidus, is_quote);
    __mmask64 to_keep = _kor_mask64(is_quote_or_solidus, 0xaaaaaaaaaaaaaaaa);

    __m512i escaped =
        _mm512_mask_blend_epi8(is_quote_or_solidus, shifted_input1, solidus);
    _mm512_mask_compressstoreu_epi8(out, to_keep, escaped);
    out += _mm_popcnt_u64(_cvtmask64_u64(to_keep));
  }
  for (; in < finalin; in++) {
    if ((*in == '\\') || (*in == '"')) {
      *out = '\\';
      out++;
    }
    *out = *in;
    out++;
  }
  return out - initout;
}

int main() {
  size_t N = 50000;
  char *test_input = (char *)malloc(N + 1);
  for (size_t i = 0; i < N; i++) {
    test_input[i] = (rand() % 96) + 32;
  }
  char *test_output = (char *)malloc(2 * N + 1);
  size_t len = avx512_escape(test_input, N, test_output);
  size_t lens = escape(test_input, N, test_output);
  if (lens != len) {
    printf("bug\n");
  }

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500'000'000;
    double bogus = 0;
    for (; finish - start < threshold;) {
      count++;
      bogus += avx512_escape(test_input, N, test_output);
      finish = nano();
    }
    if (bogus == 0.5) {
      printf("zero\n");
    }
    double t = double(finish - start) / (N * count);

    printf("%f ns\n", t);
  }

  {

    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500'000'000;
    double bogus = 0;
    for (; finish - start < threshold;) {
      count++;
      bogus += escape(test_input, N, test_output);
      finish = nano();
    }
    if (bogus == 0.5) {
      printf("zero\n");
    }
    double t = double(finish - start) / (N * count);

    printf("%f ns\n", t);
  }
}