#include <stdint.h>
#include <stdio.h>
#include <time.h>
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

#define UINT128(hi, lo) (((__uint128_t)(hi)) << 64 | (lo))

uint64_t mod_inverse(uint64_t m) {
  uint64_t x = m;               // Start with x ≡ m^{-1} mod 2
  for (int i = 0; i < 5; i++) { // 5 iterations suffice for 64 bits
    x = x * (2 - m * x);        // Newton's method: x = x * (2 - m * x)
  }
  return x;
}

uint64_t montgomery_mul(uint64_t a, uint64_t b, uint64_t m, uint64_t m_prime) {
  __uint128_t t = (__uint128_t)a * b; // t = a * b
  uint64_t t_low = (uint64_t)t;       // Lower 64 bits
  uint64_t k =
      (t_low * m_prime) & 0xFFFFFFFFFFFFFFFFULL; // k = (t_low * m') mod 2^64
  __uint128_t u = t + (__uint128_t)k * m;        // u = t + k * m
  uint64_t result = (uint64_t)(u >> 64);         // result = u / 2^64
  if (result >= m)
    result -= m; // Ensure result < m
  return result;
}

uint64_t montgomery_pow_mod(uint64_t base, uint64_t exp, uint64_t mod) {
  if (mod == 1)
    return 0; // Special case: anything mod 1 is 0
  // Precompute Montgomery parameters
  uint64_t m_prime = -mod_inverse(mod);    // -m^{-1} mod 2^64
  __uint128_t temp = (__uint128_t)1 << 64; // r = 2^64
  uint64_t r_mod_m = temp % mod;           // r mod m
  uint64_t r_squared_mod_m =
      ((__uint128_t)r_mod_m * r_mod_m) % mod; // r^2 mod m

  // Convert base to Montgomery form
  uint64_t b = base % mod; // Ensure b < mod
  uint64_t b_mont =
      montgomery_mul(b, r_squared_mod_m, mod, m_prime); // b * r mod m

  // Initialize result as 1 in Montgomery form
  uint64_t result = r_mod_m; // 1 * r mod m

  // Exponentiation loop
  while (exp > 0) {
    if (exp & 1) {
      result = montgomery_mul(result, b_mont, mod,
                              m_prime); // result * b_mont * r^{-1} mod m
    }
    b_mont =
        montgomery_mul(b_mont, b_mont, mod, m_prime); // b_mont^2 * r^{-1} mod m
    exp >>= 1;
  }

  // Convert back from Montgomery form
  result = montgomery_mul(result, 1, mod, m_prime); // result * r^{-1} mod m
  return result;
}

// Computes 2^(n-1) mod n
uint64_t montgomery_compute(uint64_t n) {
  if (n <= 1)
    return 0; // Handle n = 0 and n = 1
  int r = 0;
  while (((1ULL << r) & n) == 0)
    r++;
  return montgomery_pow_mod(2, n - 1 - r, n >> r) << r;
}

__uint128_t mod_inverse128(__uint128_t m) {
  __uint128_t x = m;
  for (int i = 0; i < 7; i++) {
    x = x * ((__uint128_t)(2) - m * x);
  }
  return x;
}

uint256_t multiply128(__uint128_t a, __uint128_t b) {
  uint64_t a0 = (uint64_t)(a);
  uint64_t a1 = (uint64_t)(a >> 64);
  uint64_t b0 = (uint64_t)(b);
  uint64_t b1 = (uint64_t)(b >> 64);

  __uint128_t p0 = (__uint128_t)(a0)*b0;
  __uint128_t p1 = (__uint128_t)(a0)*b1;
  __uint128_t p2 = (__uint128_t)(a1)*b0;
  __uint128_t p3 = (__uint128_t)(a1)*b1;

  uint256_t result = {p0, 0};
  __uint128_t mid = p1 + p2;
  __uint128_t carry = mid < p1; // Overflow check
  result.low += mid << 64;
  carry += (result.low < (mid << 64));
  result.high += (mid >> 64) + carry + p3;

  return result;
}

