#ifndef M1CYCLES_H
#define M1CYCLES_H

#include <cstdint>
#include "m1cycles.h"
#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Adapted by D. Lemire
 * From:
 * Duc Tri Nguyen (CERG GMU)
 * From:
 * Dougall Johnson
 * https://gist.github.com/dougallj/5bafb113492047c865c0c8cfbc930155#file-m1_robsize-c-L390
 *
 */

#define KPERF_LIST                                                             \
  /*  ret, name, params */                                                     \
  F(int, kpc_get_counting, void)                                               \
  F(int, kpc_force_all_ctrs_set, int)                                          \
  F(int, kpc_set_counting, uint32_t)                                           \
  F(int, kpc_set_thread_counting, uint32_t)                                    \
  F(int, kpc_set_config, uint32_t, void *)                                     \
  F(int, kpc_get_config, uint32_t, void *)                                     \
  F(int, kpc_set_period, uint32_t, void *)                                     \
  F(int, kpc_get_period, uint32_t, void *)                                     \
  F(uint32_t, kpc_get_counter_count, uint32_t)                                 \
  F(uint32_t, kpc_get_config_count, uint32_t)                                  \
  F(int, kperf_sample_get, int *)                                              \
  F(int, kpc_get_thread_counters, int, unsigned int, void *)

#define F(ret, name, ...)                                                      \
  typedef ret name##proc(__VA_ARGS__);                                         \
  static name##proc *name;
KPERF_LIST
#undef F

#define CFGWORD_EL0A32EN_MASK (0x10000)
#define CFGWORD_EL0A64EN_MASK (0x20000)
#define CFGWORD_EL1EN_MASK (0x40000)
#define CFGWORD_EL3EN_MASK (0x80000)
#define CFGWORD_ALLMODES_MASK (0xf0000)

#define CPMU_NONE 0
#define CPMU_CORE_CYCLE 0x02
#define CPMU_INST_A64 0x8c
#define CPMU_INST_BRANCH 0x8d
#define CPMU_SYNC_DC_LOAD_MISS 0xbf
#define CPMU_SYNC_DC_STORE_MISS 0xc0
#define CPMU_SYNC_DTLB_MISS 0xc1
#define CPMU_SYNC_ST_HIT_YNGR_LD 0xc4
#define CPMU_SYNC_BR_ANY_MISP 0xcb
#define CPMU_FED_IC_MISS_DEM 0xd3
#define CPMU_FED_ITLB_MISS 0xd4

#define KPC_CLASS_FIXED (0)
#define KPC_CLASS_CONFIGURABLE (1)
#define KPC_CLASS_POWER (2)
#define KPC_CLASS_RAWPMU (3)
#define KPC_CLASS_FIXED_MASK (1u << KPC_CLASS_FIXED)
#define KPC_CLASS_CONFIGURABLE_MASK (1u << KPC_CLASS_CONFIGURABLE)
#define KPC_CLASS_POWER_MASK (1u << KPC_CLASS_POWER)
#define KPC_CLASS_RAWPMU_MASK (1u << KPC_CLASS_RAWPMU)

#define COUNTERS_COUNT 10
#define CONFIG_COUNT 8
#define KPC_MASK (KPC_CLASS_CONFIGURABLE_MASK | KPC_CLASS_FIXED_MASK)
uint64_t g_counters[COUNTERS_COUNT];
uint64_t g_config[COUNTERS_COUNT];

struct performance_counters {
  double cycles;
  double branches;
  double missed_branches;
  double instructions;
  performance_counters(uint64_t c, uint64_t b, uint64_t m, uint64_t i)
      : cycles(c), branches(b), missed_branches(m), instructions(i) {}
  performance_counters(double c, double b, double m, double i)
      : cycles(c), branches(b), missed_branches(m), instructions(i) {}
  performance_counters(double init)
      : cycles(init), branches(init), missed_branches(init),
        instructions(init) {}

