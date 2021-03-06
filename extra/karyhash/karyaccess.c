/**
* Idea here is to see if k-ary access using gather can be fast...
*
* For the purpose of this test, we hash 64-bit values.
*
* We check k distinct locations and return the location, if any
*/

#include <stdint.h>
#include <stdbool.h>
#include <x86intrin.h>
#include <stdio.h>

enum {K=4};

typedef struct {
  uint64_t * data;
  size_t size;// can be assumed to be a power of two
  uint64_t multiplier[K];
  __m256i vmultiplier;// = _mm256_loadu_si256((__m256i const * )set->multiplier);//
  __m256i sizemask;// _mm256_set1_epi64x(set->size-1);
 
} hashset_t;

// simple mutiply-shift
static inline uint64_t hash(uint64_t multiplier, uint64_t target) {
  return (((__uint128_t)target) * multiplier) >> 64;
}

// simple function that tests whether the hash set contains the value
static inline bool contains(hashset_t * set, uint64_t target) {
  for(int i = 0; i < K; i++) {
    size_t hashlocation = hash(set->multiplier[i], target) & (set->size - 1);
    if(set->data[hashlocation] == target) return true;
  }
  return false;
}

static inline __m256i mulhi_epu64(__m256i x, __m256i y) {
  __m256i x_hi = _mm256_srli_epi64(x, 32);
  __m256i y_hi = _mm256_srli_epi64(y, 32);
//  __m256i mask = _mm256_set1_epi64x(0xFFFFFFFFL);
 // __m256i x_lo = _mm256_and_si256(x, mask);
//  __m256i y_lo = _mm256_and_si256(y, mask);
/// masking is unnecessary because _mm256_mul_epu32 does it for us (for free):
__m256i x_lo = x;
__m256i y_lo = y;
/////////////
  __m256i result = _mm256_mul_epu32(x_lo,y_lo);
  result = _mm256_srli_epi64(result, 32);
  __m256i result1 = _mm256_mul_epu32(x_hi,y_lo);
  __m256i result2 = _mm256_mul_epu32(x_lo,y_hi);
  result = _mm256_add_epi64(result, result1);
  result = _mm256_add_epi64(result, result2);
  result = _mm256_srli_epi64(result, 32);
  __m256i result3 = _mm256_mul_epu32(x_hi,y_hi);
  result = _mm256_add_epi64(result, result3);
  return result;
}

// simple mutiply-shift
static inline __m256i avxhash(__m256i multipliers, __m256i target) {
  return mulhi_epu64(multipliers, target);
}

static inline bool avxcontains(hashset_t * set, uint64_t target) {
  __m256i vtarget = _mm256_set1_epi64x(target);
  __m256i vlocation = _mm256_and_si256(avxhash(vtarget, set->vmultiplier),set->sizemask);
  __m256i svalue = _mm256_i64gather_epi64((const long long int *) set->data,vlocation,8);
  __m256i eq = _mm256_cmpeq_epi64(vtarget,svalue);
  return _mm256_testz_si256(eq,eq) == 0;
}

int expected(hashset_t * H, uint64_t howmany, uint64_t * keys) {
  int sum = 0;
  for(uint64_t k = 0; k < howmany; ++k) {
    if(contains(H,keys[k])) sum++;
  }
  return sum;
}


int __attribute__ ((noinline)) checkthemall(hashset_t * H, uint64_t howmany, uint64_t * keys) {
  int sum = 0;
  for(uint64_t k = 0; k < howmany; ++k) {
    if(contains(H,keys[k])) sum++;
  }
  return sum;
}

int __attribute__ ((noinline)) avxcheckthemall(hashset_t * H, uint64_t howmany, uint64_t * keys) {
  int sum = 0;
  for(uint64_t k = 0; k < howmany; ++k) {
    if(avxcontains(H,keys[k])) sum++;
  }
  return sum;
}


#define RDTSC_START(cycles)                                             \
    do {                                                                \
        uint32_t cyc_high, cyc_low;                                     \
        __asm volatile("cpuid\n"                                        \
                       "rdtsc\n"                                        \
                       "mov %%edx, %0\n"                                \
                       "mov %%eax, %1" :                                \
                       "=r" (cyc_high),                                 \
                       "=r"(cyc_low) :                                  \
                       : /* no read only */                             \
                       "%rax", "%rbx", "%rcx", "%rdx" /* clobbers */    \
                       );                                               \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                \
    } while (0)

