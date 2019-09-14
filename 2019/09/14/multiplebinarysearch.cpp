#ifndef __linux__
#warning "We require Linux."
#else
#include "linux-perf-events.h"
#endif

#include <algorithm>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <x86intrin.h>

size_t branchy_search(const int *source, size_t n, int target) {
  size_t lo = 0;
  size_t hi = n;
  while (lo < hi) {
    size_t m = (lo + hi) / 2;
    if (target < source[m]) {
      hi = m;
    } else if (target > source[m]) {
      lo = m + 1;
    } else {
      return m;
    }
  }
  return hi;
}

__attribute__((noinline)) void
branchy(const std::vector<std::vector<int>> &data,
        const std::vector<int> &targets, std::vector<size_t> &solution) {
  for (size_t i = 0; i < targets.size(); i++) {
    solution[i] = branchy_search(data[i].data(), data[i].size(), targets[i]);
  }
}

size_t branchfree_search(const int *source, size_t n, int target) {
  if (n == 0)
    return 0;
  const int *base = source;
  while (n > 1) {
    size_t half = n >> 1;

    base = (base[half] < target) ? &base[half] : base;
    n -= half;
  }
  return (*base < target) + base - source;
}

__attribute__((noinline)) void
branchless(const std::vector<std::vector<int>> &data,
           const std::vector<int> &targets, std::vector<size_t> &solution) {
  for (size_t i = 0; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
  }
}

__attribute__((noinline)) void
branchless_lfence(const std::vector<std::vector<int>> &data,
                  const std::vector<int> &targets,
                  std::vector<size_t> &solution) {
  for (size_t i = 0; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
    _mm_lfence();
  }
}

void branchfree_search2(const int *source1, const int *source2, size_t n,
                        int target1, int target2, size_t *__restrict index1,
                        size_t *__restrict index2) {
  const int *base1 = source1;
  const int *base2 = source2;
  if (n == 0)
    return;
  while (n > 1) {
    size_t half = n >> 1;
    base1 = (base1[half] < target1) ? &base1[half] : base1;
    base2 = (base2[half] < target2) ? &base2[half] : base2;
    n -= half;
  }
  *index1 = (*base1 < target1) + base1 - source1;
  *index2 = (*base2 < target2) + base2 - source2;
}

__attribute__((noinline)) void
branchless2(const std::vector<std::vector<int>> &data,
            const std::vector<int> &targets, std::vector<size_t> &solution) {
  size_t i = 0;
  for (; i + 1 < targets.size(); i += 2) {
    branchfree_search2(data[i].data(), data[i + 1].data(), data[i].size(),
                       targets[i], targets[i + 1], &solution[i],
                       &solution[i + 1]);
  }
  for (; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
  }
}

__attribute__((noinline)) void
branchless2_lfence(const std::vector<std::vector<int>> &data,
                   const std::vector<int> &targets,
                   std::vector<size_t> &solution) {
  size_t i = 0;
  for (; i + 1 < targets.size(); i += 2) {
    branchfree_search2(data[i].data(), data[i + 1].data(), data[i].size(),
                       targets[i], targets[i + 1], &solution[i],
                       &solution[i + 1]);
    _mm_lfence();
  }
  for (; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
    _mm_lfence();
  }
}

__attribute__((always_inline)) inline void
branchfree_search4(const int *source1, const int *source2, const int *source3,
                   const int *source4, size_t n, int target1, int target2,
                   int target3, int target4, size_t *__restrict index1,
                   size_t *__restrict index2, size_t *__restrict index3,
                   size_t *__restrict index4) {
  const int *base1 = source1;
  const int *base2 = source2;
  const int *base3 = source3;
  const int *base4 = source4;
  if (n == 0)
    return;
  while (n > 1) {
    size_t half = n >> 1;

    base1 = (base1[half] < target1) ? &base1[half] : base1;
    base2 = (base2[half] < target2) ? &base2[half] : base2;
    base3 = (base3[half] < target3) ? &base3[half] : base3;
    base4 = (base4[half] < target4) ? &base4[half] : base4;
    n -= half;
  }
  *index1 = (*base1 < target1) + base1 - source1;
  *index2 = (*base2 < target2) + base2 - source2;
  *index3 = (*base3 < target3) + base3 - source3;
  *index4 = (*base4 < target4) + base4 - source4;
}

void fake_branchfree_search4(const int *source1, const int *source2,
                             const int *source3, const int *source4, size_t n,
                             int target1, int target2, int target3, int target4,
                             size_t *__restrict index1,
                             size_t *__restrict index2,
                             size_t *__restrict index3,
                             size_t *__restrict index4, int *__restrict bogus) {
  const int *base1 = source1;
  const int *base2 = source2;
  const int *base3 = source3;
  const int *base4 = source4;
  if (n == 0)
    return;
  int b1 = 0;
  int b2 = 0;
  int b3 = 0;
  int b4 = 0;
  while (n > 1) {
    size_t half = n >> 1;
    b1 ^= base1[half];
    b2 ^= base2[half];
    b3 ^= base3[half];
    b4 ^= base4[half];
    //*bogus ^= base1[half] ^ base2[half] ^ base3[half] ^ base4[half];
    n -= half;
  }
  *index1 = b1;
  *index2 = b2;
  *index3 = b3;
  *index4 = b4;
}

__attribute__((noinline)) void
branchless4(const std::vector<std::vector<int>> &data,
            const std::vector<int> &targets, std::vector<size_t> &solution) {
  size_t i = 0;
  for (; i + 3 < targets.size(); i += 4) {
    branchfree_search4(data[i].data(), data[i + 1].data(), data[i + 2].data(),
                       data[i + 3].data(), data[i].size(), targets[i],
                       targets[i + 1], targets[i + 2], targets[i + 3],
                       &solution[i], &solution[i + 1], &solution[i + 2],
                       &solution[i + 3]);
    _mm_lfence();
  }
  for (; i + 1 < targets.size(); i += 2) {
    branchfree_search2(data[i].data(), data[i + 1].data(), data[i].size(),
                       targets[i], targets[i + 1], &solution[i],
                       &solution[i + 1]);
    _mm_lfence();
  }
  for (; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
    _mm_lfence();
  }
}

__attribute__((noinline)) void
branchless4_lfence(const std::vector<std::vector<int>> &data,
                   const std::vector<int> &targets,
                   std::vector<size_t> &solution) {
  size_t i = 0;
  for (; i + 3 < targets.size(); i += 4) {
    branchfree_search4(data[i].data(), data[i + 1].data(), data[i + 2].data(),
                       data[i + 3].data(), data[i].size(), targets[i],
                       targets[i + 1], targets[i + 2], targets[i + 3],
                       &solution[i], &solution[i + 1], &solution[i + 2],
                       &solution[i + 3]);
  }
  for (; i + 1 < targets.size(); i += 2) {
    branchfree_search2(data[i].data(), data[i + 1].data(), data[i].size(),
                       targets[i], targets[i + 1], &solution[i],
                       &solution[i + 1]);
  }
  for (; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
  }
}

