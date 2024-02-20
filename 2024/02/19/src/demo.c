#include "powercap-rapl.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <x86intrin.h>

__attribute__((noinline)) __attribute__((optimize("no-tree-vectorize"))) float
sum(float *data, size_t N) {
  double counter = 0;
  for (size_t i = 0; i < N; i++) {
    counter += data[i];
  }
  return counter;
}

__attribute__((noinline)) float sumvec(float *data, size_t N) {
  __m512d counter = _mm512_setzero_pd();
  for (size_t i = 0; i < N; i += 16) {
    __m512 v = _mm512_loadu_ps((__m512 *)&data[i]);
    __m512d part1 = _mm512_cvtps_pd(_mm512_extractf32x8_ps(v, 0));
    __m512d part2 = _mm512_cvtps_pd(_mm512_extractf32x8_ps(v, 1));

    counter = _mm512_add_pd(counter, part1);
    counter = _mm512_add_pd(counter, part2);
  }
  double sum = _mm512_reduce_add_pd(counter);
  for (size_t i = N / 16 * 16; i < N; i++) {
    sum += data[i];
  }
  return sum;
}

static const char *to_string(powercap_rapl_zone zone) {
  switch (zone) {
  case POWERCAP_RAPL_ZONE_PACKAGE:
    return "package (CPU socket)";
  case POWERCAP_RAPL_ZONE_CORE:
    return "core (CPU)";
  case POWERCAP_RAPL_ZONE_UNCORE:
    return "uncore (GPU, etc.)";
  case POWERCAP_RAPL_ZONE_DRAM:
    return "DRAM";
  case POWERCAP_RAPL_ZONE_PSYS:
    return "system (PSys)";
  default:
    return "unknown";
  }
}

int main() {
  struct timespec start, stop;

  uint32_t npackages = powercap_rapl_get_num_instances();
  printf("Number of RAPL packages: %d\n", npackages);
  powercap_rapl_pkg pkg[npackages];

  for (size_t i = 0; i < npackages; i++) {
    printf("Initializing RAPL package %d\n", i);
    if (powercap_rapl_init(i, &pkg[i], true)) {
      printf("Error initializing RAPL, you may need privileged access\n");
      powercap_rapl_destroy(&pkg[i]);
      return EXIT_FAILURE;
    }
  }
  printf("Initializing array\n");
  size_t N = 120040001;
  float *data = (float *)malloc(N * sizeof(float));
  for (size_t i = 0; i < N; i++) {
    data[i] = (float)1;
  }
  powercap_rapl_zone zones[] = {
      POWERCAP_RAPL_ZONE_PACKAGE, POWERCAP_RAPL_ZONE_CORE,
      POWERCAP_RAPL_ZONE_UNCORE, POWERCAP_RAPL_ZONE_DRAM,
      POWERCAP_RAPL_ZONE_PSYS};
  for (size_t z = 0; z < sizeof(zones) / sizeof(powercap_rapl_zone); z++) {
    powercap_rapl_zone zone = zones[z];
    printf("\nTesting %s\n", to_string(zone));
    bool supported[npackages];
    bool has_one_supported = false;
    for (size_t i = 0; i < npackages; i++) {
      //  Returns 1 if supported, 0 if unsupported, a negative value in case of
      //  error.
      supported[i] = powercap_rapl_is_zone_supported(&pkg[i], zone);
      if (supported[i] != 1) {
        printf("RAPL is not supported on package %d\n", i);
      } else {
        has_one_supported = true;
      }
    }
    if (!has_one_supported) {
      printf("No supported packages for %s\n", to_string(zone));
      continue;
    }

    for (size_t trial = 0; trial < 3; trial++) {

      printf("trial %d\n", trial);

      uint64_t energy_uj1[npackages];
      uint64_t energy_uj2[npackages];

      for (size_t j = 0; j < npackages; j++) {
        if (powercap_rapl_get_energy_uj(&pkg[j], zone, &energy_uj1[j])) {
          printf("failed to get energy on package %d\n", j);
          break;
        }
      }
      clock_gettime(CLOCK_REALTIME, &start);
      printf("sum %f\n", sum(data, N));
      clock_gettime(CLOCK_REALTIME, &stop);
      for (size_t j = 0; j < npackages; j++) {
        if (powercap_rapl_get_energy_uj(&pkg[j], zone, &energy_uj2[j])) {
          printf("failed to get energy on package %d\n", j);
          break;
        }
      }
      double nano = (stop.tv_sec - start.tv_sec) * 1e9 +
                    (double)(stop.tv_nsec - start.tv_nsec);
      for (size_t j = 0; j < npackages; j++) {
        printf("package %d energy (uj): %lu, per iter %f per nano %f\n", j,
               energy_uj2[j] - energy_uj1[j],
               (double)(energy_uj2[j] - energy_uj1[j]) / N,
               (double)(energy_uj2[j] - energy_uj1[j]) / nano);
      }

      for (size_t j = 0; j < npackages; j++) {
        if (powercap_rapl_get_energy_uj(&pkg[j], zone, &energy_uj1[j])) {
          printf("failed to get energy on package %d\n", j);
          break;
        }
      }
      clock_gettime(CLOCK_REALTIME, &start);
      printf("sumvec %f\n", sumvec(data, N));
      clock_gettime(CLOCK_REALTIME, &stop);
      nano = (stop.tv_sec - start.tv_sec) * 1e9 +
             (double)(stop.tv_nsec - start.tv_nsec);
      for (size_t j = 0; j < npackages; j++) {
        if (powercap_rapl_get_energy_uj(&pkg[j], zone, &energy_uj2[j])) {
          printf("failed to get energy on package %d\n", j);
          break;
        }
      }
      for (size_t j = 0; j < npackages; j++) {
        printf("package %d energy (uj): %lu, per iter %f per nano %f\n", j,
               energy_uj2[j] - energy_uj1[j],
               (double)(energy_uj2[j] - energy_uj1[j]) / N,
               (double)(energy_uj2[j] - energy_uj1[j]) / nano);
      }
      printf("\n");
    }
  }
  free(data);

  for (size_t i = 0; i < npackages; i++) {
    powercap_rapl_destroy(&pkg[i]);
  }
}