#include "C2LR2.h"

#include <cstring>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>

uint64_t nano() {
  return std::chrono::duration_cast< ::std::chrono::nanoseconds>(
      std::chrono::steady_clock::now().time_since_epoch()).count();
}
size_t counter = 0;

int qcompare(const void *a, const void *b) {
  int x, y;
  memcpy(&x, a, sizeof(x));
  memcpy(&y, b, sizeof(y));
  counter++;
  int result = (x < y) ? -1 : ((y > x) ? 0 : 1);
  return result;
}

int main() {
  size_t N = 1 << 10;

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 1000000000);
  for (size_t times = 0; times < 10; times++) {
    counter = 0;

    int **buffer = new int *[N];
    // srandom(1234);
    for (size_t i = 0; i < N; i++) {
      buffer[i] = new int();
      *buffer[i] = dist(rng);
    }
    cut2lr2((void **)buffer, 0, N - 1, qcompare);
    printf("cut2lr2 comparisons %f\n ", float(counter) / N);
    for (size_t i = 1; i < N; i++) {
      if (*buffer[i - 1] > *buffer[i]) {
        printf("bug\n");
      }
    }
    for (size_t i = 0; i < N; i++) {
      delete buffer[i];
    }
    delete[] buffer;
    counter = 0;

    int *sbuffer = new int[N];
    for (size_t i = 0; i < N; i++) {
      sbuffer[i] = dist(rng);
    }
    qsort((void *)sbuffer, N, sizeof(int), qcompare);
    printf("qsort comparisons %f\n ", float(counter) / N);
    for (size_t i = 1; i < N; i++) {
      if (sbuffer[i - 1] > sbuffer[i]) {
        printf("bug\n");
      }
    }
    delete[] sbuffer;

    printf("\n");
  }
}
