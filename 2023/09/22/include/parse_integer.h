#ifndef PARSE_INTEGER_H
#define PARSE_INTEGER_H

#include <cstdint>
/*
The `parse_unsigned_avx512` function is between 1.5x (GCC) to 3x faster (clang)
than conventional number parsing routines. Importantly, it does not 'cheat': it
does not assume that the input is padded in any way. However, we require that
you pass a pointer to the end of the digits.
*/
bool parse_unsigned(const char *src, uint64_t& value) noexcept;

// end_digits should be the *end* of the digit sequence.
bool parse_unsigned_bounded(const char *start_digits,
                                               const char *end_digits, uint64_t& value) noexcept;
// end_digits should be the *end* of the digit sequence.
bool
parse_unsigned_bounded_reverse(const char *start_digits,
                               const char *end_digits, uint64_t& value) noexcept;
// end_digits should be the *end* of the digit sequence.
bool parse_unsigned_avx512(const char *start_digits,
                                              const char *end_digits, uint64_t& value) noexcept;
bool parse_unsigned_avx512_perforated(const char *start_digits,
                                              const char *end_digits, uint64_t& value) noexcept;
#endif // PARSE_INTEGER_H
