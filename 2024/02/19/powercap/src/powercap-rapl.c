/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * RAPL implementation of powercap.
 *
 * @author Connor Imes
 * @date 2016-05-12
 */
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "powercap.h"
#include "powercap-common.h"
#include "powercap-rapl.h"
#include "powercap-sysfs.h"

#ifndef MAX_NAME_SIZE
  #define MAX_NAME_SIZE 64
#endif

#define CONTROL_TYPE "intel-rapl"

#define CONSTRAINT_NAME_LONG "long_term"
#define CONSTRAINT_NAME_SHORT "short_term"

#define ZONE_NAME_PREFIX_PKG "package"
#define ZONE_NAME_CORE "core"
#define ZONE_NAME_UNCORE "uncore"
#define ZONE_NAME_DRAM "dram"
#define ZONE_NAME_PSYS "psys"

static powercap_constraint* get_constraint_by_rapl_name(powercap_rapl_zone_files* fds, const uint32_t* zones, uint32_t depth, uint32_t constraint) {
  assert(fds != NULL);
  char name[MAX_NAME_SIZE];
  if (powercap_sysfs_constraint_get_name(CONTROL_TYPE, zones, depth, constraint, name, sizeof(name)) < 0) {
    return NULL;
  }
  if (!strncmp(name, CONSTRAINT_NAME_LONG, sizeof(CONSTRAINT_NAME_LONG))) {
    return &fds->constraint_long;
  } else if (!strncmp(name, CONSTRAINT_NAME_SHORT, sizeof(CONSTRAINT_NAME_SHORT))) {
    return &fds->constraint_short;
  } else {
    LOG(ERROR, "powercap-rapl: Unrecognized constraint name: %s\n", name);
    errno = EINVAL;
  }
  return NULL;
}

static int open_all(const uint32_t* zones, uint32_t depth, powercap_rapl_zone_files* fds, int ro) {
  assert(fds != NULL);
  char buf[PATH_MAX] = { 0 };
  powercap_constraint* pc;
  uint32_t i = 0;
  if (powercap_zone_open(&fds->zone, buf, sizeof(buf), CONTROL_TYPE, zones, depth, ro)) {
    LOG(ERROR, "powercap-rapl: %s: %s\n", buf, strerror(errno));
    return -errno;
  }
  // constraint 0 is supposed to be long_term and constraint 1 (if exists) should be short_term
  // note: never actually seen this problem, but not 100% sure it can't happen, so check anyway...
  while (!powercap_sysfs_constraint_exists(CONTROL_TYPE, zones, depth, i)) {
    if ((pc = get_constraint_by_rapl_name(fds, zones, depth, i)) == NULL) {
      return -errno;
    }
    // "power_limit_uw" is picked arbitrarily, but it is a required file
    if (pc->power_limit_uw) {
      if (depth == 1) {
        LOG(ERROR, "powercap-rapl: Duplicate constraint detected at zone: %"PRIu32"\n", zones[0]);
      } else {
        LOG(ERROR, "powercap-rapl: Duplicate constraint detected at zone: %"PRIu32":%"PRIu32"\n", zones[0], zones[1]);
      }
      errno = EINVAL;
      return -errno;
    }
    buf[0] = '\0';
    if (powercap_constraint_open(pc, buf, sizeof(buf), CONTROL_TYPE, zones, depth, i, ro)) {
      LOG(ERROR, "powercap-rapl: %s: %s\n", buf, strerror(errno));
      return -errno;
    }
    i++;
  }
  // powercap_sysfs_constraint_exists returns error code when constraint does not exist - make sure it's not our fault
  assert(errno != EINVAL);
  assert(errno != ENOBUFS);
  return 0;
}

static const powercap_rapl_zone_files* get_files(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone) {
  assert(pkg != NULL);
  switch (zone) {
    case POWERCAP_RAPL_ZONE_PACKAGE:
      return &pkg->pkg;
    case POWERCAP_RAPL_ZONE_CORE:
      return &pkg->core;
    case POWERCAP_RAPL_ZONE_UNCORE:
      return &pkg->uncore;
    case POWERCAP_RAPL_ZONE_DRAM:
      return &pkg->dram;
    case POWERCAP_RAPL_ZONE_PSYS:
      return &pkg->psys;
    default:
      // somebody passed a bad zone type
      LOG(ERROR, "powercap-rapl: Bad powercap_rapl_zone: %d\n", zone);
      errno = EINVAL;
      return NULL;
  }
}

