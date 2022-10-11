#include "C2LR2.h"

#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}
struct data_point {
  int key;
  double playload;
};
size_t counter = 0;

int compare(const void *a, const void *b) {
  counter++;
  return reinterpret_cast<const data_point *>(a)->key <
                 reinterpret_cast<const data_point *>(b)->key
             ? -1
             : (reinterpret_cast<const data_point *>(a)->key >
                        reinterpret_cast<const data_point *>(b)->key
                    ? 1
                    : 0);
}

int qcompare(const void *a, const void *b) {
  counter++;
  return *(int *)a < *(int *)b ? -1 : (*(int *)a > *(int *)b ? 1 : 0);
}
int main() {
  size_t N = 1000000;

  for (size_t times = 0; times < 5; times++) {
    counter = 0;

    data_point **buffer = new data_point *[N];
    srandom(1234);
    for (size_t i = 0; i < N; i++) {
      buffer[i] = new data_point();
      buffer[i]->key = rand();
    }
    cut2lr2((void **)buffer, 0, N - 1, compare);
    printf("cut2lr2 comparisons %f\n ", float(counter) / N);
    for (size_t i = 0; i < N; i++) {
      delete buffer[i];
    }
    delete[] buffer;
    counter = 0;

    int *sbuffer = new int[N];
    for (size_t i = 0; i < N; i++) {
      sbuffer[i] = rand();
    }
    qsort((void *)sbuffer, N, sizeof(int), qcompare);
    printf("qsort comparisons %f\n ", float(counter) / N);
    delete[] sbuffer;

    printf("\n");
  }
}