__attribute__((noinline)) void
fake_branchless4(const std::vector<std::vector<int>> &data,
                 const std::vector<int> &targets,
                 std::vector<size_t> &solution) {
  size_t i = 0;
  int bogus = 0;
  for (; i + 3 < targets.size(); i += 4) {
    fake_branchfree_search4(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i].size(), targets[i], targets[i + 1],
        targets[i + 2], targets[i + 3], &solution[i], &solution[i + 1],
        &solution[i + 2], &solution[i + 3], &bogus);
  }
  if ((bogus & 0xFCA6) == 0xFCA6)
    printf("bogus\n");
}

__attribute__((always_inline)) inline void branchfree_search8(
    const int *source1, const int *source2, const int *source3,
    const int *source4, const int *source5, const int *source6,
    const int *source7, const int *source8, size_t n, int target1, int target2,
    int target3, int target4, int target5, int target6, int target7,
    int target8, size_t *index1, size_t *index2, size_t *index3, size_t *index4,
    size_t *index5, size_t *index6, size_t *index7, size_t *index8) {
  const int *base1 = source1;
  const int *base2 = source2;
  const int *base3 = source3;
  const int *base4 = source4;
  const int *base5 = source5;
  const int *base6 = source6;
  const int *base7 = source7;
  const int *base8 = source8;
  if (n == 0)
    return;
  while (n > 1) {
    size_t half = n >> 1;

    base1 = (base1[half] < target1) ? &base1[half] : base1;
    base2 = (base2[half] < target2) ? &base2[half] : base2;
    base3 = (base3[half] < target3) ? &base3[half] : base3;
    base4 = (base4[half] < target4) ? &base4[half] : base4;
    base5 = (base5[half] < target5) ? &base5[half] : base5;
    base6 = (base6[half] < target6) ? &base6[half] : base6;
    base7 = (base7[half] < target7) ? &base7[half] : base7;
    base8 = (base8[half] < target8) ? &base8[half] : base8;

    n -= half;
  }
  *index1 = (*base1 < target1) + base1 - source1;
  *index2 = (*base2 < target2) + base2 - source2;
  *index3 = (*base3 < target3) + base3 - source3;
  *index4 = (*base4 < target4) + base4 - source4;
  *index5 = (*base5 < target5) + base5 - source5;
  *index6 = (*base6 < target6) + base6 - source6;
  *index7 = (*base7 < target7) + base7 - source7;
  *index8 = (*base8 < target8) + base8 - source8;
}

__attribute__((always_inline)) inline void fake_branchfree_search6(
    const int *source1, const int *source2, const int *source3,
    const int *source4, const int *source5, const int *source6, size_t n,
    int target1, int target2, int target3, int target4, int target5,
    int target6, size_t *index1, size_t *index2, size_t *index3, size_t *index4,
    size_t *index5, size_t *index6, int *__restrict bogus) {
  const int *base1 = source1;
  const int *base2 = source2;
  const int *base3 = source3;
  const int *base4 = source4;
  const int *base5 = source5;
  const int *base6 = source6;

  int b1 = 0;
  int b2 = 0;
  int b3 = 0;
  int b4 = 0;
  int b5 = 0;
  int b6 = 0;

  if (n == 0)
    return;
  while (n > 1) {
    size_t half = n >> 1;
    b1 ^= base1[half];
    b2 ^= base2[half];
    b3 ^= base3[half];
    b4 ^= base4[half];
    b5 ^= base5[half];
    b6 ^= base6[half];
    n -= half;
  }
  *index1 = b1;
  *index2 = b2;
  *index3 = b3;
  *index4 = b4;
  *index5 = b5;
  *index6 = b6;
}

__attribute__((always_inline)) inline void fake_branchfree_search8(
    const int *source1, const int *source2, const int *source3,
    const int *source4, const int *source5, const int *source6,
    const int *source7, const int *source8, size_t n, int target1, int target2,
    int target3, int target4, int target5, int target6, int target7,
    int target8, size_t *index1, size_t *index2, size_t *index3, size_t *index4,
    size_t *index5, size_t *index6, size_t *index7, size_t *index8) {
  const int *base1 = source1;
  const int *base2 = source2;
  const int *base3 = source3;
  const int *base4 = source4;
  const int *base5 = source5;
  const int *base6 = source6;
  const int *base7 = source7;
  const int *base8 = source8;

  int b1 = 0;
  int b2 = 0;
  int b3 = 0;
  int b4 = 0;
  int b5 = 0;
  int b6 = 0;
  int b7 = 0;
  int b8 = 0;

  if (n == 0)
    return;
  while (n > 1) {
    size_t half = n >> 1;
    b1 ^= base1[half];
    b2 ^= base2[half];
    b3 ^= base3[half];
    b4 ^= base4[half];
    b5 ^= base5[half];
    b6 ^= base6[half];
    b7 ^= base7[half];
    b8 ^= base8[half];
    n -= half;
  }
  *index1 = b1;
  *index2 = b2;
  *index3 = b3;
  *index4 = b4;
  *index5 = b5;
  *index6 = b6;
  *index7 = b7;
  *index8 = b8;
}

__attribute__((noinline)) void
fake_branchless6(const std::vector<std::vector<int>> &data,
                 const std::vector<int> &targets,
                 std::vector<size_t> &solution) {
  size_t i = 0;
  int bogus = 0;
  for (; i + 5 < targets.size(); i += 6) {
    fake_branchfree_search6(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i].size(), targets[i], targets[i + 1], targets[i + 2],
        targets[i + 3], targets[i + 4], targets[i + 5], &solution[i],
        &solution[i + 1], &solution[i + 2], &solution[i + 3], &solution[i + 4],
        &solution[i + 5], &bogus);
  }
  for (; i + 3 < targets.size(); i += 4) {
    fake_branchfree_search4(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i].size(), targets[i], targets[i + 1],
        targets[i + 2], targets[i + 3], &solution[i], &solution[i + 1],
        &solution[i + 2], &solution[i + 3], &bogus);
  }
  if ((bogus & 0xFCA6) == 0xFCA6)
    printf("bogus\n");
}

__attribute__((noinline)) void
fake_branchless8(const std::vector<std::vector<int>> &data,
                 const std::vector<int> &targets,
                 std::vector<size_t> &solution) {
  size_t i = 0;
  for (; i + 7 < targets.size(); i += 8) {
    fake_branchfree_search8(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i].size(), targets[i],
        targets[i + 1], targets[i + 2], targets[i + 3], targets[i + 4],
        targets[i + 5], targets[i + 6], targets[i + 7], &solution[i],
        &solution[i + 1], &solution[i + 2], &solution[i + 3], &solution[i + 4],
        &solution[i + 5], &solution[i + 6], &solution[i + 7]);
  }
}

