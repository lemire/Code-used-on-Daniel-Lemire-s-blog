// gcc -O3 -o bitmapdecode bitmapdecode.c  -march=native -lm
#include "benchmark.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

size_t bitmap_decode_supernaive(uint64_t *bitmap, size_t bitmapsize,
                                uint32_t *out) {
  size_t pos = 0;
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    for (int i = 0; i < 64; i++) {
      if ((bitset >> i) & 0x1)
        out[pos++] = p + i;
    }
  }
  return pos;
}

size_t bitmap_decode_naive(uint64_t *bitmap, size_t bitmapsize, uint32_t *out) {
  size_t pos = 0;
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    while (bitset != 0) {
      if (bitset & 0x1) {
        out[pos++] = p;
      }
      bitset >>= 1;
      p += 1;
    }
  }
  return pos;
}

size_t bitmap_decode_ctz(uint64_t *bitmap, size_t bitmapsize, uint32_t *out) {
  size_t pos = 0;
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    while (bitset != 0) {
      uint64_t t = bitset & -bitset;
      int r = __builtin_ctzll(bitset);
      out[pos++] = k * 64 + r;
      bitset ^= t;
    }
  }
  return pos;
}

size_t bitmap_decode_ctz__asm(uint64_t *bitmap, size_t bitmapsize, uint32_t *out) {
  uint32_t* orig_out = out;
  uint64_t index;
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    asm volatile(
        "test %[bitset], %[bitset]          \n"
        "jz 1f                              \n"
        "2:                                 \n"
        "tzcnt %[bitset], %%rax             \n" // int r = __builtin_ctzll(bitset);
        "add $4, %[out]                     \n" // out++
        "add %[k], %%rax                    \n" // r += (k * 64)
        "blsr %[bitset], %[bitset]          \n" // bitset ^= (bitset & -bitset)
        "movl -4(%[out]), %%eax             \n" // *(out - 1) = uint32_t(r)
        "jnz 2b                             \n"
        "1:                                 \n"
        : [out] "+r" (&out[0])
        : [bitset] "r" (bitset)
        , [index] "a" (index) // access to %eax is required
        , [k] "r" (k * 64)
    );
  }
  return out - orig_out;
}

size_t bitmap_decode_block3(uint64_t *bitmap, size_t bitmapsize, uint32_t *out) {
  size_t pos = 0;
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    while (bitset != 0) {
        switch (bitset & 0x7) {
            case 0:
                break;

            case 1:
                out[pos++] = p;
                break;

            case 2:
                out[pos++] = p + 1;
                break;

            case 3:
                out[pos++] = p;
                out[pos++] = p + 1;
                break;

            case 4:
                out[pos++] = p + 2;
                break;

            case 5:
                out[pos++] = p;
                out[pos++] = p + 2;
                break;

            case 6:
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                break;

            case 7:
                out[pos++] = p;
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                break;
        } // switch

        bitset >>= 3;
        p += 3;
    }
  }
  return pos;
}

size_t bitmap_decode_block4(uint64_t *bitmap, size_t bitmapsize, uint32_t *out) {
  size_t pos = 0;
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    while (bitset != 0) {
        switch (bitset & 0xf) {
            case 0:
                break;

            case 1:
                out[pos++] = p;
                break;

            case 2:
                out[pos++] = p + 1;
                break;

            case 3:
                out[pos++] = p;
                out[pos++] = p + 1;
                break;

            case 4:
                out[pos++] = p + 2;
                break;

            case 5:
                out[pos++] = p;
                out[pos++] = p + 2;
                break;

            case 6:
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                break;

            case 7:
                out[pos++] = p;
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                break;

            case 8:
                out[pos++] = p + 3;
                break;

            case 9:
                out[pos++] = p;
                out[pos++] = p + 3;
                break;

            case 10:
                out[pos++] = p + 1;
                out[pos++] = p + 3;
                break;

            case 11:
                out[pos++] = p;
                out[pos++] = p + 1;
                out[pos++] = p + 3;
                break;

            case 12:
                out[pos++] = p + 2;
                out[pos++] = p + 3;
                break;

            case 13:
                out[pos++] = p;
                out[pos++] = p + 2;
                out[pos++] = p + 3;
                break;

            case 14:
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                out[pos++] = p + 3;
                break;

            case 15:
                out[pos++] = p;
                out[pos++] = p + 1;
                out[pos++] = p + 2;
                out[pos++] = p + 3;
                break;
        }

        bitset >>= 4;
        p += 4;
    }
  }
  return pos;
}

size_t bitmap_decode_supernaive_callback(uint64_t *bitmap, size_t bitmapsize,
                                         void (*callback)(int)) {
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    for (int i = 0; i < 64; i++) {
      if ((bitset >> i) & 0x1)
        callback(p + i);
    }
  }
  return 0;
}

size_t bitmap_decode_naive_callback(uint64_t *bitmap, size_t bitmapsize,
                                    void (*callback)(int)) {
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    while (bitset != 0) {
      if (bitset & 0x1) {
        callback(p);
      }
      bitset >>= 1;
      p += 1;
    }
  }
  return 0;
}

size_t bitmap_decode_ctz_callpack(uint64_t *bitmap, size_t bitmapsize,
                                  void (*callback)(int)) {
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    while (bitset != 0) {
      uint64_t t = bitset & -bitset;
      int r = __builtin_ctzll(bitset);
      callback(k * 64 + r);
      bitset ^= t;
    }
  }
  return 0;
}

