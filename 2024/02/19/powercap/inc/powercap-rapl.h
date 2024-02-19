/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * A simple interface for configuring RAPL through a powercap control type.
 * Note that not all RAPL zones support short_term constraints.
 * Unless otherwise stated, all functions return 0 on success or a negative value on error.
 *
 * Setter functions do not verify that written values are accepted by RAPL.
 * Users may want to add a debug option to their software that follows writes/sets with a read/get.
 *
 * These operations do basic I/O - it may reasonably be expected that callers need to handle I/O errors.
 * For example, it has been seen that "powercap_rapl_get_max_power_uw" sets errno=ENODATA for power zones.
 *
 * The terms "package" and "pkg" refer to top-level (parent) RAPL instances, which aren't necessarily physical packages.
 * Prior to Cascade Lake CPUs (2019), RAPL top-level instances mapped one-to-one with physical sockets/packages.
 * Some systems now support multiple die on a physical socket/package, resulting in multiple top-level instances per
 * physical socket/package.
 * It is also possible that the scope of a top-level instances could change again in the future.
 * Thus, it should not be assumed that a 'powercap_rapl_pkg' instance or a 'POWERCAP_RAPL_ZONE_PACKAGE' zone maps
 * one-to-one with a physical socket.
 * Intel's backward compatibility _appears_ to be in a zone's name, but even this is not explicitly guaranteed - it is
 * the user's responsibility to interpret what a top-level RAPL instance actually is.
 *
 * @author Connor Imes
 * @date 2016-05-12
 */
#ifndef _POWERCAP_RAPL_H_
#define _POWERCAP_RAPL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <unistd.h>
#include "powercap.h"

#if !defined(POWERCAP_DEPRECATED)
#if defined(POWERCAP_ALLOW_DEPRECATED)
#define POWERCAP_DEPRECATED(x)
#elif defined(__GNUC__) || defined(__clang__)
#define POWERCAP_DEPRECATED(x) __attribute__((deprecated(x)))
#elif defined(_MSC_VER)
#define POWERCAP_DEPRECATED(x) __declspec(deprecated(x))
#else
#define POWERCAP_DEPRECATED(x)
#endif
#endif

/**
 * Files for each zone.
 */
typedef struct powercap_rapl_zone_files {
  powercap_zone zone;
  powercap_constraint constraint_long;
  powercap_constraint constraint_short;
} powercap_rapl_zone_files;

/**
 * All files for a top-level RAPL instance.
 */
typedef struct powercap_rapl_pkg {
  powercap_rapl_zone_files pkg;
  powercap_rapl_zone_files core;
  powercap_rapl_zone_files uncore;
  powercap_rapl_zone_files dram;
  powercap_rapl_zone_files psys;
} powercap_rapl_pkg;

/**
 * Zone enumeration.
 */
typedef enum powercap_rapl_zone {
  POWERCAP_RAPL_ZONE_PACKAGE,
  POWERCAP_RAPL_ZONE_CORE,
  POWERCAP_RAPL_ZONE_UNCORE,
  POWERCAP_RAPL_ZONE_DRAM,
  POWERCAP_RAPL_ZONE_PSYS
} powercap_rapl_zone;

/**
 * Long/short term constraint enumeration.
 */
typedef enum powercap_rapl_constraint {
  POWERCAP_RAPL_CONSTRAINT_LONG,
  POWERCAP_RAPL_CONSTRAINT_SHORT
} powercap_rapl_constraint;

/**
 * Check if RAPL is supported.
 * Returns 1 if enabled, 0 if disabled, a negative value in case of error.
 */
int powercap_rapl_control_is_supported(void);

/**
 * Check if RAPL control is enabled.
 * Returns 1 if enabled, 0 if disabled, a negative value in case of error.
 */
int powercap_rapl_control_is_enabled(void);

/**
 * Enable/disable RAPL control.
 */
int powercap_rapl_control_set_enabled(int val);

/**
 * Get the number of top-level (parent) RAPL instances found.
 * Returns 0 and sets errno if none are found.
 */
uint32_t powercap_rapl_get_num_instances(void);

/**
 * @deprecated Use powercap_rapl_get_num_instances() instead.
 *
 * This function's name no longer accurately describes its scope.
 *
 * Get the number of top-level (parent) RAPL instances found.
 * Returns 0 and sets errno if none are found.
 */
