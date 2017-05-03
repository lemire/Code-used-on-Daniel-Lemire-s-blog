// gcc -O3 -o substring substring.c -march=native
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <stdint.h>
#include <math.h>
#include <x86intrin.h>


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

char *create_random_string(size_t count) {
  char *targets = malloc((count + 1) * sizeof(char));
  for (size_t i = 0; i < count; i++)
    targets[i] = pcg32_random() % 10 +63;
  targets[count] = 0;
  return targets;
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

#define BEST_TIME(test,  refresh, expected,            \
                            repeat, N)                              \
  do {                                                                         \
    printf("%-60s\t: ", #test);                                                \
    fflush(NULL);                                                              \
    uint64_t cycles_start, cycles_final, cycles_diff;                          \
    uint64_t min_diff = (uint64_t)-1;                                          \
    int sum = 0;                                                               \
    for (size_t j = 0; j < repeat; j++) {                               \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      refresh;\
      RDTSC_START(cycles_start);                                               \
      if( test != expected) printf("bug %d %d \n", test,expected);                              \
      RDTSC_FINAL(cycles_final);                                               \
      cycles_diff = (cycles_final - cycles_start);                             \
      if (cycles_diff < min_diff)                                              \
        min_diff = cycles_diff;                                                \
      sum += cycles_diff;                                                      \
    }                                                                          \
    uint64_t S = repeat * N;                                                \
    double cycle_per_op = sum / (double)S;                                     \
    printf("%f \n", cycle_per_op);                                             \
  } while (0)
__m128i tovector(char * c) {
  char buffer[16];
  strncpy(buffer,c,16);
  return _mm_loadu_si128((const __m128i *)buffer);
}

int sse4search(char * hay, int size, char *needle, int needlesize) {
  assert(needlesize <= 16);
  __m128i n = tovector(needle);
  for(int i = 0; i + 15 < size; i+=16) {
    __m128i v = _mm_loadu_si128((const __m128i *)(hay + i));
    __m128 x = _mm_cmpistrm(n,v,_SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ORDERED | _SIDD_BIT_MASK);
    int r = _mm_cvtsi128_si32(x);
    if(r != 0) {
      int offset = __builtin_ctz(r);
                  if (memcmp(hay + i + offset + 1, needle + 1, needlesize - 2) == 0) {
                      return i + offset;
                  }

//      v = _mm_loadu_si128((const __m128i *)(hay + i + offset));
  //    x = _mm_cmpistrm(n,v,_SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ORDERED | _SIDD_BIT_MASK);
    //  r = _mm_cvtsi128_si32(x);
      //if((r & 1 ) != 0) return i + offset;
    }
  }
  return size;
}

int sse4searchp2(char * hay, int size, char *needle, int needlesize) {
  assert(needlesize <= 16);
  __m128i n = tovector(needle);
  for(int i = 0; i + 15 < size; i+=16) {
    __m128i v = _mm_loadu_si128((const __m128i *)(hay + i));
    __m128 x = _mm_cmpistrm(n,v,_SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ORDERED | _SIDD_BIT_MASK);
    int r = _mm_cvtsi128_si32(x);
    if(r != 0) {
      int offset = __builtin_ctz(r);
      v = _mm_loadu_si128((const __m128i *)(hay + i + offset));
      x = _mm_cmpistrm(n,v,_SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ORDERED | _SIDD_BIT_MASK);
      r = _mm_cvtsi128_si32(x);
      if((r & 1 ) != 0) return i + offset;
    }
  }
  return size;
}

// implements scheme described in http://0x80.pl/articles/simd-friendly-karp-rabin.html
size_t  avx2_strstr_anysize(const char* s, size_t n, const char* needle, size_t k) {
    assert(k > 0);
    assert(n > 0);
    const __m256i first = _mm256_set1_epi8(needle[0]);
    const __m256i last  = _mm256_set1_epi8(needle[k - 1]);
    for (size_t i = 0; i < n; i += 32) {

        const __m256i block_first = _mm256_loadu_si256((const __m256i*)(s + i));
        const __m256i block_last  = _mm256_loadu_si256((const __m256i*)(s + i + k - 1));

        const __m256i eq_first = _mm256_cmpeq_epi8(first, block_first);
        const __m256i eq_last  = _mm256_cmpeq_epi8(last, block_last);

        uint32_t mask = _mm256_movemask_epi8(_mm256_and_si256(eq_first, eq_last));

        while (mask != 0) {
            int bitpos = __builtin_ctz(mask);
            // this bit can be optimized as per Mula's code since k is fixed
            if (memcmp(s + i + bitpos + 1, needle + 1, k - 2) == 0) {
                return i + bitpos;
            }
            mask ^= mask & (-mask);
        }
    }

    return n;
}

void demo() {
  const int N = 2048;
  char *hay = create_random_string(N);
  char needle[16 + 1];
  int expected;
  memcpy(needle,hay + (expected = rand() % (N-16)),16);
  needle[16] = 0;
  int32_t bogus = 0;
  const int repeat = 10;
  BEST_TIME(strstr(hay,needle) - hay, ,expected,
                        repeat, N);
  BEST_TIME(sse4search(hay,N,needle,16) , ,expected,
                        repeat, N);
  BEST_TIME(sse4searchp2(hay,N,needle,16) , ,expected,
                        repeat, N);
  BEST_TIME(avx2_strstr_anysize(hay,N,needle,16) , ,expected,
                        repeat, N);
  free(hay);
}
int main() {
  printf("results are randomized from one pass to the other\n");
  for(int k = 0; k < 10; ++k) {
    demo();
    printf("\n");
  }
  return 0;
}
