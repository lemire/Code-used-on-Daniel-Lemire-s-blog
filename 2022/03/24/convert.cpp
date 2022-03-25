//original:
//https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/blob/master/2021/11/17/convert.cpp
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <intrin.h>

#define __SSE2__
#define __SSE4_1__

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}
template <typename PROCEDURE>
double bench(PROCEDURE f, uint64_t threshold = 200'000'000) {
  uint64_t start = nano();
  uint64_t finish = start;
  size_t count{0};
  for (; finish - start < threshold;) {
    count += f();
    finish = nano();
  }
  return double(finish - start) / count;
}
void to_string_backlinear(uint64_t x, char *out) {
  for (int z = 0; z < 16; z++) {
    out[15 - z] = (x % 10) + 0x30;
    x /= 10;
  }
}

void to_string_linear(uint64_t x, char *out) {
  out[0] = x / 1000000000000000 + 0x30;
  x %= 1000000000000000;
  out[1] = x / 100000000000000 + 0x30;
  x %= 100000000000000;
  out[2] = x / 10000000000000 + 0x30;
  x %= 10000000000000;
  out[3] = x / 1000000000000 + 0x30;
  x %= 1000000000000;
  out[4] = x / 100000000000 + 0x30;
  x %= 100000000000;
  out[5] = x / 10000000000 + 0x30;
  x %= 10000000000;
  out[6] = x / 1000000000 + 0x30;
  x %= 1000000000;
  out[7] = x / 100000000 + 0x30;
  x %= 100000000;
  out[8] = x / 10000000 + 0x30;
  x %= 10000000;
  out[9] = x / 1000000 + 0x30;
  x %= 1000000;
  out[10] = x / 100000 + 0x30;
  x %= 100000;
  out[11] = x / 10000 + 0x30;
  x %= 10000;
  out[12] = x / 1000 + 0x30;
  x %= 1000;
  out[13] = x / 100 + 0x30;
  x %= 100;
  out[14] = x / 10 + 0x30;
  x %= 10;
  out[15] = x + 0x30;
}

// credit: Paul Khuong
uint64_t encode_ten_thousands(uint64_t hi, uint64_t lo) {
  uint64_t merged = hi | (lo << 32);
  /* Truncate division by 100: 10486 / 2**20 ~= 1/100. */
  uint64_t top = ((merged * 10486ULL) >> 20) & ((0x7FULL << 32) | 0x7FULL);
  /* Trailing 2 digits in the 1e4 chunks. */
  uint64_t bot = merged - 100ULL * top;
  uint64_t hundreds;
  uint64_t tens;

  /*
   * We now have 4 radix-100 digits in little-endian order, each
   * in its own 16 bit area.
   */
  hundreds = (bot << 16) + top;

  /* Divide and mod by 10 all 4 radix-100 digits in parallel. */
  tens = (hundreds * 103ULL) >> 10;
  tens &= (0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL;
  tens += (hundreds - 10ULL * tens) << 8;

  return tens;
}

uint64_t encode_ten_thousands2(uint64_t hi, uint64_t lo) {
  uint64_t merged = hi | (lo << 32);
  /* Truncate division by 100: 10486 / 2**20 ~= 1/100. */
  uint64_t top = ((merged * 0x147b) >> 19) & ((0x7FULL << 32) | 0x7FULL);
  /* Trailing 2 digits in the 1e4 chunks. */
  uint64_t bot = merged - 100ULL * top;
  uint64_t hundreds;
  uint64_t tens;

  hundreds = (bot << 16) + top;

  /* Divide and mod by 10 all 4 radix-100 digits in parallel. */
  tens = (hundreds * 103ULL) >> 10;
  tens &= (0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL;
  tens += (hundreds - 10ULL * tens) << 8;
  return tens;
}

void to_string_khuong(uint64_t x, char *out) {
  uint64_t top = x / 100000000;
  uint64_t bottom = x % 100000000;
  uint64_t first =
      0x3030303030303030 + encode_ten_thousands(top / 10000, top % 10000);
  memcpy(out, &first, sizeof(first));
  uint64_t second =
      0x3030303030303030 + encode_ten_thousands(bottom / 10000, bottom % 10000);
  memcpy(out + 8, &second, sizeof(second));
}

// take a 16-digit integer, < 10000000000000000,
// and write it out.
void to_string_tree(uint64_t x, char *out) {
  uint64_t top = x / 100000000;
  uint64_t bottom = x % 100000000;
  //
  uint64_t toptop = top / 10000;
  uint64_t topbottom = top % 10000;
  uint64_t bottomtop = bottom / 10000;
  uint64_t bottombottom = bottom % 10000;
  //
  uint64_t toptoptop = toptop / 100;
  uint64_t toptopbottom = toptop % 100;

  uint64_t topbottomtop = topbottom / 100;
  uint64_t topbottombottom = topbottom % 100;

  uint64_t bottomtoptop = bottomtop / 100;
  uint64_t bottomtopbottom = bottomtop % 100;

  uint64_t bottombottomtop = bottombottom / 100;
  uint64_t bottombottombottom = bottombottom % 100;
  //
  out[0] = toptoptop / 10 + 0x30;
  out[1] = toptoptop % 10 + 0x30;
  out[2] = toptopbottom / 10 + 0x30;
  out[3] = toptopbottom % 10 + 0x30;
  out[4] = topbottomtop / 10 + 0x30;
  out[5] = topbottomtop % 10 + 0x30;
  out[6] = topbottombottom / 10 + 0x30;
  out[7] = topbottombottom % 10 + 0x30;
  out[8] = bottomtoptop / 10 + 0x30;
  out[9] = bottomtoptop % 10 + 0x30;
  out[10] = bottomtopbottom / 10 + 0x30;
  out[11] = bottomtopbottom % 10 + 0x30;
  out[12] = bottombottomtop / 10 + 0x30;
  out[13] = bottombottomtop % 10 + 0x30;
  out[14] = bottombottombottom / 10 + 0x30;
  out[15] = bottombottombottom % 10 + 0x30;
}

void write_tenthousand(uint64_t z, char *out) {
  z = 429538 * z;
  out[0] = 0x30 + ((z * 10) >> 32);
  z = (z * 10) & 0xffffffff;
  out[1] = 0x30 + ((z * 10) >> 32);
  z = (z * 10) & 0xffffffff;
  out[2] = 0x30 + ((z * 10) >> 32);
  z = (z * 10) & 0xffffffff;
  out[3] = 0x30 + ((z * 10) >> 32);
}

void to_string_tree_str(uint64_t x, char *out) {
  uint64_t top = x / 100000000;
  uint64_t bottom = x % 100000000;
  //
  uint64_t toptop = top / 10000;
  uint64_t topbottom = top % 10000;
  uint64_t bottomtop = bottom / 10000;
  uint64_t bottombottom = bottom % 10000;
  //
  write_tenthousand(toptop, out);
  write_tenthousand(topbottom, out + 4);
  write_tenthousand(bottomtop, out + 8);
  write_tenthousand(bottombottom, out + 12);
}

void to_string_tree_table(uint64_t x, char *out) {
  static const char table[200] = {
      0x30, 0x30, 0x30, 0x31, 0x30, 0x32, 0x30, 0x33, 0x30, 0x34, 0x30, 0x35,
      0x30, 0x36, 0x30, 0x37, 0x30, 0x38, 0x30, 0x39, 0x31, 0x30, 0x31, 0x31,
      0x31, 0x32, 0x31, 0x33, 0x31, 0x34, 0x31, 0x35, 0x31, 0x36, 0x31, 0x37,
      0x31, 0x38, 0x31, 0x39, 0x32, 0x30, 0x32, 0x31, 0x32, 0x32, 0x32, 0x33,
      0x32, 0x34, 0x32, 0x35, 0x32, 0x36, 0x32, 0x37, 0x32, 0x38, 0x32, 0x39,
      0x33, 0x30, 0x33, 0x31, 0x33, 0x32, 0x33, 0x33, 0x33, 0x34, 0x33, 0x35,
      0x33, 0x36, 0x33, 0x37, 0x33, 0x38, 0x33, 0x39, 0x34, 0x30, 0x34, 0x31,
      0x34, 0x32, 0x34, 0x33, 0x34, 0x34, 0x34, 0x35, 0x34, 0x36, 0x34, 0x37,
      0x34, 0x38, 0x34, 0x39, 0x35, 0x30, 0x35, 0x31, 0x35, 0x32, 0x35, 0x33,
      0x35, 0x34, 0x35, 0x35, 0x35, 0x36, 0x35, 0x37, 0x35, 0x38, 0x35, 0x39,
      0x36, 0x30, 0x36, 0x31, 0x36, 0x32, 0x36, 0x33, 0x36, 0x34, 0x36, 0x35,
      0x36, 0x36, 0x36, 0x37, 0x36, 0x38, 0x36, 0x39, 0x37, 0x30, 0x37, 0x31,
      0x37, 0x32, 0x37, 0x33, 0x37, 0x34, 0x37, 0x35, 0x37, 0x36, 0x37, 0x37,
      0x37, 0x38, 0x37, 0x39, 0x38, 0x30, 0x38, 0x31, 0x38, 0x32, 0x38, 0x33,
      0x38, 0x34, 0x38, 0x35, 0x38, 0x36, 0x38, 0x37, 0x38, 0x38, 0x38, 0x39,
      0x39, 0x30, 0x39, 0x31, 0x39, 0x32, 0x39, 0x33, 0x39, 0x34, 0x39, 0x35,
      0x39, 0x36, 0x39, 0x37, 0x39, 0x38, 0x39, 0x39,
  };
  uint64_t top = x / 100000000;
  uint64_t bottom = x % 100000000;
  //
  uint64_t toptop = top / 10000;
  uint64_t topbottom = top % 10000;
  uint64_t bottomtop = bottom / 10000;
  uint64_t bottombottom = bottom % 10000;
  //
  uint64_t toptoptop = toptop / 100;
  uint64_t toptopbottom = toptop % 100;

  uint64_t topbottomtop = topbottom / 100;
  uint64_t topbottombottom = topbottom % 100;

  uint64_t bottomtoptop = bottomtop / 100;
  uint64_t bottomtopbottom = bottomtop % 100;

  uint64_t bottombottomtop = bottombottom / 100;
  uint64_t bottombottombottom = bottombottom % 100;
  //
  memcpy(out, &table[2 * toptoptop], 2);
  memcpy(out + 2, &table[2 * toptopbottom], 2);
  memcpy(out + 4, &table[2 * topbottomtop], 2);
  memcpy(out + 6, &table[2 * topbottombottom], 2);
  memcpy(out + 8, &table[2 * bottomtoptop], 2);
  memcpy(out + 10, &table[2 * bottomtopbottom], 2);
  memcpy(out + 12, &table[2 * bottombottomtop], 2);
  memcpy(out + 14, &table[2 * bottombottombottom], 2);
}
// http://www.cs.uiowa.edu/~jones/bcd/decimal.html
/*
void putdec( uint64_t n, char *out ) {
        uint32_t d4, d3, d2, d1, d0, q;

        d0 = n       & 0xFFFF;
        d1 = (n>>16) & 0xFFFF;
        d2 = (n>>32) & 0xFFFF;
        d3 = (n>>48) & 0xFFFF;

        d0 = 656 * d3 + 7296 * d2 + 5536 * d1 + d0;
        q = d0 / 10000;
        d0 = d0 % 10000;

        d1 = q + 7671 * d3 + 9496 * d2 + 6 * d1;
        q = d1 / 10000;
        d1 = d1 % 10000;

        d2 = q + 4749 * d3 + 42 * d2;
        q = d2 / 10000;
        d2 = d2 % 10000;

        d3 = q + 281 * d3;
        q = d3 / 10000;
        d3 = d3 % 10000;

        d4 = q;

        printf( "%4.4u", d4 );
        printf( "%4.4u", d3 );
        printf( "%4.4u", d2 );
        printf( "%4.4u", d1 );
        printf( "%4.4u", d0 );
 }
 */


#ifdef __SSE2__
// mula
//#include <x86intrin.h>
void to_string_sse2(uint64_t v, char *out) {

  // v is 16-digit number = abcdefghijklmnop
  const __m128i div_10000 = _mm_set1_epi32(0xd1b71759);
  const __m128i mul_10000 = _mm_set1_epi32(10000);
  const int div_10000_shift = 45;

  const __m128i div_100 = _mm_set1_epi16(0x147b);
  const __m128i mul_100 = _mm_set1_epi16(100);
  const int div_100_shift = 3;

  const __m128i div_10 = _mm_set1_epi16(0x199a);
  const __m128i mul_10 = _mm_set1_epi16(10);

  const __m128i ascii0 = _mm_set1_epi8('0');

  // can't be easliy done in SSE
  const uint32_t a = v / 100000000; // 8-digit number: abcdefgh
  const uint32_t b = v % 100000000; // 8-digit number: ijklmnop

  //                [ 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1
  //                | 0 ]
  // x            = [       0       |      ijklmnop |       0       | abcdefgh ]
  __m128i x = _mm_set_epi64x(b, a);

  // x div 10^4   = [       0       |          ijkl |       0       | abcd ]
  __m128i x_div_10000;
  x_div_10000 = _mm_mul_epu32(x, div_10000);
  x_div_10000 = _mm_srli_epi64(x_div_10000, div_10000_shift);

  // x mod 10^4   = [       0       |          mnop |       0       | efgh ]
  __m128i x_mod_10000;
  x_mod_10000 = _mm_mul_epu32(x_div_10000, mul_10000);
  x_mod_10000 = _mm_sub_epi32(x, x_mod_10000);

  // y            = [          mnop |          ijkl |          efgh | abcd ]
  __m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));

  // y_div_100    = [   0   |    mn |   0   |    ij |   0   |    ef |   0   | ab
  // ]
  __m128i y_div_100;
  y_div_100 = _mm_mulhi_epu16(y, div_100);
  y_div_100 = _mm_srli_epi16(y_div_100, div_100_shift);

  // y_mod_100    = [   0   |    op |   0   |    kl |   0   |    gh |   0   | cd
  // ]
  __m128i y_mod_100;
  y_mod_100 = _mm_mullo_epi16(y_div_100, mul_100);
  y_mod_100 = _mm_sub_epi16(y, y_mod_100);

  // z            = [    mn |    op |    ij |    kl |    ef |    gh |    ab | cd
  // ]
  __m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));

  // z_div_10     = [ 0 | m | 0 | o | 0 | i | 0 | k | 0 | e | 0 | g | 0 | a | 0
  // | c ]
  __m128i z_div_10 = _mm_mulhi_epu16(z, div_10);

  // z_mod_10     = [ 0 | n | 0 | p | 0 | j | 0 | l | 0 | f | 0 | h | 0 | b | 0
  // | d ]
  __m128i z_mod_10;
  z_mod_10 = _mm_mullo_epi16(z_div_10, mul_10);
  z_mod_10 = _mm_sub_epi16(z, z_mod_10);

  // interleave z_mod_10 and z_div_10 -
  // tmp          = [ m | n | o | p | i | j | k | l | e | f | g | h | a | b | c
  // | d ]
  __m128i tmp = _mm_or_si128(z_div_10, _mm_slli_epi16(z_mod_10, 8));

  // convert to ascii
  tmp = _mm_add_epi8(tmp, ascii0);

  // and save result
  _mm_storeu_si128((__m128i *)out, tmp);
}
#endif

