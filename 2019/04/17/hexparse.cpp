#include "linux-perf-events.h"
#include <cassert>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <libgen.h>
#include <random>
#include <string>
#include <vector>
#include <x86intrin.h>

const signed char digittoval[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1};
// returns a value with the high 16 bits set if not valid
// otherwise returns the conversion of the 4 hex digits at src into the bottom
// 16 bits of the 32-bit
// return register
//
__attribute__ ((noinline)) // we do not want the compiler to rewrite the problem
uint32_t hex_to_u32_lookup(
    const uint8_t *src) { // strictly speaking, static inline is a C-ism
  uint32_t v1 =
      digittoval[src[0]]; // uint32_t v1 = -1 becomes uint32_t v1 = 0xFFFFFFFF.
  uint32_t v2 = digittoval[src[1]];
  uint32_t v3 = digittoval[src[2]];
  uint32_t v4 = digittoval[src[3]];
  return static_cast<uint32_t>(v1 << 12 | v2 << 8 | v3 << 4 | v4);
}


static inline uint32_t convertone(uint8_t c) {
  uint32_t v = (c & 0xF) + 9 * (c >> 6);
  return v;
}

// no error checking
__attribute__ ((noinline)) // we do not want the compiler to rewrite the problem
uint32_t hex_to_u32_math(const uint8_t *src) {
  uint32_t v1 = convertone(src[0]);
  uint32_t v2 = convertone(src[1]);
  uint32_t v3 = convertone(src[2]);
  uint32_t v4 = convertone(src[3]);
  return static_cast<uint32_t>(v1 << 12 | v2 << 8 | v3 << 4 | v4);
}

// no error checking
// http://0x80.pl/notesen/2014-10-09-pext-convert-ascii-hex-to-num.html
__attribute__ ((noinline)) // we do not want the compiler to rewrite the problem
uint32_t hex_to_u32_mula(const uint8_t *src) {
    uint32_t val;
    memcpy(&val, src, 4);
    const uint32_t input  = __builtin_bswap32(val);
    const uint32_t letter = input & 0x40404040;
    const uint32_t shift  = letter >> 3 | letter >> 6; // 9 if char ~ [a-fA-F], 0 otherwise
    const uint32_t adjusted   = input + shift;
    // gather lower nibbles of each byte
    return _pext_u32(adjusted, 0x0f0f0f0f); // slow on AMD!
}
// no error checking
// https://johnnylee-sde.github.io/Fast-hex-number-string-to-int/
__attribute__ ((noinline)) // we do not want the compiler to rewrite the problem
uint32_t hex_to_u32_lee(const uint8_t *src) {
    uint32_t val;
    memcpy(&val, src, 4);
    val = (val & 0xf0f0f0f) + 9 * (val >> 6 & 0x1010101);
    val = (val | val << 12) & 0xff00ff00;
    return (val>>24 | val) & 0xffff;
}

__attribute__ ((noinline)) // we do not want the compiler to rewrite the problem
uint32_t hex_to_u32_aqrit(const uint8_t *src) {
        uint32_t in;
        uint64_t v, x;
        const int64_t magic = INT64_C(0x1001001000000000);
        memcpy(&in, src, 4);
        v = in;
        x = (((0x00404040 & v) >> 6) * 9) + (v & 0x000F0F0F); // do 3
        x = (((uint64_t)((int64_t)x * magic)) >> 48) & ~15; // bswap and pack
        v = ((v >> 30) * 9) + ((v >> 24) & 0x0F); // do the 4th
        return (x | v);
}

uint64_t hex_to_u64_sse(const uint8_t* string) {
    
    /*
        '0' .. '9' = 0x30 .. 0x39
        'a' .. 'f' = 0x61 .. 0x66
        'A' .. 'F' = 0x41 .. 0x46
    */

    __m128i input = _mm_loadu_si128((const __m128i*)string);
    const __m128i mask  = _mm_set1_epi8(0x0f);
    const __m128i hi_nibbles = _mm_and_si128(_mm_srli_epi32(input, 4), mask);
    const __m128i lo_nibbles = _mm_and_si128(input, mask);

    const int digits  = 0x80;
    const int letters = 0x40;
    const int error1  = 0x10;
    const int error2  = 0x20;

    // pretend that input & 0xf0 == 0x30
    const __m128i lo_ascii_09 = _mm_shuffle_epi8(_mm_setr_epi8(
                                   0 + digits,
                                   1 + digits,
                                   2 + digits,
                                   3 + digits,
                                   4 + digits,
                                   5 + digits,
                                   6 + digits,
                                   7 + digits,
                                   8 + digits,
                                   9 + digits,
                                   error1, error1, error1, error1, error1, error1), lo_nibbles);

    // pretend that input & 0xf0 == 0x40 or 0x60
    const __m128i lo_ascii_af = _mm_shuffle_epi8(_mm_setr_epi8(
                                   error1,
                                   10 + letters,
                                   11 + letters,
                                   12 + letters,
                                   13 + letters,
                                   14 + letters,
                                   15 + letters,
                                   error1,
                                   error1,
                                   error1,
                                   error1, error1, error1, error1, error1, error1), lo_nibbles);
    
    // check value of higher nibble 
    const __m128i hi_class = _mm_shuffle_epi8(_mm_setr_epi8(
                                            error2, error2, error2, digits /* 0x30 */,
                                            letters /* 0x40 */, error2, letters /* 0x60 */, error2,
                                            error2, error2, error2, error2,
                                            error2, error2, error2, error2), hi_nibbles);

    // and find out which preposition was true
    const __m128i ascii_09_mask = _mm_cmpeq_epi8(_mm_and_si128(hi_class, lo_ascii_09), hi_class);
    const __m128i ascii_af_mask = _mm_cmpeq_epi8(_mm_and_si128(hi_class, lo_ascii_af), hi_class);
    const __m128i ascii_09 = _mm_and_si128(ascii_09_mask, lo_ascii_09);
    const __m128i ascii_af = _mm_and_si128(ascii_af_mask, lo_ascii_af);

#if 0
    // MSBs of error vector indicates invalid characters
    const __m128i errorvec = _mm_xor_si128(ascii_09_mask | ascii_af_mask, _mm_set1_epi8(-1));
    if (_mm_movemask_epi8(errorvec))
        abort();
#endif

    const __m128i result = _mm_and_si128(_mm_or_si128(ascii_09, ascii_af), mask);

    // now each byte of result have value 0 .. 15, we're going to merge nibbles:

    const __m128i t0 = result;
    const __m128i t1 = _mm_srli_epi32(result, 4);
    const __m128i t3 = _mm_or_si128(t0, t1);
    const __m128i t4 = _mm_and_si128(t3, _mm_set1_epi16(0x00ff)); // keep just lower bytes in words
    const __m128i t5 = _mm_packus_epi16(t4, t4); // now lower part of the reg has 8-byte value

    return _mm_cvtsi128_si64x(t5);
}

