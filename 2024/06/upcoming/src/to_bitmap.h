#pragma once

#include <cstdint>
#include <span>
#include <x86intrin.h>

// Convert 64 indices to a 64-bit mask.
__attribute__ ((noinline))
uint64_t to_bitmap(std::span<uint8_t> &indices) {
  uint64_t result = 0;
  for (uint8_t i : indices) {
    result |= uint64_t(1) << i;
  }
  return result;
}

// see https://www.corsix.org/content/abusing-gf2p8affineqb-indices-into-bits
static inline __m512i _mm512_setr_epi8(
    char s0, char s1, char s2, char s3, char s4, char s5, char s6, char s7,
    char s8, char s9, char s10, char s11, char s12, char s13, char s14,
    char s15, char s16, char s17, char s18, char s19, char s20, char s21,
    char s22, char s23, char s24, char s25, char s26, char s27, char s28,
    char s29, char s30, char s31, char s32, char s33, char s34, char s35,
    char s36, char s37, char s38, char s39, char s40, char s41, char s42,
    char s43, char s44, char s45, char s46, char s47, char s48, char s49,
    char s50, char s51, char s52, char s53, char s54, char s55, char s56,
    char s57, char s58, char s59, char s60, char s61, char s62, char s63) {
  return _mm512_set_epi8(s63, s62, s61, s60, s59, s58, s57, s56, s55, s54, s53,
                         s52, s51, s50, s49, s48, s47, s46, s45, s44, s43, s42,
                         s41, s40, s39, s38, s37, s36, s35, s34, s33, s32, s31,
                         s30, s29, s28, s27, s26, s25, s24, s23, s22, s21, s20,
                         s19, s18, s17, s16, s15, s14, s13, s12, s11, s10, s9,
                         s8, s7, s6, s5, s4, s3, s2, s1, s0);
}

__mmask64 simd_impl(__m512i indices, __mmask64 valids) {
  // Convert indices to bits within each qword lane.
  __m512i khi = _mm512_setr_epi8(
      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
      0x02, 0x02, 0x02, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
      0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10,
      0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
      0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80,
      0x80, 0x80, 0x80, 0x80);
  __m512i hi0 = _mm512_permutexvar_epi8(indices, khi);
  __m512i klo = _mm512_set1_epi64(0x0102040810204080);
  __m512i lo0 = _mm512_maskz_shuffle_epi8(valids, klo, indices);
  __m512i kid = _mm512_set1_epi64(0x8040201008040201);
  __m512i hi1 = _mm512_gf2p8affine_epi64_epi8(kid, hi0, 0);
  __m512i lo1 = _mm512_gf2p8affine_epi64_epi8(kid, lo0, 0);
  __m512i x0 = _mm512_gf2p8affine_epi64_epi8(hi1, lo1, 0);
  // Combine results from various qword lanes.
  __m512i ktr = _mm512_setr_epi8(
      0, 8, 16, 24, 32, 40, 48, 56, 1, 9, 17, 25, 33, 41, 49, 57, 2, 10, 18, 26,
      34, 42, 50, 58, 3, 11, 19, 27, 35, 43, 51, 59, 4, 12, 20, 28, 36, 44, 52,
      60, 5, 13, 21, 29, 37, 45, 53, 61, 6, 14, 22, 30, 38, 46, 54, 62, 7, 15,
      23, 31, 39, 47, 55, 63);
  __m512i x1 = _mm512_permutexvar_epi8(ktr, x0);
  __m512i x2 = _mm512_gf2p8affine_epi64_epi8(kid, x1, 0);
  // Reduce 64 bytes down to 64 bits.
  __m512i kff = _mm512_set1_epi8(0xff);
  __m512i x3 = _mm512_gf2p8affine_epi64_epi8(x2, kff, 0);
  return _mm512_movepi8_mask(x3);
}

// assuming that indices are distinct
__mmask64 simd_impl_distinct(__m512i indices, __mmask64 valids) {
  // Convert indices to bits within each qword lane.
  __m512i khi = _mm512_setr_epi8(
      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
      0x02, 0x02, 0x02, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
      0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10,
      0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
      0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80,
      0x80, 0x80, 0x80, 0x80);
  __m512i hi0 = _mm512_permutexvar_epi8(indices, khi);
  __m512i klo = _mm512_set1_epi64(0x0102040810204080);
  __m512i lo0 = _mm512_maskz_shuffle_epi8(valids, klo, indices);
  __m512i kid = _mm512_set1_epi64(0x8040201008040201);
  __m512i hi1 = _mm512_gf2p8affine_epi64_epi8(kid, hi0, 0);
  __m512i lo1 = _mm512_gf2p8affine_epi64_epi8(kid, lo0, 0);
  __m512i x0 = _mm512_gf2p8affine_epi64_epi8(hi1, lo1, 0);
  // Combine results from various qword lanes.
  __m512i ktr = _mm512_setr_epi8(
      0, 8, 16, 24, 32, 40, 48, 56, 1, 9, 17, 25, 33, 41, 49, 57, 2, 10, 18, 26,
      34, 42, 50, 58, 3, 11, 19, 27, 35, 43, 51, 59, 4, 12, 20, 28, 36, 44, 52,
      60, 5, 13, 21, 29, 37, 45, 53, 61, 6, 14, 22, 30, 38, 46, 54, 62, 7, 15,
      23, 31, 39, 47, 55, 63);
  __m512i x1 = _mm512_permutexvar_epi8(ktr, x0);
  __m512i x2 = _mm512_gf2p8affine_epi64_epi8(kid, x1, 0);
  // Reduce 64 bytes down to 64 bits.
  return _mm512_cmpneq_epi8_mask(x2, _mm512_setzero_si512());
}

__attribute__ ((noinline))
uint64_t to_bitmap_simd(std::span<uint8_t> &indices) {
  __mmask64 valids = 0xFFFFFFFFFFFFFFFF >> (64 - indices.size());
  __m512i indices_simd = _mm512_maskz_loadu_epi8(valids, indices.data());
  return simd_impl_distinct(indices_simd, valids);
}