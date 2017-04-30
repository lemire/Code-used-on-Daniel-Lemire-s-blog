// gcc -O3 -o simdbinsearch16 simdbinsearch16.c -march=native
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <stdint.h>
#include <math.h>
#include <x86intrin.h>

typedef uint16_t value_t;

// good old bin. search
int32_t __attribute__((noinline))
binary_search(uint16_t *array, int32_t lenarray, uint16_t ikey) {
  int32_t low = 0;
  int32_t high = lenarray - 1;
  while (low <= high) {
    int32_t middleIndex = (low + high) >> 1;
    int32_t middleValue = array[middleIndex];
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
int32_t __attribute__ ((noinline)) branchless_binary_search(uint16_t* source, int32_t n, uint16_t target) {
    uint16_t * base = source;
    if(n == 0) return -1;
    if(target > source[n-1]) return - n - 1;// without this we have a buffer overrun
    while(n>1) {
        int32_t half = n >> 1;
        base = (base[half] < target) ? &base[half] : base;
        n -= half;
    }
    base += *base < target;
    return *base == target ? base - source : source - base -1;
}

static inline int32_t 
inline_binary_search(uint16_t *array, int32_t lenarray, uint16_t ikey) {
  int32_t low = 0;
  int32_t high = lenarray - 1;
  while (low <= high) {
    int32_t middleIndex = (low + high) >> 1;
    int32_t middleValue = array[middleIndex];
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


int32_t __attribute__((noinline))
linear(uint16_t *array, int32_t lenarray, uint16_t ikey) {
  int32_t low = 0;
  for (; low < lenarray; ++low) {
    uint16_t val = array[low];
    if (val >= ikey) {
      if (val == ikey)
        return low;
      else
        return -(low + 1);
    }
  }
  return -(lenarray + 1);
}

void print16(__m256i x) {
  uint16_t buffer[16];
  _mm256_storeu_si256((__m256i *)buffer, x);
  for (int k = 0; k < 16; ++k)
    printf("%d ", buffer[k]);
  printf("\n");
}

// attempts to find where data and vkey agree, returns -1 if this never happens
// otherwise it returns the index where they agree (it assumed that it will only
// happen at one index)
static inline int locateEqual(__m256i data, __m256i vkey) {
  __m256i matches = _mm256_cmpeq_epi16(data, vkey);
  if (!_mm256_testz_si256(matches, matches)) {
    int M = _mm256_movemask_epi8(matches);
    return _tzcnt_u32(M) / 2;
  } else {
    //return -1 ;
    // no key is equal // we could bypass this extra work
    __m256i subs = _mm256_subs_epu16(vkey, data);
    __m256i diff = _mm256_cmpeq_epi16(subs, _mm256_setzero_si256());
    int M = _mm256_movemask_epi8(diff);
    if(M == 0) return - (int) sizeof(__m256i)/(int) sizeof(uint16_t) - 1;
    return - (_tzcnt_u32(M) / 2) - 1;
  }
}

// mostly branchless search within an array of 128 uint16_t values.
// if the value is present, this returns its location, otherwise it returns a
// negative value.
static inline int searchIn128(uint16_t *array, uint16_t ikey) {
  __m256i vkey = _mm256_set1_epi16(ikey);
  __m256i every32bytes = _mm256_i32gather_epi32(
      (const int *) array, _mm256_set_epi32(8 * 32 - 4, 7 * 32 - 4, 6 * 32 - 4, 5 * 32 - 4,
                              4 * 32 - 4, 3 * 32 - 4, 2 * 32 - 4, 32 - 4),
      1);
  __m256i subs = _mm256_subs_epu16(vkey, every32bytes);
  __m256i diff = _mm256_cmpeq_epi16(subs, _mm256_setzero_si256());
  int M = _mm256_movemask_epi8(diff);
  int adv = _tzcnt_u32(M) / 4;
  int withinblock =
      locateEqual(_mm256_loadu_si256((const __m256i *)array + adv), vkey);
  if (withinblock < 0) {
    return withinblock - adv * sizeof(__m256i) / sizeof(uint16_t);
  }
  return adv * sizeof(__m256i) / sizeof(uint16_t) + withinblock;
}

// mostly branchless search within an array of 1024 uint16_t values.
// if the value is present, this returns its location, otherwise it returns a
// negative value
static inline int searchIn1024(uint16_t *array, uint16_t ikey) {
  __m256i vkey = _mm256_set1_epi16(ikey);
  __m256i every32bytes = _mm256_i32gather_epi32(
      (const int *)array, _mm256_set_epi32(8 * 128 * 2 - 4, 7 * 128 * 2 - 4, 6 * 128 * 2 - 4,
                              5 * 128 * 2 - 4, 4 * 128 * 2 - 4, 3 * 128 * 2 - 4,
                              2 * 128 * 2 - 4, 128 * 2 - 4),
      1);
  __m256i subs = _mm256_subs_epu16(vkey, every32bytes);
  __m256i diff = _mm256_cmpeq_epi16(subs, _mm256_setzero_si256());
  int M = _mm256_movemask_epi8(diff);
  if(M == 0) return -1;
  int adv = _tzcnt_u32(M) / 4;
  int r = searchIn128(array + adv * 128, ikey);
  if( r < 0) {
    return r - adv * 128;
  }
  return r + adv * 128;
}

static inline int32_t shortavx_binary_search(uint16_t *array, int32_t lenarray, uint16_t ikey) {
  int32_t low = 0;
  int32_t high = lenarray - 1;
  while (low + 16 <= high) {
    int32_t middleIndex = (low + high) >> 1;
    int32_t middleValue = array[middleIndex];
    if (middleValue < ikey) {
      low = middleIndex + 1;
    } else if (middleValue > ikey) {
      high = middleIndex - 1;
    } else {
      return middleIndex;
    }
  }
  // here we have low + 16 > high
  // or high - low < 16
   // or high - low + 1 <= 16
  __m256i vkey = _mm256_set1_epi16(ikey);
  __m256i data;
  if(high-low+1 == 16) {
    data =_mm256_loadu_si256((const __m256i *)(array + low));
  } else {
    uint16_t buffer[16];
    memset(buffer,0xFF,16*sizeof(uint16_t));
    memcpy(buffer,array+low,(high-low+1)*sizeof(uint16_t));
    data =_mm256_loadu_si256((const __m256i *)buffer);
  }
  int withinblock =
      locateEqual(data, vkey);
  if (withinblock < 0) {
    return withinblock - low;
  }
  return low + withinblock;
}


int32_t __attribute__((noinline))
avx_binary_search(uint16_t *array, int32_t lenarray, uint16_t ikey) {
   int offset = 0;
  if (lenarray >= 1024) {
    int k = 1024 - 1;
    for (; k < lenarray; k += 1024) {
       if (array[k] >= ikey) {
        int r = searchIn1024(array + k + 1 - 1024, ikey);
        if (r < 0)
          return r - (k + 1 - 1024);
        return r + k + 1 - 1024;
      }
    }
    offset = lenarray - (lenarray % 1024);
  }
  if (lenarray - offset >= 128) {
    int k = 128 - 1;
    for (; k < lenarray - offset ; k += 128) {
      if (array[k + offset] >= ikey) {
        int startpoint = k + 1 - 128 + offset;
        int r = searchIn128(array + startpoint, ikey);
        if (r < 0) {
          return r - (startpoint);
        }
        return r + startpoint;
      }
    }
    offset = lenarray - (lenarray % 128);
  }
  // here bin search should be over less than 128
  int r = inline_binary_search(array + offset, lenarray-offset, ikey);
  if( r < 0) return r - offset;
  return r + offset;
}

int32_t __attribute__((noinline))
fullavx_binary_search(uint16_t *array, int32_t lenarray, uint16_t ikey) {
   int offset = 0;
  if (lenarray >= 1024) {
    int k = 1024 - 1;
    for (; k < lenarray; k += 1024) {
       if (array[k] >= ikey) {
        int r = searchIn1024(array + k + 1 - 1024, ikey);
        if (r < 0)
          return r - (k + 1 - 1024);
        return r + k + 1 - 1024;
      }
    }
    offset = lenarray - (lenarray % 1024);
  }
  if (lenarray - offset >= 128) {
    int k = 128 - 1;
    for (; k < lenarray - offset ; k += 128) {
      if (array[k + offset] >= ikey) {
        int startpoint = k + 1 - 128 + offset;
        int r = searchIn128(array + startpoint, ikey);
        if (r < 0) {
          return r - (startpoint);
        }
        return r + startpoint;
      }
    }
    offset = lenarray - (lenarray % 128);
  }
  // here bin search should be over less than 128
  int r = shortavx_binary_search(array + offset, lenarray-offset, ikey);
  if( r < 0) return r - offset;
  return r + offset;
}

struct pcg_state_setseq_64 { // Internals are *Private*.
  uint64_t state;            // RNG state.  All values are possible.
  uint64_t inc;              // Controls which RNG sequence (stream) is
                             // selected. Must *always* be odd.
};
typedef struct pcg_state_setseq_64 pcg32_random_t;

static pcg32_random_t pcg32_global = {0x853c49e6748fea9bULL,
                                      0xda3e39cb94b95bdbULL};

static inline uint32_t pcg32_random_r(pcg32_random_t *rng) {
  uint64_t oldstate = rng->state;
  rng->state = oldstate * 6364136223846793005ULL + rng->inc;
  uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
  uint32_t rot = oldstate >> 59u;
  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static inline uint32_t pcg32_random() { return pcg32_random_r(&pcg32_global); }
int qsort_compare_value_t(const void *a, const void *b) {
  return (*(value_t *)a - *(value_t *)b);
}
value_t *create_sorted_array(size_t length) {
  value_t *array = malloc(length * sizeof(value_t));
  for (size_t i = 0; i < length; i++)
    array[i] = (value_t)pcg32_random();
  qsort(array, length, sizeof(*array), qsort_compare_value_t);
  return array;
}

value_t *create_random_array(size_t count) {
  value_t *targets = malloc(count * sizeof(value_t));
  for (size_t i = 0; i < count; i++)
    targets[i] = (value_t)pcg32_random();
  return targets;
}

// flushes the array from cache
void array_cache_flush(value_t *B, int32_t length) {
  const int32_t CACHELINESIZE = 64; // 64 bytes per cache line
  for (int32_t k = 0; k < length; k += CACHELINESIZE / sizeof(value_t)) {
    __builtin_ia32_clflush(B + k);
  }
}

// tries to put the array in cache
void array_cache_prefetch(value_t *B, int32_t length) {
  const int32_t CACHELINESIZE = 64; // 64 bytes per cache line
  for (int32_t k = 0; k < length; k += CACHELINESIZE / sizeof(value_t)) {
    __builtin_prefetch(B + k);
  }
}

#define RDTSC_START(cycles)                                                    \
  do {                                                                         \
    register unsigned cyc_high, cyc_low;                                       \
    __asm volatile("cpuid\n\t"                                                 \
                   "rdtsc\n\t"                                                 \
                   "mov %%edx, %0\n\t"                                         \
                   "mov %%eax, %1\n\t"                                         \
                   : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx",    \
                     "%rdx");                                                  \
    (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                           \
  } while (0)

#define RDTSC_FINAL(cycles)                                                    \
  do {                                                                         \
    register unsigned cyc_high, cyc_low;                                       \
    __asm volatile("rdtscp\n\t"                                                \
                   "mov %%edx, %0\n\t"                                         \
                   "mov %%eax, %1\n\t"                                         \
                   "cpuid\n\t"                                                 \
                   : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx",    \
                     "%rdx");                                                  \
    (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                           \
  } while (0)

/*
 * This is like BEST_TIME except that ... it runs functions "test" using the
 * first parameter "base" and various parameters from "testvalues" (there
 * are nbrtestvalues), calling pre on base between tests
 */
#define BEST_TIME_PRE_ARRAY(base, length, test, pre, testvalues,               \
                            nbrtestvalues, bogus)                              \
  do {                                                                         \
    printf("%-60s\t: ", #test);                                                \
    fflush(NULL);                                                              \
    uint64_t cycles_start, cycles_final, cycles_diff;                          \
    uint64_t min_diff = (uint64_t)-1;                                          \
    int sum = 0;                                                               \
    for (size_t j = 0; j < nbrtestvalues; j++) {                               \
      pre(base, length);                                                       \
      pre(base, length);                                                       \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      RDTSC_START(cycles_start);                                               \
      bogus += test(base, length, testvalues[j]);                              \
      RDTSC_FINAL(cycles_final);                                               \
      cycles_diff = (cycles_final - cycles_start);                             \
      if (cycles_diff < min_diff)                                              \
        min_diff = cycles_diff;                                                \
      sum += cycles_diff;                                                      \
    }                                                                          \
    uint64_t S = nbrtestvalues;                                                \
    double cycle_per_op = sum / (double)S;                                     \
    printf("%f \n", cycle_per_op);                                             \
  } while (0)

#define ASSERT_PRE_ARRAY(base, length, test, testvalues, nbrtestvalues)        \
  do {                                                                         \
    int error = 0;                                                             \
    for (size_t j = 0; j < nbrtestvalues; j++) {                               \
      int32_t re = test(base, length, testvalues[j]);                          \
      if (re >= 0) {                                                           \
        if (base[re] != testvalues[j]) {                                       \
          error = 1;   assert(false);                                                        \
          break;                                                               \
        }                                                                      \
      } else {                                                                 \
        int32_t ip = -re - 1;                                                  \
        if (ip < (int32_t)length) {                                            \
          if (base[ip] <= testvalues[j]) {                                     \
            printf(" Index returned is %d which points at %d for target %d. ", \
                   ip, base[ip], testvalues[j]);                               \
            error = 3;assert(false);                                                         \
            break;                                                             \
          }                                                                    \
        }                                                                      \
        if (ip > 0) {                                                          \
          if (base[ip - 1] >= testvalues[j]) {                                 \
            printf(" Index returned is %d which points at %d for target %d. ", \
                   ip, base[ip], testvalues[j]);                               \
            printf(" Previous index is %d which points at %d for target %d. ", \
                   ip - 1, base[ip - 1], testvalues[j]);                       \
            error = 4;       assert(false);                                                  \
            break;                                                             \
          }                                                                    \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    if (error != 0)                                                            \
      printf("[%s error: %d]\n", #test, error);                                \
  } while (0)

#define ASSERT_PRE_ARRAY_POSITIVE(base, length, test, testvalues,              \
                                  nbrtestvalues)                               \
  do {                                                                         \
    int error = 0;                                                             \
    for (size_t j = 0; j < nbrtestvalues; j++) {                               \
      int32_t re = test(base, length, testvalues[j]);                          \
      if (re >= 0) {                                                           \
        if (base[re] != testvalues[j]) {                                       \
          error = 1;                                                           \
          break;                                                               \
        }                                                                      \
      } else {                                                                 \
        if (linear(base, length, testvalues[j]) >= 0) {                        \
          error = 2;                                                           \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    if (error != 0)                                                            \
      printf("[%s error: %d]\n", #test, error);                                \
  } while (0)

void demo() {
  size_t nbrtestvalues = 10000;
  value_t *testvalues = create_random_array(nbrtestvalues);
  printf("# N, prefetched seek, fresh seek  (in cycles) then same values "
         "normalized by tree height\n");
  int32_t bogus = 0;
  for (int Nd = 16; Nd <= 4096; Nd += rand() % Nd) { // sqrt(2)
    size_t N = Nd;
    printf("N = %zu \n", N);
    value_t *source = create_sorted_array(N);

    ASSERT_PRE_ARRAY(source, N, linear, testvalues, nbrtestvalues);
    ASSERT_PRE_ARRAY(source, N, binary_search, testvalues, nbrtestvalues);
    ASSERT_PRE_ARRAY(source, N, avx_binary_search, testvalues,
                              nbrtestvalues);
    ASSERT_PRE_ARRAY(source, N, fullavx_binary_search, testvalues,
                              nbrtestvalues);
    ASSERT_PRE_ARRAY(source, N, shortavx_binary_search, testvalues,
                              nbrtestvalues);
    printf("in cache\n");
    BEST_TIME_PRE_ARRAY(source, N, linear, array_cache_prefetch, testvalues,
                        nbrtestvalues, bogus);
    BEST_TIME_PRE_ARRAY(source, N, branchless_binary_search, array_cache_prefetch,
                        testvalues, nbrtestvalues, bogus);
    BEST_TIME_PRE_ARRAY(source, N, binary_search, array_cache_prefetch,
                        testvalues, nbrtestvalues, bogus);
    BEST_TIME_PRE_ARRAY(source, N, avx_binary_search, array_cache_prefetch,
                        testvalues, nbrtestvalues, bogus);
  //  BEST_TIME_PRE_ARRAY(source, N, fullavx_binary_search, array_cache_prefetch,
  //                      testvalues, nbrtestvalues, bogus);
  //  BEST_TIME_PRE_ARRAY(source, N, shortavx_binary_search, array_cache_prefetch,
  //                      testvalues, nbrtestvalues, bogus);
    printf("out-of-cache cache\n");
    BEST_TIME_PRE_ARRAY(source, N, linear, array_cache_flush, testvalues,
                        nbrtestvalues, bogus);
    BEST_TIME_PRE_ARRAY(source, N, branchless_binary_search, array_cache_flush,
                        testvalues, nbrtestvalues, bogus);
    BEST_TIME_PRE_ARRAY(source, N, binary_search, array_cache_flush,
                        testvalues, nbrtestvalues, bogus);
    BEST_TIME_PRE_ARRAY(source, N, avx_binary_search, array_cache_flush,
                        testvalues, nbrtestvalues, bogus);
     free(source);
    printf("\n");
  }
  printf("bogus = %d \n", bogus);
  free(testvalues);
}
int main() {
  demo();
  return 0;
}
