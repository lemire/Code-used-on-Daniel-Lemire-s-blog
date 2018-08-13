// gcc -O3 -o bubble bubble.c -march=native -lpthread
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef __AVX512DQ__
#include <immintrin.h>
#endif

atomic_int acnt = 0;

void bsort(uint64_t *arr, size_t n) {
  for (size_t i = 0; i < n - 1; i++) {
    for (size_t j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        arr[j] ^= arr[j + 1];
        arr[j + 1] ^= arr[j];
        arr[j] ^= arr[j + 1];
      }
    }
  }
}

void *athread(void *arg) {

  const size_t size = 64;
  uint64_t arr[size];
  uint64_t factor = *(uint64_t *)arg;

  for (int i = 0; i < size; i++) {
      arr[i] = 0;
  }

  while (1) {
    if (acnt >= 50000000) {
      return NULL;
    }
    acnt++;

    bsort(arr, size);

    for (int i = 0; i < size;) {
#ifdef __AVX512DQ__
      _mm512_storeu_si512(
          &arr[i],
          _mm512_mullo_epi64(_mm512_loadu_si512(&arr[i]),
                             _mm512_set1_epi64(factor)));
      i += 8;
#else
      arr[i] *= factor;
      i++;
#endif
    }
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  size_t nthreads = 1;
  if (argc > 1) {
    nthreads = atoi(argv[1]);
  }
#ifdef __AVX512DQ__
  printf("using avx512\n");
#else 
  printf("not using avx512\n");
#endif
  pthread_t threads[nthreads];
  uint64_t factors[nthreads];

  for (int i = 0; i < nthreads; i++) {
    factors[i] = i;
  }

  for (int i = 0; i < nthreads; i++) {
    pthread_create(&threads[i], NULL, athread, &factors[i]);
  }

  for (int i = 0; i < nthreads; i++) {
    pthread_join(threads[i], NULL);
  }
}
