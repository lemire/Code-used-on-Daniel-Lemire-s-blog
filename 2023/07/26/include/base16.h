#ifndef BASE16_H
#define BASE16_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// returns how many bytes were consumed.
// the number of words decoded can be computed as half that assuming
// that the numbers come in pairs.

size_t base16hex_simple(uint8_t *dst, const uint8_t *src);

size_t base16hex_simd(uint8_t *dst, const uint8_t *src);

size_t base16hex_avx(uint8_t *dst, const uint8_t *src);

size_t base16hex_simd_geoff(uint8_t *dst, const uint8_t *src);

size_t base16hex_simdzone_fallback(uint8_t *dst, const uint8_t *src);

// decode an even number of characters, while skipping spaces.
size_t base16hex_simd_skipspace(uint8_t *dst, const uint8_t *src,
                                size_t *output_size);
#endif // BASE16_H
