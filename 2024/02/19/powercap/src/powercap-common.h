/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Common utilities, like logging.
 *
 * @author Connor Imes
 * @date 2017-05-09
 */
#ifndef _POWERCAP_COMMON_H_
#define _POWERCAP_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>
#include <stdio.h>
/* Main powercap header only used for enums */
#include "powercap.h"

#pragma GCC visibility push(hidden)

typedef enum powercap_loglevel {
  DEBUG = 0,
  INFO,
  WARN,
  ERROR,
  OFF,
} powercap_loglevel;

#ifndef POWERCAP_LOG_LEVEL
  #define POWERCAP_LOG_LEVEL WARN
#endif

#define TO_FILE(severity) (severity) >= WARN ? stderr : stdout

#define TO_LOG_PREFIX(severity) \
  (severity) == DEBUG ? "[DEBUG]" : \
  (severity) == INFO  ? "[INFO] " : \
  (severity) == WARN  ? "[WARN] " : \
                        "[ERROR]"

#define LOG(severity, ...) \
  do { if ((severity) >= POWERCAP_LOG_LEVEL) { \
      fprintf(TO_FILE((severity)), "%s [powercap] ", TO_LOG_PREFIX((severity))); \
      fprintf(TO_FILE((severity)), __VA_ARGS__); \
    } } while (0)

/* PATH_MAX should be defined in limits.h */
#ifndef PATH_MAX
  #pragma message("Warning: PATH_MAX was not defined")
  #define PATH_MAX 4096
#endif

/* buf must not be NULL and size >= 1 */
ssize_t read_string_safe(int fd, char* buf, size_t size);

/* Return number of bytes read (including terminating NULL char) on success, negative error code on failure */
ssize_t read_string(int fd, char* buf, size_t size);

/* Return 0 on success, negative error code on failure */
int read_u64(int fd, uint64_t* val);

/* Return 0 on success, negative error code on failure */
int write_u64(int fd, uint64_t val);

/* Simple names only, trying to look outside the powercap directory is not allowed */
int is_valid_control_type(const char* control_type);

/*
 * Return is like snprintf, except if the output was truncated due to the size limit, the return value is still > size,
 * but not necessarily the number of characters (excluding the terminating null byte) which would have been written to
 * the final string if enough space had been available.
 */
int snprintf_base_path(char* buf, size_t size, const char* control_type, const uint32_t* zones, uint32_t depth);

/* Return is like snprintf */
int snprintf_control_type_file(char* buf, size_t size, powercap_control_type_file type);

/* Return is like snprintf */
int snprintf_zone_file(char* buf, size_t size, powercap_zone_file type);

/* Return is like snprintf */
int snprintf_constraint_file(char* buf, size_t size, powercap_constraint_file type, uint32_t constraint);

/* Return is like snprintf_base_path */
int snprintf_control_type_file_path(char* path, size_t size, const char* control_type, powercap_control_type_file type);

/* Return is like snprintf_base_path */
int snprintf_zone_file_path(char* path, size_t size, const char* control_type, const uint32_t* zones, uint32_t depth,
                            powercap_zone_file type);

/* Return is like snprintf_base_path */
int snprintf_constraint_file_path(char* path, size_t size, const char* control_type, const uint32_t* zones,
                                  uint32_t depth, uint32_t constraint, powercap_constraint_file type);

/* Return fd on success, negative error code if path is too large, -1 on open failure */
int open_control_type_file(char* path, size_t size, const char* control_type, powercap_control_type_file type,
                           int flags);

/* Return fd on success, negative error code if path is too large, -1 on open failure */
int open_zone_file(char* path, size_t size, const char* control_type, const uint32_t* zones, uint32_t depth,
                   powercap_zone_file type, int flags);

/* Return fd on success, negative error code if path is too large, -1 on open failure */
int open_constraint_file(char* path, size_t size, const char* control_type, const uint32_t* zones, uint32_t depth,
                         uint32_t constraint, powercap_constraint_file type, int flags);

/*
 * Open all files in a control type, if they exist.
 * Return 0 on success or ENOENT, -1 if buf is too small or on open failure.
 */
int powercap_control_type_open(powercap_control_type* pct, char* buf, size_t bsize, const char* ct_name, int ro);

/*
 * Open all files in a zone, if they exist.
 * Return 0 on success or ENOENT, -1 if buf is too small or on open failure.
 */
int powercap_zone_open(powercap_zone* pz, char* buf, size_t bsize, const char* ct_name, const uint32_t* zones,
                       uint32_t depth, int ro);

/*
 * Open all files in a constraint, if they exist.
 * Return 0 on success or ENOENT, -1 if buf is too small or on open failure.
 */
int powercap_constraint_open(powercap_constraint* pc, char* buf, size_t bsize, const char* ct_name,
                             const uint32_t* zones, uint32_t depth, uint32_t constraint, int ro);

/*
 * Close all files in a control type.
 * Return 0 on success, negative error code on failure.
 */
int powercap_control_type_close(powercap_control_type* pct);

/*
 * Close all files in a zone.
 * Return 0 on success, negative error code on failure.
 */
int powercap_zone_close(powercap_zone* pz);

/*
 * Close all files in a constraint.
 * Return 0 on success, negative error code on failure.
 */
int powercap_constraint_close(powercap_constraint* pc);

#pragma GCC visibility pop

#ifdef __cplusplus
}
#endif

#endif