__attribute__((noinline)) void
branchless8(const std::vector<std::vector<int>> &data,
            const std::vector<int> &targets, std::vector<size_t> &solution) {
  size_t i = 0;
  for (; i + 7 < targets.size(); i += 8) {
    branchfree_search8(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i].size(), targets[i],
        targets[i + 1], targets[i + 2], targets[i + 3], targets[i + 4],
        targets[i + 5], targets[i + 6], targets[i + 7], &solution[i],
        &solution[i + 1], &solution[i + 2], &solution[i + 3], &solution[i + 4],
        &solution[i + 5], &solution[i + 6], &solution[i + 7]);
  }
  for (; i + 3 < targets.size(); i += 4) {
    branchfree_search4(data[i].data(), data[i + 1].data(), data[i + 2].data(),
                       data[i + 3].data(), data[i].size(), targets[i],
                       targets[i + 1], targets[i + 2], targets[i + 3],
                       &solution[i], &solution[i + 1], &solution[i + 2],
                       &solution[i + 3]);
  }
  for (; i + 1 < targets.size(); i += 2) {
    branchfree_search2(data[i].data(), data[i + 1].data(), data[i].size(),
                       targets[i], targets[i + 1], &solution[i],
                       &solution[i + 1]);
  }
  for (; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
  }
}

__attribute__((noinline)) void
branchless8_lfence(const std::vector<std::vector<int>> &data,
                   const std::vector<int> &targets,
                   std::vector<size_t> &solution) {
  size_t i = 0;
  for (; i + 7 < targets.size(); i += 8) {
    branchfree_search8(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i].size(), targets[i],
        targets[i + 1], targets[i + 2], targets[i + 3], targets[i + 4],
        targets[i + 5], targets[i + 6], targets[i + 7], &solution[i],
        &solution[i + 1], &solution[i + 2], &solution[i + 3], &solution[i + 4],
        &solution[i + 5], &solution[i + 6], &solution[i + 7]);
    _mm_lfence();
  }
  for (; i + 3 < targets.size(); i += 4) {
    branchfree_search4(data[i].data(), data[i + 1].data(), data[i + 2].data(),
                       data[i + 3].data(), data[i].size(), targets[i],
                       targets[i + 1], targets[i + 2], targets[i + 3],
                       &solution[i], &solution[i + 1], &solution[i + 2],
                       &solution[i + 3]);
    _mm_lfence();
  }
  for (; i + 1 < targets.size(); i += 2) {
    branchfree_search2(data[i].data(), data[i + 1].data(), data[i].size(),
                       targets[i], targets[i + 1], &solution[i],
                       &solution[i + 1]);
    _mm_lfence();
  }
  for (; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
    _mm_lfence();
  }
}

__attribute__((always_inline)) inline void branchfree_search16(
    const int *source1, const int *source2, const int *source3,
    const int *source4, const int *source5, const int *source6,
    const int *source7, const int *source8, const int *source9,
    const int *source10, const int *source11, const int *source12,
    const int *source13, const int *source14, const int *source15,
    const int *source16, size_t n, int target1, int target2, int target3,
    int target4, int target5, int target6, int target7, int target8,
    int target9, int target10, int target11, int target12, int target13,
    int target14, int target15, int target16, size_t *index1, size_t *index2,
    size_t *index3, size_t *index4, size_t *index5, size_t *index6,
    size_t *index7, size_t *index8, size_t *index9, size_t *index10,
    size_t *index11, size_t *index12, size_t *index13, size_t *index14,
    size_t *index15, size_t *index16) {
  const int *base1 = source1;
  const int *base2 = source2;
  const int *base3 = source3;
  const int *base4 = source4;
  const int *base5 = source5;
  const int *base6 = source6;
  const int *base7 = source7;
  const int *base8 = source8;
  const int *base9 = source9;
  const int *base10 = source10;
  const int *base11 = source11;
  const int *base12 = source12;
  const int *base13 = source13;
  const int *base14 = source14;
  const int *base15 = source15;
  const int *base16 = source16;
  if (n == 0)
    return;
  while (n > 1) {
    size_t half = n >> 1;
    base1 = (base1[half] < target1) ? &base1[half] : base1;
    base2 = (base2[half] < target2) ? &base2[half] : base2;
    base3 = (base3[half] < target3) ? &base3[half] : base3;
    base4 = (base4[half] < target4) ? &base4[half] : base4;
    base5 = (base5[half] < target5) ? &base5[half] : base5;
    base6 = (base6[half] < target6) ? &base6[half] : base6;
    base7 = (base7[half] < target7) ? &base7[half] : base7;
    base8 = (base8[half] < target8) ? &base8[half] : base8;
    base9 = (base9[half] < target9) ? &base9[half] : base9;
    base10 = (base10[half] < target10) ? &base10[half] : base10;
    base11 = (base11[half] < target11) ? &base11[half] : base11;
    base12 = (base12[half] < target12) ? &base12[half] : base12;
    base13 = (base13[half] < target13) ? &base13[half] : base13;
    base14 = (base14[half] < target14) ? &base14[half] : base14;
    base15 = (base15[half] < target15) ? &base15[half] : base15;
    base16 = (base16[half] < target16) ? &base16[half] : base16;

    n -= half;
  }
  *index1 = (*base1 < target1) + base1 - source1;
  *index2 = (*base2 < target2) + base2 - source2;
  *index3 = (*base3 < target3) + base3 - source3;
  *index4 = (*base4 < target4) + base4 - source4;
  *index5 = (*base5 < target5) + base5 - source5;
  *index6 = (*base6 < target6) + base6 - source6;
  *index7 = (*base7 < target7) + base7 - source7;
  *index8 = (*base8 < target8) + base8 - source8;
  *index9 = (*base9 < target9) + base9 - source9;
  *index10 = (*base10 < target10) + base10 - source10;
  *index11 = (*base11 < target11) + base11 - source11;
  *index12 = (*base12 < target12) + base12 - source12;
  *index13 = (*base13 < target13) + base13 - source13;
  *index14 = (*base14 < target14) + base14 - source14;
  *index15 = (*base15 < target15) + base15 - source15;
  *index16 = (*base16 < target16) + base16 - source16;
}

