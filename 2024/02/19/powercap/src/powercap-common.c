/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Common functions.
 *
 * @author Connor Imes
 * @date 2017-08-24
 */
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
/* Main powercap header only used for enums */
#include "powercap.h"
#include "powercap-common.h"

#define MAX_U64_SIZE 24

#ifdef USE_VIRTUAL_DEVICES
  #define POWERCAP_PATH "/sys/devices/virtual/powercap"
#else
  #define POWERCAP_PATH "/sys/class/powercap"
#endif

/* These enums MUST align with powercap_control_type_file in powercap.h */
static const char* CONTROL_TYPE_FILE[] = {
  "enabled"
};

/* These enums MUST align with powercap_zone_file in powercap.h */
static const char* ZONE_FILE[] = {
  "max_energy_range_uj",
  "energy_uj",
  "max_power_range_uw",
  "power_uw",
  "enabled",
  "name"
};

/* These enums MUST align with powercap_constraint_file in powercap.h */
static const char* CONSTRAINT_FILE_SUFFIX[] = {
  "power_limit_uw",
  "time_window_us",
  "max_power_uw",
  "min_power_uw",
  "max_time_window_us",
  "min_time_window_us",
  "name"
};

ssize_t read_string_safe(int fd, char* buf, size_t size) {
  ssize_t ret;
  if ((ret = pread(fd, buf, size - 1, 0)) > 0) {
    /* force a terminating character in the buffer */
    if (buf[ret - 1] == '\n') {
      /* also remove newline character */
      buf[ret - 1] = '\0';
    } else {
      buf[ret] = '\0';
    }
  } else if (ret < 0) {
    ret = -errno;
  } else {
    errno = ENODATA;
    ret = -errno;
  }
  return ret;
}

ssize_t read_string(int fd, char* buf, size_t size) {
  if (!buf) {
    errno = EINVAL;
    return (ssize_t) -errno;
  } else if (!size) {
    errno = ENOBUFS;
    return (ssize_t) -errno;
  }
  return read_string_safe(fd, buf, size);
}

int read_u64(int fd, uint64_t* val) {
  char buf[MAX_U64_SIZE];
  char* end;
  if (!val) {
    errno = EINVAL;
  } else if (read_string_safe(fd, buf, sizeof(buf)) > 0) {
    errno = 0;
    *val = strtoull(buf, &end, 0);
    if (buf != end && errno != ERANGE) {
      return 0;
    }
  }
  return -errno;
}

int write_u64(int fd, uint64_t val) {
  char buf[MAX_U64_SIZE];
  ssize_t written;
  snprintf(buf, sizeof(buf), "%"PRIu64, val);
  if ((written = pwrite(fd, buf, sizeof(buf), 0)) < 0) {
    return -errno;
  }
  if (!written) {
    /* Is there a better error code? */
    errno = EIO;
    return -errno;
  }
  return 0;
}

int is_valid_control_type(const char* control_type) {
  return control_type && strlen(control_type) && strcspn(control_type, "./") == strlen(control_type);
}

int snprintf_base_path(char* buf, size_t size, const char* control_type, const uint32_t* zones, uint32_t depth) {
  int w;
  int tot;
  uint32_t i;
  uint32_t j;
  if ((tot = snprintf(buf, size, POWERCAP_PATH"/%s/", control_type)) < 0) {
    return tot;
  }
  for (j = 1; j <= depth; j++) {
    if ((size_t) tot >= size) {
      return size + 1; // strictly > size since there was still more work to do
    }
    if ((w = snprintf(buf + (size_t) tot, size - (size_t) tot, "%s", control_type)) < 0) {
      return w;
    }
    tot += w;
    if ((size_t) tot >= size) {
      return size + 1; // strictly > size since there was still more work to do
    }
    for (i = 0; i < j && (size_t) tot < size; i++, tot += w) {
      if ((w = snprintf(buf + (size_t) tot, size - (size_t) tot, ":%x", zones[i])) < 0) {
        return w;
      }
    }
    if ((size_t) tot >= size) {
      return size + 1; // strictly > size since there was still more work to do
    }
    buf[tot++] = '/';
  }
  if ((size_t) tot < size) {
    buf[tot] = '\0';
  }
  return tot;
}

int snprintf_control_type_file(char* buf, size_t size, powercap_control_type_file type) {
  return snprintf(buf, size, "%s", CONTROL_TYPE_FILE[type]);
}

int snprintf_zone_file(char* buf, size_t size, powercap_zone_file type) {
  return snprintf(buf, size, "%s", ZONE_FILE[type]);
}

int snprintf_constraint_file(char* buf, size_t size, powercap_constraint_file type, uint32_t constraint) {
  return snprintf(buf, size, "constraint_%"PRIu32"_%s", constraint, CONSTRAINT_FILE_SUFFIX[type]);
}

