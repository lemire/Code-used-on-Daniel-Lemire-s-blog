#pragma once

#include <cstdint>
#include <span>
void NaiveAdvanceString(const char *&start, const char *end) {
  for (;start < end; start++) {
    if(*start == '<' || *start == '&' || *start == '\r' || *start == '\0') {
      return;
    }
  }
}
#if defined(__aarch64__) 
#include <arm_neon.h>


void AdvanceString(const char *&start, const char *end) {
  uint8x16_t quote_mask = vmovq_n_u8('<');
  uint8x16_t escape_mask = vmovq_n_u8('&');
  uint8x16_t newline_mask = vmovq_n_u8('\r');
  uint8x16_t zero_mask = vmovq_n_u8('\0');
  uint8x16_t bit_mask = {16, 15, 14, 13, 12, 11, 10, 9, 8,
                            7, 6, 5, 4, 3, 2, 1};
  static constexpr auto stride = 16;
  for (; start + (stride - 1) < end; start += stride) {
    uint8x16_t data = vld1q_u8(reinterpret_cast<const uint8_t *>(start));
    uint8x16_t quotes = vceqq_u8(data, quote_mask);
    uint8x16_t escapes = vceqq_u8(data, escape_mask);
    uint8x16_t newlines = vceqq_u8(data, newline_mask);
    uint8x16_t zeros = vceqq_u8(data, zero_mask);
    uint8x16_t mask = vorrq_u8(vorrq_u8(quotes,zeros), vorrq_u8(escapes, newlines));
    uint8x16_t matches = vandq_u8(bit_mask, mask);
    int m = vmaxvq_u8(matches);
    if(m != 0) {
      start += 16 - m;
      return;
    }
  }  
  for (;start < end; start++) {
    if(*start == '<' || *start == '&' || *start == '\r' || *start == '\0') {
      return;
    }
  }
}

void AdvanceStringTable(const char *&start, const char *end) {
  uint8x16_t low_nibble_mask = {0b0001, 0, 0, 0, 0, 0, 0b0100, 0, 0, 0, 0, 0, 0b0010, 0b1000, 0, 0};
  uint8x16_t high_nibble_mask = {0b1001, 0, 0b0100, 0b0010, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8x16_t v0f = vmovq_n_u8(0xf);
  uint8x16_t bit_mask = {16, 15, 14, 13, 12, 11, 10, 9, 8,
                            7, 6, 5, 4, 3, 2, 1};
  static constexpr auto stride = 16;
  for (; start + (stride - 1) < end; start += stride) {
    uint8x16_t data = vld1q_u8(reinterpret_cast<const uint8_t *>(start));
    uint8x16_t lowpart = vqtbl1q_u8(low_nibble_mask, vandq_u8(data, v0f));
    uint8x16_t highpart = vqtbl1q_u8(high_nibble_mask, vshrq_n_u8(data, 4));
    uint8x16_t classify = vandq_u8(lowpart, highpart);
    uint8x16_t matchesones = vtstq_u8(classify, vdupq_n_u8(0xFF));
    uint8x16_t matches = vandq_u8(bit_mask, matchesones);
    int m = vmaxvq_u8(matches);
    if(m != 0) {
      start += 16 - m;
      return;
    }
  }  
  for (;start < end; start++) {
    if(*start == '<' || *start == '&' || *start == '\r' || *start == '\0') {
      return;
    }
  }
}

void AdvanceStringTableSimpler(const char *&start, const char *end) {
  uint8x16_t low_nibble_mask = {0, 0, 0, 0, 0, 0, 0x26, 0, 0, 0, 0, 0, 0x3c, 0xd, 0, 0};
  uint8x16_t v0f = vmovq_n_u8(0xf);
  uint8x16_t bit_mask = {16, 15, 14, 13, 12, 11, 10, 9, 8,
                            7, 6, 5, 4, 3, 2, 1};
  static constexpr auto stride = 16;
  for (; start + (stride - 1) < end; start += stride) {
    uint8x16_t data = vld1q_u8(reinterpret_cast<const uint8_t *>(start));
    uint8x16_t lowpart = vqtbl1q_u8(low_nibble_mask, vandq_u8(data, v0f));
    uint8x16_t matchesones = vceqq_u8(lowpart, data);
    if(vmaxvq_u32(vreinterpretq_u32_u8(matchesones)) != 0) {
      uint8x16_t matches = vandq_u8(bit_mask, matchesones);
      int m = vmaxvq_u8(matches);
      start += 16 - m;
      return;    
    }

  }  
  for (;start < end; start++) {
    if(*start == '<' || *start == '&' || *start == '\r' || *start == '\0') {
      return;
    }
  }
}
#else
#include <emmintrin.h>

void AdvanceString(const char*& start, const char* end) {
    const __m128i quote_mask = _mm_set1_epi8('<');
    const __m128i escape_mask = _mm_set1_epi8('&');
    const __m128i newline_mask = _mm_set1_epi8('\r');
    const __m128i zero_mask = _mm_set1_epi8('\0');

    static constexpr auto stride = 16;
    for (; start + (stride - 1) < end; start += stride) {
        __m128i data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(start));
        __m128i quotes = _mm_cmpeq_epi8(data, quote_mask);
        __m128i escapes = _mm_cmpeq_epi8(data, escape_mask);
        __m128i newlines = _mm_cmpeq_epi8(data, newline_mask);
        __m128i zeros = _mm_cmpeq_epi8(data, zero_mask);
        __m128i mask = _mm_or_si128(_mm_or_si128(quotes, zeros), _mm_or_si128(escapes, newlines));
        int m = _mm_movemask_epi8(mask);
        if (m != 0) {
            start += __builtin_ctz(m);
            return;
        }
    }

    // Process any remaining bytes (less than 16)
    while (start < end) {
        if (*start == '<' || *start == '&' || *start == '\r' || *start == '\0') {
            return;
        }
        start++;
    }
}

void AdvanceStringTable(const char *&start, const char *end) {
  AdvanceString(start, end);
}

void AdvanceStringTableSimpler(const char *&start, const char *end) {
  AdvanceString(start, end);
}
#endif