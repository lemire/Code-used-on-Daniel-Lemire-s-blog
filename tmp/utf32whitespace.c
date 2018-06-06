#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

static inline bool naive_is_whitespace(uint32_t char32) {
  return (char32 == 0x9) || (char32 ==  0xA) || (char32 ==  0xB) || (char32 ==  0xC) || (char32 ==  0xD) || (char32 ==  0x20) || (char32 ==  0x85) || (char32 ==  0xA0) || (char32 ==  0x1680) || (char32 ==  0x2000) || (char32 ==  0x2001) || (char32 ==  0x2002) || (char32 ==  0x2003) || (char32 ==  0x2004) || (char32 ==  0x2005) || (char32 ==  0x2006) || (char32 ==  0x2007) || (char32 ==  0x2008) || (char32 ==  0x2009) || (char32 ==  0x200A) || (char32 ==  0x2028) || (char32 ==  0x2029) || (char32 ==  0x202F) || (char32 ==  0x205F) || (char32 ==  0x3000);
}

// naive
size_t removewhite(uint32_t * charlist, size_t length) {
  size_t pos = 0;
  for(size_t i = 0; i < length; i++) {
    uint32_t char32 = charlist[i];
    if(!naive_is_whitespace(__builtin_bswap32(char32))){
      charlist[pos++] = char32;
    }
  }
  return pos;
}

uint8_t nibble1[16] = {156, 16, 16, 16, 16, 18, 16, 16, 48, 49, 17, 1, 1, 1, 0, 96};
uint8_t nibble2[16] = {145, 0, 36, 0, 0, 64, 0, 0, 10, 0, 4, 0, 0, 0, 0, 0};
uint8_t nibble3[16] = {247, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t nibble4[16] = {7, 8, 112, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// less naive, but still naive
size_t lu_removewhite(uint32_t * charlist, size_t length) {
  size_t pos = 0;
  for(size_t i = 0; i < length; i++) {
    uint32_t char32 = charlist[i];
    uint8_t n1 = nibble1[(char32 >> (32 - 8)) & 0xF];
    uint8_t n2 = nibble2[char32 >> (32 - 4)];
    uint8_t n3 = nibble3[(char32 >> (32 - 16)) & 0xF];
    uint8_t n4 = nibble4[(char32 >> (32 - 12)) & 0xF];

    bool is_white_space = ((n1 & n2 & n3 & n4) != 0) && ((char32 &0xFFFF) == 0);
    if(!is_white_space){
      charlist[pos++] = char32;
    }
  }
  return pos;
}

void fillwithascii(uint32_t * data, size_t length) {
  memset(data,0, length * sizeof(uint32_t));
  for(size_t i = 0; i < length; i++) {
    data[i] = __builtin_bswap32(rand() & 0x7f);
  }
}

void demo(size_t N) {
  uint32_t * data = malloc(N * sizeof(uint32_t));
  fillwithascii(data,N);
  uint32_t * buffer = malloc(N * sizeof(uint32_t));
  memcpy(buffer,data,N * sizeof(uint32_t));
  size_t expected = removewhite(buffer,N);
  int repeat = 5;
  BEST_TIME(
      removewhite(buffer,N),
      expected, memcpy(buffer,data,N * sizeof(uint32_t)), repeat, N, true);
  BEST_TIME(
      lu_removewhite(buffer,N),
      expected, memcpy(buffer,data,N * sizeof(uint32_t)), repeat, N, true);
  free(data);
}

int main() {

  demo(10000000);
  return EXIT_SUCCESS;
}
