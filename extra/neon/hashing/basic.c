#define __USE_POSIX199309
#define __USE_XOPEN
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
//#include <arm_neon.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

const uint32_t base = 53;

// this is the basic hash function used by Java and C++ (typically)

uint32_t hash(const char *c, size_t length) {
  uint32_t answer = 0;
  for (size_t i = 0; i < length; i++) {
    answer = base * answer + c[i];
  }
  return answer;
}

// same deal with unrolling
uint32_t hash2(const char *c, size_t length) {
  uint32_t answer = 0;
  size_t i = 0;
  for (; i + 1 < length; i += 2) {
    answer = base * base * answer + base * c[i] + c[i + 1];
  }
  for (; i < length; i++) {
    answer = base * answer + c[i];
  }
  return answer;
}

// more unrolling
uint32_t hash4(const char *c, size_t length) {
  uint32_t answer = 0;
  size_t i = 0;
  for (; i + 3 < length; i += 4) {
    answer = base * base * base * base * answer + base * base * base * c[i] +
             base * base * c[i + 1] + base * c[i + 2] + c[i + 3];
  }
  for (; i < length; i++) {
    answer = base * answer + c[i];
  }
  return answer;
}

int main() {
  const int N = 1024;
  char *array = malloc(N);
  for (int k = 0; k < N; k++)
    array[k] = 12 + 3 * k;
  uint32_t expected = hash(array, N);
  uint32_t answer;

  struct timespec now;
  struct timespec last;
  long nanosec_diff;
  for (int k = 0; k < 10; ++k) {
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &last);
    answer = hash(array, N);
    if (answer != expected) {
      printf("bug");
    }
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
    nanosec_diff = now.tv_nsec - last.tv_nsec;
    printf("hash Elapsed time : %d ns.\n", nanosec_diff);

    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &last);
    answer = hash2(array, N);
    if (answer != expected) {
      printf("bug");
    }
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
    nanosec_diff = now.tv_nsec - last.tv_nsec;
    printf("hash2 Elapsed time : %d ns.\n", nanosec_diff);

    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &last);
    answer = hash4(array, N);
    if (answer != expected) {
      printf("bug");
    }
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
    nanosec_diff = now.tv_nsec - last.tv_nsec;
    printf("hash4 Elapsed time : %d ns.\n", nanosec_diff);
    printf("\n");
  }
  free(array);
}
