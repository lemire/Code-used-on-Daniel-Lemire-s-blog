//////////////
// So  x^128 + x^7 + x^2 + x + 1 is an irreducible polynomial
// We want to reduce 256-bit values with  x^128 + x^7 + x^2 + x + 1.
// Let us represent a 256-bit word as  64-bit words in little endian mode
// IH1 IH0 IL1 IL0
//
// The values IL1 IL0 are already part of the modulo reduction, without
// effort. So we can XOR them at the end.
//
// To reduce IH0, we simply need to multiply it by x^7 + x^2 + x + 1, the 64-bit
// result is reduced.
// For IH1, we can proceed in two steps... if we multiply it by x^7 + x^2 + x + 1,
// getting mul2 (written as mul2_2 mul2_1)
// then we effectively shift down by 128 bits...
// so we have 0 mul2_2 mul2_1 0
// so we can add mul2_2 to IH1 and reduce them together.
/////////////
__m128i Reduce(const __m128i& IH, const __m128i& IL) {
  // GCM modulus (without the implicit x^128)
  const __m128i p = _mm_set1_epi64(0x87ULL);  // x^7 + x^2 + x + 1
  const __m128i zero = _mm_setzero_si128();
  const __m128i H1mul = _mm_clmulepi64_si128(IH, p, 0x11); // multiply the most significant 64 bits of IH by (x^7 + x^2 + x + 1)
  const __m128i H1mul_L0 = _mm_unpacklo_epi64(zero, H1mul); // grab the lowest 64 bits of H1mul
  const __m128i H1mul_0H = _mm_unpackhi_epi64(H1mul, zero); // grab the highest 64 bits of H1mul
  const __m128i mul2 = _mm_clmulepi64_si128(H1mul_L0 ^ IH, p, 0x00);
  return mul2 ^ (Hmul1_0H ^ IL);
}
