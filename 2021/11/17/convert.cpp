
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

static 
unsigned put_dec_helper4(char *buf, unsigned x)
{
        uint32_t q = (x * (uint64_t)0x346DC5D7) >> 43;

        put_dec_full4(buf, x - q * 10000);
        return q;
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


#ifdef __SSE2__
// mula
#include <x86intrin.h>
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
  for (size_t i = 0; i < 3; i++) {
    std::cout << "khuong     ";
    std::cout << bench(khuong_approach) << std::endl;
    std::cout << "backlinear ";
    std::cout << bench(backlinear_approach) << std::endl;
    std::cout << "linear ";
    std::cout << bench(linear_approach) << std::endl;
    std::cout << "tree   ";
    std::cout << bench(tree_approach) << std::endl;
    std::cout << "treet  ";

    std::cout << bench(tree_table_approach) << std::endl;
    std::cout << "treebt ";

    std::cout << bench(tree_bigtable_approach) << std::endl;
#ifdef __SSE2__
    std::cout << "sse2   ";
    std::cout << bench(sse2_approach) << std::endl;
#endif
    std::cout << std::endl;
  }
  delete[] buf;
}