__attribute__ ((noinline)) // we do not want the compiler to rewrite the problem
uint32_t hex_to_u64_sse_wrapper(const uint8_t* string) {
    // wrap to fullfil the template constraint
    return hex_to_u64_sse(string);
}


uint32_t lookup2[65536];

void init_lookup2() {

   for (int i = 0; i < 0x10000; i++) {
     lookup2[i] = (uint32_t) -1;
   }

   for (int i = 0; i < 256; i++) {
     char digits[3];
     sprintf(digits, "%02x", i);

     uint16_t lvalue;
     memcpy(&lvalue, digits, 2);
     lookup2[lvalue] = i;

     char digits_upper[3];
     digits_upper[0] = toupper(digits[0]);
     digits_upper[1] = toupper(digits[1]);
     digits_upper[2] = 0;

     if ((digits_upper[0] != digits[0]) ||
     (digits_upper[1] != digits[1])) {

       memcpy(&lvalue, digits_upper, 2);
       lookup2[lvalue] = i;
     }
   }
 }


__attribute__ ((noinline)) // we do not want the compiler to rewrite the problem
 uint32_t hex_2bytes_lookup(const uint8_t *src) {
   uint32_t v1 = lookup2[((uint16_t*) src)[0]];
   uint32_t v2 = lookup2[((uint16_t*) src)[1]];
   return v1 << 8 | v2;
 }

template <uint32_t (*F)(const uint8_t *src)> void test(size_t N) {
  uint8_t *x = (uint8_t *)malloc(sizeof(uint8_t) * (N + 3));
  srand(1235);
  for (size_t i = 0; i < N + 3; i++) {
    int digit = (rand() % 16);
    if (digit < 10)
      x[i] = 48 + digit;
    else
      x[i] = 65 + digit - 10;
  }
  for (size_t i = 0; i < N; i++) {
    assert(hex_to_u32_lookup(x + i) == hex_to_u32_math(x + i));
  }
  size_t iterations = 50;
  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);
  evts.push_back(PERF_COUNT_HW_BRANCH_MISSES);
  evts.push_back(PERF_COUNT_HW_CACHE_REFERENCES);
  evts.push_back(PERF_COUNT_HW_CACHE_MISSES);
  evts.push_back(PERF_COUNT_HW_REF_CPU_CYCLES);
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);
  std::vector<unsigned long long> results;

  std::vector<std::vector<unsigned long long>> allresults;
  results.resize(evts.size());

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 0xFFFF);

  uint64_t sum = 0;
  for (uint32_t i = 0; i < iterations; i++) {
    unified.start();
    for (size_t i = 0; i < N; i++) {
      sum += F(x + i);
    }
    unified.end(results);
    allresults.push_back(results);
  }
  if (sum == 0)
    printf("bogus\n.");
  std::vector<unsigned long long> mins = compute_mins(allresults);
  std::vector<double> avg = compute_averages(allresults);

  printf("instructions per cycle %4.2f, cycles per 4-character hex string:  %4.3f, "
         "instructions per 4-character hex string: %4.3f \n",
         double(mins[1]) / mins[0], double(mins[0]) / N, double(mins[1]) / N);
  // first we display mins
  printf("min: %8llu cycles, %8llu instructions, \t%8llu branch mis., %8llu "
         "cache ref., %8llu cache mis.\n",
         mins[0], mins[1], mins[2], mins[3], mins[4]);
  printf("avg: %8.1f cycles, %8.1f instructions, \t%8.1f branch mis., %8.1f "
         "cache ref., %8.1f cache mis.\n",
         avg[0], avg[1], avg[2], avg[3], avg[4]);
  free(x);
  printf("sum = %zu \n", (size_t)sum);
}

int main() {
  init_lookup2();
  size_t N = 1000 * 1000;
  printf("N= %zu \n", N);
  printf("lookup:\n");
  test<hex_to_u32_lookup>(N);
  printf("math:\n");
  test<hex_to_u32_math>(N);
  printf("mula:\n");
  test<hex_to_u32_mula>(N);
  printf("lee:\n");
  test<hex_to_u32_lee>(N);
  printf("aqrit:\n");
  test<hex_to_u32_aqrit>(N);
  printf("SSE (uint64_t):\n");
  test<hex_to_u64_sse_wrapper>(N);
  printf("big lookup:\n");
  test<hex_2bytes_lookup>(N);
}
