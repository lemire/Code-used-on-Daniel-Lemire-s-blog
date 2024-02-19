/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * I/O operations for powercap sysfs files.
 *
 * @author Connor Imes
 * @date 2016-06-01
 */
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include "powercap.h"
#include "powercap-common.h"

int powercap_control_type_file_get_name(powercap_control_type_file type, char* buf, size_t size) {
  /* check type in case users pass bad int value instead of enum; int cast silences clang compiler */
  if (!buf || !size || (int) type < 0 || (int) type > POWERCAP_CONTROL_TYPE_FILE_ENABLED) {
    errno = EINVAL;
    return -errno;
  }
  return snprintf_control_type_file(buf, size, type);
}

int powercap_zone_file_get_name(powercap_zone_file type, char* buf, size_t size) {
  /* check type in case users pass bad int value instead of enum; int cast silences clang compiler */
  if (!buf || !size || (int) type < 0 || (int) type > POWERCAP_ZONE_FILE_NAME) {
    errno = EINVAL;
    return -errno;
  }
  return snprintf_zone_file(buf, size, type);
}

int powercap_constraint_file_get_name(powercap_constraint_file type, uint32_t constraint, char* buf, size_t size) {
  /* check type in case users pass bad int value instead of enum; int cast silences clang compiler */
  if (!buf || !size || (int) type < 0 || (int) type > POWERCAP_CONSTRAINT_FILE_NAME) {
    errno = EINVAL;
    return -errno;
  }
  return snprintf_constraint_file(buf, size, type, constraint);
}

#if 0
/**
 * Get the full path to a control type if depth is 0 (in which case zones may be NULL), or a control type's zone.
 * Return is like snprintf, except if the output was truncated due to the size limit, the return value is still > size,
 * but not necessarily the number of characters (excluding the terminating null byte) which would have been written to
 * the final string if enough space had been available.
 * Returns a negative value in case of other error.
 */
int powercap_get_path(const char* control_type_name, const uint32_t* zones, uint32_t depth, char* buf, size_t size) {
  if (!is_valid_control_type(control_type_name) || !buf || !size || (depth && !zones)) {
    errno = EINVAL;
    return -errno;
  }
  return snprintf_base_path(buf, size, control_type_name, zones, depth);
}

/**
 * Get the full path to a control type file.
 * Return is like snprintf, except if the output was truncated due to the size limit, the return value is still > size,
 * but not necessarily the number of characters (excluding the terminating null byte) which would have been written to
 * the final string if enough space had been available.
 * Returns a negative value in case of other error.
 */
int powercap_control_type_file_get_path(powercap_control_type_file type, const char* control_type_name, char* buf,
                                        size_t size) {
  /* check type in case users pass bad int value instead of enum; int cast silences clang compiler */
  if (!is_valid_control_type(control_type_name) || !buf || !size ||
      (int) type < 0 || (int) type > POWERCAP_CONTROL_TYPE_FILE_ENABLED) {
    errno = EINVAL;
    return -errno;
  }
  return snprintf_control_type_file_path(buf, size, control_type_name, type);
}

/**
 * Get the full path to a zone file.
 * Return is like snprintf, except if the output was truncated due to the size limit, the return value is still > size,
 * but not necessarily the number of characters (excluding the terminating null byte) which would have been written to
 * the final string if enough space had been available.
 * Returns a negative value in case of other error.
 */
int powercap_zone_file_get_path(powercap_zone_file type, const char* control_type_name, const uint32_t* zones,
                                uint32_t depth, char* buf, size_t size) {
  /* check type in case users pass bad int value instead of enum; int cast silences clang compiler */
  if (!is_valid_control_type(control_type_name) || !buf || !size ||
      (int) type < 0 || (int) type > POWERCAP_ZONE_FILE_NAME || (depth && !zones)) {
    errno = EINVAL;
    return -errno;
  }
  return snprintf_zone_file_path(buf, size, control_type_name, zones, depth, type);
}

/**
 * Get the full path to a constraint file.
 * Return is like snprintf, except if the output was truncated due to the size limit, the return value is still > size,
 * but not necessarily the number of characters (excluding the terminating null byte) which would have been written to
 * the final string if enough space had been available.
 * Returns a negative value in case of other error.
 */
