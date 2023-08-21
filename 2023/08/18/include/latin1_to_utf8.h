#ifndef NAME_TO_DNSWIRE_H
#define NAME_TO_DNSWIRE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


size_t latin1_to_utf8(const char* buf, size_t len, char* utf8_output);
size_t latin1_to_utf8_avx512(const char* buf, size_t len, char* utf8_output);
size_t latin1_to_utf8_avx512_InstLatX64(const char* buf, size_t len, char* utf8_output);
size_t latin1_to_utf8_avx512_InstLatX64_2(const char *buf, size_t len, char *utf8_output);
size_t latin1_to_utf8_avx512_my_branch1(const char *buf, size_t len, char *utf8_output);
size_t latin1_to_utf8_avx512_my_branch0(const char *buf, size_t len, char *utf8_output);
size_t latin1_to_utf8_avx512_my_nobranch(const char *buf, size_t len, char *utf8_output);
#endif // NAME_TO_DNSWIRE_H