#define RDTSC_STOP(cycles)                                              \
    do {                                                                \
        uint32_t cyc_high, cyc_low;                                     \
        __asm volatile("rdtscp\n"                                       \
                       "mov %%edx, %0\n"                                \
                       "mov %%eax, %1\n"                                \
                       "cpuid" :                                        \
                       "=r"(cyc_high),                                  \
                       "=r"(cyc_low) :                                  \
                       /* no read only registers */ :                   \
                       "%rax", "%rbx", "%rcx", "%rdx" /* clobbers */    \
                       );                                               \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                \
    } while (0)

static __attribute__ ((noinline))
uint64_t rdtsc_overhead_func(uint64_t dummy) {
    return dummy;
}

uint64_t global_rdtsc_overhead = (uint64_t) UINT64_MAX;

#define RDTSC_SET_OVERHEAD(test, repeat)			      \
  do {								      \
    uint64_t cycles_start, cycles_final, cycles_diff;		      \
    uint64_t min_diff = UINT64_MAX;				      \
    for (size_t i = 0; i < repeat; i++) {			      \
      __asm volatile("" ::: /* pretend to clobber */ "memory");	      \
      RDTSC_START(cycles_start);				      \
      test;							      \
      RDTSC_STOP(cycles_final);                                       \
      cycles_diff = (cycles_final - cycles_start);		      \
      if (cycles_diff < min_diff) min_diff = cycles_diff;	      \
    }								      \
    global_rdtsc_overhead = min_diff;				      \
    printf("rdtsc_overhead set to %ld\n", global_rdtsc_overhead);     \
  } while (0)							      \


#define RDTSC_BEST(test, answer, reset, repeat, num_ops)			\
  do {									\
    if (global_rdtsc_overhead == UINT64_MAX) {				\
      RDTSC_SET_OVERHEAD(rdtsc_overhead_func(1), repeat);		\
    }									\
    printf("%s: ", #test);						\
    fflush(NULL);							\
    uint64_t cycles_start, cycles_final, cycles_diff;			\
    uint64_t min_diff = UINT64_MAX;					\
    for (size_t i = 0; i < repeat; i++) {				\
      reset;                                                            \
      __asm volatile("" ::: /* pretend to clobber */ "memory");		\
      RDTSC_START(cycles_start);					\
      if(test != answer) printf("bug");				\
      RDTSC_STOP(cycles_final);                                         \
      cycles_diff = (cycles_final - cycles_start);			\
      if (cycles_diff < min_diff) min_diff = cycles_diff;		\
    }									\
    if (min_diff <= global_rdtsc_overhead) min_diff = 0;		\
    else min_diff = min_diff - global_rdtsc_overhead;			\
    float cycles_per_op = min_diff / (double)(num_ops);			\
    printf(" %.2f cycles per operation", cycles_per_op);		\
    printf(" (%ld cycles / %ld ops)", min_diff, (uint64_t) num_ops);	\
    printf("\n");							\
    fflush(NULL);							\
  } while (0)

void cache_flush(hashset_t * H, uint64_t howmany, uint64_t * keys) {
    const size_t CACHELINESIZE =
        64;
    for (size_t k = 0; k <  H->size;
         k += CACHELINESIZE / sizeof(uint64_t)) {
        __builtin_ia32_clflush(H->data + k);
    }
    for (size_t k = 0; k <  howmany;
         k += CACHELINESIZE / sizeof(uint64_t)) {
        __builtin_ia32_clflush(keys + k);
    }
}

uint64_t getrand64() {
    return rand() | ((uint64_t) rand() << 32);
}

int main() {
  hashset_t H;
  for(int k = 0; k < K; ++k) H.multiplier[k] = getrand64();
  H.vmultiplier= _mm256_loadu_si256((__m256i const * )H.multiplier);
  uint64_t howmany = 100;
  uint64_t * keys = malloc(sizeof(uint64_t) * howmany);
  for(uint64_t k = 0; k < howmany; ++k) keys[k] = getrand64();
  for(H.size = 1024; H.size < (UINT64_C(1) << 32) ; H.size *=2) {
    H.sizemask = _mm256_set1_epi64x(H.size-1);
    printf("alloc size  = %f MB \n", H.size * sizeof(uint64_t) / (1024 * 1024.0));
    H.data = calloc(H.size , sizeof(uint64_t));
    for(int j = 0; j < howmany; j += 2) H.data[hash(H.multiplier[0],j) & (H.size - 1)] = j;
    int answer = expected(&H,howmany,keys);
    RDTSC_BEST(checkthemall(&H,howmany,keys), answer, cache_flush(&H,howmany,keys), 50,howmany);
    RDTSC_BEST(avxcheckthemall(&H,howmany,keys), answer, cache_flush(&H,howmany,keys),  50,howmany);
    free(H.data);
  }
  free(keys);
}