__attribute__((always_inline)) inline void branchfree_search32(
    const int *source1, const int *source2, const int *source3,
    const int *source4, const int *source5, const int *source6,
    const int *source7, const int *source8, const int *source9,
    const int *source10, const int *source11, const int *source12,
    const int *source13, const int *source14, const int *source15,
    const int *source16, const int *source17, const int *source18,
    const int *source19, const int *source20, const int *source21,
    const int *source22, const int *source23, const int *source24,
    const int *source25, const int *source26, const int *source27,
    const int *source28, const int *source29, const int *source30,
    const int *source31, const int *source32, size_t n, const int target1,
    const int target2, const int target3, const int target4, const int target5,
    const int target6, const int target7, const int target8, const int target9,
    const int target10, const int target11, const int target12,
    const int target13, const int target14, const int target15,
    const int target16, const int target17, const int target18,
    const int target19, const int target20, const int target21,
    const int target22, const int target23, const int target24,
    const int target25, const int target26, const int target27,
    const int target28, const int target29, const int target30,
    const int target31, const int target32, size_t *index1, size_t *index2,
    size_t *index3, size_t *index4, size_t *index5, size_t *index6,
    size_t *index7, size_t *index8, size_t *index9, size_t *index10,
    size_t *index11, size_t *index12, size_t *index13, size_t *index14,
    size_t *index15, size_t *index16, size_t *index17, size_t *index18,
    size_t *index19, size_t *index20, size_t *index21, size_t *index22,
    size_t *index23, size_t *index24, size_t *index25, size_t *index26,
    size_t *index27, size_t *index28, size_t *index29, size_t *index30,
    size_t *index31, size_t *index32) {
  const int *base1 = source1;
  const int *base2 = source2;
  const int *base3 = source3;
  const int *base4 = source4;
  const int *base5 = source5;
  const int *base6 = source6;
  const int *base7 = source7;
  const int *base8 = source8;
  const int *base9 = source9;
  const int *base10 = source10;
  const int *base11 = source11;
  const int *base12 = source12;
  const int *base13 = source13;
  const int *base14 = source14;
  const int *base15 = source15;
  const int *base16 = source16;
  const int *base17 = source17;
  const int *base18 = source18;
  const int *base19 = source19;
  const int *base20 = source20;
  const int *base21 = source21;
  const int *base22 = source22;
  const int *base23 = source23;
  const int *base24 = source24;
  const int *base25 = source25;
  const int *base26 = source26;
  const int *base27 = source27;
  const int *base28 = source28;
  const int *base29 = source29;
  const int *base30 = source30;
  const int *base31 = source31;
  const int *base32 = source32;
  if (n == 0)
    return;
  while (n > 1) {
    size_t half = n >> 1;
    base1 = (base1[half] < target1) ? &base1[half] : base1;
    base2 = (base2[half] < target2) ? &base2[half] : base2;
    base3 = (base3[half] < target3) ? &base3[half] : base3;
    base4 = (base4[half] < target4) ? &base4[half] : base4;
    base5 = (base5[half] < target5) ? &base5[half] : base5;
    base6 = (base6[half] < target6) ? &base6[half] : base6;
    base7 = (base7[half] < target7) ? &base7[half] : base7;
    base8 = (base8[half] < target8) ? &base8[half] : base8;
    base9 = (base9[half] < target9) ? &base9[half] : base9;
    base10 = (base10[half] < target10) ? &base10[half] : base10;
    base11 = (base11[half] < target11) ? &base11[half] : base11;
    base12 = (base12[half] < target12) ? &base12[half] : base12;
    base13 = (base13[half] < target13) ? &base13[half] : base13;
    base14 = (base14[half] < target14) ? &base14[half] : base14;
    base15 = (base15[half] < target15) ? &base15[half] : base15;
    base16 = (base16[half] < target16) ? &base16[half] : base16;
    base17 = (base17[half] < target17) ? &base17[half] : base17;
    base18 = (base18[half] < target18) ? &base18[half] : base18;
    base19 = (base19[half] < target19) ? &base19[half] : base19;
    base20 = (base20[half] < target20) ? &base20[half] : base20;
    base21 = (base21[half] < target21) ? &base21[half] : base21;
    base22 = (base22[half] < target22) ? &base22[half] : base22;
    base23 = (base23[half] < target23) ? &base23[half] : base23;
    base24 = (base24[half] < target24) ? &base24[half] : base24;
    base25 = (base25[half] < target25) ? &base25[half] : base25;
    base26 = (base26[half] < target26) ? &base26[half] : base26;
    base27 = (base27[half] < target27) ? &base27[half] : base27;
    base28 = (base28[half] < target28) ? &base28[half] : base28;
    base29 = (base29[half] < target29) ? &base29[half] : base29;
    base30 = (base30[half] < target30) ? &base30[half] : base30;
    base31 = (base31[half] < target31) ? &base31[half] : base31;
    base32 = (base32[half] < target32) ? &base32[half] : base32;
    n -= half;
  }
  *index1 = (*base1 < target1) + base1 - source1;
  *index2 = (*base2 < target2) + base2 - source2;
  *index3 = (*base3 < target3) + base3 - source3;
  *index4 = (*base4 < target4) + base4 - source4;
  *index5 = (*base5 < target5) + base5 - source5;
  *index6 = (*base6 < target6) + base6 - source6;
  *index7 = (*base7 < target7) + base7 - source7;
  *index8 = (*base8 < target8) + base8 - source8;
  *index9 = (*base9 < target9) + base9 - source9;
  *index10 = (*base10 < target10) + base10 - source10;
  *index11 = (*base11 < target11) + base11 - source11;
  *index12 = (*base12 < target12) + base12 - source12;
  *index13 = (*base13 < target13) + base13 - source13;
  *index14 = (*base14 < target14) + base14 - source14;
  *index15 = (*base15 < target15) + base15 - source15;
  *index16 = (*base16 < target16) + base16 - source16;
  *index17 = (*base17 < target17) + base17 - source17;
  *index18 = (*base18 < target18) + base18 - source18;
  *index19 = (*base19 < target19) + base19 - source19;
  *index20 = (*base20 < target20) + base20 - source20;
  *index21 = (*base21 < target21) + base21 - source21;
  *index22 = (*base22 < target22) + base22 - source22;
  *index23 = (*base23 < target23) + base23 - source23;
  *index24 = (*base24 < target24) + base24 - source24;
  *index25 = (*base25 < target25) + base25 - source25;
  *index26 = (*base26 < target26) + base26 - source26;
  *index27 = (*base27 < target27) + base27 - source27;
  *index28 = (*base28 < target28) + base28 - source28;
  *index29 = (*base29 < target29) + base29 - source29;
  *index30 = (*base30 < target30) + base30 - source30;
  *index31 = (*base31 < target31) + base31 - source31;
  *index32 = (*base32 < target32) + base32 - source32;
}

__attribute__((noinline)) void
branchless16(const std::vector<std::vector<int>> &data,
             const std::vector<int> &targets, std::vector<size_t> &solution) {
  size_t i = 0;
  for (; i + 15 < targets.size(); i += 16) {
    branchfree_search16(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i + 8].data(),
        data[i + 9].data(), data[i + 10].data(), data[i + 11].data(),
        data[i + 12].data(), data[i + 13].data(), data[i + 14].data(),
        data[i + 15].data(), data[i].size(), targets[i], targets[i + 1],
        targets[i + 2], targets[i + 3], targets[i + 4], targets[i + 5],
        targets[i + 6], targets[i + 7], targets[i + 8], targets[i + 9],
        targets[i + 10], targets[i + 11], targets[i + 12], targets[i + 13],
        targets[i + 14], targets[i + 15], &solution[i], &solution[i + 1],
        &solution[i + 2], &solution[i + 3], &solution[i + 4], &solution[i + 5],
        &solution[i + 6], &solution[i + 7], &solution[i + 8], &solution[i + 9],
        &solution[i + 10], &solution[i + 11], &solution[i + 12],
        &solution[i + 13], &solution[i + 14], &solution[i + 15]);
  }

  for (; i + 7 < targets.size(); i += 8) {
    branchfree_search8(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i].size(), targets[i],
        targets[i + 1], targets[i + 2], targets[i + 3], targets[i + 4],
        targets[i + 5], targets[i + 6], targets[i + 7], &solution[i],
        &solution[i + 1], &solution[i + 2], &solution[i + 3], &solution[i + 4],
        &solution[i + 5], &solution[i + 6], &solution[i + 7]);
  }
  for (; i + 3 < targets.size(); i += 4) {
    branchfree_search4(data[i].data(), data[i + 1].data(), data[i + 2].data(),
                       data[i + 3].data(), data[i].size(), targets[i],
                       targets[i + 1], targets[i + 2], targets[i + 3],
                       &solution[i], &solution[i + 1], &solution[i + 2],
                       &solution[i + 3]);
  }
  for (; i + 1 < targets.size(); i += 2) {
    branchfree_search2(data[i].data(), data[i + 1].data(), data[i].size(),
                       targets[i], targets[i + 1], &solution[i],
                       &solution[i + 1]);
  }
  for (; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
  }
}