size_t bitmap_decode_block3_callback(uint64_t *bitmap, size_t bitmapsize,
                                    void (*callback)(int)) {
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    while (bitset != 0) {
        switch (bitset & 0x7) {
            case 0:
                break;

            case 1:
                callback(p);
                break;

            case 2:
                callback(p + 1);
                break;

            case 3:
                callback(p);
                callback(p + 1);
                break;

            case 4:
                callback(p + 2);
                break;

            case 5:
                callback(p);
                callback(p + 2);
                break;

            case 6:
                callback(p + 1);
                callback(p + 2);
                break;

            case 7:
                callback(p);
                callback(p + 1);
                callback(p + 2);
                break;
        } // switch

        bitset >>= 3;
        p += 3;
      } // for
  }
  return 0;
}

size_t bitmap_decode_block4_callback(uint64_t *bitmap, size_t bitmapsize,
                                    void (*callback)(int)) {
  uint64_t bitset;
  for (size_t k = 0; k < bitmapsize; ++k) {
    bitset = bitmap[k];
    size_t p = k * 64;
    while (bitset != 0) {
        switch (bitset & 0xf) {
            case 0:
                break;

            case 1:
                callback(p);
                break;

            case 2:
                callback(p + 1);
                break;

            case 3:
                callback(p);
                callback(p + 1);
                break;

            case 4:
                callback(p + 2);
                break;

            case 5:
                callback(p);
                callback(p + 2);
                break;

            case 6:
                callback(p + 1);
                callback(p + 2);
                break;

            case 7:
                callback(p);
                callback(p + 1);
                callback(p + 2);
                break;

            case 8:
                callback(p + 3);
                break;

            case 9:
                callback(p);
                callback(p + 3);
                break;

            case 10:
                callback(p + 1);
                callback(p + 3);
                break;

            case 11:
                callback(p);
                callback(p + 1);
                callback(p + 3);
                break;

            case 12:
                callback(p + 2);
                callback(p + 3);
                break;

            case 13:
                callback(p);
                callback(p + 2);
                callback(p + 3);
                break;

            case 14:
                callback(p + 1);
                callback(p + 2);
                callback(p + 3);
                break;

            case 15:
                callback(p);
                callback(p + 1);
                callback(p + 2);
                callback(p + 3);
                break;
        }

        bitset >>= 4;
        p += 4;
      } // for
  }
  return 0;
}


size_t bitmap_count(uint64_t *bitmap, size_t bitmapcount) {
  uint64_t count = 0;
  for (size_t i = 0; i < bitmapcount; ++i) {
    count += __builtin_popcountll(bitmap[i]);
  }
  return count;
}

int globalcount = 0;
void defaultcallback(int x) { globalcount += x; }

void bitmap_decoding() {
  printf("[bitmap decoding]");
  int repeat = 5;
  size_t N = 1000;
  uint64_t *bitmap = malloc(N * sizeof(uint64_t));
  uint32_t *receiver = malloc((N + 64) * 64 * sizeof(uint32_t));

  for (double ratio = 0.03125; ratio < 2; ratio = ratio * 2) {
    for (size_t k = 0; k < N; ++k) {
      bitmap[k] = 0;
    }
    size_t appbitcount = (size_t)ceil(ratio * N * 64);
    if (appbitcount > N * 64 / 2)
      break;
    size_t ccount = 0;
    while (ccount < appbitcount) {
      int bit = rand() % (N * 64);
      uint64_t bef = bitmap[bit / 64];
      uint64_t aft = bef | (UINT64_C(1) << (bit % 64));
      if (bef != aft)
        ccount++;
      bitmap[bit / 64] = aft;
    }
    size_t bitcount = bitmap_count(bitmap, N);
    assert(bitcount == ccount);

    printf("bitmap density %3.2f  \n", bitcount / (N * sizeof(uint64_t) * 8.0));
    BEST_TIME(bitmap_decode_supernaive(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);
    BEST_TIME(bitmap_decode_naive(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);
    BEST_TIME(bitmap_decode_block3(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);
    BEST_TIME(bitmap_decode_block4(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);
    BEST_TIME(bitmap_decode_ctz(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);
    BEST_TIME(bitmap_decode_ctz__asm(bitmap, N, receiver), bitcount, , repeat,
              bitcount, true);

    BEST_TIME(bitmap_decode_supernaive_callback(bitmap, N, defaultcallback), 0,
              , repeat, bitcount, true);
    BEST_TIME(bitmap_decode_naive_callback(bitmap, N, defaultcallback), 0, ,
              repeat, bitcount, true);
    BEST_TIME(bitmap_decode_block3_callback(bitmap, N, defaultcallback), 0, ,
              repeat, bitcount, true);
    BEST_TIME(bitmap_decode_block4_callback(bitmap, N, defaultcallback), 0, ,
              repeat, bitcount, true);
    BEST_TIME(bitmap_decode_ctz_callpack(bitmap, N, defaultcallback), 0, ,
              repeat, bitcount, true);
  }

  free(bitmap);
  free(receiver);
}

int main() {
  bitmap_decoding();
  return EXIT_SUCCESS;
}
