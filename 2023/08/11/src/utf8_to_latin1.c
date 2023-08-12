
#include "utf8_to_latin1.h"
#include <string.h>
#include <stdint.h>
#include <x86intrin.h> // update if we need to support Windows.
#include <stdio.h>


size_t utf8_to_latin1(const char* buf, size_t len, char* latin_output) {
 const uint8_t *data = (const uint8_t *)(buf);
  size_t pos = 0;
  char* start = latin_output;

  while (pos < len) {
    // try to convert the next block of 16 ASCII bytes
    if (pos + 16 <= len) { // if it is safe to read 16 more bytes, check that they are ascii
      uint64_t v1;
      memcpy(&v1, data + pos, sizeof(uint64_t));
      uint64_t v2;
      memcpy(&v2, data + pos + sizeof(uint64_t), sizeof(uint64_t));
      uint64_t v = v1 | v2; // We are only interested in these bits: 1000 1000 1000 1000 .... etc
      if ((v & 0x8080808080808080) == 0) { // if NONE of these are set, e.g. all of them are zero, then everything is ASCII
        size_t final_pos = pos + 16;
        while(pos < final_pos) {
          *latin_output++ = (char)(buf[pos]);
          pos++;
        }
        continue;
      }
    }

    // suppose it is not an all ASCII byte sequence
    uint8_t leading_byte = data[pos]; // leading byte
    if (leading_byte < 0b10000000) {
      // converting one ASCII byte !!!
      *latin_output++ = (char)(leading_byte);
      pos++;
    } else if ((leading_byte & 0b11100000) == 0b11000000) { // the first three bits indicate:
      // We have a two-byte UTF-8
      if(pos + 1 >= len) {
         return 0; } // minimal bound checking
      if ((data[pos + 1] & 0b11000000) != 0b10000000) { return 0; } // checks if the next byte is a valid continuation byte in UTF-8. A valid continuation byte starts with 10.
      // range check -
      uint32_t code_point = (leading_byte & 0b00011111) << 6 | (data[pos + 1] & 0b00111111); // assembles the Unicode code point from the two bytes. It does this by discarding the leading 110 and 10 bits from the two bytes, shifting the remaining bits of the first byte, and then combining the results with a bitwise OR operation.
      if (code_point < 0x80 || 0xFF < code_point) {
        return 0; // We only care about the range 129-255 which is Non-ASCII latin1 characters. A code_point beneath 0x80 is invalid as it's already covered by bytes whose leading bit is zero. 
      }
      *latin_output++ = (char)(code_point);
      pos += 2;
    } else {
      return 0;
    }
  }
  return (size_t)(latin_output - start);
}
void printbinary(uint64_t n) {
  for (size_t i = 0; i < 64; i++) {
    if (n & 1)
      printf("1");
    else
      printf("0");

    n >>= 1;
  }
  printf("\n");
}
void print8(const char *name, __m512i x) {
  printf("%.32s : ", name);
  uint8_t buffer[64];
  _mm512_storeu_si512((__m512i *)buffer, x);
  for (size_t i = 0; i < 64; i++) {
    printf("%02x ", buffer[i]);
  }
  printf("\n");
}
size_t utf8_to_latin1_avx512(const char* buf, size_t len, char* latin_output) {
  char* start = latin_output;
  size_t pos = 0;
  __m512i mask_80808080 = _mm512_set1_epi32((int)0x80808080);

  while(pos + 64 <= len) {
    __m512i input = _mm512_loadu_si512((__m512i *)(buf + pos));
    __mmask64 ascii = _mm512_cmplt_epu8_mask(input, mask_80808080);
    if(ascii == 0) {
      _mm512_storeu_si512(latin_output, input);
      latin_output += 64;
      pos += 64;
      continue;
    }
    __mmask64 continuation = _mm512_cmplt_epi8_mask(input, _mm512_set1_epi8(-64));
    __mmask64 leading = _kxnor_mask64(ascii, continuation);
    //
    // All continuation bytes *must* be preceded by a leading byte
    if((continuation>>1)&~leading) { abort();return 0; } // some continuation byte is not preceeded by a leading byte
    // We cannot start with a continuation byte:
    if(continuation & 1) { abort();return 0; }
    // All leading bytes *must* be followed by a continuation byte... except maybe for the last one
    if((leading<<1)^continuation) { abort();return 0; } // some continuation byte is not preceeded by a leading byte
    bool end_with_leading = leading & 0x8000000000000000; // that's possible, if so we are missing the continuation byte
    // Only two possible leading byte value: 
    // 0b11000011 and 0b11000010. Or 194 and 195 as unsigned byte values.
    // Adding 62 to it, we get 0 or 1.
    __m512i highbits = _mm512_maskz_add_epi8(leading, input, _mm512_set1_epi8(62));
    __mmask64 invalid_leading_bytes = _mm512_mask_cmpgt_epu8_mask(leading, highbits, _mm512_set1_epi8(1));
    if(invalid_leading_bytes) { abort();return 0; } // We have an invalid leading byte.
    highbits = _mm512_slli_epi16(highbits, 6); // shift in position
    input = _mm512_mask_blend_epi8(leading, input, highbits);
    //
    // We are going to load the ascii + continuation
    __m512i ascii_continuation = _mm512_maskz_compress_epi8(ascii | continuation, input);
    __m512i ascii_leading = _mm512_maskz_compress_epi8(ascii | leading, input);
    __m512i output = _mm512_or_si512(ascii_continuation, ascii_leading);
    size_t written_out = (size_t)(64 - _popcnt64(continuation));
    if(end_with_leading) { written_out -= 1;}
    __mmask64 store_mask = (1ULL << written_out) - 1;
    _mm512_mask_storeu_epi8((__m512i*)latin_output, store_mask, output); 
    latin_output += written_out;
    pos += 64;
    if(end_with_leading) { pos -= 1; }
  }
  // We repeat the code, this could be reengineered to be nicer.
  if(pos < len) {
    __mmask64 load_mask = (1ULL<<(len-pos)) - 1;
    __m512i input = _mm512_mask_loadu_epi8(_mm512_setzero_si512(), load_mask, (__m512i *)(buf + pos));
    __mmask64 ascii = _mm512_cmplt_epu8_mask(input, mask_80808080);
    __mmask64 fake_ascii = ascii &~ load_mask;
    __mmask64 continuation = _mm512_cmplt_epi8_mask(input, _mm512_set1_epi8(-64));
    __mmask64 leading = _kxnor_mask64(ascii, continuation);
    //
    // All continuation bytes *must* be preceded by a leading byte
    if((continuation>>1)&~leading) { abort(); return 0; } // some continuation byte is not preceeded by a leading byte
    // We cannot start with a continuation byte:
    if(continuation & 1) { abort(); return 0; }
    // All leading bytes *must* be followed by a continuation byte... except maybe for the last one
    if((leading<<1)^continuation) { abort(); return 0; } // some continuation byte is not preceeded by a leading byte
    // Only two possible leading byte value: 
    // 0b11000011 and 0b11000010. Or 194 and 195 as unsigned byte values.
    // Adding 62 to it, we get 0 or 1.
    __m512i highbits = _mm512_maskz_add_epi8(leading, input, _mm512_set1_epi8(62));
    __mmask64 invalid_leading_bytes = _mm512_mask_cmpgt_epu8_mask(leading, highbits, _mm512_set1_epi8(1));
    if(invalid_leading_bytes) { abort(); return 0; } // We have an invalid leading byte.
    highbits = _mm512_slli_epi16(highbits, 6); // shift in position
    input = _mm512_mask_blend_epi8(leading, input, highbits);
    //
    // We are going to load the ascii + continuation
    __m512i ascii_continuation = _mm512_maskz_compress_epi8(ascii | continuation, input);
    __m512i ascii_leading = _mm512_maskz_compress_epi8(ascii | leading, input);
    __m512i output = _mm512_or_si512(ascii_continuation, ascii_leading);
    size_t written_out = (size_t)(64 - _popcnt64(continuation|fake_ascii));
    __mmask64 store_mask = (1ULL << written_out) - 1;
    _mm512_mask_storeu_epi8((__m512i*)latin_output, store_mask, output); 
    latin_output += written_out;
  }
  return (size_t)(latin_output - start);
}