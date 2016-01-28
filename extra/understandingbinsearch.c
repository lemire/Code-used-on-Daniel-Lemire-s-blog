// gcc -mavx2 -mbmi -std=c99 -O3 -o understandingbinsearch understandingbinsearch.c -Wall -Wextra -lm

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <x86intrin.h>

typedef uint16_t value_t;

struct pcg_state_setseq_64 {    // Internals are *Private*.
    uint64_t state;             // RNG state.  All values are possible.
    uint64_t inc;               // Controls which RNG sequence (stream) is
    // selected. Must *always* be odd.
};
typedef struct pcg_state_setseq_64 pcg32_random_t;


static pcg32_random_t pcg32_global = { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL };


static inline uint32_t pcg32_random_r(pcg32_random_t* rng) {
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static inline uint32_t pcg32_random() {
    return pcg32_random_r(&pcg32_global);
}
int qsort_compare_value_t(const void *a, const void *b) {
    return ( *(value_t *)a - *(value_t *)b );
}
value_t *create_sorted_array(size_t length) {
    value_t *array = malloc(length * sizeof(value_t));
    for (size_t i = 0; i < length; i++) array[i] = (value_t) pcg32_random();
    qsort(array, length, sizeof(*array), qsort_compare_value_t);
    return array;
}

value_t *create_random_array(size_t count) {
    value_t *targets = malloc(count * sizeof(value_t));
    for (size_t i = 0; i < count; i++) targets[i] = (value_t) pcg32_random();
    return targets;
}

// flushes the array from cache
void array_cache_flush(value_t* B, int32_t length) {
    const int32_t CACHELINESIZE = 64;// 64 bytes per cache line
    for(int32_t  k = 0; k < length; k += CACHELINESIZE/sizeof(value_t)) {
        __builtin_ia32_clflush(B + k);
    }
}

// tries to put the array in cache
void array_cache_prefetch(value_t* B, int32_t length) {
    const int32_t CACHELINESIZE = 64;// 64 bytes per cache line
    for(int32_t  k = 0; k < length; k += CACHELINESIZE/sizeof(value_t)) {
        __builtin_prefetch(B + k);
    }
}

// could be faster, but we just want it to be correct
int32_t ilog2(size_t lenarray)  {
    int32_t low = 0;
    int32_t high = (int32_t) lenarray - 1;
    int32_t counter = 0;
    while( low <= high) {
        int32_t middleIndex = (low+high) >> 1;
        high = middleIndex - 1;
        ++counter;
    }
    return counter;
}


// good old bin. search
int32_t __attribute__ ((noinline)) binary_search(uint16_t * array, int32_t lenarray, uint16_t ikey )  {
    int32_t low = 0;
    int32_t high = lenarray - 1;
    while( low <= high) {
        int32_t middleIndex = (low+high) >> 1;
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
int32_t __attribute__ ((noinline)) binary_search32(uint16_t * array, int32_t lenarray, uint16_t ikey )  {
    int32_t low = 0;
    int32_t high = lenarray - 1;
    while( low + 32 <= high) {
        int32_t middleIndex = (low+high) >> 1;
        int32_t middleValue = array[middleIndex];
        if (middleValue < ikey) {
            low = middleIndex + 1;
        } else if (middleValue > ikey) {
            high = middleIndex - 1;
        } else {
            return middleIndex;
        }
    }
    for(; low<= high; ++low) {
        uint16_t val = array[low];
        if(val >= ikey) {
            if(val == ikey) return low;
            break;
        }
    }
    return -(low + 1);
}

int32_t __attribute__ ((noinline)) linear(uint16_t * array, int32_t lenarray, uint16_t ikey )  {
    int32_t low = 0;
    for(; low< lenarray; ++low) {
        uint16_t val = array[low];
        if(val >= ikey) {
            if(val == ikey) return low;
            else return -(low + 1);
        }
    }
    return -(lenarray + 1);
}

int32_t __attribute__ ((noinline)) linear128_16(uint16_t * array, int32_t length, uint16_t ikey )  {
  int32_t k = 0;
  for(; k + 127 <= length; k += 128) {
      if(array[k + 127] >= ikey) {
        break;
      }
  }
  for(; k + 15 <= length; k += 16) {
      if(array[k + 15] >= ikey) {
        break;
      }
  }
  for(; k  < length; k ++) {
      uint16_t val = array[k];
      if(val >= ikey) {
          if(val == ikey) return k;
          else return - k - 1;
      }
  }
  return - length - 1;
}

int32_t __attribute__ ((noinline)) linear256_16(uint16_t * array, int32_t length, uint16_t ikey )  {
  int32_t k = 0;
  for(; k + 255 <= length; k += 256) {
      if(array[k + 255] >= ikey) {
        break;
      }
  }
  for(; k + 15 <= length; k += 16) {
      if(array[k + 15] >= ikey) {
        break;
      }
  }
  for(; k  < length; k ++) {
      uint16_t val = array[k];
      if(val >= ikey) {
          if(val == ikey) return k;
          else return - k - 1;
      }
  }
  return - length - 1;
}

int32_t __attribute__ ((noinline)) linear256_32(uint16_t * array, int32_t length, uint16_t ikey )  {
  int32_t k = 0;
  for(; k + 255 <= length; k += 256) {
      if(array[k + 255] >= ikey) {
        break;
      }
  }
  for(; k + 31 <= length; k += 32) {
      if(array[k + 31] >= ikey) {
        break;
      }
  }
  for(; k  < length; k ++) {
      uint16_t val = array[k];
      if(val >= ikey) {
          if(val == ikey) return k;
          else return - k - 1;
      }
  }
  return - length - 1;
}


int32_t __attribute__ ((noinline)) linear16(uint16_t * array, int32_t length, uint16_t ikey )  {
  int32_t k = 0;
  const int32_t delta = 16;
  const int32_t deltaminusone = delta - 1;
  for(; k + delta <= length; k += delta) {
      if(array[k + deltaminusone] >= ikey) {
        break;
      }
  }
  for(; k  < length; k ++) {
      uint16_t val = array[k];
      if(val >= ikey) {
          if(val == ikey) return k;
          else return - k - 1;
      }
  }
  return - length - 1;
}

int32_t __attribute__ ((noinline)) mixed(uint16_t * array, int32_t lenarray, uint16_t ikey )  {
    if(lenarray < 128) return linear (array,lenarray,ikey);
    return binary_search(array,lenarray,ikey);
}

int32_t __attribute__ ((noinline)) mixedhybrid(uint16_t * array, int32_t lenarray, uint16_t ikey )  {
    if(lenarray < 128) return linear (array,lenarray,ikey);
    return binary_search32(array,lenarray,ikey);
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

int32_t __attribute__ ((noinline)) branchless_binary_search_wp(uint16_t* source, int32_t n, uint16_t target) {
    uint16_t * base = source;
    if(n == 0) return -1;
    if(target > source[n-1]) return - n - 1;// without this we have a buffer overrun
    while(n>1) {
        int32_t half = n >> 1;
        __builtin_prefetch(base+(half>>1),0,0);
        __builtin_prefetch(base+half+(half>>1),0,0);
        base = (base[half] < target) ? &base[half] : base;
        n -= half;
    }
    base += *base < target;
    return *base == target ? base - source : source - base -1;
}


int32_t  __attribute__ ((noinline)) does_nothing(uint16_t * array, int32_t length, uint16_t ikey )  {
    (void) array;
    (void) length;
    return ikey;
}

int32_t  __attribute__ ((noinline)) return_middle_value(uint16_t * array, int32_t length, uint16_t ikey )  {
    (void) ikey;
    int32_t low = 0;
    int32_t high = length - 1;
    int32_t middleIndex = (low+high) >> 1;
    int32_t middleValue = array[middleIndex];
    return middleValue;
}

int32_t  __attribute__ ((noinline)) simd_linear_search(uint16_t * array, int32_t length, uint16_t ikey )  {
    uint16_t offset = 1<<15;
    __m256i target = _mm256_set1_epi16(ikey + offset);
    __m256i conv = _mm256_set1_epi16(offset);

    const int32_t delta = sizeof(__m256i) / sizeof(uint16_t);// = 16
    const int32_t deltaminusone = delta  - 1;
    int32_t k = 0;
    for(; k + delta <= length; k += delta) {
        if(array[k + deltaminusone] >= ikey) {// when we found a block of 16 values, we examine it in one go
            __m256i data = _mm256_lddqu_si256((const __m256i *) (array + k));
            data = _mm256_add_epi16(data,conv);
            __m256i m = _mm256_cmpeq_epi16(data,target);
            int32_t bits = _mm256_movemask_epi8(m);
            //todo: check that this is correct
            if(bits != 0) {
                int32_t bit_pos = __builtin_ffs(bits) / 2;
                return k + bit_pos;
            } else {
                m = _mm256_cmpgt_epi16(data,target);
                bits = _mm256_movemask_epi8(m);
                int32_t bit_pos = _tzcnt_u32(bits)  / 2;
                return - k - bit_pos - 1;
            }
        }
    }
    for(; k  < length; k ++) {
        uint16_t val = array[k];
        if(val >= ikey) {
            if(val == ikey) return k;
            else return - k - 1;
        }
    }
    return - length - 1;
}

int32_t  __attribute__ ((noinline)) simd_linear_search32(uint16_t * array, int32_t length, uint16_t ikey )  {
    uint16_t offset = 1<<15;
    __m256i target = _mm256_set1_epi16(ikey + offset);
    __m256i conv = _mm256_set1_epi16(offset);

    const int32_t delta = 2 * sizeof(__m256i) / sizeof(uint16_t);// = 32
    const int32_t deltaminusone = delta  - 1;
    int32_t k = 0;
    for(; k + delta <= length; k += delta) {
        if(array[k + deltaminusone] >= ikey) {// when we found a block of 16 values, we examine it in one go

            __m256i data, m;
            int32_t bits;
            if(array[k + delta/2 - 1] >= ikey) {
            data = _mm256_lddqu_si256((const __m256i *) (array + k));
            data = _mm256_add_epi16(data,conv);
            m = _mm256_cmpeq_epi16(data,target);
            bits = _mm256_movemask_epi8(m);
            if(bits != 0) {
                int32_t bit_pos = __builtin_ffs(bits) / 2;
                return k + bit_pos;
            } else {
                m = _mm256_cmpgt_epi16(data,target);
                bits = _mm256_movemask_epi8(m);
                int32_t bit_pos = _tzcnt_u32(bits)  / 2;
                return - k - bit_pos - 1;
            }
          } else {
            data = _mm256_lddqu_si256((const __m256i *) (array + k + delta/2));
            data = _mm256_add_epi16(data,conv);
            m = _mm256_cmpeq_epi16(data,target);
            bits = _mm256_movemask_epi8(m);
            if(bits != 0) {
                int32_t bit_pos = __builtin_ffs(bits) / 2;
                return k + delta/2 + bit_pos;
            } else {
                m = _mm256_cmpgt_epi16(data,target);
                bits = _mm256_movemask_epi8(m);
                int32_t bit_pos = _tzcnt_u32(bits)  / 2;
                return - k - delta/2 - bit_pos - 1;
            }
            }
        }
    }
    for(; k  < length; k ++) {
        uint16_t val = array[k];
        if(val >= ikey) {
            if(val == ikey) return k;
            else return - k - 1;
        }
    }
    return - length - 1;
}


#define RDTSC_START(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "cpuid\n\t"                                                       \
            "rdtsc\n\t"                                                       \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)

#define RDTSC_FINAL(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "rdtscp\n\t"                                                      \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            "cpuid\n\t"                                                       \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)



/*
 * This is like BEST_TIME except that ... it runs functions "test" using the
 * first parameter "base" and various parameters from "testvalues" (there
 * are nbrtestvalues), calling pre on base between tests
 */
#define BEST_TIME_PRE_ARRAY(base, length, test, pre,   testvalues, nbrtestvalues, cycle_per_op, bogus)        \
    do {                                                                                \
        fflush(NULL);                                                                   \
        uint64_t cycles_start, cycles_final, cycles_diff;                               \
        uint64_t min_diff = (uint64_t)-1;                                               \
        int sum = 0;                                                                    \
        for (size_t j = 0; j < nbrtestvalues; j++) {                                    \
            pre(base,length);                                                           \
            pre(base,length);                                                           \
            __asm volatile("" ::: /* pretend to clobber */ "memory");                   \
            RDTSC_START(cycles_start);                                                  \
            bogus += test(base,length,testvalues[j]);                                            \
            RDTSC_FINAL(cycles_final);                                                  \
            cycles_diff = (cycles_final - cycles_start);                                \
            if (cycles_diff < min_diff) min_diff = cycles_diff;                         \
            sum += cycles_diff;                                                           \
        }                                                                               \
        uint64_t S = nbrtestvalues;                                                     \
        cycle_per_op = sum / (double)S;                                                 \
    } while (0)

#define ASSERT_PRE_ARRAY(base, length, test,   testvalues, nbrtestvalues)               \
    do {                                                                                \
        int error = 0;                                                                  \
        for (size_t j = 0; j < nbrtestvalues; j++) {                                    \
            int32_t re = test(base,length,testvalues[j]);                               \
            if(re >= 0) {                                                               \
              if(base[re] != testvalues[j]) {                                           \
                error=1; break;                                                         \
              }                                                                         \
            } else {                                                                    \
               int32_t ip = -re - 1;                                                    \
               if(ip < (int32_t) length) {                                              \
                 if(base[ip] <= testvalues[j]) {                                        \
                   printf(" Index returned is %d which points at %d for target %d. ",ip,base[ip],testvalues[j]);\
                   error = 3;                                                           \
                   break;                                                               \
                 }                                                                      \
               }                                                                        \
               if( ip > 0 ){                                                            \
                 if(base[ip - 1] >= testvalues[j]) {                                    \
                   printf(" Index returned is %d which points at %d for target %d. ",ip,base[ip],testvalues[j]);\
                  printf(" Previous index is %d which points at %d for target %d. ",ip-1,base[ip-1],testvalues[j]);\
                   error = 4;                                                           \
                   break;                                                               \
                 }                                                                      \
               }                                                                        \
            }                                                                           \
        }                                                                               \
        if(error != 0) printf("[%s error: %d]\n",#test,error);                     \
     } while (0)






void demo() {
    size_t nbrtestvalues = 10000;
    value_t * testvalues = create_random_array(nbrtestvalues);
    printf("# N, prefetched seek, fresh seek  (in cycles) then same values normalized by tree height\n");
    int32_t bogus = 0;
    for(size_t N = 32; N <= 4096; N*=2) {
        value_t * source = create_sorted_array(N);
        ASSERT_PRE_ARRAY(source,N,linear,testvalues,nbrtestvalues);
        ASSERT_PRE_ARRAY(source,N,binary_search,testvalues,nbrtestvalues);
        ASSERT_PRE_ARRAY(source,N,mixed,testvalues,nbrtestvalues);
        ASSERT_PRE_ARRAY(source,N,mixedhybrid,testvalues,nbrtestvalues);
        ASSERT_PRE_ARRAY(source,N,branchless_binary_search,testvalues,nbrtestvalues);
        ASSERT_PRE_ARRAY(source,N,simd_linear_search,testvalues,nbrtestvalues);
        ASSERT_PRE_ARRAY(source,N,simd_linear_search32,testvalues,nbrtestvalues);
        ASSERT_PRE_ARRAY(source,N,linear16,testvalues,nbrtestvalues);
        ASSERT_PRE_ARRAY(source,N,linear128_16,testvalues,nbrtestvalues);

        float cycle_per_op_empty, cycle_per_op_flush,cycle_per_op_flush32,cycle_per_op_mixed,cycle_per_op_mixedhybrid,
              cycle_per_op_branchless,cycle_per_op_branchless_wp, cycle_per_op_linear,
              cycle_per_op_linear16, cycle_per_op_linear128_16, cycle_per_op_linear256_16, cycle_per_op_linear256_32,
              cycle_per_op_simdlinear, cycle_per_op_simdlinear32;

        BEST_TIME_PRE_ARRAY(source, N, does_nothing,                array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_empty, bogus);
        BEST_TIME_PRE_ARRAY(source, N, binary_search,               array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_flush, bogus);
        BEST_TIME_PRE_ARRAY(source, N, binary_search32,               array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_flush32, bogus);
        BEST_TIME_PRE_ARRAY(source, N, mixed,               array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_mixed, bogus);
        BEST_TIME_PRE_ARRAY(source, N, mixedhybrid,               array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_mixedhybrid, bogus);
        BEST_TIME_PRE_ARRAY(source, N, branchless_binary_search,    array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_branchless, bogus);
        BEST_TIME_PRE_ARRAY(source, N, branchless_binary_search_wp, array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_branchless_wp, bogus);
        BEST_TIME_PRE_ARRAY(source, N, linear, array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_linear, bogus);
        BEST_TIME_PRE_ARRAY(source, N, linear16, array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_linear16, bogus);
        BEST_TIME_PRE_ARRAY(source, N, linear128_16, array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_linear128_16, bogus);
        BEST_TIME_PRE_ARRAY(source, N, linear256_16, array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_linear256_16, bogus);
        BEST_TIME_PRE_ARRAY(source, N, linear256_32, array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_linear256_32, bogus);

        BEST_TIME_PRE_ARRAY(source, N, simd_linear_search, array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_simdlinear, bogus);
        BEST_TIME_PRE_ARRAY(source, N, simd_linear_search32, array_cache_flush,   testvalues, nbrtestvalues, cycle_per_op_simdlinear32, bogus);

        printf("N=%10d ilog2=%5d func. call = %.2f,  branchy = %.2f hybrid= %.2f mixed= %.2f mixedhybrid= %.2f branchless = %.2f branchless+prefetching = %.2f linear = %2.f linear16 = %2.f linear128_16 = %2.f linear256_16 = %2.f linear256_32 = %2.f simdlinear = %2.f simdlinear32 = %2.f \n",
               (int)N,ilog2(N),cycle_per_op_empty,cycle_per_op_flush,cycle_per_op_flush32,cycle_per_op_mixed, cycle_per_op_mixedhybrid,
               cycle_per_op_branchless,cycle_per_op_branchless_wp,cycle_per_op_linear,cycle_per_op_linear16,
               cycle_per_op_linear128_16,cycle_per_op_linear256_16,cycle_per_op_linear256_32,
               cycle_per_op_simdlinear,cycle_per_op_simdlinear32);
        free(source);
    }
    printf("bogus = %d \n",bogus);
    free(testvalues);
}
int main() {
    demo();
    return 0;
}
