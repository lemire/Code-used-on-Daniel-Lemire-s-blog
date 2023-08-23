#ifndef SSE_INET_ATON_H
#define SSE_INET_ATON_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Convert a string in the form YYYYMMDDHHmmSS to an integer (stored in
// destination) representing the time in second since epoch. The function
// returns true upon success.
// Because we use an unsigned 32-bit integer, we can represent all time between
// 1970 and 2106. We cannot represent time before 1970.
bool parse_time(const char *date_string, uint32_t *time_in_second);

// Convert a string in the form YYYYMMDDHHmmSS to an integer (stored in
// destination) representing the time in second since epoch. The function
// returns true upon success. This function may read up to 16 bytes in
// date_string but will only process the first 14 bytes.
// Because we use an unsigned 32-bit integer, we can represent all time between
// 1970 and 2106. We cannot represent time before 1970.
//
// In Unix time, we omit leap seconds, so that the SS cannot be 60 or above.
bool sse_parse_time(const char *date_string, uint32_t *time_in_second);
bool sse_parse_time_alt(const char *date_string, uint32_t *time_in_second);

#endif // SSE_INET_ATON_H