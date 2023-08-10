// simplified version of text-to-wire from simdzone
#include "name_to_dnswire.h"

#include <stdint.h>
#include <x86intrin.h> // update if we need to support Windows.

typedef struct name_block name_block_t;
struct name_block {
  __m128i input;
  uint64_t delimiters;
  uint64_t labels;
};

__attribute__((always_inline))
static inline void copy_name_block(
  name_block_t *block, const char *text, uint8_t *wire)
{
  // Delimiters for strings consisting of a contiguous set of characters
  // 0x00        :       end-of-file : 0b0000_0000
  // 0x20        :             space : 0b0010_0000
  // 0x22        :             quote : 0b0010_0010
  // 0x28        :  left parenthesis : 0b0010_1000
  // 0x29        : right parenthesis : 0b0010_1001
  // 0x09        :               tab : 0b0000_1001
  // 0x0a        :         line feed : 0b0000_1010
  // 0x3b        :         semicolon : 0b0011_1011
  // 0x0d        :   carriage return : 0b0000_1101
  const __m128i delimiters = _mm_setr_epi8(
    0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x28, 0x09, 0x0a, 0x3b, 0x00, 0x0d, 0x00, 0x00);

  const __m128i mask = _mm_setr_epi8(
    -33, -1, -1, -1, -1, -1, -1, -1, -1, -33, -1, -1, -1, -1, -1, -1);

  block->input = _mm_loadu_si128((const __m128i *)text);

  // setup delimiter pattern for comparison
  __m128i pattern = _mm_shuffle_epi8(delimiters, block->input);
  // clear 5th bit (match 0x20 with 0x00, match 0x29 using 0x09)
  const __m128i masked = _mm_and_si128(block->input, _mm_shuffle_epi8(mask, block->input));
  const __m128i matched = _mm_cmpeq_epi8(masked, pattern);
  const __m128i labels = _mm_cmpeq_epi8(block->input, _mm_set1_epi8('.'));

  _mm_storeu_si128((__m128i *)wire, block->input);

  block->delimiters = (uint16_t)_mm_movemask_epi8(matched);
  block->labels = (uint16_t)_mm_movemask_epi8(labels);
}

size_t name_to_dnswire_scalar_labels(const char *src, uint8_t *dst)
{
  name_block_t block = { 0 };
  const char *text = src;
  uint8_t *label = dst, *wire = dst + 1;

  while (1) {
    copy_name_block(&block, text, wire);

    // backslash check stripped for comparison with @lemire's algorithm
    block.labels &= block.delimiters - 1;
    const size_t size = __builtin_ctzll(block.delimiters | (1llu << 16));
    text += size;
    wire += size;

    if (block.labels) {
      uint64_t count = 0, last = 0;
      const uint64_t labels = __builtin_popcountll(block.labels);
      for (uint64_t i = 0; i < labels; i++) {
        const uint64_t length = __builtin_ctzll(block.labels) - last;
        block.labels &= block.labels - 1;
        *label += (uint8_t)length;
        label += *label + 1;
        *label = 0;
        last += length + 1;
      }
      *label += (wire - label) - 1;
    } else {
      *label += (uint8_t)size;
      if (*label > 63)
        return 0;
    }

    if (block.delimiters)
      break;
  }

  return wire - dst;
}