int snprintf_control_type_file_path(char* path, size_t size, const char* control_type, powercap_control_type_file type) {
  int tot;
  int w;
  if ((tot = snprintf_base_path(path, size, control_type, NULL, 0)) < 0) {
    return tot;
  }
  if ((size_t) tot >= size) {
    return size + 1; // strictly > size since there was still more work to do
  }
  if ((w = snprintf_control_type_file(path + (size_t) tot, size - (size_t) tot, type)) < 0) {
    return w;
  }
  return tot + w;
}

int snprintf_zone_file_path(char* path, size_t size, const char* control_type, const uint32_t* zones, uint32_t depth,
                            powercap_zone_file type) {
  int tot;
  int w;
  if ((tot = snprintf_base_path(path, size, control_type, zones, depth)) < 0) {
    return tot;
  }
  if ((size_t) tot >= size) {
    return size + 1; // strictly > size since there was still more work to do
  }
  if ((w = snprintf_zone_file(path + (size_t) tot, size - (size_t) tot, type)) < 0) {
    return w;
  }
  return tot + w;
}

int snprintf_constraint_file_path(char* path, size_t size, const char* control_type, const uint32_t* zones,
                                  uint32_t depth, uint32_t constraint, powercap_constraint_file type) {
  int tot;
  int w;
  if ((tot = snprintf_base_path(path, size, control_type, zones, depth)) < 0) {
    return tot;
  }
  if ((size_t) tot >= size) {
    return size + 1; // strictly > size since there was still more work to do
  }
  if ((w = snprintf_constraint_file(path + tot, size - (size_t) tot, type, constraint)) < 0) {
    return w;
  }
  return tot + w;
}

int open_control_type_file(char* path, size_t size, const char* control_type, powercap_control_type_file type,
                           int flags) {
  int w = snprintf_control_type_file_path(path, size, control_type, type);
  if (w < 0) {
    // POSIX says snprintf should only fail if size > INT_MAX, which it's not, so this code branch should never run
    // If we're here, we don't even know what the error should be, so assert that errno is set
    assert(errno);
    return w;
  }
  if ((size_t) w >= size) {
    errno = ENOBUFS;
    return -1;
  }
  return open(path, flags);
}

int open_zone_file(char* path, size_t size, const char* control_type, const uint32_t* zones, uint32_t depth,
                   powercap_zone_file type, int flags) {
  int w = snprintf_zone_file_path(path, size, control_type, zones, depth, type);
  if (w < 0) {
    // POSIX says snprintf should only fail if size > INT_MAX, which it's not, so this code branch should never run
    // If we're here, we don't even know what the error should be, so assert that errno is set
    assert(errno);
    return w;
  }
  if ((size_t) w >= size) {
    errno = ENOBUFS;
    return -1;
  }
  return open(path, flags);
}

int open_constraint_file(char* path, size_t size, const char* control_type, const uint32_t* zones, uint32_t depth,
                         uint32_t constraint, powercap_constraint_file type, int flags) {
  int w = snprintf_constraint_file_path(path, size, control_type, zones, depth, constraint, type);
  if (w < 0) {
    // POSIX says snprintf should only fail if size > INT_MAX, which it's not, so this code branch should never run
    // If we're here, we don't even know what the error should be, so assert that errno is set
    assert(errno);
    return w;
  }
  if ((size_t) w >= size) {
    errno = ENOBUFS;
    return -1;
  }
  return open(path, flags);
}

// like open(2), but returns 0 on ENOENT (No such file or directory)
static int maybe_open_control_type_file(char* buf, size_t bsize, const char* ct_name, powercap_control_type_file type,
                                        int flags) {
  int fd = open_control_type_file(buf, bsize, ct_name, type, flags);
  return (fd < 0 && errno == ENOENT) ? 0 : fd;
}

// like open(2), but returns 0 on ENOENT (No such file or directory)
static int maybe_open_zone_file(char* buf, size_t bsize, const char* ct_name, const uint32_t* zones, uint32_t depth,
                                powercap_zone_file type, int flags) {
  int fd = open_zone_file(buf, bsize, ct_name, zones, depth, type, flags);
  return (fd < 0 && errno == ENOENT) ? 0 : fd;
}

// like open(2), but returns 0 on ENOENT (No such file or directory)
static int maybe_open_constraint_file(char* buf, size_t bsize, const char* ct_name, const uint32_t* zones,
                                      uint32_t depth, uint32_t constraint, powercap_constraint_file type, int flags) {
  int fd = open_constraint_file(buf, bsize, ct_name, zones, depth, constraint, type, flags);
  return (fd < 0 && errno == ENOENT) ? 0 : fd;
}

int powercap_control_type_open(powercap_control_type* pct, char* buf, size_t bsize, const char* ct_name, int ro) {
  return ((pct->enabled = maybe_open_control_type_file(buf, bsize, ct_name, POWERCAP_CONTROL_TYPE_FILE_ENABLED,
                                                       ro ? O_RDONLY : O_RDWR)) < 0)
         ? -1 : 0;
}

int powercap_zone_open(powercap_zone* pz, char* buf, size_t bsize, const char* ct_name, const uint32_t* zones,
                       uint32_t depth, int ro) {
  return ((pz->max_energy_range_uj =
            maybe_open_zone_file(buf, bsize, ct_name, zones, depth,
                                 POWERCAP_ZONE_FILE_MAX_ENERGY_RANGE_UJ, O_RDONLY)) < 0) ||
         // special case for energy_uj - it's allowed to be either RW or RO
         (
          ((pz->energy_uj =
              maybe_open_zone_file(buf, bsize, ct_name, zones, depth,
                                   POWERCAP_ZONE_FILE_ENERGY_UJ, ro ? O_RDONLY : O_RDWR)) < 0) &&
          (ro ||
            ((pz->energy_uj =
                maybe_open_zone_file(buf, bsize, ct_name, zones, depth,
                                     POWERCAP_ZONE_FILE_ENERGY_UJ, O_RDONLY)) < 0))
         ) ||
         ((pz->max_power_range_uw =
            maybe_open_zone_file(buf, bsize, ct_name, zones, depth,
                                 POWERCAP_ZONE_FILE_MAX_POWER_RANGE_UW, O_RDONLY)) < 0) ||
         ((pz->power_uw =
            maybe_open_zone_file(buf, bsize, ct_name, zones, depth,
                                 POWERCAP_ZONE_FILE_POWER_UW, O_RDONLY)) < 0) ||
         ((pz->enabled =
            maybe_open_zone_file(buf, bsize, ct_name, zones, depth,
                                 POWERCAP_ZONE_FILE_ENABLED, ro ? O_RDONLY : O_RDWR)) < 0) ||
         ((pz->name =
            maybe_open_zone_file(buf, bsize, ct_name, zones, depth,
                                 POWERCAP_ZONE_FILE_NAME, O_RDONLY)) < 0)
         ? -1 : 0;
}

int powercap_constraint_open(powercap_constraint* pc, char* buf, size_t bsize, const char* ct_name,
                             const uint32_t* zones, uint32_t depth, uint32_t constraint, int ro) {
  return ((pc->power_limit_uw =
            maybe_open_constraint_file(buf, bsize, ct_name, zones, depth, constraint,
                                       POWERCAP_CONSTRAINT_FILE_POWER_LIMIT_UW, ro ? O_RDONLY : O_RDWR)) < 0) ||
         ((pc->time_window_us =
            maybe_open_constraint_file(buf, bsize, ct_name, zones, depth, constraint,
                                       POWERCAP_CONSTRAINT_FILE_TIME_WINDOW_US, ro ? O_RDONLY : O_RDWR)) < 0) ||
         ((pc->max_power_uw =
            maybe_open_constraint_file(buf, bsize, ct_name, zones, depth, constraint,
                                       POWERCAP_CONSTRAINT_FILE_MAX_POWER_UW, O_RDONLY)) < 0) ||
         ((pc->min_power_uw =
            maybe_open_constraint_file(buf, bsize, ct_name, zones, depth, constraint,
                                       POWERCAP_CONSTRAINT_FILE_MIN_POWER_UW, O_RDONLY)) < 0) ||
         ((pc->max_time_window_us =
            maybe_open_constraint_file(buf, bsize, ct_name, zones, depth, constraint,
                                       POWERCAP_CONSTRAINT_FILE_MAX_TIME_WINDOW_US, O_RDONLY)) < 0) ||
         ((pc->min_time_window_us =
            maybe_open_constraint_file(buf, bsize, ct_name, zones, depth, constraint,
                                       POWERCAP_CONSTRAINT_FILE_MIN_TIME_WINDOW_US, O_RDONLY)) < 0) ||
         ((pc->name =
            maybe_open_constraint_file(buf, bsize, ct_name, zones, depth, constraint,
                                       POWERCAP_CONSTRAINT_FILE_NAME, O_RDONLY)) < 0)
         ? -1 : 0;
}

static int powercap_close(int fd) {
  return (fd > 0 && close(fd)) ? -1 : 0;
}

int powercap_control_type_close(powercap_control_type* pct) {
  return powercap_close(pct->enabled);
}

int powercap_zone_close(powercap_zone* pz) {
  int rc = 0;
  rc |= powercap_close(pz->max_energy_range_uj);
  rc |= powercap_close(pz->energy_uj);
  rc |= powercap_close(pz->max_power_range_uw);
  rc |= powercap_close(pz->power_uw);
  rc |= powercap_close(pz->enabled);
  rc |= powercap_close(pz->name);
  return rc;
}

int powercap_constraint_close(powercap_constraint* pc) {
  int rc = 0;
  rc |= powercap_close(pc->power_limit_uw);
  rc |= powercap_close(pc->time_window_us);
  rc |= powercap_close(pc->max_power_uw);
  rc |= powercap_close(pc->min_power_uw);
  rc |= powercap_close(pc->max_time_window_us);
  rc |= powercap_close(pc->min_time_window_us);
  rc |= powercap_close(pc->name);
  return rc;
}
