#ifndef STORE_FORWARD_H
#define STORE_FORWARD_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Each latency function repeats a dependent store then load n times.
// The loaded value is the next store's input, so the processor cannot overlap
// the round trips.

uint64_t lat_reg(uint64_t x, int n);
uint64_t lat_same(unsigned char *p, uint64_t x, int n);
uint64_t lat_low32(unsigned char *p, uint64_t x, int n);
uint64_t lat_hi32(unsigned char *p, uint64_t x, int n);
uint64_t lat_u32(unsigned char *p, uint64_t x, int n);
uint64_t lat_byte(unsigned char *p, uint64_t x, int n);
uint64_t lat_two(unsigned char *p, uint64_t x, int n);
uint64_t lat_overlap4(unsigned char *p, uint64_t x, int n);
uint64_t lat_overlap1(unsigned char *p, uint64_t x, int n);
uint64_t lat_plus64(unsigned char *p, uint64_t x, int n);
uint64_t lat_plus4k(unsigned char *p, uint64_t x, int n);

// Throughput loops. flip_byte reloads a word after a byte store.
// flip_word updates the word in a register. join_mem writes two 32-bit
// values into one scratch buffer and reloads them. join_reg shifts them
// together and never stores.
uint64_t thr_flip_byte(uint64_t *a, size_t n);
uint64_t thr_flip_word(uint64_t *a, size_t n);
uint64_t thr_join_mem(unsigned char *tmp, const uint32_t *a, const uint32_t *b,
                      size_t n);
uint64_t thr_join_reg(const uint32_t *a, const uint32_t *b, size_t n);

#ifdef __cplusplus
}
#endif

#endif
