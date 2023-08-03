#ifndef NAME_TO_DNSWIRE_H
#define NAME_TO_DNSWIRE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// This expectation is that this takes a name, such as "lemire.me" or "6c 65 6d 69 72 65 2e 6d 65"
// (the string can be followed by anything, including a null character), and converts it to the wire format:
// "06 6c 65 6d 69 72 65 02 6d 65 00".
// Returns the number of bytes consumed, at most 15 bytes are consumed, 
// if more, then it will not have processed the whole name.
size_t name_to_dnswire_simd(const char *src, uint8_t *dst);
size_t name_to_dnswire_simd_fast(const char *src, uint8_t *dst);

size_t name_to_dnswire(const char *src, uint8_t *dst);

#endif // NAME_TO_DNSWIRE_H
