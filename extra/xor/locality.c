#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

uint32_t reduce(uint32_t hash, uint32_t n) {
  // http://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
  return (uint32_t)(((uint64_t)(hash) * (uint64_t)(n)) >> 32);
}

static inline uint64_t murmur64(uint64_t h) {
  h ^= h >> 33;
  h *= 0xff51afd7ed558ccd;
  h ^= h >> 33;
  h *= 0xc4ceb9fe1a85ec53;
  h ^= h >> 33;
  return h;
}

static inline uint64_t rotl64(uint64_t n, int c) {
  return (n << (uint32_t)(c & 63)) | (n >> (uint32_t)((-c) & 63));
}

uint64_t mixsplit(uint64_t key, uint64_t seed) { return murmur64(key + seed); }

__attribute__((noinline)) void local(uint64_t *keys, size_t length, uint32_t *H,
                                     uint64_t Seed, uint32_t SegmentCountLength,
                                     uint32_t SegmentLength,
                                     uint32_t SegmentLengthMask) {
  for (size_t i = 0; i < length; i++) {
    uint64_t hash = mixsplit(keys[i], Seed);
    uint64_t hi = ((__uint128_t)hash * SegmentCountLength) >> 64;
    uint32_t index1 = hi;
    uint32_t index2 = hi + SegmentLength;
    uint32_t index3 = hi + 2 * SegmentLength;
    index2 ^= (hash >> 18) & SegmentLengthMask;
    index3 ^= (hash)&SegmentLengthMask;
    H[index1]++;
    H[index2]++;
    H[index3]++;
  }
}

__attribute__((noinline)) void local_indexes(uint64_t *keys, size_t length,
                                             uint32_t *indexes, uint64_t Seed,
                                             uint32_t SegmentCountLength,
                                             uint32_t SegmentLength,
                                             uint32_t SegmentLengthMask) {
  for (size_t i = 0; i < length; i++) {
    uint64_t hash = mixsplit(keys[i], Seed);
    uint64_t hi = ((__uint128_t)hash * SegmentCountLength) >> 64;
    uint32_t index1 = hi;
    uint32_t index2 = hi + SegmentLength;
    uint32_t index3 = hi + 2 * SegmentLength;
    index2 ^= (hash >> 18) & SegmentLengthMask;
    index3 ^= (hash)&SegmentLengthMask;
    indexes[3 * i] = index1;
    indexes[3 * i + 1] = index2;
    indexes[3 * i + 2] = index3;
  }
}

__attribute__((noinline)) void non_local(uint64_t *keys, size_t length,
                                         uint32_t *H, uint32_t Seed,
                                         uint32_t BlockLength) {

  for (size_t i = 0; i < length; i++) {
    uint64_t hash = mixsplit(keys[i], Seed);
    uint32_t r0 = hash;
    uint32_t r1 = rotl64(hash, 21);
    uint32_t r2 = rotl64(hash, 42);
    uint32_t index1 = reduce(r0, BlockLength);
    uint32_t index2 = reduce(r1, BlockLength) + BlockLength;
    uint32_t index3 = reduce(r2, BlockLength) + 2 * BlockLength;
    H[index1]++;
    H[index2]++;
    H[index3]++;
  }
}

__attribute__((noinline)) void indexed(uint32_t *H, size_t length,
                                       uint32_t *I) {

  for (size_t i = 0; i < length; i++) {
    H[I[3 * i]]++;
    H[I[3 * i + 1]]++;
    H[I[3 * i + 2]]++;
  }
}

__attribute__((noinline)) void non_local_indexes(uint64_t *keys, size_t length,
                                                 uint32_t *indexes,
                                                 uint32_t Seed,
                                                 uint32_t BlockLength) {

  for (size_t i = 0; i < length; i++) {
    uint64_t hash = mixsplit(keys[i], Seed);
    uint32_t r0 = hash;
    uint32_t r1 = rotl64(hash, 21);
    uint32_t r2 = rotl64(hash, 42);
    uint32_t index1 = reduce(r0, BlockLength);
    uint32_t index2 = reduce(r1, BlockLength) + BlockLength;
    uint32_t index3 = reduce(r2, BlockLength) + 2 * BlockLength;
    indexes[3 * i] = index1;
    indexes[3 * i + 1] = index2;
    indexes[3 * i + 2] = index3;
  }
}

int main() {
  size_t SegmentLength = 65536;
  size_t SegmentLengthMask = 65535;
  size_t SegmentCountLength = 11141120;
  size_t BlockLength = 4100010;
  size_t Size = 10000000;
  size_t Seed = 1;
  size_t N = 3 * BlockLength;
  printf("N = %zu, %.1f MB\n", N, N * sizeof(uint32_t) / (1024. * 1024));
  uint32_t *H = (uint32_t *)malloc(N * sizeof(uint32_t));
  uint64_t *keys = (uint64_t *)malloc(Size * sizeof(uint64_t));

  for (size_t i = 0; i < Size; i++) {
    keys[i] = mixsplit(i, Seed);
  }
  uint32_t *indexes_local = (uint32_t *)malloc(3 * Size * sizeof(uint32_t));
  uint32_t *indexes_nonlocal = (uint32_t *)malloc(3 * Size * sizeof(uint32_t));
  non_local_indexes(keys, Size, indexes_nonlocal, Seed, BlockLength);
  local_indexes(keys, Size, indexes_local, Seed, SegmentCountLength,
                SegmentLength, SegmentLengthMask);

  for (uint32_t trial = 0; trial < 3; trial++) {
    printf("========= trial %u \n", trial + 1);

    struct timespec start, stop;

    size_t total_trials = 5;
    size_t idx = 0;
    size_t idxp = 0;

    size_t cns2 = 0;
    size_t cns2p = 0;
    size_t ns2 = 0;
    size_t ns2p = 0;
    for (size_t t = 0; t < total_trials; t++) {
      clock_gettime(CLOCK_REALTIME, &start);
      local_indexes(keys, Size, indexes_local, Seed, SegmentCountLength,
                    SegmentLength, SegmentLengthMask);
      clock_gettime(CLOCK_REALTIME, &stop);
      idxp += (stop.tv_sec - start.tv_sec) * 1000000000 +
              (stop.tv_nsec - start.tv_nsec);

      clock_gettime(CLOCK_REALTIME, &start);
      non_local_indexes(keys, Size, indexes_nonlocal, Seed, BlockLength);
      clock_gettime(CLOCK_REALTIME, &stop);
      idx += (stop.tv_sec - start.tv_sec) * 1000000000 +
             (stop.tv_nsec - start.tv_nsec);

      clock_gettime(CLOCK_REALTIME, &start);
      local(keys, Size, H, Seed, SegmentCountLength, SegmentLength,
            SegmentLengthMask);
      clock_gettime(CLOCK_REALTIME, &stop);
      cns2p += (stop.tv_sec - start.tv_sec) * 1000000000 +
               (stop.tv_nsec - start.tv_nsec);

      clock_gettime(CLOCK_REALTIME, &start);
      non_local(keys, Size, H, Seed, BlockLength);
      clock_gettime(CLOCK_REALTIME, &stop);
      cns2 += (stop.tv_sec - start.tv_sec) * 1000000000 +
              (stop.tv_nsec - start.tv_nsec);

      clock_gettime(CLOCK_REALTIME, &start);
      indexed(H, Size, indexes_local);
      clock_gettime(CLOCK_REALTIME, &stop);
      ns2p += (stop.tv_sec - start.tv_sec) * 1000000000 +
              (stop.tv_nsec - start.tv_nsec);

      clock_gettime(CLOCK_REALTIME, &start);
      indexed(H, Size, indexes_nonlocal);
      clock_gettime(CLOCK_REALTIME, &stop);
      ns2 += (stop.tv_sec - start.tv_sec) * 1000000000 +
             (stop.tv_nsec - start.tv_nsec);
    }
    printf("just comp. random   : %.3f ns\n", (double)idx / N / total_trials);
    printf("just comp. localized  : %.3f ns\n",
           (double)idxp / N / total_trials);

    printf("difference : %.1f %% \n", ((double)idx / (double)idxp * 100) - 100);
    printf("computed random   : %.3f ns\n", (double)cns2 / N / total_trials);
    printf("computed localized  : %.3f ns\n", (double)cns2p / N / total_trials);

    printf("difference : %.1f %% \n",
           ((double)cns2 / (double)cns2p * 100) - 100);

    printf("indexed random   : %.3f ns\n", (double)ns2 / N / total_trials);
    printf("indexed localized  : %.3f ns\n", (double)ns2p / N / total_trials);

    printf("difference : %.1f %% \n", ((double)ns2 / (double)ns2p * 100) - 100);
    printf("=========\n");
  }

  free(keys);
  free(H);
  free(indexes_local);
  free(indexes_nonlocal);

  return EXIT_SUCCESS;
}
