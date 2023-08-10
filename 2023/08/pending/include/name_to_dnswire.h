#ifndef NAME_TO_DNSWIRE_H
#define NAME_TO_DNSWIRE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Convert a DNS name into wire format: dots are replaced by length.
// This expectation is that this takes a name, such as "lemire.me" or "6c 65 6d 69 72 65 2e 6d 65"
// (the string can be followed by anything, including a null character), and converts it to the wire format:
// "06 6c 65 6d 69 72 65 02 6d 65 00".
// Returns the number of bytes consumed.
size_t name_to_dnswire_simd(const char *src, uint8_t *dst);
size_t name_to_dnswire_avx(const char *src, uint8_t *dst);
size_t name_to_dnswire_idx_avx(const char *src, uint8_t *dst);

size_t name_to_dnswire(const char *src, uint8_t *dst);

#endif // NAME_TO_DNSWIRE_H