uint256_t add(uint256_t a, uint256_t b) {
  uint256_t result;
  result.low = a.low + b.low;
  __uint128_t carry = (result.low < a.low);
  result.high = a.high + b.high + carry;
  return result;
}

__uint128_t montgomery_mul128(__uint128_t a, __uint128_t b, __uint128_t m,
                              __uint128_t m_prime) {
  uint256_t t = multiply128(a, b);
  __uint128_t t_low = t.low;
  __uint128_t k = t_low * m_prime; // mod 2^128 automatic
  uint256_t km = multiply128(k, m);
  uint256_t u = add(t, km);
  __uint128_t result = u.high;
  if (result >= m)
    result -= m;
  return result;
}

__uint128_t montgomery_pow_mod128(__uint128_t base, __uint128_t exp,
                                  __uint128_t mod) {
  if (mod == 1)
    return 0;
  __uint128_t m_prime = -mod_inverse128(mod);
  uint256_t two_to_128 = {0, 1};
  __uint128_t r_mod_m = mod256(two_to_128, mod);
  __uint128_t r_squared_mod_m = mod256(multiply128(r_mod_m, r_mod_m), mod);

  __uint128_t b = base % mod;
  __uint128_t b_mont = montgomery_mul128(b, r_squared_mod_m, mod, m_prime);
  __uint128_t result = r_mod_m;

  while (exp > 0) {
    if (exp & 1) {
      result = montgomery_mul128(result, b_mont, mod, m_prime);
    }
    b_mont = montgomery_mul128(b_mont, b_mont, mod, m_prime);
    exp >>= 1;
  }
  result = montgomery_mul128(result, 1, mod, m_prime);
  return result;
}

__uint128_t montgomery_compute128(__uint128_t n) {
  if (n <= 1)
    return 0;
  int r = 0;
  while ((((__uint128_t)(1) << r) & n) == 0)
    r++;
  return montgomery_pow_mod128(2, n - 1 - r, n >> r) << r;
}

int main() {
  for (uint64_t i = 1; i < 1000000000000000000ULL; i *= 10) {
    printf("%llu: %llu %llu %llu\n", i, compute(i), montgomery_compute(i),
           (uint64_t)montgomery_compute128(i));
  }

  clock_t start, end;
  double time_taken;
  long iterations = 1000000; // 1 million iterations

  // Record start time
  start = clock();

  uint64_t result = 0;

  // Execute compute() 1 million times
  for (int i = 0; i < iterations; i++) {
    result += compute(1000000000000000000ULL + i);
  }

  // Record end time
  end = clock();

  // Calculate time taken in milliseconds
  time_taken = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;

  printf("Time taken to execute compute() %ld times: %.2f milliseconds\n",
         iterations, time_taken);
  printf("%llu\n", result);



  // Record start time
  start = clock();

  result = 0;

  // Execute montgomery_compute() 1 million times
  for (int i = 0; i < iterations; i++) {
    result += montgomery_compute(1000000000000000000ULL + i);
  }

  // Record end time
  end = clock();

  // Calculate time taken in milliseconds
  time_taken = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;

  printf("Time taken to execute montgomery_compute() %ld times: %.2f milliseconds\n",
         iterations, time_taken);
         printf("%llu\n", result);




  // Record start time
  start = clock();

  result = 0;

  // Execute montgomery_compute128() 1 million times
  for (int i = 0; i < iterations; i++) {
    result += montgomery_compute128(1000000000000000000ULL + i);
  }

  // Record end time
  end = clock();

  // Calculate time taken in milliseconds
  time_taken = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;

  printf("Time taken to execute montgomery_compute128() %ld times: %.2f milliseconds\n",
         iterations, time_taken);
         printf("%llu\n", result);
}
