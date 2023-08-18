
#include "latin1_to_utf8.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <x86intrin.h> // update if we need to support Windows.

size_t latin1_to_utf8(const char *buf, size_t len, char *utf8_output) {
  const unsigned char *data = (const unsigned char *)(buf);
  size_t pos = 0;
  char *start = utf8_output;
  while (pos < len) {
    // try to convert the next block of 16 ASCII bytes
    if (pos + 16 <=
        len) { // if it is safe to read 16 more bytes, check that they are ascii
      uint64_t v1;
      memcpy(&v1, data + pos, sizeof(uint64_t));
      uint64_t v2;
      memcpy(&v2, data + pos + sizeof(uint64_t), sizeof(uint64_t));
      uint64_t v =
          v1 | v2; // We are only interested in these bits: 1000 1000 1000 1000,
                   // so it makes sense to concatenate everything
      if ((v & 0x8080808080808080) ==
          0) { // if NONE of these are set, e.g. all of them are zero, then
               // everything is ASCII
        size_t final_pos = pos + 16;
        while (pos < final_pos) {
          *utf8_output++ = (char)(buf[pos]);
          pos++;
        }
        continue;
      }
    }

    unsigned char byte = data[pos];
    if ((byte & 0x80) == 0) { // if ASCII
      // will generate one UTF-8 bytes
      *utf8_output++ = (char)(byte);
      pos++;
    } else {
      // will generate two UTF-8 bytes
      *utf8_output++ = (char)((byte >> 6) | 0b11000000);
      *utf8_output++ = (char)((byte & 0b111111) | 0b10000000);
      pos++;
    }
  }
  return (size_t)(utf8_output - start);
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

size_t latin1_to_utf8_avx512(const char *buf, size_t len, char *utf8_output) {
  char *start = utf8_output;
  size_t pos = 0;
  __m512i byteflip = _mm512_setr_epi64(0x0607040502030001, 0x0e0f0c0d0a0b0809,
                                       0x0607040502030001, 0x0e0f0c0d0a0b0809,
                                       0x0607040502030001, 0x0e0f0c0d0a0b0809,
                                       0x0607040502030001, 0x0e0f0c0d0a0b0809);
  // Note that
  for (; pos + 32 <= len; pos += 32) {
    __m256i input = _mm256_loadu_si256((__m256i *)(buf + pos));
    __mmask32 nonascii = _mm256_movepi8_mask(input);
    int output_size = 32 + _popcnt32(nonascii);
    uint64_t compmask = ~_pdep_u64(~nonascii, UINT64_C(0x5555555555555555));
    __mmask32 sixth =
        _mm256_cmpge_epu8_mask(input, _mm256_set1_epi8((char)192));
    __m512i input16 = _mm512_cvtepu8_epi16(input);
        input16 =_mm512_add_epi16(input16, _mm512_set1_epi16((short int)0xc200));
    __m512i output16 =
        _mm512_mask_add_epi16(input16, sixth, input16,
                              _mm512_set1_epi16(0x00c0));
    output16 = _mm512_shuffle_epi8(output16, byteflip);
    __m512i output = _mm512_maskz_compress_epi8(compmask, output16);
    __mmask64 write_mask = _bzhi_u64(~0ULL, (long long unsigned int)output_size);
    _mm512_mask_storeu_epi8(utf8_output, write_mask, output);
    utf8_output += output_size;
  }
  // We repeat the code, this could be reengineered to be nicer.
  if (pos < len) {
    __mmask32 load_mask = _bzhi_u32(~0U, (unsigned int)(len - pos));
    __m256i input = _mm256_maskz_loadu_epi8(load_mask, (__m256i *)(buf + pos));
    __mmask32 nonascii = _mm256_movepi8_mask(input);
    long long unsigned int output_size =
        (long long unsigned int)(len - pos + (size_t)_popcnt32(nonascii));

    uint64_t compmask = ~_pdep_u64(~nonascii, UINT64_C(0x5555555555555555));
    __mmask32 sixth =
        _mm256_cmpge_epu8_mask(input, _mm256_set1_epi8((char)192));
    __m512i input16 = _mm512_cvtepu8_epi16(input);
    input16 =_mm512_add_epi16(input16, _mm512_set1_epi16((short int)0xc200));
     __m512i output16 =   _mm512_mask_add_epi16(input16, sixth, input16,
                              _mm512_set1_epi16(0x00c0));
     output16 = _mm512_shuffle_epi8(output16, byteflip);
    __m512i output = _mm512_maskz_compress_epi8(compmask, output16);

    __mmask64 write_mask = _bzhi_u64(~0ULL, output_size);
    _mm512_mask_storeu_epi8(utf8_output, write_mask, output);
    utf8_output += output_size;
  }
  return (size_t)(utf8_output - start);
}


