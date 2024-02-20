/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Read/write powercap sysfs files.
 *
 * The "control_type" parameter to functions is the name of the control type as it appears in the filesystem at
 * /sys/class/powercap, excluding zone/subzone identifiers, e.g., "intel-rapl".
 * The parameter cannot be NULL, empty, or contain any '.' or '/' characters.
 *
 * It is assumed that the folder tree structure includes the control type name at each directory level, then appends
 * zone indices after colons.
 * For example, a subpath to an Intel RAPL power plane looks like: "intel-rapl/intel-rapl:0/intel-rapl:0:1/".
 *
 * The "zones" parameter to functions is an array of at least "depth" length and describes the index to use at each
 * level in a control type's zone hierarchy tree.
 * The value at each index in the array, from zones[0] up to and including zones[depth-1], is the zone/subzone to
 * operate on at that index's level of the tree.
 * To operate on a top-level zone, only zones[0] needs to be set and "depth" must be 1.
 * To operate on a subzone of a top-level zone, zones[0] must be set to the top-level zone index, zones[1] must be set
 * to the subzone index, and "depth" must be 2.
 * The parameter must not be NULL.
 *
 * The "name" parameters also must not be NULL, and their "size" must be at least 2 characters.
 *
 * @author Connor Imes
 * @date 2017-08-24
 */

#ifndef _POWERCAP_SYSFS_H
#define _POWERCAP_SYSFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <unistd.h>

/**
 * Determine if a a control type exists.
 *
 * @param control_type
 * @return 0 if control type exists, a negative error code otherwise.
 */
int powercap_sysfs_control_type_exists(const char* control_type);

/**
 * Determine if a zone exists.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @return 0 if zone exists, a negative error code otherwise.
 */
int powercap_sysfs_zone_exists(const char* control_type, const uint32_t* zones, uint32_t depth);

/**
 * Determine if a constraint exists.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param constraint
 * @return 0 if constraint exists, a negative error code otherwise.
 */
int powercap_sysfs_constraint_exists(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t constraint);

/**
 * Enable/disable a control type.
 *
 * @param control_type
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_control_type_set_enabled(const char* control_type, uint32_t val);

/**
 * Get whether a control type is enabled or disabled.
 *
 * @param control_type
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_control_type_get_enabled(const char* control_type, uint32_t* val);

/**
 * Get max_energy_range_uj for a zone.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_zone_get_max_energy_range_uj(const char* control_type, const uint32_t* zones, uint32_t depth, uint64_t* val);

/**
 * Reset energy_uj for a zone.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_zone_reset_energy_uj(const char* control_type, const uint32_t* zones, uint32_t depth);

/**
 * Get energy_uj for a zone.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_zone_get_energy_uj(const char* control_type, const uint32_t* zones, uint32_t depth, uint64_t* val);


/**
 * Get max_power_range_uw for a zone.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_zone_get_max_power_range_uw(const char* control_type, const uint32_t* zones, uint32_t depth, uint64_t* val);

/**
 * Get power_uw for a zone.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_zone_get_power_uw(const char* control_type, const uint32_t* zones, uint32_t depth, uint64_t* val);

/**
 * Enable/disable a zone.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_zone_set_enabled(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t val);

/**
 * Get whether a zone is enabled or disabled.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_zone_get_enabled(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t* val);

/**
 * Get name for a zone.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param buf
 * @param size
 * @return number of bytes read (always > 0), a negative error code otherwise.
 */
ssize_t powercap_sysfs_zone_get_name(const char* control_type, const uint32_t* zones, uint32_t depth, char* buf, size_t size);

/**
 * Set power_limit_uw for a constraint.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_constraint_set_power_limit_uw(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t constraint, uint64_t val);

/**
 * Get power_limit_uw for a constraint.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_constraint_get_power_limit_uw(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t constraint, uint64_t* val);

/**
 * Set time_window_us for a constraint.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_constraint_set_time_window_us(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t constraint, uint64_t val);

/**
 * Get time_window_us for a constraint.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_constraint_get_time_window_us(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t constraint, uint64_t* val);

/**
 * Get max_power_uw for a constraint.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_constraint_get_max_power_uw(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t constraint, uint64_t* val);

/**
 * Get get_min_power_uw for a constraint.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_constraint_get_min_power_uw(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t constraint, uint64_t* val);

/**
 * Get max_time_window_us for a constraint.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_constraint_get_max_time_window_us(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t constraint, uint64_t* val);

/**
 * Get min_time_window_us for a constraint.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
int powercap_sysfs_constraint_get_min_time_window_us(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t constraint, uint64_t* val);

/**
 * Get name for a constraint.
 *
 * @param control_type
 * @param zones
 * @param depth
 * @param constraint
 * @param buf
 * @param size
 * @return number of bytes read (always > 0), a negative error code otherwise.
 */
ssize_t powercap_sysfs_constraint_get_name(const char* control_type, const uint32_t* zones, uint32_t depth, uint32_t constraint, char* buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif
