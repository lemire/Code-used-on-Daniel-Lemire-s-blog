/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Read/write RAPL sysfs files.
 *
 * @author Connor Imes
 * @date 2017-08-24
 */
#include <inttypes.h>
#include <unistd.h>
#include "powercap-sysfs.h"
#include "powercap-rapl-sysfs.h"

#define CONTROL_TYPE "intel-rapl"

#define DECL_ZONES() \
  uint32_t zones[2]; \
  zones[0] = zone; \
  zones[1] = sz;

#define DEPTH() (is_sz ? 2 : 1)

int rapl_sysfs_zone_exists(uint32_t zone, uint32_t sz, int is_sz) {
  DECL_ZONES();
  return powercap_sysfs_zone_exists(CONTROL_TYPE, zones, DEPTH());
}

int rapl_sysfs_pkg_exists(uint32_t zone) {
  return powercap_sysfs_zone_exists(CONTROL_TYPE, &zone, 1);
}

int rapl_sysfs_sz_exists(uint32_t zone, uint32_t sz) {
  DECL_ZONES();
  return powercap_sysfs_zone_exists(CONTROL_TYPE, zones, 2);
}

int rapl_sysfs_constraint_exists(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint) {
  DECL_ZONES();
  return powercap_sysfs_constraint_exists(CONTROL_TYPE, zones, DEPTH(), constraint);
}

int rapl_sysfs_zone_reset_energy_uj(uint32_t zone, uint32_t sz, int is_sz) {
  DECL_ZONES();
  return powercap_sysfs_zone_reset_energy_uj(CONTROL_TYPE, zones, DEPTH());
}

int rapl_sysfs_zone_get_max_energy_range_uj(uint32_t zone, uint32_t sz, int is_sz, uint64_t* val) {
  DECL_ZONES();
  return powercap_sysfs_zone_get_max_energy_range_uj(CONTROL_TYPE, zones, DEPTH(), val);
}

int rapl_sysfs_zone_get_energy_uj(uint32_t zone, uint32_t sz, int is_sz, uint64_t* val) {
  DECL_ZONES();
  return powercap_sysfs_zone_get_energy_uj(CONTROL_TYPE, zones, DEPTH(), val);
}

int rapl_sysfs_zone_get_max_power_range_uw(uint32_t zone, uint32_t sz, int is_sz, uint64_t* val) {
  DECL_ZONES();
  return powercap_sysfs_zone_get_max_power_range_uw(CONTROL_TYPE, zones, DEPTH(), val);
}

int rapl_sysfs_zone_get_power_uw(uint32_t zone, uint32_t sz, int is_sz, uint64_t* val) {
  DECL_ZONES();
  return powercap_sysfs_zone_get_power_uw(CONTROL_TYPE, zones, DEPTH(), val);
}

int rapl_sysfs_zone_set_enabled(uint32_t zone, uint32_t sz, int is_sz, uint32_t val) {
  DECL_ZONES();
  return powercap_sysfs_zone_set_enabled(CONTROL_TYPE, zones, DEPTH(), val);
}

int rapl_sysfs_zone_get_enabled(uint32_t zone, uint32_t sz, int is_sz, uint32_t* val) {
  DECL_ZONES();
  return powercap_sysfs_zone_get_enabled(CONTROL_TYPE, zones, DEPTH(), val);
}

ssize_t rapl_sysfs_zone_get_name(uint32_t zone, uint32_t sz, int is_sz, char* buf, size_t size) {
  DECL_ZONES();
  return powercap_sysfs_zone_get_name(CONTROL_TYPE, zones, DEPTH(), buf, size);
}

int rapl_sysfs_constraint_set_power_limit_uw(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t val) {
  DECL_ZONES();
  return powercap_sysfs_constraint_set_power_limit_uw(CONTROL_TYPE, zones, DEPTH(), constraint, val);
}

int rapl_sysfs_constraint_get_power_limit_uw(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val) {
  DECL_ZONES();
  return powercap_sysfs_constraint_get_power_limit_uw(CONTROL_TYPE, zones, DEPTH(), constraint, val);
}

int rapl_sysfs_constraint_set_time_window_us(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t val) {
  DECL_ZONES();
  return powercap_sysfs_constraint_set_time_window_us(CONTROL_TYPE, zones, DEPTH(), constraint, val);
}

int rapl_sysfs_constraint_get_time_window_us(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val) {
  DECL_ZONES();
  return powercap_sysfs_constraint_get_time_window_us(CONTROL_TYPE, zones, DEPTH(), constraint, val);
}

int rapl_sysfs_constraint_get_max_power_uw(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val) {
  DECL_ZONES();
  return powercap_sysfs_constraint_get_max_power_uw(CONTROL_TYPE, zones, DEPTH(), constraint, val);
}

int rapl_sysfs_constraint_get_min_power_uw(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val) {
  DECL_ZONES();
  return powercap_sysfs_constraint_get_min_power_uw(CONTROL_TYPE, zones, DEPTH(), constraint, val);
}

int rapl_sysfs_constraint_get_max_time_window_us(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val) {
  DECL_ZONES();
  return powercap_sysfs_constraint_get_max_time_window_us(CONTROL_TYPE, zones, DEPTH(), constraint, val);
}

int rapl_sysfs_constraint_get_min_time_window_us(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val) {
  DECL_ZONES();
  return powercap_sysfs_constraint_get_min_time_window_us(CONTROL_TYPE, zones, DEPTH(), constraint, val);
}

ssize_t rapl_sysfs_constraint_get_name(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, char* buf, size_t size) {
  DECL_ZONES();
  return powercap_sysfs_constraint_get_name(CONTROL_TYPE, zones, DEPTH(), constraint, buf, size);
}
