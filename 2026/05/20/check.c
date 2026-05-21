#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void) {
  for (uint32_t n = 1; n <= 16; ++n) {
    uint32_t max_value = (UINT32_C(1) << n) - 1;
    uint32_t max_product = max_value * max_value;
    uint64_t bit_count = (uint64_t)max_product + 1;
    uint64_t word_count = (bit_count + 63) / 64;
    uint64_t memory_bytes = word_count * sizeof(uint64_t);

    uint64_t *bitset = (uint64_t *)calloc(word_count, sizeof(uint64_t));
    if (bitset == NULL) {
      fprintf(stderr,
              "Allocation failed for n=%u (%llu bytes)\n",
              n,
              (unsigned long long)memory_bytes);
      return 1;
    }

    uint64_t distinct = 0;

    for (uint32_t a = 0; a <= max_value; ++a) {
      for (uint32_t b = 0; b <= max_value; ++b) {
        uint32_t product = a * b;
        uint64_t word_index = product >> 6;
        uint64_t mask = UINT64_C(1) << (product & 63);

        if ((bitset[word_index] & mask) == 0) {
          bitset[word_index] |= mask;
          ++distinct;
        }
      }
    }

    double fraction  = (double)distinct / pow(2, 2*n);

    printf("n=%u bits -> distinct 32-bit products: %llu (%.2f%%)\n",
           n,
           (unsigned long long)distinct,
           fraction * 100);
    free(bitset);
  }

  return 0;
}
