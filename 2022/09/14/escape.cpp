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
#include "escape.h"

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

int main() {
  size_t N = 50000;
  char *test_input = (char *)malloc(N + 1);
  for (size_t i = 0; i < N; i++) {
    test_input[i] = (rand() % 96) + 32;
  }
  char *test_output = (char *)malloc(2 * N + 1);
  size_t len = Optimized::escape(test_input, N, test_output);
  size_t lens = Original::escape(test_input, N, test_output);
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
      bogus += Optimized::escape(test_input, N, test_output);
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
      bogus += Original::escape(test_input, N, test_output);
      finish = nano();
    }
    if (bogus == 0.5) {
      printf("zero\n");
    }
    double t = double(finish - start) / (N * count);

    printf("%f ns\n", t);
  }
}
