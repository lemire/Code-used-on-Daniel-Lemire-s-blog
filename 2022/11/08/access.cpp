#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits.h>
#include <x86intrin.h>

#ifndef PAGESIZE
#include <unistd.h>
#endif

static inline size_t page_size() {
  size_t page_size;
#if defined(PAGESIZE) // defined in limits.h
  page_size = PAGESIZE;
#else // assume some POSIX OS
  page_size = sysconf(_SC_PAGESIZE);
#endif
  return page_size;
}
enum state { UNKNOWN, RIGHT_BEFORE, AVX512_LOAD, AVX512_STORE, RIGHT_AFTER };
int status = UNKNOWN;
static void catch_function(int sig) {
  if (status == RIGHT_AFTER) {
    printf("The AVX-512 masked loads and stores did not cause a segmentation "
           "fault.\n");
  } else {
        printf("The AVX-512 code caused a segmentation "
           "fault.\n");
  }
  exit(sig);
}

int main() {
  if (signal(SIGSEGV, catch_function) == SIG_ERR) {
    fputs("An error occurred while setting a signal handler.\n", stderr);
    return EXIT_FAILURE;
  }

  uint8_t *data = new uint8_t[1024];
  size_t ps = page_size();
  // round up to the end of the page:
  uintptr_t page_limit = ps - (reinterpret_cast<uintptr_t>(data) % ps) - 1;
  __m128i ones = _mm_set1_epi8(1); // register filled with ones
  for (int z = 0;; z++) {
    status = RIGHT_BEFORE;
    data[z * ps + page_limit] = 1;
    status = AVX512_STORE;

    _mm_mask_storeu_epi8(data + z * ps + page_limit, 1, ones);
    status = AVX512_LOAD;

    __m128i oneandzeroes = _mm_maskz_loadu_epi8(1, data + z * ps + page_limit);
    if (_mm_extract_epi8(oneandzeroes, 0) != 1) {
      printf("bug");
    }

    status = RIGHT_AFTER;
    data[z * ps + page_limit + 1] = 1;
  }

  delete[] data;
  return EXIT_SUCCESS;
}