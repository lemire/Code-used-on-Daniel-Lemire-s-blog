#include <cstdint>
#include <iostream>
#include <x86intrin.h>

uint64_t slow_bit_shuffle(uint64_t w, uint8_t indexes[64]) {
  uint64_t out{};
  for (size_t i = 0; i < 64; i++) {
    bool bit_set = w & (uint64_t(1) << indexes[i]);
    out |= (uint64_t(bit_set) << i);
  }
  return out;
}

// shuffle the bits within w
uint64_t bit_shuffle(uint64_t w, uint8_t indexes[64]) {
  __mmask64 as_mask = _cvtu64_mask64(w);
  __m512i as_vec_register =
      _mm512_maskz_mov_epi8(as_mask, _mm512_set1_epi8(0xFF));
  __m512i as_vec_register_shuf =
      _mm512_permutexvar_epi8(_mm512_loadu_epi8(indexes), as_vec_register);
  return _cvtmask64_u64(_mm512_movepi8_mask(as_vec_register_shuf));
}

// shuffle the bits within w
uint64_t faster_bit_shuffle(uint64_t w, uint8_t indexes[64]) {
  __m512i as_vec_register = _mm512_set1_epi64(w);
  __mmask64 as_mask = _mm512_bitshuffle_epi64_mask(as_vec_register, _mm512_loadu_si512(indexes));
  return _cvtmask64_u64(as_mask);
}

int main() {
  uint64_t w = 0xabcdef0123456789;
  uint8_t indexes[64] = {63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51,
                         50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38,
                         37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25,
                         24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,
                         11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1,  0};
  std::cout << std::hex << w << std::endl;
  std::cout << slow_bit_shuffle(w, indexes) << std::endl;
  std::cout << bit_shuffle(w, indexes) << std::endl;
  std::cout << bit_shuffle(bit_shuffle(w, indexes), indexes) << std::endl;
  std::cout << faster_bit_shuffle(w, indexes) << std::endl;
  std::cout << faster_bit_shuffle(faster_bit_shuffle(w, indexes), indexes) << std::endl;
}