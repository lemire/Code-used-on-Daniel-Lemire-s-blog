// gcc -fno-inline -march=native -std=c99 -Wall -O3 -g  mallocspeed.c -o mallocspeed
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

#define RDTSC_LOOP(test,check,reset,repeat, num_vecs)                      \
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
            if (!(check)) {printf("error");}                                       \
            reset;                                                     \
        }                                                               \
        if (min_diff <= global_rdtsc_overhead) min_diff = 0;		\
        else min_diff = min_diff - global_rdtsc_overhead;               \
        float cycles_per_vec = min_diff / (double)(num_vecs);           \
        printf(" %.2f cycles per alloc", cycles_per_vec);              \
        printf(" (%ld cycs / %ld value)", (long) min_diff, (long) num_vecs); \
        printf("\n");							\
        fflush(NULL);							\
        } while (0)


void bigmalloc(uint8_t ** elems, size_t number, size_t size) {
  for(size_t i =0; i < number; i++) {
    elems[i] = malloc(size);
  }
}

void bigmallocmemset(uint8_t ** elems, size_t number, size_t size) {
  for(size_t i =0; i < number; i++) {
    elems[i] = malloc(size);
    memset(elems[i],0,size);
  }
}

void bigfree(uint8_t ** elems, size_t number, size_t size) {
  for(size_t i =0; i < number; i++) free(elems[i]);
}
void bigmallocandfree(uint8_t ** elems, size_t number, size_t size) {
  for(size_t i =0; i < number; i++) {
    elems[i] = malloc(size);
    free(elems[i]);
    elems[i] = NULL;
  }
}

void demo(int length) {
    int mallocsize = 16;// tiny mallocs
    printf("time (in cycles) to allocate %d blocks of %d bytes \n",length,mallocsize);
    uint8_t **tmp = malloc(length * sizeof(uint8_t *));
    int repeat = 5;
    RDTSC_LOOP(bigmallocmemset(tmp,length,mallocsize),tmp[0] != NULL ,bigfree(tmp,length,mallocsize), repeat, length);
    RDTSC_LOOP(bigmalloc(tmp,length,mallocsize),tmp[0] != NULL ,bigfree(tmp,length,mallocsize), repeat, length);
    RDTSC_LOOP(bigmallocandfree(tmp,length,mallocsize),tmp[0] == NULL,, repeat, length);

    free(tmp);
}

int main(/* int argc, char **argv */) {
    for(int numberofmallocs = 1; numberofmallocs <= 1000; numberofmallocs *= 10) demo(numberofmallocs);
    return 0;
}
