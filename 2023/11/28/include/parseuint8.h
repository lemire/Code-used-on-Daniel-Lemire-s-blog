#ifndef ESCAPE_H
#define ESCAPE_H
#include <cstdint>
#include <cstddef>

int parse_uint8_swar(const char *str, size_t len, uint8_t *num);
int parse_uint8_fastswar(const char *str, size_t len, uint8_t *num);
int parse_uint8_fromchars(const char *str, size_t len, uint8_t *num);
int parse_uint8_naive(const char *str, size_t len, uint8_t *num);
#endif // ESCAPE_H