__attribute__((noinline)) void
branchless32(const std::vector<std::vector<int>> &data,
             const std::vector<int> &targets, std::vector<size_t> &solution) {
  size_t i = 0;
  for (; i + 31 < targets.size(); i += 32) {
    branchfree_search32(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i + 8].data(),
        data[i + 9].data(), data[i + 10].data(), data[i + 11].data(),
        data[i + 12].data(), data[i + 13].data(), data[i + 14].data(),
        data[i + 15].data(), data[i + 16].data(), data[i + 17].data(),
        data[i + 18].data(), data[i + 19].data(), data[i + 20].data(),
        data[i + 21].data(), data[i + 22].data(), data[i + 23].data(),
        data[i + 24].data(), data[i + 25].data(), data[i + 26].data(),
        data[i + 27].data(), data[i + 28].data(), data[i + 29].data(),
        data[i + 30].data(), data[i + 31].data(), data[i].size(), targets[i],
        targets[i + 1], targets[i + 2], targets[i + 3], targets[i + 4],
        targets[i + 5], targets[i + 6], targets[i + 7], targets[i + 8],
        targets[i + 9], targets[i + 10], targets[i + 11], targets[i + 12],
        targets[i + 13], targets[i + 14], targets[i + 15], targets[i + 16],
        targets[i + 17], targets[i + 18], targets[i + 19], targets[i + 20],
        targets[i + 21], targets[i + 22], targets[i + 23], targets[i + 24],
        targets[i + 25], targets[i + 26], targets[i + 27], targets[i + 28],
        targets[i + 29], targets[i + 30], targets[i + 31], &solution[i],
        &solution[i + 1], &solution[i + 2], &solution[i + 3], &solution[i + 4],
        &solution[i + 5], &solution[i + 6], &solution[i + 7], &solution[i + 8],
        &solution[i + 9], &solution[i + 10], &solution[i + 11],
        &solution[i + 12], &solution[i + 13], &solution[i + 14],
        &solution[i + 15], &solution[i + 16], &solution[i + 17],
        &solution[i + 18], &solution[i + 19], &solution[i + 20],
        &solution[i + 21], &solution[i + 22], &solution[i + 23],
        &solution[i + 24], &solution[i + 25], &solution[i + 26],
        &solution[i + 27], &solution[i + 28], &solution[i + 29],
        &solution[i + 30], &solution[i + 31]);
  }

  for (; i + 15 < targets.size(); i += 16) {
    branchfree_search16(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i + 8].data(),
        data[i + 9].data(), data[i + 10].data(), data[i + 11].data(),
        data[i + 12].data(), data[i + 13].data(), data[i + 14].data(),
        data[i + 15].data(), data[i].size(), targets[i], targets[i + 1],
        targets[i + 2], targets[i + 3], targets[i + 4], targets[i + 5],
        targets[i + 6], targets[i + 7], targets[i + 8], targets[i + 9],
        targets[i + 10], targets[i + 11], targets[i + 12], targets[i + 13],
        targets[i + 14], targets[i + 15], &solution[i], &solution[i + 1],
        &solution[i + 2], &solution[i + 3], &solution[i + 4], &solution[i + 5],
        &solution[i + 6], &solution[i + 7], &solution[i + 8], &solution[i + 9],
        &solution[i + 10], &solution[i + 11], &solution[i + 12],
        &solution[i + 13], &solution[i + 14], &solution[i + 15]);
  }

  for (; i + 7 < targets.size(); i += 8) {
    branchfree_search8(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i].size(), targets[i],
        targets[i + 1], targets[i + 2], targets[i + 3], targets[i + 4],
        targets[i + 5], targets[i + 6], targets[i + 7], &solution[i],
        &solution[i + 1], &solution[i + 2], &solution[i + 3], &solution[i + 4],
        &solution[i + 5], &solution[i + 6], &solution[i + 7]);
  }
  for (; i + 3 < targets.size(); i += 4) {
    branchfree_search4(data[i].data(), data[i + 1].data(), data[i + 2].data(),
                       data[i + 3].data(), data[i].size(), targets[i],
                       targets[i + 1], targets[i + 2], targets[i + 3],
                       &solution[i], &solution[i + 1], &solution[i + 2],
                       &solution[i + 3]);
  }
  for (; i + 1 < targets.size(); i += 2) {
    branchfree_search2(data[i].data(), data[i + 1].data(), data[i].size(),
                       targets[i], targets[i + 1], &solution[i],
                       &solution[i + 1]);
  }
  for (; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
  }
}

__attribute__((noinline)) void
branchless16_lfence(const std::vector<std::vector<int>> &data,
                    const std::vector<int> &targets,
                    std::vector<size_t> &solution) {
  size_t i = 0;
  for (; i + 15 < targets.size(); i += 16) {
    branchfree_search16(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i + 8].data(),
        data[i + 9].data(), data[i + 10].data(), data[i + 11].data(),
        data[i + 12].data(), data[i + 13].data(), data[i + 14].data(),
        data[i + 15].data(), data[i].size(), targets[i], targets[i + 1],
        targets[i + 2], targets[i + 3], targets[i + 4], targets[i + 5],
        targets[i + 6], targets[i + 7], targets[i + 8], targets[i + 9],
        targets[i + 10], targets[i + 11], targets[i + 12], targets[i + 13],
        targets[i + 14], targets[i + 15], &solution[i], &solution[i + 1],
        &solution[i + 2], &solution[i + 3], &solution[i + 4], &solution[i + 5],
        &solution[i + 6], &solution[i + 7], &solution[i + 8], &solution[i + 9],
        &solution[i + 10], &solution[i + 11], &solution[i + 12],
        &solution[i + 13], &solution[i + 14], &solution[i + 15]);
    _mm_lfence();
  }

  for (; i + 7 < targets.size(); i += 8) {
    branchfree_search8(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i].size(), targets[i],
        targets[i + 1], targets[i + 2], targets[i + 3], targets[i + 4],
        targets[i + 5], targets[i + 6], targets[i + 7], &solution[i],
        &solution[i + 1], &solution[i + 2], &solution[i + 3], &solution[i + 4],
        &solution[i + 5], &solution[i + 6], &solution[i + 7]);
    _mm_lfence();
  }
  for (; i + 3 < targets.size(); i += 4) {
    branchfree_search4(data[i].data(), data[i + 1].data(), data[i + 2].data(),
                       data[i + 3].data(), data[i].size(), targets[i],
                       targets[i + 1], targets[i + 2], targets[i + 3],
                       &solution[i], &solution[i + 1], &solution[i + 2],
                       &solution[i + 3]);
    _mm_lfence();
  }
  for (; i + 1 < targets.size(); i += 2) {
    branchfree_search2(data[i].data(), data[i + 1].data(), data[i].size(),
                       targets[i], targets[i + 1], &solution[i],
                       &solution[i + 1]);
    _mm_lfence();
  }
  for (; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
    _mm_lfence();
  }
}

