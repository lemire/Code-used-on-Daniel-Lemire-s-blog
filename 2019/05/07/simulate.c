#include <math.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wyhash.h"

uint64_t random_range(uint64_t range) {
    // Fast Random Integer Generation in an Interval,  
    // ACM Transactions on Modeling and Computer Simulation 29 (1), 2019
    return (uint64_t) (((__uint128_t)wyhash64() * range) >> 64);
}

double random_double() {
     return (wyhash64() % (UINT64_C(1)<<53)) / (double) (UINT64_C(1)<<53);
}

int uint64_comparison(const void * a, const void * b) {
     if ( (*(uint64_t *) a) < (*(uint64_t *) b)) return -1;
     if ( (*(uint64_t *) a) > (*(uint64_t *) b)) return 1;
     return 0;
}

// this starts at 0 with proba. p, then 1 with proba. (1-p) *p
// add one to the result if needed
uint64_t random_geometric(double p) { // p in (0,1)
    double logp = log1p(-p);// for very small ps, this is more accurate than  c;
    double val = random_double();
    return (uint64_t)floor(log(val) / logp);
}

/**
 * Pick N values in [range_min, range_max).
 * Assume N << sizeof [range_min, range_max).
 * If N is close to [range_min, range_max), then 
 **/
size_t pick_N(uint64_t range_min, uint64_t range_max, size_t N, uint64_t *out) {
    uint64_t range_size = range_max - range_min;
    for(size_t i = 0; i < N; i++) {
      out[i] = random_range(range_size) + range_min;
    }
    qsort(out, N, sizeof(uint64_t), uint64_comparison);
    size_t pos = 0;
    out[pos++] = out[0];
    for(size_t i = 1; i < N; i++) {
        if(out[i] > out[i-1]) {
            out[pos++] = out[i];
        } 
    }
    return pos;
}

size_t fast_pick_N(uint64_t range_min, uint64_t range_max, size_t N, uint64_t *out) {
    uint64_t range_size = range_max - range_min;
    if(N == 0) return 0;
    if(range_size <= N) return 0;
    double p = (double) N  / range_size; // this assumes no collision
    uint64_t previous = random_geometric(p); 
    if(previous >= range_max) return 0;
    out[0] = previous + range_min; 
    size_t i = 1;
    for(; i < N; i++) {
      range_size = range_max - range_min - previous - 1;
      if(range_size <= (N - i)) {
          break;
      }
      p = (double) (N - i)  / range_size; 
      uint64_t newoffset = random_geometric(p);
      previous += newoffset + 1;
      if(previous >= range_max) {
          break;
      }
      out[i] = previous + range_min;
    }
    return i;
}

void demo(size_t N, uint64_t range_min, uint64_t range_max) {
    size_t volume = range_max - range_min;
    printf("Generating %zu values in [%zu, %zu), density: %f %%\n", N, range_min, range_max, N*100.0 / (range_max-range_min));
    uint64_t * array = (uint64_t *) malloc(sizeof(uint64_t) * N);
    if(array == NULL) return;
     clock_t bef1 = clock();
    size_t c1 = pick_N(range_min, range_max, N, array);
         clock_t aft1 = clock();
     clock_t bef2 = clock();

    size_t c2 = fast_pick_N(range_min, range_max, N, array);
         clock_t aft2 = clock();
    printf("timings: %f s %f s \n",(double)(aft1-bef1)/CLOCKS_PER_SEC, (double)(aft2-bef2)/CLOCKS_PER_SEC);
    printf("timings per value: %f ns %f ns \n",(double)(aft1-bef1 )* 1000  * 1000  * 1000/(volume * CLOCKS_PER_SEC), (double)(aft2-bef2)* 1000 * 1000 * 1000/(volume * CLOCKS_PER_SEC));

    printf("actual counts: %zu %zu \n",c1, c2);
    free(array);
}

// in case you do not trust my geometric approx.
void test_geometric(double p) {
    size_t N = 1024 * 1024;
    double * counters = (double *) malloc(N * sizeof(double));
    memset(counters, 0, N * sizeof(double));
    size_t hits = 0;
    size_t total = 1024 * 1024 * 10;

    for(size_t i = 0; i < total;i++) {
      uint64_t val = random_geometric(p);
      if(val >= N) continue;
      counters[val] ++;
      hits++;
    } 
    printf("hits = %zu total = %zu \n", hits, total);
    double sum =  0;
    for(size_t i = 0; i < 20;i++) {
      counters[i] /= hits;
      sum += counters[i];
      printf("%zu : %f (%f)... cumul %f\n", i, counters[i], pow(1-p,i) * p, sum);
    } 
    free(counters);
}

int main () {
  wyhash64_seed(1243);
  for(size_t N = 1000000 ; N <= 100000000; N*=10) {
    demo(N, 0, UINT64_C(40000000000));
    printf("\n");
  }
}