#ifdef __SSE4_1__
void to_string_sse2__pshufb(uint64_t v, char *out) {

  // v is 16-digit number = abcdefghijklmnop
  const __m128i div_10000 = _mm_set1_epi32(0xd1b71759);
  const __m128i mul_10000 = _mm_set1_epi32(10000);
  const int div_10000_shift = 45;

  const __m128i div_100 = _mm_set1_epi16(0x147b);
  const __m128i mul_100 = _mm_set1_epi16(100);
  const int div_100_shift = 3;

  const __m128i div_10 = _mm_set1_epi16(0x199a);
  const __m128i mul_10 = _mm_set1_epi16(10);

  const __m128i ascii0 = _mm_set1_epi8('0');

  // can't be easliy done in SSE
  const uint32_t a = v / 100000000; // 8-digit number: abcdefgh
  const uint32_t b = v % 100000000; // 8-digit number: ijklmnop

  //                [ 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1
  //                | 0 ]
  // x            = [       0       |      ijklmnop |       0       | abcdefgh ]
  __m128i x = _mm_set_epi64x(b, a);

  // x div 10^4   = [       0       |          ijkl |       0       | abcd ]
  __m128i x_div_10000;
  x_div_10000 = _mm_mul_epu32(x, div_10000);
  x_div_10000 = _mm_srli_epi64(x_div_10000, div_10000_shift);

  // x mod 10^4   = [       0       |          mnop |       0       | efgh ]
  __m128i x_mod_10000;
  x_mod_10000 = _mm_mul_epu32(x_div_10000, mul_10000);
  x_mod_10000 = _mm_sub_epi32(x, x_mod_10000);

  // y            = [          mnop |          ijkl |          efgh | abcd ]
  __m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));

  // y_div_100    = [   0   |    mn |   0   |    ij |   0   |    ef |   0   | ab
  // ]
  __m128i y_div_100;
  y_div_100 = _mm_mulhi_epu16(y, div_100);
  y_div_100 = _mm_srli_epi16(y_div_100, div_100_shift);

  // y_mod_100    = [   0   |    op |   0   |    kl |   0   |    gh |   0   | cd
  // ]
  __m128i y_mod_100;
  y_mod_100 = _mm_mullo_epi16(y_div_100, mul_100);
  y_mod_100 = _mm_sub_epi16(y, y_mod_100);

  // z            = [    mn |    ij |    ef |    ab |    op |    kl |    gh | cd
  // ]
  __m128i z = _mm_packus_epi32(y_div_100, y_mod_100);

  // z_div_10     = [ 0 | m | 0 | i | 0 | e | 0 | a | 0 | o | 0 | k | 0 | g | 0
  // | c ]
  __m128i z_div_10 = _mm_mulhi_epu16(z, div_10);

  // z_mod_10     = [ 0 | n | 0 | j | 0 | f | 0 | b | 0 | p | 0 | l | 0 | h | 0
  // | d ]
  __m128i z_mod_10;
  z_mod_10 = _mm_mullo_epi16(z_div_10, mul_10);
  z_mod_10 = _mm_sub_epi16(z, z_mod_10);

  // interleave z_mod_10 and z_div_10 -
  // tmp          = [ m | i | e | a | o | k | g | c | n | j | f | b | p | l | h
  // | d ]
  __m128i tmp = _mm_packus_epi16(z_div_10, z_mod_10);

  const __m128i reorder =
      _mm_set_epi8(15, 7, 11, 3, 14, 6, 10, 2, 13, 5, 9, 1, 12, 4, 8, 0);
  tmp = _mm_shuffle_epi8(tmp, reorder);

  // convert to ascii
  tmp = _mm_add_epi8(tmp, ascii0);

  // and save result
  _mm_storeu_si128((__m128i *)out, tmp);
}
#endif