__attribute__((noinline)) void
branchless32_lfence(const std::vector<std::vector<int>> &data,
                    const std::vector<int> &targets,
                    std::vector<size_t> &solution) {
  size_t i = 0;
  for (; i + 31 < targets.size(); i += 32) {
    branchfree_search32(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i + 8].data(),
        data[i + 9].data(), data[i + 10].data(), data[i + 11].data(),
        data[i + 12].data(), data[i + 13].data(), data[i + 14].data(),
        data[i + 15].data(), data[i + 16].data(), data[i + 17].data(),
        data[i + 18].data(), data[i + 19].data(), data[i + 20].data(),
        data[i + 21].data(), data[i + 22].data(), data[i + 23].data(),
        data[i + 24].data(), data[i + 25].data(), data[i + 26].data(),
        data[i + 27].data(), data[i + 28].data(), data[i + 29].data(),
        data[i + 30].data(), data[i + 31].data(), data[i].size(), targets[i],
        targets[i + 1], targets[i + 2], targets[i + 3], targets[i + 4],
        targets[i + 5], targets[i + 6], targets[i + 7], targets[i + 8],
        targets[i + 9], targets[i + 10], targets[i + 11], targets[i + 12],
        targets[i + 13], targets[i + 14], targets[i + 15], targets[i + 16],
        targets[i + 17], targets[i + 18], targets[i + 19], targets[i + 20],
        targets[i + 21], targets[i + 22], targets[i + 23], targets[i + 24],
        targets[i + 25], targets[i + 26], targets[i + 27], targets[i + 28],
        targets[i + 29], targets[i + 30], targets[i + 31], &solution[i],
        &solution[i + 1], &solution[i + 2], &solution[i + 3], &solution[i + 4],
        &solution[i + 5], &solution[i + 6], &solution[i + 7], &solution[i + 8],
        &solution[i + 9], &solution[i + 10], &solution[i + 11],
        &solution[i + 12], &solution[i + 13], &solution[i + 14],
        &solution[i + 15], &solution[i + 16], &solution[i + 17],
        &solution[i + 18], &solution[i + 19], &solution[i + 20],
        &solution[i + 21], &solution[i + 22], &solution[i + 23],
        &solution[i + 24], &solution[i + 25], &solution[i + 26],
        &solution[i + 27], &solution[i + 28], &solution[i + 29],
        &solution[i + 30], &solution[i + 31]);
    _mm_lfence();
  }

  for (; i + 15 < targets.size(); i += 16) {
    branchfree_search16(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i + 8].data(),
        data[i + 9].data(), data[i + 10].data(), data[i + 11].data(),
        data[i + 12].data(), data[i + 13].data(), data[i + 14].data(),
        data[i + 15].data(), data[i].size(), targets[i], targets[i + 1],
        targets[i + 2], targets[i + 3], targets[i + 4], targets[i + 5],
        targets[i + 6], targets[i + 7], targets[i + 8], targets[i + 9],
        targets[i + 10], targets[i + 11], targets[i + 12], targets[i + 13],
        targets[i + 14], targets[i + 15], &solution[i], &solution[i + 1],
        &solution[i + 2], &solution[i + 3], &solution[i + 4], &solution[i + 5],
        &solution[i + 6], &solution[i + 7], &solution[i + 8], &solution[i + 9],
        &solution[i + 10], &solution[i + 11], &solution[i + 12],
        &solution[i + 13], &solution[i + 14], &solution[i + 15]);
    _mm_lfence();
  }

  for (; i + 7 < targets.size(); i += 8) {
    branchfree_search8(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i].size(), targets[i],
        targets[i + 1], targets[i + 2], targets[i + 3], targets[i + 4],
        targets[i + 5], targets[i + 6], targets[i + 7], &solution[i],
        &solution[i + 1], &solution[i + 2], &solution[i + 3], &solution[i + 4],
        &solution[i + 5], &solution[i + 6], &solution[i + 7]);
    _mm_lfence();
  }
  for (; i + 3 < targets.size(); i += 4) {
    branchfree_search4(data[i].data(), data[i + 1].data(), data[i + 2].data(),
                       data[i + 3].data(), data[i].size(), targets[i],
                       targets[i + 1], targets[i + 2], targets[i + 3],
                       &solution[i], &solution[i + 1], &solution[i + 2],
                       &solution[i + 3]);
    _mm_lfence();
  }
  for (; i + 1 < targets.size(); i += 2) {
    branchfree_search2(data[i].data(), data[i + 1].data(), data[i].size(),
                       targets[i], targets[i + 1], &solution[i],
                       &solution[i + 1]);
    _mm_lfence();
  }
  for (; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
    _mm_lfence();
  }
}

__attribute__((always_inline)) inline void fake_branchfree_search16(
    const int *source1, const int *source2, const int *source3,
    const int *source4, const int *source5, const int *source6,
    const int *source7, const int *source8, const int *source9,
    const int *source10, const int *source11, const int *source12,
    const int *source13, const int *source14, const int *source15,
    const int *source16, size_t n, int target1, int target2, int target3,
    int target4, int target5, int target6, int target7, int target8,
    int target9, int target10, int target11, int target12, int target13,
    int target14, int target15, int target16, size_t *index1, size_t *index2,
    size_t *index3, size_t *index4, size_t *index5, size_t *index6,
    size_t *index7, size_t *index8, size_t *index9, size_t *index10,
    size_t *index11, size_t *index12, size_t *index13, size_t *index14,
    size_t *index15, size_t *index16, int *__restrict bogus) {
  const int *base1 = source1;
  const int *base2 = source2;
  const int *base3 = source3;
  const int *base4 = source4;
  const int *base5 = source5;
  const int *base6 = source6;
  const int *base7 = source7;
  const int *base8 = source8;
  const int *base9 = source9;
  const int *base10 = source10;
  const int *base11 = source11;
  const int *base12 = source12;
  const int *base13 = source13;
  const int *base14 = source14;
  const int *base15 = source15;
  const int *base16 = source16;
  if (n == 0)
    return;
  while (n > 1) {
    size_t half = n >> 1;
    *bogus ^= base1[half] ^ base2[half] ^ base3[half] ^ base4[half] ^
              base5[half] ^ base6[half] ^ base7[half] ^ base8[half] ^
              base9[half] ^ base10[half] ^ base11[half] ^ base12[half] ^
              base13[half] ^ base14[half] ^ base15[half] ^ base16[half];
    n -= half;
  }
}