int powercap_constraint_file_get_path(powercap_constraint_file type, const char* control_type_name,
                                      const uint32_t* zones, uint32_t depth, uint32_t constraint, char* buf,
                                      size_t size) {
  /* check type in case users pass bad int value instead of enum; int cast silences clang compiler */
  if (!is_valid_control_type(control_type_name) || !buf || !size ||
      (int) type < 0 || (int) type > POWERCAP_CONSTRAINT_FILE_NAME || (depth && !zones)) {
    errno = EINVAL;
    return -errno;
  }
  return snprintf_constraint_file_path(buf, size, control_type_name, zones, depth, constraint, type);
}
#endif

int powercap_control_type_file_open(powercap_control_type* control, powercap_control_type_file type,
                                    const char* control_type_name, int flags) {
  char buf[PATH_MAX];
  int fd;
  /* check type in case users pass bad int value instead of enum; int cast silences clang compiler */
  if (!is_valid_control_type(control_type_name) || (int) type < 0 || (int) type > POWERCAP_CONTROL_TYPE_FILE_ENABLED) {
    errno = EINVAL;
    return -errno;
  }
  fd = open_control_type_file(buf, sizeof(buf), control_type_name, type, flags);
  if (control) {
    switch (type) {
      case POWERCAP_CONTROL_TYPE_FILE_ENABLED:
        control->enabled = fd;
        break;
      default:
        // unreachable
        errno = EINVAL;
        return -errno;
    }
  }
  return fd;
}

int powercap_zone_file_open(powercap_zone* zone, powercap_zone_file type, const char* control_type_name,
                            const uint32_t* zones, uint32_t depth, int flags) {
  char buf[PATH_MAX];
  int fd;
  /* check type in case users pass bad int value instead of enum; int cast silences clang compiler */
  if (!is_valid_control_type(control_type_name) || (int) type < 0 || (int) type > POWERCAP_ZONE_FILE_NAME ||
      (depth && !zones)) {
    errno = EINVAL;
    return -errno;
  }
  fd = open_zone_file(buf, sizeof(buf), control_type_name, zones, depth, type, flags);
  if (zone) {
    switch (type) {
      case POWERCAP_ZONE_FILE_MAX_ENERGY_RANGE_UJ:
        zone->max_energy_range_uj = fd;
        break;
      case POWERCAP_ZONE_FILE_ENERGY_UJ:
        zone->energy_uj = fd;
        break;
      case POWERCAP_ZONE_FILE_MAX_POWER_RANGE_UW:
        zone->max_power_range_uw = fd;
        break;
      case POWERCAP_ZONE_FILE_POWER_UW:
        zone->power_uw = fd;
        break;
      case POWERCAP_ZONE_FILE_ENABLED:
        zone->enabled = fd;
        break;
      case POWERCAP_ZONE_FILE_NAME:
        zone->name = fd;
        break;
      default:
        // unreachable
        errno = EINVAL;
        return -errno;
    }
  }
  return fd;
}

int powercap_constraint_file_open(powercap_constraint* constraint, powercap_constraint_file type,
                                  const char* control_type_name, const uint32_t* zones, uint32_t depth,
                                  uint32_t constraint_num, int flags) {
  char buf[PATH_MAX];
  int fd;
  if (!constraint || !is_valid_control_type(control_type_name) ||
      (int) type < 0 || (int) type > POWERCAP_CONSTRAINT_FILE_NAME || (depth && !zones)) {
    errno = EINVAL;
    return -errno;
  }
  fd = open_constraint_file(buf, sizeof(buf), control_type_name, zones, depth, constraint_num, type, flags);
  if (constraint) {
    switch (type) {
      case POWERCAP_CONSTRAINT_FILE_POWER_LIMIT_UW:
        constraint->power_limit_uw = fd;
        break;
      case POWERCAP_CONSTRAINT_FILE_TIME_WINDOW_US:
        constraint->time_window_us = fd;
        break;
      case POWERCAP_CONSTRAINT_FILE_MAX_POWER_UW:
        constraint->max_power_uw = fd;
        break;
      case POWERCAP_CONSTRAINT_FILE_MIN_POWER_UW:
        constraint->min_power_uw = fd;
        break;
      case POWERCAP_CONSTRAINT_FILE_MAX_TIME_WINDOW_US:
        constraint->max_time_window_us = fd;
        break;
      case POWERCAP_CONSTRAINT_FILE_MIN_TIME_WINDOW_US:
        constraint->min_time_window_us = fd;
        break;
      case POWERCAP_CONSTRAINT_FILE_NAME:
        constraint->name = fd;
        break;
      default:
        // unreachable
        errno = EINVAL;
        return -errno;
    }
  }
  return fd;
}

