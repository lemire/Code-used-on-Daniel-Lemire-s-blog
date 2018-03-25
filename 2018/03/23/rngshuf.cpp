// g++ -O3 -o rngshuf rngshuf.cpp && ./rngshuf
// code originally from Arseny Kapoulkine, modified by D. Lemire
// to match Java code.
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#if defined(__linux__)
double timestamp() {
  timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return double(ts.tv_sec) + 1e-9 * double(ts.tv_nsec);
}
#elif defined(_WIN32)
struct LARGE_INTEGER {
  __int64 QuadPart;
};
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(
    LARGE_INTEGER *lpPerformanceCount);
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceFrequency(
    LARGE_INTEGER *lpFrequency);

double timestamp() {
  LARGE_INTEGER freq, counter;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&counter);
  return double(counter.QuadPart) / double(freq.QuadPart);
}
#else
double timestamp() { return double(clock()) / double(CLOCKS_PER_SEC); }
#endif

#define PCG32_INITIALIZER                                                      \
  { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL }

typedef struct {
  uint64_t state;
  uint64_t inc;
} pcg32_random_t;

uint32_t pcg32_random_r(pcg32_random_t *rng) {
  uint64_t oldstate = rng->state;
  // Advance internal state
  rng->state = oldstate * 6364136223846793005ULL + (rng->inc | 1);
  // Calculate output function (XSH RR), uses old state for max ILP
  uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
  uint32_t rot = oldstate >> 59u;
  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

uint32_t random_bounded(pcg32_random_t *rng, uint32_t range) {
  uint64_t random32bit = pcg32_random_r(rng);
  uint64_t result = random32bit * range;
  return result >> 32;
}
void swap(int *a, int *b) {
  int va = *a;
  int vb = *b;
  *a = vb;
  *b = va;
}
uint32_t java_random_bounded32(pcg32_random_t *rng, uint32_t range) {
  uint32_t rkey = pcg32_random_r(rng);
  uint32_t candidate = rkey % range;
  while (rkey - candidate >
         UINT32_MAX - range + 1) { // will be predicted as false
    rkey = pcg32_random_r(rng);
    candidate = rkey % range;
  }
  return candidate;
}

static inline void java_swap(int *arr, size_t i, size_t j) {
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

void shuffle_java(int *arr, size_t size) {
  pcg32_random_t rng = PCG32_INITIALIZER;
  for (int i = size; i > 1; i--) {
    java_swap(arr, i - 1, java_random_bounded32(&rng, i));
  }
}

void shuffle_precomp(int *__restrict__ arr, size_t size,
                     int *__restrict__ indexes) {
  for (int i = size; i > 1; i--)
    java_swap(arr, i - 1, indexes[i]);
}

int main() {
  int N = 100 * 1000 * 1000;
  int *arr = new int[N];
  int *ind = new int[N];

  for (int i = 0; i < N; ++i)
    arr[i] = i;

  // for (;;)
  {
    double begin, end;
    for (int i = 0; i < N; ++i)
      arr[i] = i;
    begin = timestamp();
    {
      pcg32_random_t rng = PCG32_INITIALIZER;

      for (int i = N; i > 1; i--) {
        ind[i - 1] = java_random_bounded32(&rng, i); // number in [0,i)
      }
    }
    end = timestamp();
    printf("Reproducing the Java numbers from blog post "
           "https://lemire.me/blog/2018/03/24/"
           "when-shuffling-large-arrays-how-much-time-can-be-attributed-to-"
           "random-number-generation/#comments \n");
    printf("Caveat: we use PCG instead of the LCG from Java.\n");

    begin = timestamp();
    { shuffle_java(arr, N); }
    end = timestamp();
    printf("java-bound PCG shuffle %f s \n", end - begin);
    begin = timestamp();
    { shuffle_precomp(arr, N, ind); }
    end = timestamp();
    printf("precomp  shuffle %f s \n", end - begin);
    printf("\n\n rest is from Arseny Kapoulkine's original code: \n");

    begin = timestamp();
    {
      pcg32_random_t rng = PCG32_INITIALIZER;

      for (int i = N; i > 1; i--) {
        int p = random_bounded(&rng, i); // number in [0,i)
        swap(arr + i - 1, arr + p);      // swap the values at i-1 and p
      }
    }
    end = timestamp();
    printf("biased fast PCG shuffle %f s \n", end - begin);

    begin = timestamp();
    {
      pcg32_random_t rng = PCG32_INITIALIZER;

      for (int i = N; i > 1; i--) {
        ind[i - 1] = random_bounded(&rng, i); // number in [0,i)
      }
    }
    end = timestamp();
    printf("fast comp. of indexes %f s \n", end - begin);

    begin = timestamp();
    {
      for (int i = N; i > 1; i--) {
        int p = ind[i - 1];
        swap(arr + i - 1, arr + p); // swap the values at i-1 and p
      }
    }
    end = timestamp();
    printf("precomp shuffle %f s \n", end - begin);
    printf("\n\n\n");
  }
}