#ifdef __AVX2__
void to_string_avx2(uint64_t v, char *out) {

  // begin: copy of to_string_sse2

  // v is 16-digit number = abcdefghijklmnop
  const __m128i div_10000 = _mm_set1_epi32(0xd1b71759);
  const __m128i mul_10000 = _mm_set1_epi32(10000);
  const int div_10000_shift = 45;

  // can't be easliy done in SSE
  const uint32_t a = v / 100000000; // 8-digit number: abcdefgh
  const uint32_t b = v % 100000000; // 8-digit number: ijklmnop

  //                [ 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1 | 0 | 3 | 2 | 1
  //                | 0 ]
  // x            = [       0       |      ijklmnop |       0       | abcdefgh ]
  __m128i x = _mm_set_epi64x(b, a);

  // x div 10^4   = [       0       |          ijkl |       0       | abcd ]
  __m128i x_div_10000;
  x_div_10000 = _mm_mul_epu32(x, div_10000);
  x_div_10000 = _mm_srli_epi64(x_div_10000, div_10000_shift);

  // x mod 10^4   = [       0       |          mnop |       0       | efgh ]
  __m128i x_mod_10000;
  x_mod_10000 = _mm_mul_epu32(x_div_10000, mul_10000);
  x_mod_10000 = _mm_sub_epi32(x, x_mod_10000);

  // y            = [          mnop |          ijkl |          efgh | abcd ]
  __m128i y = _mm_or_si128(x_div_10000, _mm_slli_epi64(x_mod_10000, 32));

  // end of copy, AVX2 code now
#include "bigtable.h"

  const __m128i ascii =
      _mm_i32gather_epi32(reinterpret_cast<int const *>(&bigtable), y, 4);

  _mm_storeu_si128((__m128i *)out, ascii);
}
#endif

