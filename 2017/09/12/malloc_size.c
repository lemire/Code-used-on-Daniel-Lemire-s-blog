// gcc -O3 -o malloc_size malloc_size.c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "benchmark.h"
#ifdef __APPLE__
#include <malloc/malloc.h>
#endif
#ifdef __LINUX__
#include <malloc.h>
#endif 
 
void demo(size_t N) {
    int repeat = 5;
    uint64_t * values = (uint64_t *) malloc(N * sizeof(uint64_t));
#ifdef __APPLE__
    size_t expected = malloc_size(values);
    printf("allocated %zu, reported allocation = %zu \n", N * sizeof(uint64_t), expected);
    BEST_TIME(malloc_size(values), expected, , repeat, 1, true);
#elif defined(__LINUX__)
    size_t expected = malloc_usable_size(values);
    printf("allocated %zu, reported allocation = %zu \n", N * sizeof(uint64_t), expected);
    BEST_TIME(malloc_size(values), expected, , repeat, 1, true);
#endif
    free(values);
}
int main() {
  for(size_t N = 10; N < 100000000; N*=10) demo(N);
}
