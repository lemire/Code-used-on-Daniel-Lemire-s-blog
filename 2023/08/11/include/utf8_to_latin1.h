#ifndef NAME_TO_DNSWIRE_H
#define NAME_TO_DNSWIRE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


size_t utf8_to_latin1(const char* buf, size_t len, char* latin_output);
size_t utf8_to_latin1_avx512(const char* buf, size_t len, char* latin_output);

#endif // NAME_TO_DNSWIRE_H