#ifdef __AVX512F__
/*
The AVX512F method:
It is based on division by powers of ten:

n / 10^8
n / 10^7
..
n / 10^1

e.g.:
n = 84736251

div10pow ways:

0
8
84
847
8473
84736
847362
8473625

rotated div10pow ways:      mul10 of div10pow:      difference:
(n / 10^0 = n in the 
least significant way)
8                           0                       8
84                          80                      4
847                         840                     7
8473                        8470                    3
84736                       84730                   6
847362                      847360                  2
8473625                     8473620                 5
84736251                    84736250                1

division algorithm is the branchfree version of libdivide.h 
https://libdivide.com/

uint32_t libdivide_u32_branchfree_do(
    uint32_t numer, const struct libdivide_u32_branchfree_t *denom) {
    uint32_t q = libdivide_mullhi_u32(denom->magic, numer);
    uint32_t t = ((numer - q) >> 1) + q;
    return t >> denom->more;
}
*/

void to_string_avx512f(uint64_t n, void *out) {          
	uint64_t n_15_08			= 0;
								  _mulx_u64(n, 0xabcc77118461cefd, &n_15_08);
						n_15_08	>>= 0x1a;									//n / 10^8
	const uint64_t n_07_00		= n - (n_15_08 * 100000000);				//n % 10^8
	const __m512i bcst_h		= _mm512_set1_epi32(n_15_08);
	const __m512i bcst_l		= _mm512_set1_epi32(n_07_00);
 
    //                                                  10^(-1)       10^(-2)         10^(-3)       10^(-4)         10^(-5)       10^(-6)       10^(-7)         10^(-8)
	const __m512i mul_const		= _mm512_set_epi32(0, 0x9999999a, 0, 0x47ae147b, 0, 0x0624dd30, 0, 0xa36e2eb2, 0, 0x4f8b588f, 0, 0x0c6f7a0c, 0, 0xad7f29ac, 0, 0x5798ee24);
	const __m512i perm_const	= _mm512_set_epi32(15, 13, 11, 9, 7, 5, 3, 1, 31, 29, 27, 25, 23, 21, 19, 17);
	const __m512i srlv_const	= _mm512_broadcast_i64x4(_mm256_set_epi32(3,  6,  9, 13, 16, 19, 23, 26));

	const __m512i mul_h			= _mm512_mul_epu32(bcst_h, mul_const);
	const __m512i mul_l			= _mm512_mul_epu32(bcst_l, mul_const);

	const __m512i dwords		= _mm512_permutex2var_epi32(mul_l, perm_const, mul_h);
	const __m512i bcst_mix		= _mm512_mask_blend_epi32(0xff00, bcst_h, bcst_l);
	const __m512i temp2			= _mm512_srli_epi32(_mm512_sub_epi32(bcst_mix, dwords), 1);
	const __m512i div10pow		= _mm512_srlv_epi32(_mm512_add_epi32(temp2, dwords), srlv_const);

	const __m512i rotated		= _mm512_mask_alignr_epi32(bcst_h, 0xff7f, bcst_l, div10pow, 1);
    //mullo = better throughput
	const __m512i mul10			= _mm512_mullo_epi32(div10pow, _mm512_set1_epi32(10));
    //add = better latency
	//const __m512i mul10			= _mm512_add_epi32(_mm512_slli_epi32(div10pow, 3), _mm512_add_epi32(div10pow, div10pow));
	const __m512i diff			= _mm512_sub_epi32(_mm512_add_epi32(rotated, _mm512_set1_epi32('0')), mul10);
	_mm512_mask_cvtepi32_storeu_epi8(out, 0xffff, diff);
}