static const powercap_zone* get_zone_files(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone) {
  assert(pkg != NULL);
  const powercap_rapl_zone_files* fds = get_files(pkg, zone);
  return fds == NULL ? NULL : &fds->zone;
}

static const powercap_constraint* get_constraint_files(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint) {
  assert(pkg != NULL);
  const powercap_rapl_zone_files* fds = get_files(pkg, zone);
  if (fds == NULL) {
    return NULL;
  }
  switch (constraint) {
    case POWERCAP_RAPL_CONSTRAINT_LONG:
      return &fds->constraint_long;
    case POWERCAP_RAPL_CONSTRAINT_SHORT:
      return &fds->constraint_short;
    default:
      // somebody passed a bad constraint type
      LOG(ERROR, "powercap-rapl: Bad powercap_rapl_constraint: %d\n", constraint);
      errno = EINVAL;
      return NULL;
  }
}

static int get_zone_fd(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_zone_file file) {
  assert(pkg != NULL);
  const powercap_zone* fds = get_zone_files(pkg, zone);
  if (fds == NULL) {
    return -errno;
  }
  switch (file) {
    case POWERCAP_ZONE_FILE_MAX_ENERGY_RANGE_UJ:
      return fds->max_energy_range_uj;
    case POWERCAP_ZONE_FILE_ENERGY_UJ:
      return fds->energy_uj;
    case POWERCAP_ZONE_FILE_MAX_POWER_RANGE_UW:
      return fds->max_power_range_uw;
    case POWERCAP_ZONE_FILE_POWER_UW:
      return fds->power_uw;
    case POWERCAP_ZONE_FILE_ENABLED:
      return fds->enabled;
    case POWERCAP_ZONE_FILE_NAME:
      return fds->name;
    default:
      LOG(ERROR, "powercap-rapl: Bad powercap_zone_file: %d\n", file);
      errno = EINVAL;
      return -errno;
  }
}

static int get_constraint_fd(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, powercap_constraint_file file) {
  assert(pkg != NULL);
  const powercap_constraint* fds = get_constraint_files(pkg, zone, constraint);
  if (fds == NULL) {
    return -errno;
  }
  switch (file) {
    case POWERCAP_CONSTRAINT_FILE_POWER_LIMIT_UW:
      return fds->power_limit_uw;
    case POWERCAP_CONSTRAINT_FILE_TIME_WINDOW_US:
      return fds->time_window_us;
    case POWERCAP_CONSTRAINT_FILE_MAX_POWER_UW:
      return fds->max_power_uw;
    case POWERCAP_CONSTRAINT_FILE_MIN_POWER_UW:
      return fds->min_power_uw;
    case POWERCAP_CONSTRAINT_FILE_MAX_TIME_WINDOW_US:
      return fds->max_time_window_us;
    case POWERCAP_CONSTRAINT_FILE_MIN_TIME_WINDOW_US:
      return fds->min_time_window_us;
    case POWERCAP_CONSTRAINT_FILE_NAME:
      return fds->name;
    default:
      LOG(ERROR, "powercap-rapl: Bad powercap_constraint_file: %d\n", file);
      errno = EINVAL;
      return -errno;
  }
}

static powercap_rapl_zone_files* get_files_by_name(powercap_rapl_pkg* pkg, const uint32_t* zones, uint32_t depth) {
  assert(pkg != NULL);
  char name[MAX_NAME_SIZE];
  if (powercap_sysfs_zone_get_name(CONTROL_TYPE, zones, depth, name, sizeof(name)) < 0) {
    return NULL;
  }
  if (!strncmp(name, ZONE_NAME_PREFIX_PKG, sizeof(ZONE_NAME_PREFIX_PKG) - 1)) {
    return &pkg->pkg;
  } else if (!strncmp(name, ZONE_NAME_CORE, sizeof(ZONE_NAME_CORE))) {
    return &pkg->core;
  } else if (!strncmp(name, ZONE_NAME_UNCORE, sizeof(ZONE_NAME_UNCORE))) {
    return &pkg->uncore;
  } else if (!strncmp(name, ZONE_NAME_DRAM, sizeof(ZONE_NAME_DRAM))) {
    return &pkg->dram;
  } else if (!strncmp(name, ZONE_NAME_PSYS, sizeof(ZONE_NAME_PSYS))) {
    return &pkg->psys;
  } else {
    LOG(ERROR, "powercap-rapl: Unrecognized zone name: %s\n", name);
    errno = EINVAL;
  }
  return NULL;
}

int powercap_rapl_control_is_supported(void) {
  int ret = powercap_sysfs_control_type_exists(CONTROL_TYPE);
  return ret ? (errno == ENOSYS ? 0 : ret) : 1;
}

int powercap_rapl_control_is_enabled(void) {
  uint32_t enabled;
  int ret = powercap_sysfs_control_type_get_enabled(CONTROL_TYPE, &enabled);
  return ret ? ret : (enabled ? 1 : 0);
}

int powercap_rapl_control_set_enabled(int val) {
  return powercap_sysfs_control_type_set_enabled(CONTROL_TYPE, (uint32_t) val);
}

uint32_t powercap_rapl_get_num_instances(void) {
  uint32_t n = 0;
  while (!powercap_sysfs_zone_exists(CONTROL_TYPE, &n, 1)) {
    n++;
  }
  if (!n) {
    LOG(ERROR, "powercap-rapl: No top-level "CONTROL_TYPE" zones found - is its kernel module loaded?\n");
    errno = ENOENT;
  }
  return n;
}

uint32_t powercap_rapl_get_num_packages(void) {
  return powercap_rapl_get_num_instances();
}

int powercap_rapl_init(uint32_t id, powercap_rapl_pkg* pkg, int read_only) {
  int ret;
  int err_save;
  uint32_t zones[2] = { id, 0 };
  powercap_rapl_zone_files* files;
  if (pkg == NULL) {
    errno = EINVAL;
    return -errno;
  }
  // first need the parent zone
  if ((files = get_files_by_name(pkg, zones, 1)) == NULL) {
    return -errno;
  }
  // force all fds to 0 so we don't try to operate on invalid descriptors
  memset(pkg, 0, sizeof(powercap_rapl_pkg));
  // first populate parent zone
  if (!(ret = open_all(zones, 1, files, read_only))) {
    // get subordinate power zones
    while(!powercap_sysfs_zone_exists(CONTROL_TYPE, zones, 2) && !ret) {
      if ((files = get_files_by_name(pkg, zones, 2)) == NULL) {
        ret = -errno;
      } else if (files->zone.name) {
        // zone has already been opened ("name" is picked arbitrarily, but it is a required file)
        LOG(ERROR, "powercap-rapl: Duplicate zone type detected at %"PRIu32":%"PRIu32"\n", zones[0], zones[1]);
        errno = EBUSY;
        ret = -errno;
      } else {
        ret = open_all(zones, 2, files, read_only);
        zones[1]++;
      }
    }
  }
  if (ret) {
    err_save = errno;
    powercap_rapl_destroy(pkg);
    errno = err_save;
  }
  return ret;
}

static int fds_destroy_all(powercap_rapl_zone_files* files) {
  assert(files != NULL);
  int ret = 0;
  ret |= powercap_zone_close(&files->zone);
  ret |= powercap_constraint_close(&files->constraint_long);
  ret |= powercap_constraint_close(&files->constraint_short);
  return ret;
}

int powercap_rapl_destroy(powercap_rapl_pkg* pkg) {
  int ret = 0;
  if (pkg != NULL) {
    ret |= fds_destroy_all(&pkg->pkg);
    ret |= fds_destroy_all(&pkg->core);
    ret |= fds_destroy_all(&pkg->uncore);
    ret |= fds_destroy_all(&pkg->dram);
    ret |= fds_destroy_all(&pkg->psys);
  }
  return ret;
}

int powercap_rapl_is_zone_supported(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone) {
  // POWERCAP_ZONE_FILE_NAME is picked arbitrarily, but it is a required file
  return powercap_rapl_is_zone_file_supported(pkg, zone, POWERCAP_ZONE_FILE_NAME);
}

int powercap_rapl_is_constraint_supported(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint) {
  // POWERCAP_CONSTRAINT_FILE_POWER_LIMIT_UW is picked arbitrarily, but it is a required file
  return powercap_rapl_is_constraint_file_supported(pkg, zone, constraint, POWERCAP_CONSTRAINT_FILE_POWER_LIMIT_UW);
}

int powercap_rapl_is_zone_file_supported(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_zone_file file) {
  int fd;
  if (pkg == NULL || (fd = get_zone_fd(pkg, zone, file)) < 0) {
    errno = EINVAL;
    return -errno;
  }
  return fd > 0 ? 1 : 0;
}

int powercap_rapl_is_constraint_file_supported(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, powercap_constraint_file file) {
  int fd;
  if (pkg == NULL || (fd = get_constraint_fd(pkg, zone, constraint, file)) < 0) {
    errno = EINVAL;
    return -errno;
  }
  return fd > 0 ? 1 : 0;
}

ssize_t powercap_rapl_get_name(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, char* buf, size_t size) {
  const powercap_zone* fds = get_zone_files(pkg, zone);
  return fds == NULL ? -errno : powercap_zone_get_name(fds, buf, size);
}

int powercap_rapl_is_enabled(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone) {
  int enabled = -1;
  int ret;
  const powercap_zone* fds = get_zone_files(pkg, zone);
  if (fds == NULL) {
    enabled = -errno;
  } else if ((ret = powercap_zone_get_enabled(fds, &enabled))) {
    enabled = ret;
  }
  return enabled;
}

int powercap_rapl_set_enabled(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, int enabled) {
  const powercap_zone* fds = get_zone_files(pkg, zone);
  return fds == NULL ? -errno : powercap_zone_set_enabled(fds, enabled);
}

int powercap_rapl_get_max_energy_range_uj(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, uint64_t* val) {
  const powercap_zone* fds = get_zone_files(pkg, zone);
  return fds == NULL ? -errno : powercap_zone_get_max_energy_range_uj(fds, val);
}

int powercap_rapl_get_energy_uj(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, uint64_t* val) {
  const powercap_zone* fds = get_zone_files(pkg, zone);
  return fds == NULL ? -errno : powercap_zone_get_energy_uj(fds, val);
}

int powercap_rapl_reset_energy_uj(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone) {
  const powercap_zone* fds = get_zone_files(pkg, zone);
  return fds == NULL ? -errno : powercap_zone_reset_energy_uj(fds);
}

int powercap_rapl_get_max_power_range_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, uint64_t* val) {
  const powercap_zone* fds = get_zone_files(pkg, zone);
  return fds == NULL ? -errno : powercap_zone_get_max_power_range_uw(fds, val);
}

int powercap_rapl_get_power_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, uint64_t* val) {
  const powercap_zone* fds = get_zone_files(pkg, zone);
  return fds == NULL ? -errno : powercap_zone_get_power_uw(fds, val);
}

int powercap_rapl_get_max_power_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val) {
  const powercap_constraint* fds = get_constraint_files(pkg, zone, constraint);
  return fds == NULL ? -errno : powercap_constraint_get_max_power_uw(fds, val);
}

int powercap_rapl_get_min_power_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val) {
  const powercap_constraint* fds = get_constraint_files(pkg, zone, constraint);
  return fds == NULL ? -errno : powercap_constraint_get_min_power_uw(fds, val);
}

int powercap_rapl_get_power_limit_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val) {
  const powercap_constraint* fds = get_constraint_files(pkg, zone, constraint);
  return fds == NULL ? -errno : powercap_constraint_get_power_limit_uw(fds, val);
}

int powercap_rapl_set_power_limit_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t val) {
  const powercap_constraint* fds = get_constraint_files(pkg, zone, constraint);
  return fds == NULL ? -errno : powercap_constraint_set_power_limit_uw(fds, val);
}

int powercap_rapl_get_max_time_window_us(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val) {
  const powercap_constraint* fds = get_constraint_files(pkg, zone, constraint);
  return fds == NULL ? -errno : powercap_constraint_get_max_time_window_us(fds, val);
}

int powercap_rapl_get_min_time_window_us(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val) {
  const powercap_constraint* fds = get_constraint_files(pkg, zone, constraint);
  return fds == NULL ? -errno : powercap_constraint_get_min_time_window_us(fds, val);
}

int powercap_rapl_get_time_window_us(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val) {
  const powercap_constraint* fds = get_constraint_files(pkg, zone, constraint);
  return fds == NULL ? -errno : powercap_constraint_get_time_window_us(fds, val);
}

int powercap_rapl_set_time_window_us(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t val) {
  const powercap_constraint* fds = get_constraint_files(pkg, zone, constraint);
  return fds == NULL ? -errno : powercap_constraint_set_time_window_us(fds, val);
}

ssize_t powercap_rapl_get_constraint_name(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, char* buf, size_t size) {
  const powercap_constraint* fds = get_constraint_files(pkg, zone, constraint);
  return fds == NULL ? -errno : powercap_constraint_get_name(fds, buf, size);
}
