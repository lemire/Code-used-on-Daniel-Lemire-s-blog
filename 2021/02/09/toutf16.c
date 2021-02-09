#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

__attribute__((noinline)) void copy(const uint8_t *array, size_t N,
                                    uint16_t *out) {
  memcpy(out, array, N);
}

__attribute__((noinline)) void toutf16(const uint8_t *array, size_t N,
                                       uint16_t *out) {
  for (size_t i = 0; i < N; i++) {
    out[i] = array[i];
  }
}

int main(int argc, const char **val) {
  size_t N = 1000000;
  if (argc > 1) {
    N = atoll(val[1]);
  }
  double MB = (double)N / (1024 * 1024);

  printf("N = %zu, %f MB\n", N, MB);
  uint8_t *array = (uint8_t *)malloc(N);
  uint16_t *output = (uint16_t *)malloc(N * sizeof(uint16_t));
  for (size_t i = 0; i < N; i++) {
    array[i] = i;
  }

  printf("starting experiments.\n");
  struct timespec start, stop;
  size_t total_trials = 200;

  size_t ns = 0;
  size_t nsc = 0;
  size_t trial = 0;
  for (; (trial < total_trials) || (ns < 1000000000); trial++) {
    clock_gettime(CLOCK_REALTIME, &start);
    copy(array, N, output);
    clock_gettime(CLOCK_REALTIME, &stop);
    nsc += (stop.tv_sec - start.tv_sec) * 1000000000 +
           (stop.tv_nsec - start.tv_nsec);

    clock_gettime(CLOCK_REALTIME, &start);
    toutf16(array, N, output);
    clock_gettime(CLOCK_REALTIME, &stop);
    ns += (stop.tv_sec - start.tv_sec) * 1000000000 +
          (stop.tv_nsec - start.tv_nsec);
  }

  double avg_seconds = (double)ns / 1000000000. / trial;
  double avg_seconds_c = (double)nsc / 1000000000. / trial;
  printf("toutf16: %f GB/s\n", MB / avg_seconds / 1000.0);
  printf("copy: %f GB/s\n", MB / avg_seconds_c / 1000.0);

  return EXIT_SUCCESS;
}