/*
The IFMA decimal print method: 

Computing remainder of n by 10, 100, ..., 10^8 and then multiplication by 10.
 
n = 84736251
1              = n mod 10
51             = n mod 100
251            = n mod 1000
6251
36251
736251
4736251
84736251       = n mod 10^8

From this paper 
https://arxiv.org/abs/1902.01961
 page 8:

      uint32_t d = ...; // your divisor > 0
      // c = ceil ( (1 < <64) / d ) ; we take L = N
      uint64_t c = UINT64_C (0 xFFFFFFFFFFFFFFFF ) / d + 1;
      // fastmod computes (n mod d) given precomputed c
      uint32_t fastmod ( uint32_t n, uint64_t c, uint32_t d) {
          uint64_t lowbits = c * n;
          return (( __uint128_t ) lowbits * d) >> 64;
      }

Fastmod fits well for this AVX512FMA instruction pair: 
VPMADD52LUQ => lowbits = c * n + 0
VPMADD52HUQ => highbits = lowbits * 10 + asciiZero
just uses 52b and 104b numbers instead of 64 and 128, and highbits use 10 instead of d, and produces 8 decimal digits for 0 <= n <= 99999999.

The only problem is that in the 8th digit case the VPMADD52HUQ overflows, if we use the orginial 0x2af31dd ( = (2^53 - 1)/(10^8) + 1) constant as c in VPMADD52LUQ:

0x2af31dd * 99999999 = 0x10000001a50b23 

Solution: we use 0x2af31dc = 0x2af31dd - 1 as c, and use 0x1A1A400 bias instead of 0. 0x1A1A400 is the smallest bias, which does not underflows in case of the smallest 8-digit number:

0x2af31dc * 10000000 = 0x19999996FD600 = 450359960000000
(0x19999996FD600 + 0x1A1A400) * 10 = 0x1000000EAEC400
*/ 

