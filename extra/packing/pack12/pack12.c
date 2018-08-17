

#include "benchmark.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for comparison purposes
uint32_t get8(size_t index, uint8_t *data) { return data[index]; }

// gets a 12-bit value at index from packed 12-bit values in data
uint32_t get(size_t index, uint8_t *data) {
  size_t bitPos = index * 12;
  size_t firstBytePos = bitPos >> 3;
  uint32_t word;
  memcpy(&word, data + firstBytePos, sizeof(uint32_t));
  return (word >> (bitPos & 7)) & 0xFFF;
}

// sets a 12-bit value at index to packed 12-bit values datal, val should be a
// 12-bit value
void set(size_t index, uint32_t val, uint8_t *data) {
  size_t wordpos = (index >> 1) * 3;
  unsigned int wp = (index & 1) * 12;
  size_t offval = val << wp;
  size_t offmask = 0xFFF << wp;
  uint32_t word;
  memcpy(&word, data + wordpos, sizeof(uint32_t));
  word = (word & ~(offmask)) | offval;
  memcpy(data + wordpos, &word, sizeof(uint32_t));
}

uint32_t getthomas(size_t index, uint8_t *data) {
  size_t bitPos = index * 12;
  size_t firstBytePos = (size_t)(bitPos >> 3);
  uint32_t word = __builtin_bswap32(*((uint32_t *)(data + firstBytePos))) >> 8;
  return (uint32_t)((word >> (24 - 12 - (bitPos & 7))) & 0xFFF);
}

void setthomas(size_t index, uint32_t value, uint8_t *data) {
  size_t bitPos = index * 12;
  size_t firstBytePos = (size_t)(bitPos >> 3);
  uint32_t word = __builtin_bswap32(*((uint32_t *)(data + firstBytePos))) >> 8;
  word &= ~(0xFFF << (24 - 12 - (bitPos & 7)));
  word |= ((value & 0xFFF) << (24 - 12 - (bitPos & 7)));
  data[firstBytePos] = (uint8_t)(word >> 16);
  data[firstBytePos + 1] = (uint8_t)(word >> 8);
  data[firstBytePos + 2] = (uint8_t)word;
}

__attribute__((optimize("no-tree-vectorize"))) uint32_t sum8(uint8_t *data,
                                                             size_t number) {
  uint32_t s = 0xFFF;
  for (size_t i = 0; i < number; i++)
    s += get8(i, data);
  return s;
}

__attribute__((optimize("no-tree-vectorize"))) uint32_t sum(uint8_t *data,
                                                            size_t number) {
  uint32_t s = 0xFFF;
  for (size_t i = 0; i < number; i++)
    s += get(i, data);
  return s;
}

__attribute__((optimize("no-tree-vectorize"))) uint32_t
sumthomas(uint8_t *data, size_t number) {
  uint32_t s = 0xFFF;
  for (size_t i = 0; i < number; i++)
    s += getthomas(i, data);
  return s;
}

__attribute__((optimize("no-tree-vectorize"))) bool populate(uint8_t *data,
                                                             size_t number) {
  for (size_t i = 0; i < number; i++)
    set(i, 0xFFF - (i & 0xFFF), data);
  return true;
}
__attribute__((optimize("no-tree-vectorize"))) bool
populatethomas(uint8_t *data, size_t number) {
  for (size_t i = 0; i < number; i++)
    setthomas(i, 0xFFF - (i & 0xFFF), data);
  return true;
}

void validate(size_t number) {
  uint8_t *data =
      (uint8_t *)malloc(number + (number + 1) / 2 + sizeof(uint32_t));
  for (size_t i = 0; i < number; i++)
    set(i, 0xFFF - (i & 0xFFF), data);

  for (size_t i = 0; i < number; i++) {
    set(i, 0xFFF, data);
    assert(get(i, data) == 0xFFF);

    set(i, 0xFFF, data);
    assert(get(i, data) == 0xFFF);

    set(i, 0, data);
    assert(get(i, data) == 0);

    set(i, 1, data);
    assert(get(i, data) == 1);
  }
  free(data);
}

void demo(size_t number) {
  number = number;
  uint8_t *data =
      (uint8_t *)malloc(number + (number + 1) / 2 + sizeof(uint32_t));
  for (size_t i = 0; i < number; i++) {
    uint32_t val = 0xFFF - (i & 0xFFF);
    set(i, val, data);
  }
  int repeat = 50;
  uint32_t expected8 = sum8(data, number);
  BEST_TIME(sum8(data, number), expected8, , repeat, number, number, true);
  uint32_t expected = sum(data, number);
  BEST_TIME(sum(data, number), expected, , repeat, number, number, true);
  uint32_t expectedthomas = sumthomas(data, number);
  BEST_TIME(sumthomas(data, number), expectedthomas, , repeat, number, number,
            true);

  BEST_TIME(populate(data, number), true, , repeat, number, number, true);
  BEST_TIME(populatethomas(data, number), true, , repeat, number, number, true);

  free(data);
}

int main() {
  validate(1024);

  demo(1024 * 1024 * 16);

  return EXIT_SUCCESS;
}
