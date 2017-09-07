// CC -O3 -o groupedgalloping groupedgalloping.c

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "benchmark.h"
/*
 *  good old binary search
 */
inline int32_t binarySearch(const uint16_t *array, int32_t lenarray,
                            uint16_t ikey) {
  int32_t low = 0;
  int32_t high = lenarray - 1;
  while (low <= high) {
    int32_t middleIndex = (low + high) >> 1;
    uint16_t middleValue = array[middleIndex];
    if (middleValue < ikey) {
      low = middleIndex + 1;
    } else if (middleValue > ikey) {
      high = middleIndex - 1;
    } else {
      return middleIndex;
    }
  }
  return -(low + 1);
}

/**
 * Galloping search
 */
static inline int32_t advanceUntil(const uint16_t *array, int32_t pos,
                                   int32_t length, uint16_t min) {
  int32_t lower = pos + 1;

  if ((lower >= length) || (array[lower] >= min)) {
    return lower;
  }

  int32_t spansize = 1;

  while ((lower + spansize < length) && (array[lower + spansize] < min)) {
    spansize <<= 1;
  }
  int32_t upper = (lower + spansize < length) ? lower + spansize : length - 1;

  if (array[upper] == min) {
    return upper;
  }
  if (array[upper] < min) {
    // means
    // array
    // has no
    // item
    // >= min
    // pos = array.length;
    return length;
  }

  // we know that the next-smallest span was too small
  lower += (spansize >> 1);

  int32_t mid = 0;
  while (lower + 1 != upper) {
    mid = (lower + upper) >> 1;
    if (array[mid] == min) {
      return mid;
    } else if (array[mid] < min) {
      lower = mid;
    } else {
      upper = mid;
    }
  }
  return upper;
}

/* Computes the intersection between one small and one large set of uint16_t.
 * Stores the result into buffer and return the number of elements. */
int32_t intersect_skewed_uint16(const uint16_t *small, size_t size_s,
                                const uint16_t *large, size_t size_l,
                                uint16_t *buffer) {
  size_t pos = 0, idx_l = 0, idx_s = 0;

  if (0 == size_s) {
    return 0;
  }

  uint16_t val_l = large[idx_l], val_s = small[idx_s];

  while (true) {
    if (val_l < val_s) {
      idx_l = advanceUntil(large, (int32_t)idx_l, (int32_t)size_l, val_s);
      if (idx_l == size_l)
        break;
      val_l = large[idx_l];
    } else if (val_s < val_l) {
      idx_s++;
      if (idx_s == size_s)
        break;
      val_s = small[idx_s];
    } else {
      buffer[pos++] = val_s;
      idx_s++;
      if (idx_s == size_s)
        break;
      val_s = small[idx_s];
      idx_l = advanceUntil(large, (int32_t)idx_l, (int32_t)size_l, val_s);
      if (idx_l == size_l)
        break;
      val_l = large[idx_l];
    }
  }

  return (int32_t)pos;
}

void binarySearch4(const uint16_t *array, int32_t n, uint16_t target1,
                   uint16_t target2, uint16_t target3, uint16_t target4,
                   int32_t *index1, int32_t *index2, int32_t *index3,
                   int32_t *index4) {
  const uint16_t *base1 = array;
  const uint16_t *base2 = array;
  const uint16_t *base3 = array;
  const uint16_t *base4 = array;
  if (n == 0)
    return;
  while (n > 1) {
    int32_t half = n >> 1;
    base1 = (base1[half] < target1) ? &base1[half] : base1;
    base2 = (base2[half] < target2) ? &base2[half] : base2;
    base3 = (base3[half] < target3) ? &base3[half] : base3;
    base4 = (base4[half] < target4) ? &base4[half] : base4;
    n -= half;
  }
  *index1 = (*base1 < target1) + base1 - array;
  *index2 = (*base2 < target2) + base2 - array;
  *index3 = (*base3 < target3) + base3 - array;
  *index4 = (*base4 < target4) + base4 - array;
}

void binarySearch2(const uint16_t *array, int32_t n, uint16_t target1,
                   uint16_t target2, int32_t *index1, int32_t *index2) {
  const uint16_t *base1 = array;
  const uint16_t *base2 = array;
  if (n == 0)
    return;
  while (n > 1) {
    int32_t half = n >> 1;
    base1 = (base1[half] < target1) ? &base1[half] : base1;
    base2 = (base2[half] < target2) ? &base2[half] : base2;
    n -= half;
  }
  *index1 = (*base1 < target1) + base1 - array;
  *index2 = (*base2 < target2) + base2 - array;
}

/* Computes the intersection between one small and one large set of uint16_t.
 * Stores the result into buffer and return the number of elements.
 * Processes the small set in blocks of 4 values.
 */
