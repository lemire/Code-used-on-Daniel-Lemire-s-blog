#include <arpa/inet.h>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
extern "C" {
#include "sse_inet_aton.h"
}
// https://lemire.me/blog/2023/02/01/serializing-ips-quickly-in-c/
std::string_view ipv4(std::string &output, const uint64_t address) {
  char *point = output.data();
  char *point_end = output.data() + output.size();
  point = std::to_chars(point, point_end, uint8_t(address)).ptr;
  for (int i = 1; i <= 3; i++) {
    *point++ = '.';
    point = std::to_chars(point, point_end, uint8_t(address >> (i * 8))).ptr;
  }
  return {output.data(), size_t(point - output.data())};
}

bool test_exhaustive() {
  int errors = 0;

#pragma omp parallel for reduction(+ : errors)
  for (uint64_t x = 0; x <= 0xffffffff; x++) {
    std::string output(16, '\0');
    int err;
    if ((x & 0xffffff) == 0) {
      printf(".");
      fflush(NULL);
    }
    std::string_view view = ipv4(output, x);
    output[view.size()] = '\0';
    uint32_t ipv4;
    err = sse_inet_aton(view.data(), view.size(), &ipv4);
    if (err != 1) {
      printf("[sse_inet_aton] non-one error code\n");
      printf(" value %x \n", err);
      errors++;
    }
    if (ipv4 != x) {
      printf("[sse_inet_aton] bad value %x %x \n", ipv4, uint32_t(x));
      errors++;
    }
    err = inet_pton(AF_INET, view.data(), &ipv4);
    if (err != 1) {
      printf("[inet_pton] non-one error code\n");
      printf(" value %x \n", err);
      errors++;
    }
    if (ipv4 != x) {
      printf("[inet_pton] bad value %x %x \n", ipv4, uint32_t(x));
      errors++;
    }
  }
  printf("\n");
  if (errors) {
    return false;
  }
  printf("SUCCESS\n");
  return true;
}

bool test_bad() {
  std::string bad = "111.1111.1.111";
  bad.reserve(16);
  uint32_t ipv4;
  int err = sse_inet_aton(bad.data(), bad.size(), &ipv4);
  if (err == 1) {
    std::cout << " it should not allow this string to be valid " << bad
              << std::endl;
    return false;
  }
  return true;
}

#include <x86intrin.h>
static void bitmask_to_teststring(uint16_t bitmap, char *buf) {
  const __m128i bit_id =
      _mm_set_epi32(0x80402010, 0x08040201, 0x80402010, 0x08040201);
  const __m128i shuffle_id = _mm_set_epi32(0x01010101, 0x01010101, 0, 0);
  __m128i v = _mm_shuffle_epi8(_mm_cvtsi32_si128(bitmap), shuffle_id);
  v = _mm_and_si128(v, bit_id);
  v = _mm_cmpeq_epi8(v, bit_id);

  const __m128i digit_one = _mm_set1_epi8(0x31);
  const __m128i dot_char = _mm_set1_epi8(0x2E);

  // each set bit becomes a dot, everything else is a `1`
  __m128i r = _mm_blendv_epi8(digit_one, dot_char, v);
  _mm_storeu_si128((__m128i *)buf, r);
}

bool test_adversarial() {

  uint64_t n = 0;
#pragma omp parallel for reduction(+ : n)
  for (uint16_t i = 0; i < 0x8000; i++) {
    char buf[16];
    bitmask_to_teststring(i, buf);
    for (int j = 15; j != 0; j--) {
      uint32_t ipv4_1;
      uint32_t ipv4_2;
      int res = sse_inet_aton(buf, strlen(buf), &ipv4_1);
      int ref = inet_pton(AF_INET, buf, &ipv4_2);
      if ((ref != res) || (ref == 1 && ipv4_1 != ipv4_2)) {
        printf("%s\n", buf);
        n++;
      }
    }
  }
  printf("errors: %d\n", (uint32_t)n);
  return n == 0;
}

int main() {
  return (test_bad() && test_adversarial() && test_exhaustive()) ? EXIT_SUCCESS
                                                   : EXIT_FAILURE;
}