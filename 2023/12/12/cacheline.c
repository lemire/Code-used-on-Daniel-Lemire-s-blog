#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int size = 33554432; // 32 MB

// Copy elements from arr1 to arr2 with a given slice
__attribute__((noinline)) void stride_copy(unsigned char *arr1,
                                           unsigned char *arr2, int slice) {
  for (int i = 0; i < size; i += slice) {
    arr2[i] = arr1[i];
  }
}

uint64_t diff(struct timespec start, struct timespec end) {
  struct timespec temp;
  if ((end.tv_nsec - start.tv_nsec) < 0) {
    temp.tv_sec = end.tv_sec - start.tv_sec - 1;
    temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec - start.tv_sec;
    temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  }
  return temp.tv_sec * 1000000000 + temp.tv_nsec;
}

// Run the copy function with a given size and slice
__attribute__((noinline)) double run(int slice, unsigned char *arr1,
                                     unsigned char *arr2) {
  struct timespec time1, time2;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
  int times = 5;
  for (int i = 0; i < times * slice; i++) {
    stride_copy(arr1, arr2, slice);
  }
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
  double dur = diff(time1, time2) / (double)(times * slice);
  return dur;
}

// Return the average, minimum, and maximum of a function
void average_min_max(int slice, double *avg, double *min, double *max) {
  double sum = 0;
  double minimum = 0;
  double maximum = 0;
  const int times = 10;
  const int alignment = 128;
  for (int i = 0; i < times; i++) {
    // Allocate memory for the arrays
    unsigned char *arr1 = malloc(size);
    unsigned char *arr2 = malloc(size);

    double v = run(slice, arr1, arr2);
    sum += v;
    if (i == 0 || v < minimum) {
      minimum = v;
    }
    if (i == 0 || v > maximum) {
      maximum = v;
    }

    // Free the memory
    free(arr1);
    free(arr2);
  }
  *avg = sum / times;
  *min = minimum;
  *max = maximum;
}

int main() {
  for (int slice = 16; slice <= 512; slice++) {
    double a, m, M;
    average_min_max(slice, &a, &m, &M);
    printf("%10d %10.1f %10.1f %10.1f\n", slice, (double)size / a,
           (double)size / M, (double)size / m);
  }
  return 0;
}