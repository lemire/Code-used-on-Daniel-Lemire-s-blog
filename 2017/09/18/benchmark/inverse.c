#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

static inline uint32_t f32(uint32_t x, uint32_t y) { return y * (2 - y * x); }

static uint32_t findInverse32(uint32_t x) {
  uint32_t y = x;
  y = f32(x, y);
  y = f32(x, y);
  y = f32(x, y);
  y = f32(x, y);
  return y;
}

static uint32_t findInverse32b(uint32_t a) {
  uint32_t x = (3 * a) ^ 2;
  x *= 2 - a * x;
  x *= 2 - a * x;
  x *= 2 - a * x;
  return x;
}

uint64_t findInverse32c(uint32_t a) {
  const uint8_t t[] = {
      2, 174, 210, 190, 66, 174, 210, 254, 2, 46, 82, 190, 66, 46, 82, 254,
  };
  uint32_t x;
  x = t[(a >> 1) & 15] - a; // 8bit precision
  x *= 2 - a * x;           // 16
  x *= 2 - a * x;           // 32
  return x;
}

static inline uint64_t f64(uint64_t x, uint64_t y) { return y * (2 - y * x); }

static uint64_t findInverse64(uint64_t x) {
  uint64_t y = x;
  y = f64(x, y);
  y = f64(x, y);
  y = f64(x, y);
  y = f64(x, y);
  y = f64(x, y);
  return y;
}

static uint64_t findInverse64b(uint64_t x) {
  uint64_t y = (3 * x) ^ 2;
  ;
  y = f64(x, y);
  y = f64(x, y);
  y = f64(x, y);
  y = f64(x, y);
  return y;
}

uint64_t findInverse64c(uint64_t a) {
  const uint8_t t[] = {
      2, 174, 210, 190, 66, 174, 210, 254, 2, 46, 82, 190, 66, 46, 82, 254,
  };
  uint64_t x;
  x = t[(a >> 1) & 15] - a; // 8bit precision
  x *= 2 - a * x;           // 16
  x *= 2 - a * x;           // 32
  x *= 2 - a * x;           // 64
  return x;
}

uint32_t arrayinverse32(uint32_t *x, size_t N) {
  for (size_t i = 0; i < N; i++)
    x[i] = findInverse32(x[i]);
  return x[0];
}

uint32_t arrayinverse32b(uint32_t *x, size_t N) {
  for (size_t i = 0; i < N; i++)
    x[i] = findInverse32b(x[i]);
  return x[0];
}

uint32_t arrayinverse32c(uint32_t *x, size_t N) {
  for (size_t i = 0; i < N; i++)
    x[i] = findInverse32c(x[i]);
  return x[0];
}

uint64_t arrayinverse64(uint64_t *x, size_t N) {
  for (size_t i = 0; i < N; i++)
    x[i] = findInverse64(x[i]);
  return x[0];
}

uint64_t arrayinverse64b(uint64_t *x, size_t N) {
  for (size_t i = 0; i < N; i++)
    x[i] = findInverse64b(x[i]);
  return x[0];
}

uint64_t arrayinverse64c(uint64_t *x, size_t N) {
  for (size_t i = 0; i < N; i++)
    x[i] = findInverse64c(x[i]);
  return x[0];
}

void demo(size_t N) {
  uint32_t *data = malloc(N * sizeof(uint32_t));
  uint32_t *buffer = malloc(N * sizeof(uint32_t));
  memcpy(buffer, data, N * sizeof(uint32_t));
  for (size_t i = 0; i < N; i++) {
    data[i] = (((uint32_t)rand() << 16) ^ (uint32_t)rand()) | 1;
  }
  uint32_t expected32 = findInverse32(data[0]);
  int repeat = 5;
  BEST_TIME(arrayinverse32(buffer, N), expected32,
            memcpy(buffer, data, N * sizeof(uint32_t)), repeat, N, true);
  BEST_TIME(arrayinverse32b(buffer, N), expected32,
            memcpy(buffer, data, N * sizeof(uint32_t)), repeat, N, true);
  BEST_TIME(arrayinverse32c(buffer, N), expected32,
            memcpy(buffer, data, N * sizeof(uint32_t)), repeat, N, true);
  uint64_t expected64 = findInverse64(((uint64_t *)data)[0]);

  BEST_TIME(arrayinverse64((uint64_t *)buffer, N / 2), expected64,
            memcpy(buffer, data, N * sizeof(uint32_t)), repeat, N / 2, true);
  BEST_TIME(arrayinverse64b((uint64_t *)buffer, N / 2), expected64,
            memcpy(buffer, data, N * sizeof(uint32_t)), repeat, N / 2, true);
  BEST_TIME(arrayinverse64b((uint64_t *)buffer, N / 2), expected64,
            memcpy(buffer, data, N * sizeof(uint32_t)), repeat, N / 2, true);
  free(data);
}

int main() {

  demo(10000000);
  return EXIT_SUCCESS;
}
