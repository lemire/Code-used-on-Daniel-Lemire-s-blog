#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <random>

double random(double from, double to) {
  thread_local std::mt19937 gen(std::random_device{}());
  thread_local std::uniform_real_distribution<double> dist;
  return dist(gen, std::uniform_real_distribution<>::param_type{from, to});
}

size_t counter = 0;
size_t counter2 = 0;


int compare(const void *a, const void *b) {
  double x, y;
  memcpy(&x, a, sizeof(x));
  memcpy(&y, b, sizeof(y));
  counter++;
  if (x < y) {
    return -1;
  }
  counter2++; // records the second comparison

  if (x == y) {
    return 0;
  }
  // if(x > y) { return 1; }
  return 1;
}

bool comparestl(const double x, const double y) {
  counter++;
  return x < y;
}

int main() {
  for (size_t k = 2; k <= 18; k++) {
    printf("N = 2**%zu\n ", k);
    size_t N = size_t(1) << k;
    double *buffer = new double[N];
    double *bufferstl = new double[N];
    double *original = new double[N];
    size_t T = 1000;
    size_t qsortcount = 0;
    size_t sortcount = 0;
    size_t qsortcounttotal = 0;
    for (size_t j = 0; j < T; j++) {
      for (size_t i = 0; i < N; i++) {
        original[i] = random(0, 1);
      }
      for (size_t i = 0; i < N; i++) {
        buffer[i] = original[i];
      }
      counter = 0; counter2 = 0;
      qsort((void *)buffer, N, sizeof(double), compare);
      qsortcount += counter;
      qsortcounttotal += counter2 + counter;

      for (size_t i = 1; i < N; i++) {
        if (buffer[i] < buffer[i - 1]) {
          printf("bug\n");
        }
      }
      for (size_t i = 0; i < N; i++) {
        bufferstl[i] = original[i];
      }
      counter = 0;
      std::sort(bufferstl, bufferstl + N, comparestl);
      sortcount += counter;
      for (size_t i = 0; i < N; i++) {
        if (buffer[i] != bufferstl[i]) {
          printf("bug\n");
        }
      }
    }
    printf("qsort comparison calls %f\n ", float(qsortcount) / (T * N));
    printf("qsort comparisons %f\n ", float(qsortcounttotal) / (T * N));
    printf("std::sort comparisons %f\n ", float(sortcount) / (T * N));
    delete[] buffer;
    delete[] bufferstl;
    delete[] original;
  }
  printf("Done.\n");
  return EXIT_SUCCESS;
}
