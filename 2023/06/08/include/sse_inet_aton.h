#ifndef SSE_INET_ATON_H
#define SSE_INET_ATON_H
#include <stdint.h>
#include <stddef.h>

// convert IPv4 from text to binary form.
//
// ipv4_string points to a character string containing an IPv4 network address in dotted-decimal format
// "ddd.ddd.ddd.ddd" of length ipv4_string_length (the string does not have to be null terminated),
// where ddd is a decimal number of up to three digits in the range 0 to 255. 
// The address is converted to a 32-bit integer (destination) (in  network byte order).
//
// Important: the function will systematically read 16 bytes at the provided address (ipv4_string). However,
// only the first ipv4_string_length bytes are processed.
//
// returns 1 on success (network address was successfully converted).
//
// This function assumes that the processor supports SSE 4.1 instructions or better. That's true of most
// processors in operation today (June 2023).
//
int sse_inet_aton(const char* ipv4_string, const size_t ipv4_string_length, uint32_t * destination);

// This works like sse_inet_aton except that the length of the IP address is determined
// automatically after reading 16 bytes by selecting only the leading digits and dots.
int sse_inet_aton_16(const char* ipv4_string, uint32_t * destination);

#endif // SSE_INET_ATON_H