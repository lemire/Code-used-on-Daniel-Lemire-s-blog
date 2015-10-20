// binsearch [random-seed]
// demo to show that interleaved memory accesses reduce effective latency
// gcc -std=c99 -Wall -g -march=native -O3 binsearch.c -o binsearch
//
// -DPOWER_OF_TWO to see the bad effects of cache line aliasing
// -DLINEAR_REFERENCE to compare against a slow reference implementation
// -DDEBUG_PRINT to add verbose debugging from within routines

#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG_PRINT
#undef DEBUG_PRINT
#define DEBUG_PRINT(args...) printf(args)
#else
#define DEBUG_PRINT(...)
#endif


// parallel binary search for the 'count' 'targets' in a sorted 'array'
// matching 'indexes' are first element equal or greater than each target
void parallel_search(uint32_t *array, size_t length,
                     uint32_t *targets, size_t *indexes, size_t count) {
    memset(indexes, 0, count * sizeof(*indexes)); // set indexes to zero

    for (size_t half = length/2; half; length -= half, half = length/2) {
        DEBUG_PRINT("\n%zd\n", half);
        for (size_t c = 0; c < count; c++) {
            size_t i = indexes[c];    // load stored index for this target
            size_t test = i + half;   // test index will be halfway point
            if (targets[c] > array[test]) i = test; // update index with CMOV
            indexes[c] = i;           // always resave whether updated or not
            DEBUG_PRINT("%d %d %zu %zu %zu\n",
                        targets[c], array[test], c, i, test);
        }
    }

    // FUTURE: may be possible to integrate 'adjustment' into loop above?
    uint32_t min = array[0];
    for (size_t c = 0; c < count; c++) {
        uint32_t target = targets[c];
        size_t adjusted = indexes[c] + 1;
        if (target <= min) adjusted = 0;
        indexes[c] = adjusted;
    }

    // indexes matching-or-less-than targets ready for caller
}


// parallel binary search for the 'count' 'targets' in a sorted 'array'
// matching 'indexes' are first element equal or greater than each target
// this version does not assume cmov
void portable_parallel_search(uint32_t *array, size_t length,
                     uint32_t *targets, size_t *indexes, size_t count) {
    memset(indexes, 0, count * sizeof(*indexes)); // set indexes to zero

    for (size_t half = length/2; half; length -= half, half = length/2) {
        DEBUG_PRINT("\n%zd\n", half);
        for (size_t c = 0; c < count; c++) {
            size_t i = indexes[c];    // load stored index for this target
            i += ( ((int32_t)array[i+half]-(int32_t)targets[c])>>31) & half;
            indexes[c] = i;           // always resave whether updated or not
            DEBUG_PRINT("%d %d %zu %zu %zu\n",
                        targets[c], array[test], c, i, test);
        }
    }

    // FUTURE: may be possible to integrate 'adjustment' into loop above?
    uint32_t min = array[0];
    for (size_t c = 0; c < count; c++) {
        uint32_t target = targets[c];
        size_t adjusted = indexes[c] + 1;
        if (target <= min) adjusted = 0;
        indexes[c] = adjusted;
    }

    // indexes matching-or-less-than targets ready for caller
}


// reference implementation equivalent to parallel_search
// NB: if target > max(array), matched index = length (out-of-bounds)
void linear_search(uint32_t *array, size_t length,
                   uint32_t *targets, size_t *indexes, size_t count) {
    for (size_t c = 0; c < count; c++) {
        uint32_t target = targets[c];
        size_t n = 0;
        // save index of first element in array >= target
        while (n < length && array[n] < target) n++;
        DEBUG_PRINT("linear_search: %zd %u %zd (%u)\n", c, target, n,
                    n < length ? array[n] : UINT32_MAX);
        indexes[c] = n;
    }
}

int qsort_compare_uint32(const void *a, const void *b) {
    return ( *(uint32_t *)a - *(uint32_t *)b );
}
uint32_t *create_sorted_array(size_t length, size_t max) {
    uint32_t *array = malloc(length * sizeof(*array));
    for (size_t i = 0; i < length; i++) array[i] = rand() % max;
    qsort(array, length, sizeof(*array), qsort_compare_uint32);
    return array;
}

uint32_t *create_search_targets(size_t count, size_t max) {
    uint32_t *targets = malloc(count * sizeof(*targets));
    for (size_t i = 0; i < count; i++) targets[i] = rand() % max;
    return targets;
}