__attribute__((noinline)) void
fake_branchless16(const std::vector<std::vector<int>> &data,
                  const std::vector<int> &targets,
                  std::vector<size_t> &solution) {
  size_t i = 0;
  int bogus = 0;
  for (; i + 15 < targets.size(); i += 16) {
    fake_branchfree_search16(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i + 8].data(),
        data[i + 9].data(), data[i + 10].data(), data[i + 11].data(),
        data[i + 12].data(), data[i + 13].data(), data[i + 14].data(),
        data[i + 15].data(), data[i].size(), targets[i], targets[i + 1],
        targets[i + 2], targets[i + 3], targets[i + 4], targets[i + 5],
        targets[i + 6], targets[i + 7], targets[i + 8], targets[i + 9],
        targets[i + 10], targets[i + 11], targets[i + 12], targets[i + 13],
        targets[i + 14], targets[i + 15], &solution[i], &solution[i + 1],
        &solution[i + 2], &solution[i + 3], &solution[i + 4], &solution[i + 5],
        &solution[i + 6], &solution[i + 7], &solution[i + 8], &solution[i + 9],
        &solution[i + 10], &solution[i + 11], &solution[i + 12],
        &solution[i + 13], &solution[i + 14], &solution[i + 15], &bogus);
  }

  for (; i + 7 < targets.size(); i += 8) {
    fake_branchfree_search8(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i + 4].data(), data[i + 5].data(),
        data[i + 6].data(), data[i + 7].data(), data[i].size(), targets[i],
        targets[i + 1], targets[i + 2], targets[i + 3], targets[i + 4],
        targets[i + 5], targets[i + 6], targets[i + 7], &solution[i],
        &solution[i + 1], &solution[i + 2], &solution[i + 3], &solution[i + 4],
        &solution[i + 5], &solution[i + 6], &solution[i + 7]);
  }
  for (; i + 3 < targets.size(); i += 4) {
    fake_branchfree_search4(
        data[i].data(), data[i + 1].data(), data[i + 2].data(),
        data[i + 3].data(), data[i].size(), targets[i], targets[i + 1],
        targets[i + 2], targets[i + 3], &solution[i], &solution[i + 1],
        &solution[i + 2], &solution[i + 3], &bogus);
  }
  if ((bogus & 0xFCA6) == 0xFCA6)
    printf("bogus\n");
}

__attribute__((noinline)) size_t
cacheline_access(const std::vector<std::vector<int>> &data,
                 int *__restrict counter) {
  size_t hw = 0;
  for (size_t i = 0; i < data.size(); i++) {
    const int *__restrict z = data[i].data();
    const size_t len = data[i].size();
    for (size_t c = 0; c < len; c += 64 / sizeof(int)) {
      *counter += z[c];
      hw++;
    }
  }
  return hw;
}

__attribute__((noinline)) size_t
element_access(const std::vector<std::vector<int>> &data,
               int *__restrict counter) {
  size_t hw = 0;
  for (size_t i = 0; i < data.size(); i++) {
    const int *__restrict z = data[i].data();
    const size_t len = data[i].size();
    for (size_t c = 0; c < len; c += 1) {
      *counter += z[c];
      hw++;
    }
  }
  return hw;
}

inline uint64_t splitmix64_stateless(uint64_t index) {
  uint64_t z = (index + UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

void printvec(std::vector<unsigned long long> evts, size_t volume) {
  printf("%.2f cycles  %.2f instr  %.4f branch miss  %.2f cache ref %.2f cache "
         "miss \n",
         evts[0] * 1.0 / volume, evts[1] * 1.0 / volume, evts[2] * 1.0 / volume,
         evts[3] * 1.0 / volume, evts[4] * 1.0 / volume);
}

void demo(size_t howmany, size_t N) {
  std::cout << "Initializing... " << howmany * N * sizeof(int) / (1024 * 1024)
            << " MB" << std::endl;
  int levels = 0;
  while ((1ULL << levels) < N) {
    levels++;
  }
  std::cout << "binary search over " << levels << " steps " << std::endl;
  int cl_levels = 0;
  while ((sizeof(int) << cl_levels) < 64) {
    cl_levels++;
  }
  std::cout << "cache line levels " << cl_levels << std::endl;
  std::cout << "approx. cache lines accesses " << howmany * (levels - cl_levels)
            << std::endl;
  std::cout << "approx. access volume "
            << howmany * (levels - cl_levels) * 64 / (1024) << " kB"
            << std::endl;

  std::vector<std::vector<int>> data;
  std::vector<int> targets;
  for (size_t i = 0; i < howmany; i++) {
    data.emplace_back(std::vector<int>(N));
    for (size_t z = 0; z < N; z++) {
      data[i][z] = splitmix64_stateless(i * N + z);
    }
    // targets.push_back(data[i][0]);
    targets.push_back(data[i][0]);
    std::sort(data[i].begin(), data[i].end());
    printf(".");
    fflush(NULL);
  }
  printf("\n");
  std::vector<size_t> solution(howmany);

  std::cout << "Starting benchmark..." << std::endl;

  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);
  evts.push_back(PERF_COUNT_HW_BRANCH_MISSES);
  evts.push_back(PERF_COUNT_HW_CACHE_REFERENCES);
  evts.push_back(PERF_COUNT_HW_CACHE_MISSES);
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);

  std::vector<unsigned long long> results_cacheline;
  results_cacheline.resize(evts.size());
  std::vector<unsigned long long> results_branchy;
  results_branchy.resize(evts.size());
  std::vector<unsigned long long> results_fake_branchless4;
  results_fake_branchless4.resize(evts.size());
  std::vector<unsigned long long> results_fake_branchless6;
  results_fake_branchless6.resize(evts.size());
  std::vector<unsigned long long> results_fake_branchless8;
  results_fake_branchless8.resize(evts.size());
  std::vector<unsigned long long> results_branchless;
  results_branchless.resize(evts.size());
  std::vector<unsigned long long> results_branchless2;
  results_branchless2.resize(evts.size());
  std::vector<unsigned long long> results_branchless4;
  results_branchless4.resize(evts.size());
  std::vector<unsigned long long> results_branchless8;
  results_branchless8.resize(evts.size());
  std::vector<unsigned long long> results_branchless16;
  results_branchless16.resize(evts.size());
  std::vector<unsigned long long> results_branchless_lfence;
  results_branchless_lfence.resize(evts.size());
  std::vector<unsigned long long> results_branchless2_lfence;
  results_branchless2_lfence.resize(evts.size());
  std::vector<unsigned long long> results_branchless4_lfence;
  results_branchless4_lfence.resize(evts.size());
  std::vector<unsigned long long> results_branchless8_lfence;
  results_branchless8_lfence.resize(evts.size());
  std::vector<unsigned long long> results_branchless16_lfence;
  results_branchless16_lfence.resize(evts.size());
  std::vector<unsigned long long> results_fake_branchless16;
  results_fake_branchless16.resize(evts.size());
  std::vector<unsigned long long> results_branchless32_lfence;
  results_branchless32_lfence.resize(evts.size());
  std::vector<unsigned long long> results_branchless32;
  results_branchless32.resize(evts.size());
  //
  for (size_t trial = 0; trial < 5; trial++) {
    for (size_t i = 0; i < howmany; i++) {
      targets[i] = rand(); // reset
    }
    printf("\n ==== trial %zu\n", trial);
    int counter = 0;

    unified.start();
    size_t cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchy(data, targets, solution);
    unified.end(results_branchy);

    unified.start();
    branchy(data, targets, solution);
    unified.end(results_branchy);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless(data, targets, solution);
    unified.end(results_branchless);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless_lfence(data, targets, solution);
    unified.end(results_branchless_lfence);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless2(data, targets, solution);
    unified.end(results_branchless2);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless2_lfence(data, targets, solution);
    unified.end(results_branchless2_lfence);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless4(data, targets, solution);
    unified.end(results_branchless4);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless4_lfence(data, targets, solution);
    unified.end(results_branchless4_lfence);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    fake_branchless4(data, targets, solution);
    unified.end(results_fake_branchless4);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    fake_branchless6(data, targets, solution);
    unified.end(results_fake_branchless6);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless8(data, targets, solution);
    unified.end(results_branchless8);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless8_lfence(data, targets, solution);
    unified.end(results_branchless8_lfence);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    fake_branchless8(data, targets, solution);
    unified.end(results_fake_branchless8);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless16(data, targets, solution);
    unified.end(results_branchless16);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless16_lfence(data, targets, solution);
    unified.end(results_branchless16_lfence);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless32(data, targets, solution);
    unified.end(results_branchless32);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    branchless32_lfence(data, targets, solution);
    unified.end(results_branchless32_lfence);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    fake_branchless16(data, targets, solution);
    unified.end(results_fake_branchless16);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);

    unified.start();
    cl = cacheline_access(data, &counter);
    unified.end(results_cacheline);


    printf("branchless    %.2f cycles/value %.2f instructions/value\n",
           results_branchless[0] * 1.0 / howmany,
           results_branchless[1] * 1.0 / howmany);
    printf(
        "branchless2   %.2f cycles/value %.2f instructions/value ratio: %.2f\n",
        results_branchless2[0] * 1.0 / howmany,
        results_branchless2[1] * 1.0 / howmany,
        results_branchless[0] * 1.0 / results_branchless2[0]);
    printf(
        "branchless4   %.2f cycles/value %.2f instructions/value ratio: %.2f\n",
        results_branchless4[0] * 1.0 / howmany,
        results_branchless4[1] * 1.0 / howmany,
        results_branchless[0] * 1.0 / results_branchless4[0]);
    printf(
        "branchless8   %.2f cycles/value %.2f instructions/value ratio: %.2f\n",
        results_branchless8[0] * 1.0 / howmany,
        results_branchless8[1] * 1.0 / howmany,
        results_branchless[0] * 1.0 / results_branchless8[0]);
    printf(
        "branchless16  %.2f cycles/value %.2f instructions/value ratio: %.2f\n",
        results_branchless16[0] * 1.0 / howmany,
        results_branchless16[1] * 1.0 / howmany,
        results_branchless[0] * 1.0 / results_branchless16[0]);
    printf(
        "branchless32  %.2f cycles/value %.2f instructions/value ratio: %.2f\n",
        results_branchless32[0] * 1.0 / howmany,
        results_branchless32[1] * 1.0 / howmany,
        results_branchless[0] * 1.0 / results_branchless32[0]);
    double ele_level = (levels - cl_levels) * howmany;
    printf("\n");
    printf("cacheline     %.2f cycles/cacheline %.2f instructions/cacheline\n",
           results_cacheline[0] * 1.0 / cl, results_cacheline[1] * 1.0 / cl);
    printf("branchy       %.2f cycles/level %.2f instructions/level\n",
           results_branchy[0] * 1.0 / ele_level,
           results_branchy[1] * 1.0 / ele_level);

    printf("fake_branchless4   %.2f cycles/level %.2f instructions/level\n",
           results_fake_branchless4[0] * 1.0 / ele_level,
           results_fake_branchless4[1] * 1.0 / ele_level);
    printf("fake_branchless6   %.2f cycles/level %.2f instructions/level\n",
           results_fake_branchless6[0] * 1.0 / ele_level,
           results_fake_branchless6[1] * 1.0 / ele_level);
    printf("fake_branchless8   %.2f cycles/level %.2f instructions/level\n",
           results_fake_branchless8[0] * 1.0 / ele_level,
           results_fake_branchless8[1] * 1.0 / ele_level);
    printf("fake_branchless16  %.2f cycles/level %.2f instructions/level\n",
           results_fake_branchless16[0] * 1.0 / ele_level,
           results_fake_branchless16[1] * 1.0 / ele_level);

    printf("branchless    %.2f cycles/level %.2f instructions/level\n",
           results_branchless[0] * 1.0 / ele_level,
           results_branchless[1] * 1.0 / ele_level);
    printf("branchless2   %.2f cycles/level %.2f instructions/level\n",
           results_branchless2[0] * 1.0 / ele_level,
           results_branchless2[1] * 1.0 / ele_level);
    printf("branchless4   %.2f cycles/level %.2f instructions/level\n",
           results_branchless4[0] * 1.0 / ele_level,
           results_branchless4[1] * 1.0 / ele_level);
    printf("branchless8   %.2f cycles/level %.2f instructions/level\n",
           results_branchless8[0] * 1.0 / ele_level,
           results_branchless8[1] * 1.0 / ele_level);
    printf("branchless16  %.2f cycles/level %.2f instructions/level\n",
           results_branchless16[0] * 1.0 / ele_level,
           results_branchless16[1] * 1.0 / ele_level);
    printf("branchless32  %.2f cycles/level %.2f instructions/level\n",
           results_branchless32[0] * 1.0 / ele_level,
           results_branchless32[1] * 1.0 / ele_level);
    printf("branchless   lfence %.2f cycles/level %.2f instructions/level\n",
           results_branchless_lfence[0] * 1.0 / ele_level,
           results_branchless_lfence[1] * 1.0 / ele_level);
    printf("branchless2  lfence %.2f cycles/level %.2f instructions/level\n",
           results_branchless2_lfence[0] * 1.0 / ele_level,
           results_branchless2_lfence[1] * 1.0 / ele_level);
    printf("branchless4  lfence %.2f cycles/level %.2f instructions/level\n",
           results_branchless4_lfence[0] * 1.0 / ele_level,
           results_branchless4_lfence[1] * 1.0 / ele_level);
    printf("branchless8  lfence %.2f cycles/level %.2f instructions/level\n",
           results_branchless8_lfence[0] * 1.0 / ele_level,
           results_branchless8_lfence[1] * 1.0 / ele_level);
    printf("branchless16 lfence %.2f cycles/level %.2f instructions/level\n",
           results_branchless16_lfence[0] * 1.0 / ele_level,
           results_branchless16_lfence[1] * 1.0 / ele_level);
    printf("branchless32 lfence %.2f cycles/level %.2f instructions/level\n",
           results_branchless32_lfence[0] * 1.0 / ele_level,
           results_branchless32_lfence[1] * 1.0 / ele_level);
    printf("fake_branchless4 : ");
    printvec(results_fake_branchless4, howmany);
    /*printvec(results_fake_branchless8, howmany);
    printvec(results_fake_branchless16, howmany);*/
    printf("branchless4      : ");
    printvec(results_branchless4, howmany);
    printf("branchless8      : ");
    printvec(results_branchless8, howmany);
    printf("branchless16     : ");
    printvec(results_branchless16, howmany);
    printf("branchless32     : ");
    printvec(results_branchless32, howmany);
  }
}

int main() {
  size_t large = 64 * 1024 * 1024;
  size_t small = 1024;
  demo(small, large / small);
  // demo(small, large / small);} // 262144+1324);
  // demo(1024,45426);
}
