#ifndef BASE32_H
#define BASE32_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
// base32 encodes encodes 5 bytes to 8 characters.
// We implement base32hex.
//
// The functions return the
// number of input bytes decoded (not counting padding).
//
// The number of valid output bytes can be computed as
// (x*5)/8.
size_t base32hex_simple(uint8_t *dst, const uint8_t *src);
size_t base32hex_fast(uint8_t *dst, const uint8_t *src);

// may overwrite by 3 bytes
size_t base32hex_swar(uint8_t *dst, const uint8_t *src);
// may overwrite by 8 bytes
size_t base32hex_simd(uint8_t *dst, const uint8_t *src);
#endif // BASE32_H
