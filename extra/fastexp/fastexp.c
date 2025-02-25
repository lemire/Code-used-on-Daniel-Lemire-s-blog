#include <stdint.h>
#include <stdio.h>
// Computes (base^exp) mod mod using exponentiation by squaring
uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod) {
  __uint128_t result = 1;
  __uint128_t b = base % mod; // Ensure base < mod
  while (exp > 0) {
    if (exp & 1) { // If current bit of exp is 1
      result = (result * b) % mod;
    }
    b = (b * b) % mod; // Square the base
    exp >>= 1;         // Shift exponent right
  }
  return (uint64_t)result;
}

// Computes 2^(n-1) mod n
uint64_t compute(uint64_t n) {
  if (n <= 1)
    return 0; // Handle n = 0 and n = 1
  return pow_mod(2, n - 1, n);
}

typedef struct {
  __uint128_t low;
  __uint128_t high;
} uint256_t;

uint256_t mul256(__uint128_t a, __uint128_t b) {
  uint256_t result = {0, 0};

  // Compute a * b as a 256-bit result
  // Split a and b into 64-bit halves for manageable multiplication:
  // a = a_high * 2^64 + a_low, b = b_high * 2^64 + b_low
  uint64_t a_low = (uint64_t)a;          // Lower 64 bits of a
  uint64_t a_high = (uint64_t)(a >> 64); // Upper 64 bits of a
  uint64_t b_low = (uint64_t)b;          // Lower 64 bits of b
  uint64_t b_high = (uint64_t)(b >> 64); // Upper 64 bits of b

  // Full product: (a_high * 2^64 + a_low) * (b_high * 2^64 + b_low)
  //             = a_high * b_high * 2^128 + (a_high * b_low + a_low * b_high) *
  //             2^64 + a_low * b_low

  // Low * Low (128-bit result)
  __uint128_t low_low = (__uint128_t)a_low * b_low;
  result.low = low_low; // Lower 128 bits of result

  // Cross terms: a_high * b_low and a_low * b_high (each 128-bit results)
  __uint128_t a_high_b_low = (__uint128_t)a_high * b_low;
  __uint128_t a_low_b_high = (__uint128_t)a_low * b_high;

  // High * High (128-bit result)
  __uint128_t high_high = (__uint128_t)a_high * b_high;

  // Add cross terms and carry from low_low to the high part
  __uint128_t carry = low_low >> 64; // Upper 64 bits of low_low
  result.high = high_high + (a_high_b_low >> 64) + (a_low_b_high >> 64) + carry;

  // Add the lower 64 bits of cross terms to result.low and propagate carry
  __uint128_t cross_low = (a_high_b_low & (((__uint128_t)1 << 64) - 1)) +
                          (a_low_b_high & (((__uint128_t)1 << 64) - 1));
  result.low += cross_low << 64;
  result.high += (cross_low >> 64); // Add any carry from cross_low to high

  return result;
}
__uint128_t mod256(uint256_t a, __uint128_t b) {
  // Handle division by zero
  if (b == 0) {
    // Undefined behavior, return 0 or handle error as needed
    return 0;
  }

  // If a < b (treating b as a 256-bit number with high part 0), then a % b = a
  if (a.high == 0 && a.low < b) {
    return a.low;
  }

  // Bit-by-bit long division
  uint256_t remainder = {0, 0};
  int bits = 256;

  for (int i = bits - 1; i >= 0; i--) {
    // Shift remainder left by 1
    remainder.high = (remainder.high << 1) | (remainder.low >> 127);
    remainder.low = remainder.low << 1;

    // Add the current bit of a
    if (i >= 128) {
      if (a.high & ((__uint128_t)1 << (i - 128))) {
        remainder.low |= 1;
      }
    } else {
      if (a.low & ((__uint128_t)1 << i)) {
        remainder.low |= 1;
      }
    }

    // If remainder >= b (comparing remainder to b as a 128-bit number)
    if (remainder.high > 0 || remainder.low >= b) {
      // Subtract b from remainder
      if (remainder.high > 0) {
        remainder.high -= 1;
        remainder.low -= b;
        if (remainder.low > b) { // Check for borrow
          remainder.high += 1;
        }
      } else {
        remainder.low -= b;
      }
    }
  }

  return remainder.low;
}

// Computes (base^exp) mod mod using exponentiation by squaring
__uint128_t pow_mod128(__uint128_t base, __uint128_t exp, __uint128_t mod) {
  __uint128_t result = 1;
  __uint128_t b = base % mod; // Ensure base < mod
  while (exp > 0) {
    if (exp & 1) { // If current bit of exp is 1
      result = mod256(mul256(result, b), mod);
    }
    b = (b * b) % mod; // Square the base
    exp >>= 1;         // Shift exponent right
  }
  return result;
}

// Computes 2^(n-1) mod n
uint64_t compute128(__uint128_t n) {
  if (n <= 1)
    return 0; // Handle n = 0 and n = 1
  return pow_mod128(2, n - 1, n);
}

int main() {
  printf("%llu %llu\n", compute(10000), compute128(10000));
  printf("%llu %llu\n", compute(109551615), compute128(109551615ULL));
}