size_t verify_indexes(size_t *reference, size_t *test, size_t count,
                         uint32_t *array, uint32_t *targets, size_t length) {
    for (size_t i = 0; i < count; i++) {
        if (reference[i] != test[i]) {
            // avoid read-past-end when printing error details
            printf(" MISMATCH %zd (%u): %zd (%u) %zd (%u)",
                   i, targets[i], reference[i],
                   reference[i] < length ? array[reference[i]] : UINT32_MAX,
                   test[i], test[i] < length ? array[test[i]] : UINT32_MAX);
            return 1;
        }
    }
    return 0;
}

#define COUNT(array) (sizeof(array)/sizeof(*array))

#define RDTSC_START(cycles)                                     \
    do {                                                        \
        register unsigned cyc_high, cyc_low;                    \
        __asm volatile("cpuid\n\t"                              \
                     "rdtsc\n\t"                                \
                     "mov %%edx, %0\n\t"                        \
                     "mov %%eax, %1\n\t"                        \
                     : "=r" (cyc_high), "=r" (cyc_low)          \
                     :: "%rax", "%rbx", "%rcx", "%rdx");        \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;        \
    } while (0)

// set final cycles to current 64-bit rdtsc value
#define RDTSC_FINAL(cycles)                                     \
    do {                                                        \
        register unsigned cyc_high, cyc_low;                    \
        __asm volatile("rdtscp\n\t"                             \
                     "mov %%edx, %0\n\t"                        \
                     "mov %%eax, %1\n\t"                        \
                     "cpuid\n\t"                                \
                     : "=r" (cyc_high), "=r" (cyc_low)          \
                     :: "%rax", "%rbx", "%rcx", "%rdx");        \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;        \
    } while(0)

int main(int argc, char **argv) {
    // binsearch [random-seed]
    if (argc > 1) srand(atoi(argv[1]));

#ifdef POWER_OF_TWO
    size_t lengths[] = {
        256, 1024, 256 * 256, 256 * 1024,
        1024*1024, 16*1024*1024, 256*1024*1024
    };
#else
    size_t lengths[] = {
        100, 1000, 10*1000, 100*1000,
        1000*1000, 10*1000*1000, 100*1000*1000
    };
#endif

    size_t batches[] = { // batch sizes reduced to target count as needed
        1, 2, 4, 6, 8, 10, 12, 14, 16, 20,
        40, 60, 100, 200, 400, 1000, UINT32_MAX
    };

    for (size_t n = 0; n < COUNT(lengths); n++) {
        size_t length = lengths[n];
        size_t count = 1000*1000;

        uint32_t *targets = create_search_targets(count, 2*length);
        printf("%zd Targets with Array Length", count);
        fflush(NULL);

        uint32_t *array = create_sorted_array(length, 2*length);
        printf(" %zd\n", length);

        size_t *reference_indexes = malloc(count * sizeof(size_t));
#ifdef LINEAR_REFERENCE
        linear_search(array, length,
                      targets, reference_indexes, count);
#else
        parallel_search(array, length,
                        targets, reference_indexes, count);
#endif

        size_t *indexes = malloc(count * sizeof(*indexes));
        for (size_t i = 0; i < COUNT(batches); i++) {
            memset(indexes, 0, count * sizeof(*indexes));
            size_t batch = batches[i];
            if (batch > count) batch = count;
            printf("  Batch %3zd: ", batch);
            fflush(NULL);
            uint64_t cycles_start, cycles_final;
            RDTSC_START(cycles_start);
            for (size_t c = 0; c < count; c += batch) {
                // possible smaller batch for last iteration
                if (c > count - batch) batch = count % batch;
                parallel_search(array, length, targets + c,
                                indexes + c, batch);
            }
            RDTSC_FINAL(cycles_final);
            float cycles_per_search =
                (cycles_final - cycles_start) / (float) count;
            printf("cmov?: %.2f cycles/search ", cycles_per_search);
            verify_indexes(reference_indexes, indexes, count,
                           array, targets, length);

            RDTSC_START(cycles_start);
            for (size_t c = 0; c < count; c += batch) {
                // possible smaller batch for last iteration
                if (c > count - batch) batch = count % batch;
                portable_parallel_search(array, length, targets + c,
                                indexes + c, batch);
            }
            RDTSC_FINAL(cycles_final);
            cycles_per_search =
                (cycles_final - cycles_start) / (float) count;
            printf(", portable: %.2f cycles/search", cycles_per_search);
            printf("\n");

        }
        free(array);
        free(targets);
        free(indexes);
        free(reference_indexes);
        printf("\n");
    }
}
