#ifndef __linux__
#warning "We require Linux."
#else
#include "linux-perf-events.h"
#endif

#include <cstdint>
#include <cstdio>
#include <cstdlib>
//#include <x86intrin.h>
#include <algorithm>
#include <vector>

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

void branchy(const std::vector<std::vector<int>> &data,
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

void branchless(const std::vector<std::vector<int>> &data,
                const std::vector<int> &targets,
                std::vector<size_t> &solution) {
  for (size_t i = 0; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
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

void branchless2(const std::vector<std::vector<int>> &data,
                 const std::vector<int> &targets,
                 std::vector<size_t> &solution) {
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

void branchfree_search4(const int *source1, const int *source2,
                        const int *source3, const int *source4, size_t n,
                        int target1, int target2, int target3, int target4,
                        size_t *__restrict index1, size_t *__restrict index2,
                        size_t *__restrict index3, size_t *__restrict index4) {
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

void branchless4(const std::vector<std::vector<int>> &data,
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

void branchfree_search8(const int *source1, const int *source2,
                        const int *source3, const int *source4,
                        const int *source5, const int *source6,
                        const int *source7, const int *source8, size_t n,
                        int target1, int target2, int target3, int target4,
                        int target5, int target6, int target7, int target8,
                        size_t *index1, size_t *index2, size_t *index3,
                        size_t *index4, size_t *index5, size_t *index6,
                        size_t *index7, size_t *index8) {
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

void branchless8(const std::vector<std::vector<int>> &data,
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

void branchfree_search16(
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

void branchless16(const std::vector<std::vector<int>> &data,
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
inline uint64_t splitmix64_stateless(uint64_t index) {
  uint64_t z = (index + UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

void demo(size_t howmany, size_t N) {
  std::cout << "Initializing... " << howmany * N * sizeof(int) / (1024 * 1024)
            << "MB" << std::endl;
  int levels = 0;
  while ((1ULL << levels) < N) {
    levels++;
  }
  std::cout << "binary search over " << levels << " steps " << std::endl;
  std::vector<std::vector<int>> data;
  std::vector<int> targets;
  for (size_t i = 0; i < howmany; i++) {
    data.emplace_back(std::vector<int>(N));
    for (size_t z = 0; z < N; z++) {
      data[i][z] = splitmix64_stateless(i * N + z);
    }
    targets.push_back(data[i][0]);
    std::sort(data[i].begin(), data[i].end());
  }
  std::vector<size_t> solution(howmany);
  std::vector<size_t> solutionref(howmany);

  std::cout << "Starting benchmark..." << std::endl;

  //
  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);
  std::vector<unsigned long long> results_branchy;
  results_branchy.resize(evts.size());
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
  //
  for (size_t trial = 0; trial < 5; trial++) {
    printf("\n ==== trial %zu\n", trial);
    unified.start();
    branchy(data, targets, solution);
    unified.end(results_branchy);

    unified.start();
    branchy(data, targets, solution);
    unified.end(results_branchy);

    unified.start();
    branchless(data, targets, solution);
    unified.end(results_branchless);

    unified.start();
    branchless(data, targets, solution);
    unified.end(results_branchless);
    unified.start();
    branchless2(data, targets, solution);
    unified.end(results_branchless2);

    unified.start();
    branchless4(data, targets, solution);
    unified.end(results_branchless4);

    unified.start();
    branchless8(data, targets, solution);
    unified.end(results_branchless8);

    unified.start();
    branchless16(data, targets, solution);
    unified.end(results_branchless16);

    printf("branchy       %.2f cycles/value %.2f instructions/value\n",
           results_branchy[0] * 1.0 / howmany,
           results_branchy[1] * 1.0 / howmany);
    printf("branchless    %.2f cycles/value %.2f instructions/value\n",
           results_branchless[0] * 1.0 / howmany,
           results_branchless[1] * 1.0 / howmany);
    printf("branchless2   %.2f cycles/value %.2f instructions/value\n",
           results_branchless2[0] * 1.0 / howmany,
           results_branchless2[1] * 1.0 / howmany);
    printf("branchless4   %.2f cycles/value %.2f instructions/value\n",
           results_branchless4[0] * 1.0 / howmany,
           results_branchless4[1] * 1.0 / howmany);
    printf("branchless8   %.2f cycles/value %.2f instructions/value\n",
           results_branchless8[0] * 1.0 / howmany,
           results_branchless8[1] * 1.0 / howmany);
    printf("branchless16  %.2f cycles/value %.2f instructions/value\n",
           results_branchless16[0] * 1.0 / howmany,
           results_branchless16[1] * 1.0 / howmany);
    double ele_level = levels * howmany;
    printf("\n");
    printf("branchy       %.2f cycles/level %.2f instructions/level\n",
           results_branchy[0] * 1.0 / ele_level,
           results_branchy[1] * 1.0 / ele_level);
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
  }
}

int main() { demo(1024, 64000); }