  inline performance_counters &operator-=(const performance_counters &other) {
    cycles -= other.cycles;
    branches -= other.branches;
    missed_branches -= other.missed_branches;
    instructions -= other.instructions;
    return *this;
  }
  inline performance_counters &min(const performance_counters &other) {
    cycles = other.cycles < cycles ? other.cycles : cycles;
    branches = other.branches < branches ? other.branches : branches;
    missed_branches = other.missed_branches < missed_branches
                          ? other.missed_branches
                          : missed_branches;
    instructions =
        other.instructions < instructions ? other.instructions : instructions;
    return *this;
  }
  inline performance_counters &max(const performance_counters &other) {
      cycles = other.cycles > cycles ? other.cycles : cycles;
      branches = other.branches > branches ? other.branches : branches;
      missed_branches = other.missed_branches > missed_branches
                            ? other.missed_branches
                            : missed_branches;
      instructions =
          other.instructions > instructions ? other.instructions : instructions;
      return *this;
    }
  inline performance_counters &operator+=(const performance_counters &other) {
    cycles += other.cycles;
    branches += other.branches;
    missed_branches += other.missed_branches;
    instructions += other.instructions;
    return *this;
  }

  inline performance_counters &operator/=(double numerator) {
    cycles /= numerator;
    branches /= numerator;
    missed_branches /= numerator;
    instructions /= numerator;
    return *this;
  }
};

inline performance_counters operator-(const performance_counters &a,
                                      const performance_counters &b) {
  return performance_counters(a.cycles - b.cycles, a.branches - b.branches,
                              a.missed_branches - b.missed_branches,
                              a.instructions - b.instructions);
}

void configure_rdtsc() {
  if (kpc_set_config(KPC_MASK, g_config)) {
    printf("kpc_set_config failed\n");
    return;
  }

  if (kpc_force_all_ctrs_set(1)) {
    printf("kpc_force_all_ctrs_set failed\n");
    return;
  }

  if (kpc_set_counting(KPC_MASK)) {
    printf("kpc_set_counting failed\n");
    return;
  }

  if (kpc_set_thread_counting(KPC_MASK)) {
    printf("kpc_set_thread_counting failed\n");
    return;
  }
}

void init_rdtsc() {
  void *kperf = dlopen(
      "/System/Library/PrivateFrameworks/kperf.framework/Versions/A/kperf",
      RTLD_LAZY);
  if (!kperf) {
    printf("kperf = %p\n", kperf);
    return;
  }
#define F(ret, name, ...)                                                      \
  name = (name##proc *)(dlsym(kperf, #name));                                  \
  if (!name) {                                                                 \
    printf("%s = %p\n", #name, (void *)name);                                  \
    return;                                                                    \
  }
  KPERF_LIST
#undef F

  if (kpc_get_counter_count(KPC_MASK) != COUNTERS_COUNT) {
    printf("wrong fixed counters count\n");
    return;
  }

  if (kpc_get_config_count(KPC_MASK) != CONFIG_COUNT) {
    printf("wrong fixed config count\n");
    return;
  }
  g_config[0] = CPMU_CORE_CYCLE | CFGWORD_EL0A64EN_MASK;
  g_config[3] = CPMU_INST_BRANCH | CFGWORD_EL0A64EN_MASK;
  g_config[4] = CPMU_SYNC_BR_ANY_MISP | CFGWORD_EL0A64EN_MASK;
  g_config[5] = CPMU_INST_A64 | CFGWORD_EL0A64EN_MASK;

  configure_rdtsc();
}

void setup_performance_counters(void) {
  int test_high_perf_cores = 1;
  if (test_high_perf_cores) {
    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
  } else {
    pthread_set_qos_class_self_np(QOS_CLASS_BACKGROUND, 0);
  }
  init_rdtsc();
  configure_rdtsc();
}

performance_counters get_counters(void) {
    static bool warned = false;
    if (kpc_get_thread_counters(0, COUNTERS_COUNT, g_counters)) {
      if (!warned) {
        printf("kpc_get_thread_counters failed, run as sudo?\n");
        warned = true;
      }
      return 1;
    }
    // g_counters[3 + 2] gives you the number of instructions 'decoded'
    // whereas g_counters[1] might give you the number of instructions 'retired'.
    return performance_counters{g_counters[0 + 2], g_counters[3 + 2],
                                g_counters[4 + 2], g_counters[5 + 2]};
  }

#endif
