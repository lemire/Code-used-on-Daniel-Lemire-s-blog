
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

struct bitset_s {
  uint64_t *restrict array;
  size_t arraysize;
  size_t capacity;
};

typedef struct bitset_s bitset_t;

/* Create a new bitset able to contain size bits. Return NULL in case of
 * failure. */
bitset_t *bitset_create_with_capacity(size_t size) {
  bitset_t *bitset = NULL;
  /* Allocate the bitset itself. */
  if ((bitset = (bitset_t *)malloc(sizeof(bitset_t))) == NULL) {
    return NULL;
  }
  bitset->arraysize =
      (size + sizeof(uint64_t) * 8 - 1) / (sizeof(uint64_t) * 8);
  bitset->capacity = bitset->arraysize;
  if ((bitset->array =
           (uint64_t *)malloc(sizeof(uint64_t) * bitset->arraysize)) == NULL) {
    free(bitset);
    return NULL;
  }
  memset(bitset->array, 0, sizeof(uint64_t) * bitset->arraysize);
  return bitset;
}

/* Free memory. */
void bitset_free(bitset_t *bitset) {
  free(bitset->array);
  free(bitset);
}

/* Set all bits to zero. */
void bitset_clear(bitset_t *bitset) {
  memset(bitset->array, 0, sizeof(uint64_t) * bitset->arraysize);
}

/* Set the ith bit. Attempts to resize the bitset if needed (may silently fail)
 */
static inline void bitset_set(bitset_t *bitset, size_t i) {
  size_t shiftedi = i >> 6;
  bitset->array[shiftedi] |= ((uint64_t)1) << (i % 64);
}

/* Get the value of the ith bit.  */
/*static inline bool bitset_get(const bitset_t *bitset,  size_t i ) {
  size_t shiftedi = i >> 6;
  return ( bitset->array[shiftedi] & ( ((uint64_t)1) << (i % 64))) != 0 ;
}*/

size_t bitset_count(const bitset_t *bitset) {
  size_t card = 0;
  size_t k = 0;
  // assumes that long long is 8 bytes
  for (; k + 7 < bitset->arraysize; k += 8) {
    card += __builtin_popcountll(bitset->array[k]);
    card += __builtin_popcountll(bitset->array[k + 1]);
    card += __builtin_popcountll(bitset->array[k + 2]);
    card += __builtin_popcountll(bitset->array[k + 3]);
    card += __builtin_popcountll(bitset->array[k + 4]);
    card += __builtin_popcountll(bitset->array[k + 5]);
    card += __builtin_popcountll(bitset->array[k + 6]);
    card += __builtin_popcountll(bitset->array[k + 7]);
  }
  for (; k + 3 < bitset->arraysize; k += 4) {
    card += __builtin_popcountll(bitset->array[k]);
    card += __builtin_popcountll(bitset->array[k + 1]);
    card += __builtin_popcountll(bitset->array[k + 2]);
    card += __builtin_popcountll(bitset->array[k + 3]);
  }
  for (; k < bitset->arraysize; k++) {
    card += __builtin_popcountll(bitset->array[k]);
  }
  return card;
}

uint32_t hash16(uint32_t input, uint32_t key) {
  uint32_t hash = input * key;
  return ((hash >> 16) ^ hash) & 0xFFFF;
}

// measure the error
uint32_t avalanche(uint32_t key, uint32_t input) {
  uint32_t base = hash16(input, key);
  uint32_t sum = 0;
  for (int i = 0; i < 16; i++) {
    uint32_t newbase = hash16(input ^ (1 << i), key);
    int diff = __builtin_popcount(base ^ newbase) - 16 / 2;
    if (diff < 0)
      diff = -diff;
    sum += diff;
  }
  return sum;
}

uint32_t seek_best_avalanche() {
  srand(time(NULL));
  uint32_t bestkey = 0;
  uint64_t bestava = UINT64_MAX;
  for (uint32_t x = 1; x <= 0xFFFF; x++) {
    uint64_t ava = 0;
    if((x % 1024) == 0) {
      printf(".");
      fflush(NULL);
    }
    for (int k = 0; k <= 0xFFFF; k++) {
      ava += avalanche(x, k);
    }
    if (ava < bestava) {
      bestkey = x;
      bestava = ava;
      printf("[found %d with ava. %llu]", bestkey, bestava);

    }
  }
  printf("\n");
  printf("best key avalanche-wise is  %d (%llu) \n", bestkey, bestava);
  return bestkey;
}

uint32_t distinct_count(uint32_t key) {
  bitset_t *b = bitset_create_with_capacity(1 << 16);

  for (uint32_t input = 0; input <= 0xFFFF; input++) {
    bitset_set(b, hash16(input, key));
  }
  size_t distinct = bitset_count(b);
  printf("number of distinct 16-bit values %zu \n", distinct);
  bitset_free(b);
  return distinct;

}

int main() {
  uint32_t bestkey = seek_best_avalanche();
  // best key avalanche-wise is  683 (1005748)
  distinct_count(bestkey);
  // number of distinct 16-bit values 44114
}