//constexpr uint64_t u64_10pow(uint64_t exp) {
//	uint64_t _10pow = 1;
//	for (uint64_t p = 0; p < exp; p++)
//		_10pow *= 10;
//	return _10pow;
//}
//
//constexpr uint64_t divconst52(uint64_t divisor) {
//	return  UINT64_C(0xFFFFFFFFFFFFF) / divisor + 1;
//}

void to_string_avx512ifma(uint64_t n, void *out) {
	uint64_t n_15_08			= 0;
								  _mulx_u64(n, 0xabcc77118461cefd, &n_15_08);
						n_15_08	>>= 0x1a;									//n / 10^8
	const uint64_t n_07_00		= n - (n_15_08 * 100000000);				//n % 10^8
	const __m512i bcstq_h		= _mm512_set1_epi64(n_15_08);
	const __m512i bcstq_l		= _mm512_set1_epi64(n_07_00);
 	const __m512i zmmzero		= _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400)); //8th digit bias
 	const __m512i zmmTen		= _mm512_set1_epi64(10);
 	const __m512i asciiZero		= _mm512_set1_epi64('0');

	//const __m512i ifma_const	= _mm512_setr_epi64(divconst52(u64_10pow(8)) - 1, divconst52(u64_10pow(7)), divconst52(u64_10pow(6)), divconst52(u64_10pow(5)), divconst52(u64_10pow(4)), divconst52(u64_10pow(3)), divconst52(u64_10pow(2)), divconst52(u64_10pow(1)));
	const __m512i ifma_const	= _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b, 0x0000000010c6f7a0c, 0x00000000a7c5ac472, 0x000000068db8bac72, 0x0000004189374bc6b, 0x0000028f5c28f5c29, 0x0000199999999999a);
	const __m512i permb_const	= _mm512_castsi128_si512(_mm_set_epi8(0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00));

	const __m512i lowbits_h		= _mm512_madd52lo_epu64(zmmzero, bcstq_h, ifma_const);
	const __m512i lowbits_l		= _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
	const __m512i highbits_h	= _mm512_madd52hi_epu64(asciiZero, zmmTen, lowbits_h);
	const __m512i highbits_l	= _mm512_madd52hi_epu64(asciiZero, zmmTen, lowbits_l);
	const __m128i digits_15_0	= _mm512_castsi512_si128(_mm512_permutex2var_epi8(highbits_h, permb_const, highbits_l));
	_mm_storeu_si128((__m128i *)out, digits_15_0);
}
#endif

