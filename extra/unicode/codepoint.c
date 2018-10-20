#include <memory.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <x86intrin.h>

static const uint32_t leading_zeros_to_utf_bytes[33] = {
    1, 1, 1, 1, 1, 1, 1, 1,         // 7 bits for first one
    2, 2, 2, 2,                     // 11 bits for next
    3, 3, 3, 3, 3,                  // 16 bits for next
    4, 4, 4, 4, 4,                  // 21 bits for next
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // error
};

static const uint32_t UTF_PDEP_MASK[5] = {0x00, // error
                                          0x7f, 0x1f3f, 0x0f3f3f, 0x073f3f3f};

static const uint32_t UTF_OR_MASK[5] = {0x00, // error
                                        0x00, 0xc080, 0xe08080, 0xf0808080};

size_t utf32_to_utf8(const uint32_t code_point, char *dst_ptr) {
  uint32_t lz =
      __builtin_clz(code_point); // we hope that code_point = 0 leads to 32
  printf("lz = %u ", lz);
  uint32_t utf_bytes = leading_zeros_to_utf_bytes[lz];
  uint32_t tmp =
      _pdep_u32(code_point, UTF_PDEP_MASK[utf_bytes]) | UTF_OR_MASK[utf_bytes];
  // swap and move to the other side of the register
  tmp = __builtin_bswap32(tmp);
  tmp >>= ((4 - utf_bytes) * 8) & 31; // if utf_bytes, this could become a shift
                                      // by 32, hence the mask with 31
  // use memcpy to avoid undefined behavior:
  memcpy(dst_ptr, &tmp, sizeof(uint32_t));
  return utf_bytes;
}

// c is code point, b is a buffer, output is number
// of bytes...
size_t utf32_to_utf8_so(uint32_t c, char *b) {
  char *bb = b;
  if (c < 0x80)
    *b++ = c;
  else if (c < 0x800)
    *b++ = 192 + c / 64, *b++ = 128 + c % 64;
  else if (c - 0xd800u < 0x800)
    return 0; // error
  else if (c < 0x10000)
    *b++ = 224 + c / 4096, *b++ = 128 + c / 64 % 64, *b++ = 128 + c % 64;
  else if (c < 0x110000)
    *b++ = 240 + c / 262144, *b++ = 128 + c / 4096 % 64,
    *b++ = 128 + c / 64 % 64, *b++ = 128 + c % 64;
  else
    return 0;
  return b - bb;
}

uint32_t codepoint(const unsigned char *u, size_t l) {
  if (l < 1)
    return -1;
  unsigned char u0 = u[0];
  if (u0 >= 0 && u0 <= 127)
    return u0;
  if (l < 2)
    return -1;
  unsigned char u1 = u[1];
  if (u0 >= 192 && u0 <= 223)
    return (u0 - 192) * 64 + (u1 - 128);
  if (u[0] == 0xed && (u[1] & 0xa0) == 0xa0)
    return -1; // code points, 0xd800 to 0xdfff
  if (l < 3)
    return -1;
  unsigned char u2 = u[2];
  if (u0 >= 224 && u0 <= 239)
    return (u0 - 224) * 4096 + (u1 - 128) * 64 + (u2 - 128);
  if (l < 4)
    return -1;
  unsigned char u3 = u[3];
  if (u0 >= 240 && u0 <= 247)
    return (u0 - 240) * 262144 + (u1 - 128) * 4096 + (u2 - 128) * 64 +
           (u3 - 128);
  return -1;
}

// given a code point cp, writes to c
// the utf-8 code, outputting the length in
// bytes, if the length is zero, the code point
// is invalid
size_t codepoint_to_utf8(uint32_t cp, char *c) {
  if (cp <= 0x7F) {
    c[0] = cp;
    return 1; // ascii
  } else if (cp <= 0x7FF) {
    c[0] = (cp >> 6) + 192;
    c[1] = (cp & 63) + 128;
    return 2; // universal plane
  } else if (0xd800 <= cp && cp <= 0xdfff) {
    return 0; // surrogates
  } else if (cp <= 0xFFFF) {
    c[0] = (cp >> 12) + 224;
    c[1] = ((cp >> 6) & 63) + 128;
    c[2] = (cp & 63) + 128;
    return 3;
  } else if (cp <= 0x10FFFF) {
    c[0] = (cp >> 18) + 240;
    c[1] = ((cp >> 12) & 63) + 128;
    c[2] = ((cp >> 6) & 63) + 128;
    c[3] = (cp & 63) + 128;
    return 4;
  }
  return 0; // bad
}

int main() {
  char c1[4];
  char c2[4];
  for (uint32_t cp = 0; cp < 0x10FFFF; cp++) {
    if ((cp >= 0xD800) && (cp <= 0xDFFF)) {
      // surrogates
      continue;
    }
    // size_t l1 = utf8chr(cp,c1);
    size_t l1 = codepoint_to_utf8(cp, c1);
    uint32_t rcp = codepoint((const unsigned char *)c1, l1);
    if (rcp != cp) {

      printf("incorrect %x %x \n", rcp, cp);
      return -1;
    }
    // printf("%*s", l1,c1);
    /*size_t l2 = utf32_to_utf8(cp, c2);
    printf("l1 = %zu l2 %zu \n", l1, l2);
    if (l1 != l2) {
      printf("incorrect len %u \n", cp);
      return -1;
    }
    for (size_t i = 0; i < l1; i++) {
      if (c1[i] != c2[i]) {
        printf("incorrect bit %u \n", cp);
        return -1;
      }
    }*/
  }
  return 0;
}