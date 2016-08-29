// gcc -fno-inline -march=native -std=c99 -Wall -O3 -g  highspeedbitset.c -o highspeedcrazybitset
#include <x86intrin.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

unsigned char _bittestandset64(uint64_t *Base, uint64_t Offset)
{
  int old = 0;
  __asm__ __volatile__("btsq %2,%1\n\tsbbl %0,%0 "
    :"=r" (old),"=m" ((*(volatile long long *) Base))
    :"Ir" (Offset));
  return (old != 0);
}
#define RDTSC_START(cycles)  do {                                       \
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

#define RDTSC_STOP(cycles)  do {                                        \
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

__attribute__ ((noinline))
uint64_t rdtsc_overhead_func(uint64_t dummy) {
    return dummy;
}

uint64_t global_rdtsc_overhead = (uint64_t) UINT64_MAX;

#define RDTSC_SET_OVERHEAD(test, repeat)                                \
    do {                                                                \
        uint64_t cycles_start, cycles_final, cycles_diff;               \
        uint64_t min_diff = UINT64_MAX;                                 \
        for (size_t i = 0; i < repeat; i++) {                           \
            __asm volatile("" ::: /* pretend to clobber */ "memory");   \
            RDTSC_START(cycles_start);                                  \
            test;                                                       \
            RDTSC_STOP(cycles_final);                                   \
            cycles_diff = (cycles_final - cycles_start);                \
            if (cycles_diff < min_diff) min_diff = cycles_diff;         \
        }                                                               \
        global_rdtsc_overhead = min_diff;                               \
        printf("rdtsc_overhead set to %ld\n", (long)global_rdtsc_overhead);   \
    } while (0)                                                         \

#define RDTSC_LOOP(test,reset,repeat, num_vecs)                      \
    do {                                                                \
        if (global_rdtsc_overhead == UINT64_MAX) {                      \
            RDTSC_SET_OVERHEAD(rdtsc_overhead_func(1), repeat);         \
        }                                                               \
        printf("%45s: ", #test);                                        \
        fflush(NULL);							\
        uint64_t cycles_start, cycles_final, cycles_diff;               \
        uint64_t min_diff = UINT64_MAX;					\
        for (size_t i = 0; i < repeat; i++) {				\
            __asm volatile("" ::: /* pretend to clobber */ "memory");   \
            RDTSC_START(cycles_start);                                  \
            test;                                              \
            RDTSC_STOP(cycles_final);                                   \
            cycles_diff = (cycles_final - cycles_start);                \
            if (cycles_diff < min_diff) min_diff = cycles_diff;         \
            reset;                                                     \
        }                                                               \
        if (min_diff <= global_rdtsc_overhead) min_diff = 0;		\
        else min_diff = min_diff - global_rdtsc_overhead;               \
        float cycles_per_vec = min_diff / (double)(num_vecs);           \
        printf(" %.2f cycles per value", cycles_per_vec);              \
        printf(" (%ld cycs / %ld value)", (long) min_diff, (long) num_vecs); \
        printf("\n");							\
        fflush(NULL);							\
        } while (0)

void bitset_set_list_regular(void *bitset,
                         uint16_t *list, uint64_t length) {
    uint64_t offset, load, newload, pos, index;
    uint16_t *end = list + length;
    while(list != end) {
      pos =  *(uint16_t *)  list;
      offset = pos >> 6;
      index = pos % 64;
      load = ((uint64_t *) bitset)[offset];
      newload = load | (UINT64_C(1) << index);
      ((uint64_t *) bitset)[offset] = newload;
      list ++;
    }
}



void bitset_set_list_avx(void *bitset,
                         uint16_t *list, uint64_t length) {
    uint64_t offset, load, newload, pos, index;
    uint16_t *end = list + length;
    __m256i sillymask = _mm256_set1_epi16(63);
    __m256i justone = _mm256_set1_epi64x(1);
    __m256i justzero = _mm256_setzero_si256();
    uint16_t indexes16[16];
    uint64_t shiftsarray[16];
    uint64_t * bitset64 = (uint64_t*)bitset;
    while(list + 16 <= end) {
      __m256i vlist = _mm256_loadu_si256((const __m256i *) list);
      __m256i indexes = _mm256_srli_epi16 (vlist, 6);
      _mm256_store_si256((__m256i *) indexes16,indexes);
      __m256i shifts = _mm256_and_si256 (vlist, sillymask);
      __m256i u1 = _mm256_unpacklo_epi16(shifts,justzero);
      __m256i u2 = _mm256_unpackhi_epi16(shifts,justzero);
      __m256i shifts1 = _mm256_unpacklo_epi32(u1,justzero);
      __m256i shifts2 = _mm256_unpackhi_epi32(u1,justzero);
      __m256i shifts3 = _mm256_unpacklo_epi32(u2,justzero);
      __m256i shifts4 = _mm256_unpackhi_epi32(u2,justzero);
      _mm256_sllv_epi64(justone,shifts1);
      _mm256_sllv_epi64(justone,shifts2);
      _mm256_sllv_epi64(justone,shifts3);
      _mm256_sllv_epi64(justone,shifts4);
      _mm256_store_si256((__m256i *) shiftsarray,shifts1);
      _mm256_store_si256((__m256i *) shiftsarray + 1,shifts2);
      _mm256_store_si256((__m256i *) shiftsarray + 2,shifts3);
      _mm256_store_si256((__m256i *) shiftsarray + 3,shifts4);
      bitset64[indexes16[0]] |= shiftsarray[0];
      bitset64[indexes16[1]] |= shiftsarray[1];
      bitset64[indexes16[2]] |= shiftsarray[2];
      bitset64[indexes16[3]] |= shiftsarray[3];
      bitset64[indexes16[4]] |= shiftsarray[4];
      bitset64[indexes16[5]] |= shiftsarray[5];
      bitset64[indexes16[6]] |= shiftsarray[6];
      bitset64[indexes16[7]] |= shiftsarray[7];
      bitset64[indexes16[8]] |= shiftsarray[8];
      bitset64[indexes16[9]] |= shiftsarray[9];
      bitset64[indexes16[10]] |= shiftsarray[10];
      bitset64[indexes16[11]] |= shiftsarray[11];
      bitset64[indexes16[12]] |= shiftsarray[12];
      bitset64[indexes16[13]] |= shiftsarray[13];
      bitset64[indexes16[14]] |= shiftsarray[14];
      bitset64[indexes16[15]] |= shiftsarray[15];
      list += 16;
    }
    while(list != end) {
      pos =  *(uint16_t *)  list;
      offset = pos >> 6;
      index = pos % 64;
      load = ((uint64_t *) bitset)[offset];
      newload = load | (UINT64_C(1) << index);
      ((uint64_t *) bitset)[offset] = newload;
      list ++;
    }
}


void bitset_set_list_intrin(void *bitset,
                         uint16_t *list, uint64_t length) {
    uint64_t offset, load, newload, pos, index;
    uint16_t *end = list + length;
    while(list != end) {
      pos =  *(uint16_t *)  list;
      _bittestandset64(bitset + (pos >> 6),pos);
      list ++;
    }
}

void bitset_set_list_regular_unroll4(void *bitset,
                         uint16_t *list, uint64_t length) {
    uint64_t offset, load, newload, pos, index;
    uint16_t *end = list + length;
    for(;list + 3 < end; list += 4) {
      pos =  list[0];
      offset = pos >> 6;
      index = pos % 64;
      load = ((uint64_t *) bitset)[offset];
      newload = load | (UINT64_C(1) << index);
      ((uint64_t *) bitset)[offset] = newload;
      pos =  list[1];
      offset = pos >> 6;
      index = pos % 64;
      load = ((uint64_t *) bitset)[offset];
      newload = load | (UINT64_C(1) << index);
      ((uint64_t *) bitset)[offset] = newload;
      pos =  list[2];
      offset = pos >> 6;
      index = pos % 64;
      load = ((uint64_t *) bitset)[offset];
      newload = load | (UINT64_C(1) << index);
      ((uint64_t *) bitset)[offset] = newload;
      pos =  list[3];
      offset = pos >> 6;
      index = pos % 64;
      load = ((uint64_t *) bitset)[offset];
      newload = load | (UINT64_C(1) << index);
      ((uint64_t *) bitset)[offset] = newload;
    }
    while(list != end) {
      pos =  *(uint16_t *)  list;
      offset = pos >> 6;
      index = pos % 64;
      load = ((uint64_t *) bitset)[offset];
      newload = load | (UINT64_C(1) << index);
      ((uint64_t *) bitset)[offset] = newload;
      list ++;
    }
}



void bitset_set_list_regularasm_roll4(void *bitset,
                         uint16_t *list, uint64_t length) {
    uint64_t  pos;
    uint16_t *end = list + length;

    uint64_t shift = 6;
    uint64_t offset;
    uint64_t load;
    for(;list + 3 < end; list += 4) {
      pos =  list[0];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos =  list[1];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos =  list[2];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos =  list[3];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
    }

    while(list != end) {
      pos =  list[0];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      list ++;
    }
}

void bitset_set_list_regularasm_roll4s(void *bitset,
                         uint16_t *list, uint64_t length) {
    uint64_t  pos;
    uint16_t *end = list + length;

    uint64_t shift = 6;
    uint64_t offset;
    uint64_t load;
    for(;list + 3 < end; list += 4) {
      uint64_t fourpos = *(uint64_t*)list;
      pos =  fourpos & 0xFFFF;
      fourpos >>= 16;
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos  =  fourpos & 0xFFFF;
      fourpos >>= 16;
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos  =  fourpos & 0xFFFF;
      fourpos >>= 16;
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos  =  fourpos;
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
    }

    while(list != end) {
      pos =  list[0];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      list ++;
    }
}

void bitset_set_list_regularasm_roll8(void *bitset,
                         uint16_t *list, uint64_t length) {
    uint64_t  pos;
    uint16_t *end = list + length;

    uint64_t shift = 6;
    uint64_t offset;
    uint64_t load;
    for(;list + 7 < end; list += 8) {
      pos =  list[0];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos =  list[1];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos =  list[2];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos =  list[3];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos =  list[4];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos =  list[5];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos =  list[6];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      pos =  list[7];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));

    }

    while(list != end) {
      pos =  list[0];
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      list ++;
    }
}


void bitset_set_list_regularasm(void *bitset,
                         uint16_t *list, uint64_t length) {
    uint64_t  pos;
    uint16_t *end = list + length;

    uint64_t shift = 6;
    uint64_t offset;
    uint64_t load;

    while(list != end) {
      pos =  *(uint16_t *)  list;
    __asm volatile(
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)"
        : [load] "=&r"(load), [offset] "=&r"(offset)
        : [bitset] "r"(bitset), [shift] "r"(shift), [pos] "r"(pos));
      list ++;
    }
}

void bitset_set_list(void *bitset, const uint16_t *list, uint64_t length) {
    uint64_t offset, load, pos;
    uint64_t shift = 6;
    const uint16_t *end = list + length;
    if (!length) return;
    // bts is not available as an intrinsic in GCC
    __asm volatile(
        "1:\n"
        "movzwq (%[list]), %[pos]\n"
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "mov %[load], (%[bitset],%[offset],8)\n"
        "add $2, %[list]\n"
        "cmp %[list], %[end]\n"
        "jnz 1b"
        : [list] "+&r"(list), [load] "=&r"(load), [pos] "=&r"(pos),
          [offset] "=&r"(offset)
        : [end] "r"(end), [bitset] "r"(bitset), [shift] "r"(shift));
}

void bitset_set_list_skip(void *bitset, const uint16_t *list, uint64_t length) {
    uint64_t offset, load, pos;
    uint64_t shift = 6;
    const uint16_t *end = list + length;
    if (!length) return;
    // bts is not available as an intrinsic in GCC
    __asm volatile(
        "1:\n"
        "movzwq (%[list]), %[pos]\n"
        "shrx %[shift], %[pos], %[offset]\n"
        "mov (%[bitset],%[offset],8), %[load]\n"
        "bts %[pos], %[load]\n"
        "jc 2f\n"
        "mov %[load], (%[bitset],%[offset],8)\n"
        "2:\n"
        "add $2, %[list]\n"
        "cmp %[list], %[end]\n"
        "jnz 1b"
        : [list] "+&r"(list), [load] "=&r"(load), [pos] "=&r"(pos),
          [offset] "=&r"(offset)
        : [end] "r"(end), [bitset] "r"(bitset), [shift] "r"(shift));
}


void check(void *bitset, const uint16_t *list, uint64_t length) {
  const char * c = (const char *)bitset;
  for(uint32_t i = 0; i < length; ++i) {
    uint16_t val = list[i];
    uint32_t byte = val /8 ;
    uint32_t bit = val % 8;
    assert((c[byte] & (1<<bit)) ==(1<<bit));
  }
}

void reset(uint16_t * list, int length) {
    for (int i = 0; i < length; i++) {
        list[i] = rand();
    }
 }

void demo() {
    const int length = 256*100;
    const int mult = 3;

    uint16_t list[length];
    for (int i = 0; i < length; i++) {
        list[i] = i * mult;
    }
    const int bitsetbytes = length * mult / 8 + 1;

    uint8_t *out = malloc(bitsetbytes);
    memset(out, 0x00, bitsetbytes);
    const int repeat = 1000;
        printf("%d \n",*(int *)out);
    RDTSC_LOOP(bitset_set_list_regular(out, list, length),reset(list,length),  repeat, length);

    check(out, list, length);
    printf("%d \n",*(int *)out);
    memset(out, 0x00, bitsetbytes);
            printf("%d \n",*(int *)out);
    for (int i = 0; i < length; i++) {
        list[i] = i * mult;
    }
    RDTSC_LOOP(bitset_set_list_avx(out, list, length),reset(list,length),  repeat, length);

    //check(out, list, length);
    printf("%d \n",*(int *)out);
    memset(out, 0x00, bitsetbytes);
            printf("%d \n",*(int *)out);
    for (int i = 0; i < length; i++) {
        list[i] = i * mult;
    }
    RDTSC_LOOP(bitset_set_list_regularasm(out, list, length),reset(list,length),  repeat, length);

    check(out, list, length);
    printf("%d \n",*(int *)out);
    memset(out, 0x00, bitsetbytes);
            printf("%d \n",*(int *)out);
    for (int i = 0; i < length; i++) {
        list[i] = i * mult;
    }
    RDTSC_LOOP(bitset_set_list_regularasm_roll4(out, list, length),reset(list,length),  repeat, length);



    check(out, list, length);
    printf("%d \n",*(int *)out);
    memset(out, 0x00, bitsetbytes);
            printf("%d \n",*(int *)out);
    for (int i = 0; i < length; i++) {
        list[i] = i * mult;
    }
    RDTSC_LOOP(bitset_set_list_regularasm_roll8(out, list, length),reset(list,length),  repeat, length);

    check(out, list, length);
    printf("%d \n",*(int *)out);
    memset(out, 0x00, bitsetbytes);
            printf("%d \n",*(int *)out);
    for (int i = 0; i < length; i++) {
        list[i] = i * mult;
    }

    RDTSC_LOOP(bitset_set_list_regular_unroll4(out, list, length),reset(list,length),  repeat, length);

    check(out, list, length);
    printf("%d \n",*(int *)out);
    memset(out, 0x00, bitsetbytes);
            printf("%d \n",*(int *)out);
    for (int i = 0; i < length; i++) {
        list[i] = i * mult;
    }
    RDTSC_LOOP(bitset_set_list_skip(out, list, length),reset(list,length),  repeat, length);

    check(out, list, length);
    printf("%d \n",*(int *)out);
    for (int i = 0; i < length; i++) {
        list[i] = i * mult;
    }

    RDTSC_LOOP(bitset_set_list_skip(out, list, length),reset(list,length),  repeat, length);

    check(out, list, length);
    printf("%d \n",*(int *)out);
    memset(out, 0x00, bitsetbytes);
            printf("%d \n",*(int *)out);
    for (int i = 0; i < length; i++) {
        list[i] = i * mult;
    }

    RDTSC_LOOP(bitset_set_list(out, list, length), reset(list,length), repeat, length);
    check(out, list, length);
    printf("%d \n",*(int *)out);
    free(out);
}

int main(/* int argc, char **argv */) {
    demo();
    return 0;
}
