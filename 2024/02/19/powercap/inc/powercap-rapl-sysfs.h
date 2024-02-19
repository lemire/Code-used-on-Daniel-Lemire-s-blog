/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * This interface is deprecated - use powercap-sysfs.h directly instead.
 * By limiting zone depth to 2, this interface will break if Intel RAPL extends its tree depth in the future.
 * The only other abstraction it provides is to mask the control type parameter, which doesn't add much value.
 * Intel also added the intel-rapl-mmio control type, so even the control type abstraction is not as useful as before.
 *
 * Read/write RAPL sysfs files.
 * This is a wrapper around powercap-sysfs.h.
 *
 * The control type is "intel-rapl" and zone depth is limited to 2.
 * The "zone" parameters below are for the top-level "zone", and the optional "sz" parameters are for other control
 * planes like "core", "uncore", and "dram".
 * The "is_sz" parameter must be non-zero when working with these control planes.
 *
 * For example, zone=0, sz=0, is_sz=1 is usually for the "core" power plane and is analogous to using powercap-sysfs.h
 * with zones[2]={0, 0}, depth=2.
 *
 * The "intel-rapl" control type implements a subset of the powercap interface.
 * The following functions are not currently supported by RAPL, but are implemented here in case RAPL adds support for
 * them in the future:
 * - rapl_sysfs_zone_reset_energy_uj
 * - rapl_sysfs_zone_get_max_power_range_uw
 * - rapl_sysfs_zone_get_power_uw
 * - rapl_sysfs_constraint_get_min_power_uw
 * - rapl_sysfs_constraint_get_max_time_window_us
 * - rapl_sysfs_constraint_get_min_time_window_us
 *
 * @author Connor Imes
 * @date 2017-08-24
 * @deprecated Use powercap-sysfs.h directly instead.
 */

#ifndef _RAPL_SYSFS_H
#define _RAPL_SYSFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <unistd.h>

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
 * Determine if a zone or subzone exist.
 * It is _not_ assumed that a zone maps one-to-one with an particular physical component like a socket or die.
 *
 * Originally, a zone mapped to a physical socket/package, but this assumed mapping did not hold.
 * As of 2019, a zone maps to a CPU die, but nothing prevents Intel from changing the scope again in the future.
 * Their backward compatibility _appears_ to be in a zone's name, but even this is not explicitly guaranteed, nor does
 * this sysfs binding interface make such an assumption - it is the user's responsibility to interpret what a zone is.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @return 0 if zone exists, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_zone_exists() instead")
int rapl_sysfs_zone_exists(uint32_t zone, uint32_t sz, int is_sz);

/**
 * @deprecated Use rapl_sysfs_zone_exists() instead.
 *
 * This function's name no longer accurately describes its scope.
 * Prior to Cascade Lake CPUs (2019), RAPL top-level (parent) zones mapped one-to-one with physical sockets/packages.
 * Thus the term "pkg" made sense and was chosen over the more general term "zone".
 * Some systems now support multiple die on a physical socket/package, resulting in multiple top-level (parent) zones
 * per physical socket/package.
 * The scope of a zone could potentially change again in the future.
 *
 * This function checks if a top-level (parent) zone exists, where num(top-level zones) >= num(sockets).
 * For systems with a single die per physical socket, num(top-level zones) == num(sockets).
 * For systems with multiple die per physical socket, num(top-level zones) > num(sockets).
 *
 * @param zone
 * @return 0 if zone exists, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_zone_exists() instead")
int rapl_sysfs_pkg_exists(uint32_t zone);

/**
 * @deprecated Use rapl_sysfs_zone_exists() instead.
 *
 * Determine if a subzone exists.
 *
 * @param zone
 * @param sz
 * @return 0 if subzone exists, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_zone_exists() instead")
int rapl_sysfs_sz_exists(uint32_t zone, uint32_t sz);

/**
 * Determine if a constraint exists.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param constraint
 * @return 0 if constraint exists, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_constraint_exists() instead")
int rapl_sysfs_constraint_exists(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint);

/**
 * Get max_energy_range_uj for a zone.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_zone_get_max_energy_range_uj() instead")
int rapl_sysfs_zone_get_max_energy_range_uj(uint32_t zone, uint32_t sz, int is_sz, uint64_t* val);

/**
 * Reset energy_uj for a zone.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_zone_reset_energy_uj() instead")
int rapl_sysfs_zone_reset_energy_uj(uint32_t zone, uint32_t sz, int is_sz);

/**
 * Get energy_uj for a zone.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_zone_get_energy_uj() instead")
int rapl_sysfs_zone_get_energy_uj(uint32_t zone, uint32_t sz, int is_sz, uint64_t* val);

/**
 * Get max_power_range_uw for a zone.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_zone_get_max_power_range_uw() instead")
int rapl_sysfs_zone_get_max_power_range_uw(uint32_t zone, uint32_t sz, int is_sz, uint64_t* val);

/**
 * Get power_uw for a zone.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_zone_get_power_uw() instead")
int rapl_sysfs_zone_get_power_uw(uint32_t zone, uint32_t sz, int is_sz, uint64_t* val);

/**
 * Enable/disable a zone.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_zone_set_enabled() instead")
int rapl_sysfs_zone_set_enabled(uint32_t zone, uint32_t sz, int is_sz, uint32_t val);

/**
 * Get whether a zone is enabled or disabled.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_zone_get_enabled() instead")
int rapl_sysfs_zone_get_enabled(uint32_t zone, uint32_t sz, int is_sz, uint32_t* val);

/**
 * Get name for a zone.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param buf
 * @param size
 * @return number of bytes read, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_zone_get_name() instead")
ssize_t rapl_sysfs_zone_get_name(uint32_t zone, uint32_t sz, int is_sz, char* buf, size_t size);

/**
 * Set power_limit_uw for a constraint.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_constraint_set_power_limit_uw() instead")
int rapl_sysfs_constraint_set_power_limit_uw(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t val);

/**
 * Get power_limit_uw for a constraint.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_constraint_get_power_limit_uw() instead")
int rapl_sysfs_constraint_get_power_limit_uw(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val);

/**
 * Set time_window_us for a constraint.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_constraint_set_time_window_us() instead")
int rapl_sysfs_constraint_set_time_window_us(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t val);

/**
 * Get time_window_us for a constraint.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_constraint_get_time_window_us() instead")
int rapl_sysfs_constraint_get_time_window_us(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val);

/**
 * Get max_power_uw for a constraint.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_constraint_get_max_power_uw() instead")
int rapl_sysfs_constraint_get_max_power_uw(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val);

/**
 * Get get_min_power_uw for a constraint.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_constraint_get_min_power_uw() instead")
int rapl_sysfs_constraint_get_min_power_uw(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val);

/**
 * Get max_time_window_us for a constraint.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_constraint_get_max_time_window_us() instead")
int rapl_sysfs_constraint_get_max_time_window_us(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val);

/**
 * Get min_time_window_us for a constraint.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param constraint
 * @param val
 * @return 0 on success, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_constraint_get_min_time_window_us() instead")
int rapl_sysfs_constraint_get_min_time_window_us(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, uint64_t* val);


/**
 * Get name for a constraint.
 *
 * @param zone
 * @param sz
 * @param is_sz
 * @param constraint
 * @param buf
 * @param size
 * @return number of bytes read, a negative error code otherwise.
 */
POWERCAP_DEPRECATED("Call powercap_sysfs_constraint_get_name() instead")
ssize_t rapl_sysfs_constraint_get_name(uint32_t zone, uint32_t sz, int is_sz, uint32_t constraint, char* buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif
