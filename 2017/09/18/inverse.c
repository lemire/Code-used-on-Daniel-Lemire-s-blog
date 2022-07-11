// gcc -std=c99 -o inverse inverse.c
#include <assert.h>
#include <stdint.h> // use C99
#include <stdio.h>
#include <stdlib.h>

static inline uint16_t f16(uint16_t x, uint16_t y) { return y * (2 - y * x); }

static uint16_t findInverse16(uint16_t x) {
  uint32_t y = (3 * x) ^ 2; // 5 bits
  y = f16(x, y); // 10 bits
  y = f16(x, y); // 20 bits
  return y;
}

static inline uint32_t f32(uint32_t x, uint32_t y) { return y * (2 - y * x); }

static uint32_t findInverse32(uint32_t x) {
  uint32_t y = (3 * x) ^ 2; // 5 bits
  y = f32(x, y); // 10 bits
  y = f32(x, y); // 20 bits
  y = f32(x, y); // 30 bits
  return y;
}

static inline uint64_t f64(uint64_t x, uint64_t y) { return y * (2 - y * x); }

static uint64_t findInverse64(uint64_t x) {
  uint64_t y = (3 * x) ^ 2; // 5 bits
  y = f64(x, y); // 10 bits
  y = f64(x, y); // 20 bits
  y = f64(x, y); // 40 bits
  y = f64(x, y); // 80 bits
  return y;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("I expect an integer.\n");
    return EXIT_FAILURE;
  }
  long long val = strtoll(argv[1], NULL, 0);
  uint64_t val64 = (uint64_t)val; // wrap around negatives
  val64 |= 1;                     // make it odd if not already odd
  uint64_t inverse64 = findInverse64(val64);
  assert(inverse64 * val64 == 1);
  printf("The 64-bit inverse of %zu is %zu (%zx) \n", (size_t)val64,
         (size_t)inverse64, (size_t)inverse64);
  uint32_t val32 = (uint32_t)val64;
  uint32_t inverse32 = findInverse32(val32);
  assert(inverse32 * val32 == 1);
  printf("The 32-bit inverse of %u is %u (%x) \n", val32, inverse32, inverse32);
  uint16_t val16 = (uint16_t)val64;
  uint32_t inverse16 = findInverse16(val16);
  assert((uint16_t)(inverse16 * val16) == 1);
  printf("The 16-bit inverse of %u is %u (%x) \n", val16, inverse16, inverse16);
  return EXIT_SUCCESS;
}