void to_string_tree_bigtable(uint64_t x, char *out) {
#include "bigtable.h"

  uint64_t top = x / 100000000;
  uint64_t bottom = x % 100000000;
  //
  uint64_t toptop = top / 10000;
  uint64_t topbottom = top % 10000;
  uint64_t bottomtop = bottom / 10000;
  uint64_t bottombottom = bottom % 10000;

  memcpy(out, &bigtable[4 * toptop], 4);
  memcpy(out + 4, &bigtable[4 * topbottom], 4);
  memcpy(out + 8, &bigtable[4 * bottomtop], 4);
  memcpy(out + 12, &bigtable[4 * bottombottom], 4);
}
int main() {

  std::vector<uint64_t> data;
  for (size_t i = 0; i < 50000; i++) {
    data.push_back(rand() % 10000000000000000);
  }
  char *buf = new char[data.size() * 16];
  auto backlinear_approach = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_backlinear(val, b);
      b += 16;
    }
    return data.size();
  };
  auto linear_approach = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_linear(val, b);
      b += 16;
    }
    return data.size();
  };
  auto tree_approach = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_tree(val, b);
      b += 16;
    }
    return data.size();
  };
  auto tree_table_approach = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_tree_table(val, b);
      b += 16;
    }
    return data.size();
  };
  auto tree_str_approach = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_tree_str(val, b);
      b += 16;
    }
    return data.size();
  };

  auto tree_bigtable_approach = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_tree_bigtable(val, b);
      b += 16;
    }
    return data.size();
  };
  auto khuong_approach = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_khuong(val, b);
      b += 16;
    }
    return data.size();
  };

#ifdef __SSE2__
  auto sse2_approach = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_sse2(val, b);
      b += 16;
    }
    return data.size();
  };
#endif

#ifdef __SSE4_1__
  auto sse2_approach_v2 = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_sse2__pshufb(val, b);
      b += 16;
    }
    return data.size();
  };
#endif

#ifdef __AVX2__
  auto avx2_approach = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_avx2(val, b);
      b += 16;
    }
    return data.size();
  };
#endif

#ifdef __AVX512F__
  auto avx512f_approach = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_avx512f(val, b);
      b += 16;
    }
    return data.size();
  };

  auto avx512ifma_approach = [&data, buf]() -> size_t {
    char *b = buf;
    for (auto val : data) {
      to_string_avx512ifma(val, b);
      b += 16;
    }
    return data.size();
  };
#endif
  for (size_t i = 0; i < 3; i++) {
    std::cout << "khuong     ";
    std::cout << bench(khuong_approach) << std::endl;
    std::cout << "backlinear ";
    std::cout << bench(backlinear_approach) << std::endl;
    std::cout << "linear ";
    std::cout << bench(linear_approach) << std::endl;
    std::cout << "tree   ";
    std::cout << bench(tree_approach) << std::endl;
    std::cout << "treetst ";
    std::cout << bench(tree_str_approach) << std::endl;
    std::cout << "treest ";
    std::cout << bench(tree_table_approach) << std::endl;
    std::cout << "treebt ";

    std::cout << bench(tree_bigtable_approach) << std::endl;
#ifdef __SSE2__
    std::cout << "sse2   ";
    std::cout << bench(sse2_approach) << std::endl;
#endif

#ifdef __SSE4_1__
    std::cout << "sse2(2) ";
    std::cout << bench(sse2_approach_v2) << std::endl;
#endif

#ifdef __AVX2__
    std::cout << "avx2    ";
    std::cout << bench(avx2_approach) << std::endl;
#endif

#ifdef __AVX512F__
    std::cout << "avx512f  ";
    std::cout << bench(avx512f_approach) << std::endl;

    std::cout << "avx512ifma ";
    std::cout << bench(avx512ifma_approach) << std::endl;
#endif
    std::cout << std::endl;
  }
  delete[] buf;
}