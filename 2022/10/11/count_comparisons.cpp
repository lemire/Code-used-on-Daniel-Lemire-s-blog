#include <cstring>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>

double random(double from, double to) {
  thread_local std::mt19937 gen(std::random_device {}());
  thread_local std::uniform_real_distribution<double> dist;
  return dist(gen, std::uniform_real_distribution<>::param_type{ from, to });
}

size_t counter = 0;

int compare(const void *a, const void *b) {
  double x, y;
  memcpy(&x, a, sizeof(x));
  memcpy(&y, b, sizeof(y));
  counter++;
  int result = (x < y) ? -1 : ((y > x) ? 0 : 1);
  return result;
}

bool comparestl(const double x, const double y) {
  counter++;

  return x < y;
}

int main() {
  for (size_t k = 2; k <= 18; k++) {
    printf("N = 2** %zu\n ", k);

    size_t N = size_t(1) << k;
    double *buffer = new double[N];
    double *bufferstl = new double[N];
    double *original = new double[N];
    for (size_t i = 0; i < N; i++) {
      original[i] = random(0, 1);
    }
    for (size_t i = 0; i < N; i++) {
      buffer[i] = original[i];
    }
    counter = 0;
    qsort((void *)buffer, N, sizeof(double), compare);
    printf("qsort comparisons %f\n ", float(counter) / N);

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
    printf("std::sort comparisons %f\n ", float(counter) / N);
    printf("sorted to :");
    for (size_t i = 0; i < N; i++) {
      if (buffer[i] != bufferstl[i]) {
        printf("bug\n");
      }
    }

    delete[] buffer;
    delete[] bufferstl;
    delete[] original;
  }
  printf("Done,\n");
  return EXIT_SUCCESS;
}
