// originally from
// https://gist.github.com/simonhf/caaa33ccb87c0bf0775a863c0d6843c2 g++ -O3 -o
// cache-line-example cache-line-example.cpp
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define NUMBYTES (1024 * 1024 * 1024)

double get_time_in_seconds(void) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (double)tv.tv_sec + 1.e-6 * (double)tv.tv_usec;
}

template <bool prefetch>
void demo(int inc, int batch_size, bool rewritten = false) {
  int *p_start = (int *)malloc(batch_size * sizeof(int));
  uint64_t *c = (uint64_t *)malloc(batch_size * sizeof(uint64_t));
  char *bytes = (char *)malloc(NUMBYTES);
  for (int i = 0; i < NUMBYTES; i++) {
    bytes[i] = i & 255;
  }
  int r = 14242;
  for (int i = 0; i < batch_size; i++) {
    p_start[i] = ((NUMBYTES / batch_size) * i) + (r & 8191);
    r = r * 434231431 + 43243451; // silly random number generator
    c[i] = 0;
  }

  double t1 = get_time_in_seconds();
  if (rewritten) {
    for (int i = 0; i < batch_size; i++) {
      int p = p_start[i];
      for (int j = 0; j < (500000000 / batch_size); j++) {
        c[i] += bytes[p];
        p = (p + inc) & (NUMBYTES - 1);
      }
    }
  } else {
    if (prefetch)
      printf("prefetching\n");
    int *p = (int *)malloc(batch_size * sizeof(int));
    for (int i = 0; i < batch_size; i++) {
      p[i] = p_start[i];
    }
    for (int j = 0; j < (500000000 / batch_size); j++) {
      if (prefetch) { // prefetch is a compile-time constant
        for (int i = 0; i < batch_size; i++) {
          __builtin_prefetch(
              &bytes[p[i]], 1,
              3); // https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
        }
      }
      for (int i = 0; i < batch_size; i++) {
        c[i] += bytes[p[i]];
        p[i] = (p[i] + inc) & (NUMBYTES - 1);
      }
    }
    free(p);
  }
  double t2 = get_time_in_seconds();
  printf(" seconds: %f \n bogus: ", t2 - t1);
  for (int i = 0; i < batch_size; i++) {
    printf("%u ", c[i]);
  }
  printf("\n");

  free(p_start);
  free(c);
  free(bytes);
}

int main(void) {
  int inc = 513;
  int batch_size = 32;
  demo<false>(inc, batch_size, true);

  demo<true>(inc, batch_size);
  demo<false>(inc, batch_size);

  return EXIT_SUCCESS;
}