#define VERIFY_ARG(arg) \
  if (!(arg)) { \
    errno = EINVAL; \
    return -errno; \
  }

int powercap_control_type_set_enabled(const powercap_control_type* control_type, int val) {
  VERIFY_ARG(control_type);
  return write_u64(control_type->enabled, (uint64_t) val);
}

int powercap_control_type_get_enabled(const powercap_control_type* control_type, int* val) {
  uint64_t enabled;
  int ret;
  VERIFY_ARG(control_type);
  VERIFY_ARG(val);
  if (!(ret = read_u64(control_type->enabled, &enabled))) {
    *val = enabled ? 1 : 0;
  }
  return ret;
}

int powercap_zone_get_max_energy_range_uj(const powercap_zone* zone, uint64_t* val) {
  VERIFY_ARG(zone);
  return read_u64(zone->max_energy_range_uj, val);
}

int powercap_zone_get_energy_uj(const powercap_zone* zone, uint64_t* val) {
  VERIFY_ARG(zone);
  return read_u64(zone->energy_uj, val);
}

int powercap_zone_reset_energy_uj(const powercap_zone* zone) {
  VERIFY_ARG(zone);
  return write_u64(zone->energy_uj, 0);
}

int powercap_zone_get_max_power_range_uw(const powercap_zone* zone, uint64_t* val) {
  VERIFY_ARG(zone);
  return read_u64(zone->max_power_range_uw, val);
}

int powercap_zone_get_power_uw(const powercap_zone* zone, uint64_t* val) {
  VERIFY_ARG(zone);
  return read_u64(zone->power_uw, val);
}

int powercap_zone_set_enabled(const powercap_zone* zone, int val) {
  VERIFY_ARG(zone);
  return write_u64(zone->enabled, (uint64_t) val);
}

int powercap_zone_get_enabled(const powercap_zone* zone, int* val) {
  uint64_t enabled;
  int ret;
  VERIFY_ARG(zone);
  VERIFY_ARG(val);
  if (!(ret = read_u64(zone->enabled, &enabled))) {
    *val = enabled ? 1 : 0;
  }
  return ret;
}

ssize_t powercap_zone_get_name(const powercap_zone* zone, char* buf, size_t size) {
  VERIFY_ARG(zone);
  return read_string(zone->name, buf, size);
}

int powercap_constraint_set_power_limit_uw(const powercap_constraint* constraint, uint64_t val) {
  VERIFY_ARG(constraint)
  return write_u64(constraint->power_limit_uw, val);
}

int powercap_constraint_get_power_limit_uw(const powercap_constraint* constraint, uint64_t* val) {
  VERIFY_ARG(constraint)
  return read_u64(constraint->power_limit_uw, val);
}

int powercap_constraint_set_time_window_us(const powercap_constraint* constraint, uint64_t val) {
  VERIFY_ARG(constraint)
  return write_u64(constraint->time_window_us, val);
}

int powercap_constraint_get_time_window_us(const powercap_constraint* constraint, uint64_t* val) {
  VERIFY_ARG(constraint)
  return read_u64(constraint->time_window_us, val);
}

int powercap_constraint_get_max_power_uw(const powercap_constraint* constraint, uint64_t* val) {
  VERIFY_ARG(constraint)
  return read_u64(constraint->max_power_uw, val);
}

int powercap_constraint_get_min_power_uw(const powercap_constraint* constraint, uint64_t* val) {
  VERIFY_ARG(constraint)
  return read_u64(constraint->min_power_uw, val);
}

int powercap_constraint_get_max_time_window_us(const powercap_constraint* constraint, uint64_t* val) {
  VERIFY_ARG(constraint)
  return read_u64(constraint->max_time_window_us, val);
}

int powercap_constraint_get_min_time_window_us(const powercap_constraint* constraint, uint64_t* val) {
  VERIFY_ARG(constraint)
  return read_u64(constraint->min_time_window_us, val);
}

ssize_t powercap_constraint_get_name(const powercap_constraint* constraint, char* buf, size_t size) {
  VERIFY_ARG(constraint)
  return read_string(constraint->name, buf, size);
}
