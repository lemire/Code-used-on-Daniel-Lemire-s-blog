#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "benchmark.h"

typedef struct pcg_state_setseq_64 { // Internals are *Private*.
  uint64_t state;                    // RNG state.  All values are possible.
  uint64_t inc;                      // Controls which RNG sequence (stream) is
                                     // selected. Must *always* be odd.
} pcg32_random_t;

static inline uint32_t pcg32_random_r(pcg32_random_t *rng) {
  uint64_t oldstate = rng->state;
  rng->state = oldstate * 6364136223846793005ULL + rng->inc;
  uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
  uint32_t rot = oldstate >> 59u;
  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

uint32_t *create_random_array(size_t count) {
  uint32_t *targets;
  int r = posix_memalign((void **)&targets, 64, count * 64);
  if (r != 0) {
    printf("oh!\n\n");
    return NULL;
  }
  for (size_t i = 0; i < 16 * count; i++)
    targets[i] = i;
  return targets;
}

#define STRINGLENGTH 20

size_t mass_comparison(const char *bigarray1, const char *bigarray2,
                       int *comparisons, size_t N) {
  for (size_t i = 0; i < N; i++) {
    const char *s1 = bigarray1 + i * STRINGLENGTH;
    const char *s2 = bigarray2 + i * STRINGLENGTH;
    comparisons[i] = memcmp(s1, s2, STRINGLENGTH);
  }
  return N;
}

size_t mass_comparison0(const char *bigarray1, const char *bigarray2,
                        int *comparisons, size_t N) {
  for (size_t i = 0; i < N; i++) {
    const char *s1 = bigarray1 + i * STRINGLENGTH;
    const char *s2 = bigarray2 + i * STRINGLENGTH;
    comparisons[i] = (memcmp(s1, s2, STRINGLENGTH) == 0);
  }
  return N;
}

// 2 * 8 + 4 = 20
bool memeq20(const char *s1, const char *s2) {
  uint64_t w1, w2;
  memcpy(&w1, s1, sizeof(w1));
  memcpy(&w2, s2, sizeof(w2));
  bool answer1 = (w1 == w2);
  memcpy(&w1, s1 + sizeof(w1), sizeof(w1));
  memcpy(&w2, s2 + sizeof(w2), sizeof(w2));
  bool answer2 = (w1 == w2);
  uint32_t ww1, ww2;
  memcpy(&ww1, s1 + 2 * sizeof(w1), sizeof(ww1));
  memcpy(&ww2, s2 + 2 * sizeof(w1), sizeof(ww2));
  bool answer3 = (ww1 == ww2);
  return answer1 && answer2 && answer3;
}
// 2 * 8 + 4 = 20
bool branchymemeq20(const char *s1, const char *s2) {
  uint64_t w1, w2;
  memcpy(&w1, s1, sizeof(w1));
  memcpy(&w2, s2, sizeof(w2));
  if (w1 != w2)
    return false;
  memcpy(&w1, s1 + sizeof(w1), sizeof(w1));
  memcpy(&w2, s2 + sizeof(w2), sizeof(w2));
  if (w1 != w2)
    return false;
  uint32_t ww1, ww2;
  memcpy(&ww1, s1 + 2 * sizeof(w1), sizeof(ww1));
  memcpy(&ww2, s2 + 2 * sizeof(w1), sizeof(ww2));
  return (ww1 == ww2);
}

size_t mass_comparison_fast(const char *bigarray1, const char *bigarray2,
                            int *comparisons, size_t N) {
  int answer;
  for (size_t i = 0; i < N; i++) {
    const char *s1 = bigarray1 + i * STRINGLENGTH;
    const char *s2 = bigarray2 + i * STRINGLENGTH;
    comparisons[i] = memeq20(s1, s2);
  }
  return N;
}
size_t mass_comparison_branchyfast(const char *bigarray1, const char *bigarray2,
                                   int *comparisons, size_t N) {
  int answer;
  for (size_t i = 0; i < N; i++) {
    const char *s1 = bigarray1 + i * STRINGLENGTH;
    const char *s2 = bigarray2 + i * STRINGLENGTH;
    comparisons[i] = branchymemeq20(s1, s2);
  }
  return N;
}

size_t mass_comparison_bcmp(const char *bigarray1, const char *bigarray2,
                            int *comparisons, size_t N) {
  int answer;
  for (size_t i = 0; i < N; i++) {
    const char *s1 = bigarray1 + i * STRINGLENGTH;
    const char *s2 = bigarray2 + i * STRINGLENGTH;
    comparisons[i] = (bcmp(s1, s2, STRINGLENGTH) == 0);
  }
  return N;
}

// some Git dev. rolled their own:

static inline int word_cmp_32(uint32_t a, uint32_t b) {
  return memcmp(&a, &b, sizeof(uint32_t));
}

static inline int word_cmp_64(uint64_t a, uint64_t b) {
  return memcmp(&a, &b, sizeof(uint64_t));
}
struct object_id_20 {
  uint64_t data0;
  uint64_t data1;
  uint32_t data2;
};

int hashcmp(const unsigned char *sha1, const unsigned char *sha2) {

  struct object_id_20 obj1;
  memcpy(&obj1, sha1, sizeof(struct object_id_20));
  struct object_id_20 obj2;
  memcpy(&obj2, sha2, sizeof(struct object_id_20));
  if (obj1.data0 == obj2.data0) {
    if (obj1.data1 == obj2.data1) {
      if (obj1.data2 == obj2.data2) {
        return 0;
      }
      return word_cmp_32(obj1.data2, obj2.data2);
    }
    return word_cmp_64(obj1.data1, obj2.data1);
  }
  return word_cmp_64(obj1.data0, obj2.data0);
}

bool memeq20struct(const char *s1, const char *s2) {
  struct object_id_20 obj1;
  memcpy(&obj1, s1, sizeof(struct object_id_20));
  struct object_id_20 obj2;
  memcpy(&obj2, s2, sizeof(struct object_id_20));
  return (obj1.data0 == obj2.data0) && (obj1.data1 == obj2.data1) &&
         (obj1.data2 == obj2.data2);
}

size_t mass_comparison_faststruct(const char *bigarray1, const char *bigarray2,
                                  int *comparisons, size_t N) {
  int answer;
  for (size_t i = 0; i < N; i++) {
    const char *s1 = bigarray1 + i * STRINGLENGTH;
    const char *s2 = bigarray2 + i * STRINGLENGTH;
    comparisons[i] = memeq20struct(s1, s2);
  }
  return N;
}

size_t mass_comparison_hash(const char *bigarray1, const char *bigarray2,
                            int *comparisons, size_t N) {
  int answer;
  for (size_t i = 0; i < N; i++) {
    const char *s1 = bigarray1 + i * STRINGLENGTH;
    const char *s2 = bigarray2 + i * STRINGLENGTH;
    comparisons[i] = hashcmp(s1, s2);
  }
  return N;
}

void demo(size_t N) {

  char *bigarray1 = (char *)create_random_array(N * STRINGLENGTH / 4 + 1);
  char *bigarray2 = (char *)create_random_array(N * STRINGLENGTH / 4 + 1);
  for (size_t i = 0; i < N; i += (rand() % 3))
    bigarray2[i] = bigarray1[i];
  int *comparisonsA = malloc(N * sizeof(int));
  int *comparisonsB = malloc(N * sizeof(int));

  int repeat = 5;
  bool verbose = true;

  BEST_TIME(mass_comparison0(bigarray1, bigarray2, comparisonsA, N), N, ,
            repeat, N, N * STRINGLENGTH, verbose);
  BEST_TIME(mass_comparison(bigarray1, bigarray2, comparisonsA, N), N, , repeat,
            N, N * STRINGLENGTH, verbose);
  BEST_TIME(mass_comparison_fast(bigarray1, bigarray2, comparisonsB, N), N, ,
            repeat, N, N * STRINGLENGTH, verbose);
  BEST_TIME(mass_comparison_branchyfast(bigarray1, bigarray2, comparisonsB, N),
            N, , repeat, N, N * STRINGLENGTH, verbose);
  BEST_TIME(mass_comparison_faststruct(bigarray1, bigarray2, comparisonsB, N),
            N, , repeat, N, N * STRINGLENGTH, verbose);
  BEST_TIME(mass_comparison_bcmp(bigarray1, bigarray2, comparisonsB, N), N, ,
            repeat, N, N * STRINGLENGTH, verbose);
  for (size_t i = 0; i < N; i++) {
    if (comparisonsA[i] == 0)
      assert(comparisonsB[i]);
    if (comparisonsB[i])
      assert(comparisonsA[i] == 0);
  }

  BEST_TIME(mass_comparison_hash(bigarray1, bigarray2, comparisonsB, N), N, ,
            repeat, N, N * STRINGLENGTH, verbose);
  for (size_t i = 0; i < N; i++) {
    if (comparisonsA[i] > 0)
      assert(comparisonsB[i] > 0);
    if (comparisonsA[i] > 0)
      assert(comparisonsB[i] < 0);
    if (comparisonsA[i] == 0)
      assert(comparisonsB[i] == 0);
  }
  free(bigarray1);
  free(bigarray2);

  free(comparisonsA);
  free(comparisonsB);

  printf("\n");
}

int main() {
  demo(27);
  return EXIT_SUCCESS;
}