POWERCAP_DEPRECATED("Call powercap_rapl_get_num_instances() instead")
uint32_t powercap_rapl_get_num_packages(void);

/**
 * Initialize the struct for the parent zone with the given identifier.
 * Read-only access can be requested, which may prevent the need for elevated privileges.
 */
int powercap_rapl_init(uint32_t id, powercap_rapl_pkg* pkg, int read_only);

/**
 * Clean up file descriptors.
 */
int powercap_rapl_destroy(powercap_rapl_pkg* pkg);

/**
 * Check if a zone is supported.
 * The uncore power zone is usually only available on client-side hardware.
 * The DRAM power zone is usually only available on server-side hardware.
 * Some systems may expose zones like DRAM without actually supporting power caps for them.
 * The PSys power zone may be available on Skylake processors and later.
 * Returns 1 if supported, 0 if unsupported, a negative value in case of error.
 */
int powercap_rapl_is_zone_supported(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone);

/**
 * Check if a constraint is supported for a zone.
 * Returns 1 if supported, 0 if unsupported, a negative value in case of error.
 */
int powercap_rapl_is_constraint_supported(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint);

/**
 * Check if a file is supported for the given zone.
 * Returns 1 if supported, 0 if unsupported, a negative value in case of error.
 */
int powercap_rapl_is_zone_file_supported(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_zone_file file);

/**
 * Check if a file is supported for the given zone and constraint.
 * Returns 1 if supported, 0 if unsupported, a negative value in case of error.
 */
int powercap_rapl_is_constraint_file_supported(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, powercap_constraint_file file);

/**
 * Get the zone name.
 * Returns a non-negative value for the number of bytes read, a negative value in case of error.
 */
ssize_t powercap_rapl_get_name(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, char* buf, size_t size);

/**
 * Check if zone is enabled.
 * Returns 1 if enabled, 0 if disabled, a negative value in case of error.
 */
int powercap_rapl_is_enabled(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone);

/**
 * Enable/disable a zone.
 */
int powercap_rapl_set_enabled(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, int enabled);

/**
 * Get the max energy range in microjoules.
 */
int powercap_rapl_get_max_energy_range_uj(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, uint64_t* val);

/**
 * Get the current energy in microjoules.
 */
int powercap_rapl_get_energy_uj(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, uint64_t* val);

/**
 * Reset the energy_uj value to 0.
 * Returns 0 on success, a negative value in case of error.
 * NOTE: As of this writing, RAPL does not support resetting the energy counter.
 */
int powercap_rapl_reset_energy_uj(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone);

/**
 * Get the max power range in microwatts.
 * NOTE: As of this writing, RAPL does not support this file.
 */
int powercap_rapl_get_max_power_range_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, uint64_t* val);

/**
 * Get the current power in microwatts.
 * NOTE: As of this writing, RAPL does not support this file.
 */
int powercap_rapl_get_power_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, uint64_t* val);

/**
 * Get the maximum power in microwatts.
 */
int powercap_rapl_get_max_power_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val);

/**
 * Get the minimum power allowed in microwatts.
 * NOTE: As of this writing, RAPL does not support this file.
 */
int powercap_rapl_get_min_power_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val);

/**
 * Get the power limit in microwatts.
 */
int powercap_rapl_get_power_limit_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val);

/**
 * Set the power limit in microwatts.
 */
int powercap_rapl_set_power_limit_uw(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t val);

/**
 * Get the maximum time window in microseconds.
 * NOTE: As of this writing, RAPL does not support this file.
 */
int powercap_rapl_get_max_time_window_us(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val);

/**
 * Get the minimum time window in microseconds.
 * NOTE: As of this writing, RAPL does not support this file.
 */
int powercap_rapl_get_min_time_window_us(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val);

/**
 * Get the time window in microseconds.
 */
int powercap_rapl_get_time_window_us(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t* val);

/**
 * Set the time window in microseconds.
 */
int powercap_rapl_set_time_window_us(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, uint64_t val);

/**
 * Get the constraint name.
 * Returns a non-negative value for the number of bytes read, a negative value in case of error.
 */
ssize_t powercap_rapl_get_constraint_name(const powercap_rapl_pkg* pkg, powercap_rapl_zone zone, powercap_rapl_constraint constraint, char* buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif
