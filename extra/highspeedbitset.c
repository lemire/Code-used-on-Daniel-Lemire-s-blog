// gcc -fno-inline -march=native -std=c99 -Wall -O3 -g  highspeedbitset.c -o highspeedcrazybitset

#include <x86intrin.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
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

#define RDTSC_LOOP(test,repeat, num_vecs)                      \
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
        }                                                               \
        if (min_diff <= global_rdtsc_overhead) min_diff = 0;		\
        else min_diff = min_diff - global_rdtsc_overhead;               \
        float cycles_per_vec = min_diff / (double)(num_vecs);           \
        printf(" %.2f cycles per value", cycles_per_vec);              \
        printf(" (%ld cycs / %ld value)", (long) min_diff, (long) num_vecs); \
        printf("\n");							\
        fflush(NULL);							\
        } while (0)


void bitset_set_list(void *bitset, const uint16_t *list, uint64_t length) {
    uint64_t offset, load, pos;
    uint64_t shift = 6;
    const uint16_t *end = list + length;
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

void bitset_set_list_condwrite(void *bitset, const uint16_t *list, uint64_t length) {
    uint64_t offset, load, pos;
    uint64_t shift = 6;
    const uint16_t *end = list + length;
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
void demo() {
    const int length = 256*100;
    const int mult = 6;

    uint16_t list[length];
    for (int i = 0; i < length; i++) {
        list[i] = i * mult;
    }
    const int bitsetbytes = length * mult / 8 + 1;

    uint8_t *out = malloc(bitsetbytes);
    memset(out, 0x00, bitsetbytes);
    const int repeat = 1000;
        printf("%d \n",*(int *)out);

    RDTSC_LOOP(bitset_set_list(out, list, length),  repeat, 256);
    check(out, list, length);
    printf("%d \n",*(int *)out);
    memset(out, 0x00, bitsetbytes);
            printf("%d \n",*(int *)out);

    RDTSC_LOOP(bitset_set_list_condwrite(out, list, length), repeat, 256);
        printf("%d \n",*(int *)out);

    check(out, list, length);
    memset(out, 0x00, bitsetbytes);
    free(out);
}

int main(/* int argc, char **argv */) {
    demo();
    return 0;
}
