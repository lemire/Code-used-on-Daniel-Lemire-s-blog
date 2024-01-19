#ifndef ESCAPE_H
#define ESCAPE_H
#include <cstdint>
#include <cstddef>


// Parses ASCII numbers between 0 and 255 with validation.

// These functions return a Boolean indicating whether an 8-bit positive value
// could be parse (between 0 and 255 inclusively). The parsed value is stored
// in 'num'. You must provide the exact length of the number in bytes in the variable 'len' (1, 2, 3).

// parse_uint8_swar is not correct and left as a reference, reads 4 bytes at str with every call
int parse_uint8_swar(const char *str, size_t len, uint8_t *num);
// parse_uint8_fastswar is correct and fast, reads 4 bytes at str with every call
// Assumes a little-endian system which is almost surely correct unless you own a mainframe computer from IBM
int parse_uint8_fastswar(const char *str, size_t len, uint8_t *num);
// like parse_uint8_fastswar, but might be faster under some compilers
int parse_uint8_fastswar_bob(const char *str, size_t len, uint8_t *num);
// like parse_uint8_fastswar, but removes a branch
int parse_uint8_fastswar_david(const char *str, size_t len, uint8_t *num);
// parse_uint8_fromchars is correct
int parse_uint8_fromchars(const char *str, size_t len, uint8_t *num);
int parse_uint8_naive(const char *str, size_t len, uint8_t *num);


// look-up table, caller must first call "make_lut"
int parse_uint8_lut(const char *str, size_t len, uint8_t *num);
void make_lut();

//
int parse_uint8_switch_case(const char *str, size_t len, uint8_t *num);


void parse_uint8_nocheck(const char *str, size_t len, uint8_t *num);
#endif // ESCAPE_H
