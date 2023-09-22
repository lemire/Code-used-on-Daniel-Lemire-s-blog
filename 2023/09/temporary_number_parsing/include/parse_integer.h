#ifndef PARSE_INTEGER_H
#define PARSE_INTEGER_H

#include <cstdint>
#include <optional>
/*
The `parse_unsigned_avx512` function is between 1.5x (GCC) to 3x faster (clang)
than conventional number parsing routines. Importantly, it does not 'cheat': it
does not assume that the input is padded in any way. However, we require that
you pass a pointer to the end of the digits.
*/
std::optional<uint64_t> parse_unsigned(const char *src) noexcept;

// end_digits should be the *end* of the digit sequence.
std::optional<uint64_t> parse_unsigned_bounded(const char *start_digits,
                                               const char *end_digits) noexcept;
// end_digits should be the *end* of the digit sequence.
std::optional<uint64_t>
parse_unsigned_bounded_reverse(const char *start_digits,
                               const char *end_digits) noexcept;
// end_digits should be the *end* of the digit sequence.
std::optional<uint64_t> parse_unsigned_avx512(const char *start_digits,
                                              const char *end_digits) noexcept;

#endif // PARSE_INTEGER_H
