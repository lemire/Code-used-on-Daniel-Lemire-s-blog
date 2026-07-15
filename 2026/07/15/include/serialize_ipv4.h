#ifndef SERIALIZE_IPV4_H
#define SERIALIZE_IPV4_H

#include <x86intrin.h>
#include <cstdint>

#if defined(__AVX512IFMA__) && defined(__AVX512VBMI__)

/**
 * Convert an IPv4 address from a 32-bit integer to a string.
 * @param ip The IPv4 address in host byte order.
 * @param out The output buffer (needs 15 bytes of buffer space). No null terminator is added.
 * @return The length of the resulting string, it will range from 7 to 15 characters.
 */

size_t ipv4_to_string(uint32_t ip, char *out) { // out needs 15 bytes of buffer space
  __m128i v  = _mm_cvtsi32_si128((int)ip);
  // each octet in both words of its dword: [b0,b0, b1,b1, b2,b2, b3,b3]
  __m128i vw = _mm_shuffle_epi8(v,
      _mm_setr_epi8(0,-1,0,-1, 1,-1,1,-1, 2,-1,2,-1, 3,-1,3,-1));
  __m128i q   = _mm_mulhi_epu16(vw, _mm_set1_epi32((6554 << 16) | 656)); // [b/100, b/10]
  __m128i t10 = _mm_mullo_epi16(q, _mm_set1_epi16(10));   // [10*(b/100), 10*(b/10)]
  __m128i ht  = _mm_sub_epi16(q, _mm_slli_epi32(t10, 16)); // [h, t]
  __m128i on  = _mm_sub_epi16(vw, t10);                    // odd words = ones digit
  // per lane: pick h, t, o, and a known-zero byte (high byte of h word)
  __m128i idx = _mm_setr_epi8(0,2,18,1, 4,6,22,5, 8,10,26,9, 12,14,30,13);
  __m128i chars = _mm_add_epi8(_mm_permutex2var_epi8(ht, idx, on),
      _mm_setr_epi8('0','0','0','.', '0','0','0','.',
                    '0','0','0','.', '0','0','0','.'));
  // mask: hundreds iff b>=100, tens iff b>=10, ones/dot always, kill pos 15
  __m128i vb = _mm_shuffle_epi8(v,
      _mm_setr_epi8(0,0,0,0, 1,1,1,1, 2,2,2,2, 3,3,3,-1));
  __mmask16 m = _mm_cmpge_epu8_mask(vb,
      _mm_setr_epi8(100,10,0,0, 100,10,0,0, 100,10,0,0, 100,10,0,1));
  _mm_storeu_si128((__m128i*)out, _mm_maskz_compress_epi8(m, chars));
  return _mm_popcnt_u32((unsigned)m);
}

#else // defined(__AVX512IFMA__) && defined(__AVX512VBMI__)
#error "This code requires AVX512IFMA and AVX512VBMI support."
#endif // defined(__AVX512IFMA__) && defined(__AVX512VBMI__)

#endif // SERIALIZE_IPV4_H
