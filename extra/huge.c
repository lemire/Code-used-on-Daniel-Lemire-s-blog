// gcc/icc/clang -O2 -Wall -std=gnu99 huge.c -o huge

#include <assert.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>    
#include <malloc.h>

#ifdef __i386__
#  define RDTSC_DIRTY "%eax", "%ebx", "%ecx", "%edx"
#elif __x86_64__
#  define RDTSC_DIRTY "%rax", "%rbx", "%rcx", "%rdx"
#else
# error unknown platform
#endif

#define RDTSC_START(cycles)                                \
    do {                                                   \
        register unsigned cyc_high, cyc_low;               \
        __asm volatile("CPUID\n\t"                         \
                       "RDTSC\n\t"                         \
                       "mov %%edx, %0\n\t"                 \
                       "mov %%eax, %1\n\t"                 \
                       : "=r" (cyc_high), "=r" (cyc_low)   \
                       :: RDTSC_DIRTY);                    \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;   \
    } while (0)

#define RDTSC_STOP(cycles)                                 \
    do {                                                   \
        register unsigned cyc_high, cyc_low;               \
        __asm volatile("RDTSCP\n\t"                        \
                     "mov %%edx, %0\n\t"                   \
                     "mov %%eax, %1\n\t"                   \
                     "CPUID\n\t"                           \
                     : "=r" (cyc_high), "=r" (cyc_low)     \
                     :: RDTSC_DIRTY);                      \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;   \
    } while(0)



#define NORM_PAGE_SIZE (4096)        // 4 KB normal pages
#define NORM_PAGE_TLB1_ENTRIES 64
#define NORM_PAGE_TLB2_ENTRIES 512

#define HUGE_PAGE_SIZE (4096 * 512)  // 2 MB huge pages
#define HUGE_PAGE_TLB1_ENTRIES 32
#define HUGE_PAGE_TLB2_ENTRIES 0

// FUTURE: consider using to avoid TLB collisions
#define LARGE_ODD_PAGE_SIZE  (HUGE_PAGE_SIZE - NORM_PAGE_SIZE)

#define ALLOC_SIZE  (NORM_PAGE_TLB2_ENTRIES * HUGE_PAGE_SIZE) 
#define REPEAT_COUNT 10000000

char random_access(char *desc, char *buf, int gap, int num, int off) {
    assert(gap * num + off <= ALLOC_SIZE);
    volatile char dummy = 0;
    uint64_t tsc_before, tsc_after;

    RDTSC_START(tsc_before);
    for (int r = 0; r < REPEAT_COUNT; r++) {
        int n = rand() % num;   
        int o = rand() % off;   
        dummy = (char) (n * gap + o); 
    }
    RDTSC_STOP(tsc_after);
    int avg_baseline_cycles = (tsc_after - tsc_before) / REPEAT_COUNT;

    RDTSC_START(tsc_before);
    for (int r = 0; r < REPEAT_COUNT; r++) {
        int n = rand() % num;   
        int o = rand() % off;   
        dummy |= buf[n * gap + o];
    }
    RDTSC_STOP(tsc_after);
    int avg_access_cycles = (tsc_after - tsc_before) / REPEAT_COUNT;
    printf("%s: gap[%d] num[%d] off[%d] %9d cycles\n", desc, 
           gap, num, off, avg_access_cycles - avg_baseline_cycles);
    
    return dummy;
}

int main(int c, char **argv) {

    // norm uses 4KB standard pages
    char  *norm = mmap(NULL, ALLOC_SIZE, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE| MAP_ANONYMOUS, -1, 0);

    // huge uses 2MB huge pages
    char  *huge = mmap(NULL, ALLOC_SIZE, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE| MAP_ANONYMOUS, -1, 0);
    madvise(huge, ALLOC_SIZE, MADV_HUGEPAGE);

    // initialize arrays to something
    memset(norm, 'n', ALLOC_SIZE);
    memset(huge, 'h', ALLOC_SIZE);

    printf("\nRandom with 2MB (should fit in L3):\n");
    random_access("norm", norm, NORM_PAGE_SIZE, 512, NORM_PAGE_SIZE);
    random_access("huge", huge, NORM_PAGE_SIZE, 512, NORM_PAGE_SIZE);

    printf("\nRandom from 8MB (should fit in L3):\n");
    random_access("norm", norm, HUGE_PAGE_SIZE, 4, HUGE_PAGE_SIZE);
    random_access("huge", huge, HUGE_PAGE_SIZE, 4, HUGE_PAGE_SIZE);

    printf("\nRandom from 64MB (mostly main memory)\n");
    random_access("norm", norm, HUGE_PAGE_SIZE, 32, HUGE_PAGE_SIZE);
    random_access("huge", huge, HUGE_PAGE_SIZE, 32, HUGE_PAGE_SIZE);

    printf("\nRandom from 128MB (mostly main memory)\n");
    random_access("norm", norm, HUGE_PAGE_SIZE, 64, HUGE_PAGE_SIZE);
    random_access("huge", huge, HUGE_PAGE_SIZE, 64, HUGE_PAGE_SIZE);

    printf("\nRandom from 256MB (mostly main memory)\n");
    random_access("norm", norm, HUGE_PAGE_SIZE, 128, HUGE_PAGE_SIZE);
    random_access("huge", huge, HUGE_PAGE_SIZE, 128, HUGE_PAGE_SIZE);

    printf("\nRandom from 512MB (mostly main memory)\n");
    random_access("norm", norm, HUGE_PAGE_SIZE, 256, HUGE_PAGE_SIZE);
    random_access("huge", huge, HUGE_PAGE_SIZE, 256, HUGE_PAGE_SIZE);

    printf("\n64MB but only from first 4K of each 2MB\n");
    random_access("norm", norm, HUGE_PAGE_SIZE, 32, NORM_PAGE_SIZE);
    random_access("huge", huge, HUGE_PAGE_SIZE, 32, NORM_PAGE_SIZE);

    printf("\n128MB but only from first 4K of each 2MB\n");
    random_access("norm", norm, HUGE_PAGE_SIZE, 64, NORM_PAGE_SIZE);
    random_access("huge", huge, HUGE_PAGE_SIZE, 64, NORM_PAGE_SIZE);

    printf("\n256MB but only from first 4K of each 2MB\n");
    random_access("norm", norm, HUGE_PAGE_SIZE, 128, NORM_PAGE_SIZE);
    random_access("huge", huge, HUGE_PAGE_SIZE, 128, NORM_PAGE_SIZE);

    printf("\n512MB but only from first 4K of each 2MB\n");
    random_access("norm", norm, HUGE_PAGE_SIZE, 256, NORM_PAGE_SIZE);
    random_access("huge", huge, HUGE_PAGE_SIZE, 256, NORM_PAGE_SIZE);

    return 0;
}
