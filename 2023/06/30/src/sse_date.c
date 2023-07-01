
#include "sse_date.h"

#include <string.h>
#ifndef __USE_XOPEN
#define __USE_XOPEN
#endif
#include <time.h>
#include <x86intrin.h> // update if we need to support Windows.

/* Number of days per month (except for February in leap years). */
static const int mdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const int mdays_cumulative[] = {0,31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

static int is_leap_year(int year) {
  return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

static int leap_days(int y1, int y2) {
  --y1;
  --y2;
  return (y2 / 4 - y1 / 4) - (y2 / 100 - y1 / 100) + (y2 / 400 - y1 / 400);
}

/*
 * Code adapted from Python 2.4.1 sources (Lib/calendar.py).
 */
static time_t mktime_from_utc(const struct tm *tm) {
  int year = 1900 + tm->tm_year;
  time_t days = 365 * (year - 1970) + leap_days(1970, year);
  time_t hours;
  time_t minutes;
  time_t seconds;
  int i;

  for (i = 0; i < tm->tm_mon; ++i) {
    days += mdays[i];
  }
  if (tm->tm_mon > 1 && is_leap_year(year)) {
    ++days;
  }
  days += tm->tm_mday - 1;

  hours = days * 24 + tm->tm_hour;
  minutes = hours * 60 + tm->tm_min;
  seconds = minutes * 60 + tm->tm_sec;

  return seconds;
}

// return true on success, copying
bool parse_time(const char *date_string, uint32_t *time_in_second) {
  const char *end = NULL;
  struct tm tm;
  if ((end = strptime(date_string, "%Y%m%d%H%M%S", &tm)) == NULL) {
    return false;
  }
  *time_in_second = (uint32_t)mktime_from_utc(&tm);
  return true;
}

bool sse_parse_time(const char *date_string, uint32_t *time_in_second) {
  __m128i v = _mm_loadu_si128((const __m128i *)date_string);
  // loaded YYYYMMDDHHmmSS.....
  v = _mm_sub_epi8(v, _mm_set1_epi8(0x30));
  // subtracting by 0x30 (or '0'), turns all values into a byte value between 0
  // and 9 if the initial input was made of digits.
  __m128i limit =
      _mm_setr_epi8(9, 9, 9, 9, 1, 9, 3, 9, 2, 9, 5, 9, 5, 9, -1, -1);
  // credit @aqrit
  // overflows are still possible, if hours are in the range 24 to 29
  // of if days are in the range 32 to 39
  // or if months are in the range 12 to 19.
  __m128i abide_by_limits = _mm_cmpeq_epi8(_mm_max_epu8(v, limit), limit);

  if (!_mm_test_all_ones(abide_by_limits)) {
    return false;
  }
  // shift+add allows a subtract of `lane1_mul * 8` from `lane0_mul`
  // so we can fit a weight of "(36000/100) - ((3600/100) * 8)" in a single
  // byte. the hsum needs 17-bits...  (max possible hsum is 107999 "29:59.59")
  // so also strip trailing zeros from the minute lane
  // (the scalar mul steals bits -which are zero- from an adjoining lower lane).
  v = _mm_add_epi8(v, _mm_slli_epi16(v, 11));
  //
  const __m128i weights =
      _mm_set_epi8(0, 0, 1, 2, 15, 30, 36, 72, 1, 2, 1, 2, 1, 2, 1, 2);
  v = _mm_maddubs_epi16(v, weights);
  // 0x000000SS0mmm0HHH`00DD00MM00YY00YY

  uint64_t hi = (uint64_t)_mm_extract_epi64(v, 1);
  //
  uint64_t seconds = (hi * 0x0019000100004000) >> 46;
  if (seconds > 86399) {
    return false;
  }

  uint64_t lo = (uint64_t)_mm_extract_epi64(v, 0);
  uint64_t yr = (lo * 0x64000100000000) >> 48;
  uint64_t mo = ((lo & 0x000000ff00000000) >> 32) - 1;
  if (mo >= 12) {
    return false;
  }

  uint64_t dy = (lo & 0x000000ff000000000000) >> (32 + 16);

  bool is_leap_yr = is_leap_year((int)yr);

  if (dy > (uint64_t)mdays[mo]) {
    if (!(mo == 1 && is_leap_yr)) {
      return false;
    }
  }
  uint64_t days = 365 * (yr - 1970) + (uint64_t)leap_days(1970, (int)yr);

  days += (uint64_t)mdays_cumulative[mo];
  if (mo > 1 && is_leap_yr) {
    ++days;
  }
  days += dy - 1;
  *time_in_second = (uint32_t)(seconds + days * 60 * 60 * 24);
  return true;
}
