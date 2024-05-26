__mmask64 simd_impl(__m512i indices, __mmask64 valids) {
  // Convert indices to bits within each qword lane.
  __m512i khi = _mm512_setr_epi8(
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
  );
  __m512i hi0 = _mm512_permutexvar_epi8(indices, khi);
  __m512i klo = _mm512_set1_epi64(0x0102040810204080);
  __m512i lo0 = _mm512_maskz_shuffle_epi8(valids, klo, indices);
  __m512i kid = _mm512_set1_epi64(0x8040201008040201);
  __m512i hi1 = _mm512_gf2p8affine_epi64_epi8(kid, hi0, 0);
  __m512i lo1 = _mm512_gf2p8affine_epi64_epi8(kid, lo0, 0);
  __m512i x0  = _mm512_gf2p8affine_epi64_epi8(hi1, lo1, 0);
  // Combine results from various qword lanes.
  __m512i ktr = _mm512_setr_epi8(
    0,  8, 16, 24, 32, 40, 48, 56,
    1,  9, 17, 25, 33, 41, 49, 57,
    2, 10, 18, 26, 34, 42, 50, 58,
    3, 11, 19, 27, 35, 43, 51, 59,
    4, 12, 20, 28, 36, 44, 52, 60,
    5, 13, 21, 29, 37, 45, 53, 61,
    6, 14, 22, 30, 38, 46, 54, 62,
    7, 15, 23, 31, 39, 47, 55, 63);
  __m512i x1  = _mm512_permutexvar_epi8(ktr, x0);
  __m512i x2  = _mm512_gf2p8affine_epi64_epi8(kid, x1, 0);
  // Reduce 64 bytes down to 64 bits.
  __m512i kff = _mm512_set1_epi8(0xff);
  __m512i x3  = _mm512_gf2p8affine_epi64_epi8(x2, kff, 0);
  return _mm512_movepi8_mask(x3);
}


// assuming that indices are distinct
__mmask64 simd_impl_distinct(__m512i indices, __mmask64 valids) {
  // Convert indices to bits within each qword lane.
  __m512i khi = _mm512_setr_epi8(
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
  );
  __m512i hi0 = _mm512_permutexvar_epi8(indices, khi);
  __m512i klo = _mm512_set1_epi64(0x0102040810204080);
  __m512i lo0 = _mm512_maskz_shuffle_epi8(valids, klo, indices);
  __m512i kid = _mm512_set1_epi64(0x8040201008040201);
  __m512i hi1 = _mm512_gf2p8affine_epi64_epi8(kid, hi0, 0);
  __m512i lo1 = _mm512_gf2p8affine_epi64_epi8(kid, lo0, 0);
  __m512i x0  = _mm512_gf2p8affine_epi64_epi8(hi1, lo1, 0);
  // Combine results from various qword lanes.
  __m512i ktr = _mm512_setr_epi8(
    0,  8, 16, 24, 32, 40, 48, 56,
    1,  9, 17, 25, 33, 41, 49, 57,
    2, 10, 18, 26, 34, 42, 50, 58,
    3, 11, 19, 27, 35, 43, 51, 59,
    4, 12, 20, 28, 36, 44, 52, 60,
    5, 13, 21, 29, 37, 45, 53, 61,
    6, 14, 22, 30, 38, 46, 54, 62,
    7, 15, 23, 31, 39, 47, 55, 63);
  __m512i x1  = _mm512_permutexvar_epi8(ktr, x0);
  __m512i x2  = _mm512_gf2p8affine_epi64_epi8(kid, x1, 0);
  // Reduce 64 bytes down to 64 bits.
  return _mm512_cmpneq_epi8_mask(x2, _mm512_setzero_si512());
}