int32_t batched_intersect_skewed_uint16(const uint16_t *small, size_t size_s,
                                        const uint16_t *large, size_t size_l,
                                        uint16_t *buffer) {
  size_t pos = 0, idx_l = 0, idx_s = 0;

  if (0 == size_s) {
    return 0;
  }

  while ((idx_s + 4 <= size_s) && (idx_l < size_l)) {
    int32_t index1, index2, index3, index4;
    uint16_t target1 = small[idx_s];
    uint16_t target2 = small[idx_s + 1];
    uint16_t target3 = small[idx_s + 2];
    uint16_t target4 = small[idx_s + 3];
    binarySearch4(large + idx_l, size_l - idx_l, target1, target2, target3,
                  target4, &index1, &index2, &index3, &index4);
    if ((index1 + idx_l < size_l) && (large[idx_l + index1] == target1)) {
      buffer[pos++] = target1;
    }
    if ((index2 + idx_l < size_l) && (large[idx_l + index2] == target2)) {
      buffer[pos++] = target2;
    }
    if ((index3 + idx_l < size_l) && (large[idx_l + index3] == target3)) {
      buffer[pos++] = target3;
    }
    if ((index4 + idx_l < size_l) && (large[idx_l + index4] == target4)) {
      buffer[pos++] = target4;
    }

    idx_s += 4;
    idx_l += index1;
  }
  /*  if((idx_s + 2 <= size_s) && (idx_l < size_l)){
      int32_t index1, index2;
      uint16_t target1 = small[idx_s];
      uint16_t target2 = small[idx_s + 1];
      binarySearch2(large + idx_l, size_l - idx_l,
        target1,  target2,
        &index1, &index2);
      if((index1 + idx_l < size_l) && (large[idx_l + index1] == target1)) {
            buffer[pos++] = target1;
      }
      if((index2 + idx_l < size_l) && (large[idx_l + index2] == target2)) {
            buffer[pos++] = target2;
      }

      idx_s += 1;
      idx_l += index1;
}*/

  if (idx_s < size_s) {
    uint16_t val_l = large[idx_l], val_s = small[idx_s];
    while (true) {
      if (val_l < val_s) {
        idx_l = advanceUntil(large, (int32_t)idx_l, (int32_t)size_l, val_s);
        if (idx_l == size_l)
          break;
        val_l = large[idx_l];
      } else if (val_s < val_l) {
        idx_s++;
        if (idx_s == size_s)
          break;
        val_s = small[idx_s];
      } else {
        buffer[pos++] = val_s;
        idx_s++;
        if (idx_s == size_s)
          break;
        val_s = small[idx_s];
        idx_l = advanceUntil(large, (int32_t)idx_l, (int32_t)size_l, val_s);
        if (idx_l == size_l)
          break;
        val_l = large[idx_l];
      }
    }
  }
  return (int32_t)pos;
}
int32_t batched2_intersect_skewed_uint16(const uint16_t *small, size_t size_s,
                                         const uint16_t *large, size_t size_l,
                                         uint16_t *buffer) {
  size_t pos = 0, idx_l = 0, idx_s = 0;

  if (0 == size_s) {
    return 0;
  }

  while ((idx_s + 4 <= size_s) && (idx_l < size_l)) {
    int32_t index1, index2, index3, index4;
    uint16_t target1 = small[idx_s];
    uint16_t target2 = small[idx_s + 1];
    uint16_t target3 = small[idx_s + 2];
    uint16_t target4 = small[idx_s + 3];
    binarySearch4(large + idx_l, size_l - idx_l, target1, target2, target3,
                  target4, &index1, &index2, &index3, &index4);
    if ((index1 + idx_l < size_l) && (large[idx_l + index1] == target1)) {
      buffer[pos++] = target1;
    }
    if ((index2 + idx_l < size_l) && (large[idx_l + index2] == target2)) {
      buffer[pos++] = target2;
    }
    if ((index3 + idx_l < size_l) && (large[idx_l + index3] == target3)) {
      buffer[pos++] = target3;
    }
    if ((index4 + idx_l < size_l) && (large[idx_l + index4] == target4)) {
      buffer[pos++] = target4;
    }

    idx_s += 4;
    idx_l += index1;
  }
  if ((idx_s + 2 <= size_s) && (idx_l < size_l)) {
    int32_t index1, index2;
    uint16_t target1 = small[idx_s];
    uint16_t target2 = small[idx_s + 1];
    binarySearch2(large + idx_l, size_l - idx_l, target1, target2, &index1,
                  &index2);
    if ((index1 + idx_l < size_l) && (large[idx_l + index1] == target1)) {
      buffer[pos++] = target1;
    }
    if ((index2 + idx_l < size_l) && (large[idx_l + index2] == target2)) {
      buffer[pos++] = target2;
    }

    idx_s += 2;
    idx_l += index1;
  }

  if (idx_s < size_s) {
    uint16_t val_l = large[idx_l], val_s = small[idx_s];
    while (true) {
      if (val_l < val_s) {
        idx_l = advanceUntil(large, (int32_t)idx_l, (int32_t)size_l, val_s);
        if (idx_l == size_l)
          break;
        val_l = large[idx_l];
      } else if (val_s < val_l) {
        idx_s++;
        if (idx_s == size_s)
          break;
        val_s = small[idx_s];
      } else {
        buffer[pos++] = val_s;
        idx_s++;
        if (idx_s == size_s)
          break;
        val_s = small[idx_s];
        idx_l = advanceUntil(large, (int32_t)idx_l, (int32_t)size_l, val_s);
        if (idx_l == size_l)
          break;
        val_l = large[idx_l];
      }
    }
  }
  return (int32_t)pos;
}

/* Computes the intersection between one small and one large set of uint16_t.
 * Stores the result into buffer and return the number of elements.
 * Processes the small set in blocks of 4 values.
 */
int32_t batched3_intersect_skewed_uint16(const uint16_t *small, size_t size_s,
                                         const uint16_t *large, size_t size_l,
                                         uint16_t *buffer) {
  size_t pos = 0, idx_l = 0, idx_s = 0;

  if (0 == size_s) {
    return 0;
  }

  while ((idx_s + 4 <= size_s) && (idx_l < size_l)) {
    int32_t index1, index2, index3, index4;
    uint16_t target1 = small[idx_s];
    uint16_t target2 = small[idx_s + 1];
    uint16_t target3 = small[idx_s + 2];
    uint16_t target4 = small[idx_s + 3];
    binarySearch4(large + idx_l, size_l - idx_l, target1, target2, target3,
                  target4, &index1, &index2, &index3, &index4);
    if ((index1 + idx_l < size_l) && (large[idx_l + index1] == target1)) {
      buffer[pos++] = target1;
    }
    if ((index2 + idx_l < size_l) && (large[idx_l + index2] == target2)) {
      buffer[pos++] = target2;
    }
    if ((index3 + idx_l < size_l) && (large[idx_l + index3] == target3)) {
      buffer[pos++] = target3;
    }
    if ((index4 + idx_l < size_l) && (large[idx_l + index4] == target4)) {
      buffer[pos++] = target4;
    }

    idx_s += 4;
    idx_l += index1;
  }
  if ((idx_s + 2 <= size_s) && (idx_l < size_l)) {
    int32_t index1, index2;
    uint16_t target1 = small[idx_s];
    uint16_t target2 = small[idx_s + 1];
    binarySearch2(large + idx_l, size_l - idx_l, target1, target2, &index1,
                  &index2);
    if ((index1 + idx_l < size_l) && (large[idx_l + index1] == target1)) {
      buffer[pos++] = target1;
    }
    if ((index2 + idx_l < size_l) && (large[idx_l + index2] == target2)) {
      buffer[pos++] = target2;
    }

    idx_s += 2;
    idx_l += index1;
  }

  if ((idx_s < size_s) && (idx_l < size_l)) {
    uint16_t val_s = small[idx_s];
    int32_t index = binarySearch(large + idx_l, size_l - idx_l, val_s);
    if (index >= 0)
      buffer[pos++] = val_s;
  }

  return (int32_t)pos;
}
static int uint16_compare(const void *a, const void *b) {
  return (*(uint16_t *)a - *(uint16_t *)b);
}

void randomtest() {
  const int repeat = 50;
  const int lenA = (rand() % 64);
  const int lenB = (rand() % 4096) + 4096;

  uint16_t *A = (uint16_t *)malloc(lenA * sizeof(uint16_t));
  uint16_t *B = (uint16_t *)malloc(lenB * sizeof(uint16_t));
  if (lenA > 0) {
    A[0] = rand() % 10;
    for (int i = 1; i < lenA; i++)
      A[i] = A[i - 1] + 1 + (rand() % (50000 / lenA));
    qsort(A, lenA, sizeof(uint16_t), uint16_compare);
  }
  if (lenB > 0) {
    B[0] = rand() % 10;
    for (int i = 1; i < lenB; i++)
      B[i] = B[i - 1] + 1 + (rand() % (50000 / lenB));
    qsort(B, lenB, sizeof(uint16_t), uint16_compare);
  }

  uint16_t *buffer1 = (uint16_t *)malloc((lenB + lenA) * sizeof(uint16_t));
  uint16_t *buffer2 = (uint16_t *)malloc((lenB + lenA) * sizeof(uint16_t));

  // first side
  int f1 = intersect_skewed_uint16(A, lenA, B, lenB, buffer1);
  int g1 = batched_intersect_skewed_uint16(A, lenA, B, lenB, buffer2);
  printf("%d %d \n", f1, g1);
  assert(f1 == g1);
  for (int k = 0; k < g1; ++k)
    assert(buffer1[k] == buffer2[k]);

  BEST_TIME(intersect_skewed_uint16(A, lenA, B, lenB, buffer1), f1, , repeat,
            lenA + lenB, true);
  BEST_TIME(batched_intersect_skewed_uint16(A, lenA, B, lenB, buffer2), g1, ,
            repeat, lenA + lenB, true);
  BEST_TIME(batched2_intersect_skewed_uint16(A, lenA, B, lenB, buffer2), g1, ,
            repeat, lenA + lenB, true);
  BEST_TIME(batched3_intersect_skewed_uint16(A, lenA, B, lenB, buffer2), g1, ,
            repeat, lenA + lenB, true);

  // cleaning
  free(A);
  free(B);
  free(buffer1);
  free(buffer2);
}

int main() {
  for (int k = 0; k < 1000; ++k) {
    printf("\n\n\n");
    randomtest();
  }
}
