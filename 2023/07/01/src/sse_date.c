
#include "sse_date.h"

#include <string.h>
#ifndef __USE_XOPEN
#define __USE_XOPEN
#endif
#include <time.h>
#include <x86intrin.h> // update if we need to support Windows.

/* Number of days per month (except for February in leap years). */
static const int mdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const int mdays_cumulative[] = {0,   31,  59,  90,  120, 151, 181,
                                       212, 243, 273, 304, 334, 365};

static inline uint64_t is_leap_year(int year) {
  return (year % 4 == 0) & ((year % 100 != 0) | (year % 400 == 0));
}

static inline int leap_days(int y1, int y2) {
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

  days += mdays_cumulative[tm->tm_mon];

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
  // We load the block of digits. We subtract 0x30 (the code point value of the
  // character '0'), and all bytes values should be between 0 and 9,
  // inclusively. We know that some character must be smaller that 9, for
  // example, we cannot have more than 59 seconds and never 60 seconds, in the
  // time stamp string. So one character must be between 0 and 5. Similarly, we
  // start the hours at 00 and end at 23, so one character must be between 0
  // and 2. We do a saturating subtraction of the maximum: the result of such a
  // subtraction should be zero if the value is no larger. We then use a special
  // instruction to multiply one byte by 10, and sum it up with the next byte,
  // getting a 16-bit value. We then repeat the same approach as before,
  // checking that the result is not too large.
  //
  __m128i v = _mm_loadu_si128((const __m128i *)date_string);
  // loaded YYYYMMDDHHmmSS.....
  v = _mm_xor_si128(v, _mm_set1_epi8(0x30));
  // W can use _mm_sub_epi8 or _mm_xor_si128 for the subtraction above.
  // subtracting by 0x30 (or '0'), turns all values into a byte value between 0
  // and 9 if the initial input was made of digits.
  __m128i limit =
      _mm_setr_epi8(9, 9, 9, 9, 1, 9, 3, 9, 2, 9, 5, 9, 5, 9, -1, -1);
  // credit @aqrit
  // overflows are still possible, if hours are in the range 24 to 29
  // of if days are in the range 32 to 39
  // or if months are in the range 12 to 19.
  __m128i abide_by_limits = _mm_subs_epu8(v, limit); // must be all zero

  __m128i byteflip = _mm_setr_epi64((__m64)0x0607040502030001ULL,
                                    (__m64)0x0e0f0c0d0a0b0809ULL);

  __m128i little_endian = _mm_shuffle_epi8(v, byteflip);
  __m128i limit16 = _mm_setr_epi16(0x0909, 0x0909, 0x0102, 0x0301, 0x0203,
                                   0x0509, 0x0509, -1);
  __m128i abide_by_limits16 =
      _mm_subs_epu16(little_endian, limit16); // must be all zero

  __m128i combined_limits =
      _mm_or_si128(abide_by_limits16, abide_by_limits); // must be all zero
  // We want to disallow 0s for days and months... and we want to make
  // sure that we don't go back in time prior to 1900.
  __m128i limit16_low = _mm_setr_epi16(0x0109, 0, 0x0001, 0x0001, 0, 0, 0, 0);

  __m128i abide_by_limits16_low =
      _mm_subs_epu16(limit16_low, little_endian); // must be all zero
  combined_limits = _mm_or_si128(combined_limits, abide_by_limits16_low);

  if (!_mm_test_all_zeros(combined_limits, combined_limits)) {
    return false;
  }
  // 0x000000SS0mmm0HHH`00DD00MM00YY00YY
  //////////////////////////////////////////////////////
  // pmaddubsw has a high latency (e.g., 5 cycles) and is
  // likely a performance bottleneck.
  /////////////////////////////////////////////////////
  const __m128i weights = _mm_setr_epi8(
      //     Y   Y   Y   Y   m   m   d   d   H   H   M   M   S   S   -   -
      10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 0, 0);
  v = _mm_maddubs_epi16(v, weights);

  uint64_t hi = (uint64_t)_mm_extract_epi64(v, 1);
  uint64_t seconds = (hi * 0x0384000F00004000) >> 46;
  uint64_t lo = (uint64_t)_mm_extract_epi64(v, 0);
  uint64_t yr = (lo * 0x64000100000000) >> 48;

  // We checked above that dy and mo are >= 1
  uint64_t mo = ((lo >> 32) & 0xff) - 1;
  uint64_t dy = (uint64_t)_mm_extract_epi8(v, 6);

  bool is_leap_yr = is_leap_year((int)yr);

  if (dy > (uint64_t)mdays[mo]) { // unlikely branch
    if (mo == 1 && is_leap_yr) {
      if (dy != 29) {
        return false;
      }
    } else {
      return false;
    }
  }
  uint64_t days = 365 * (yr - 1970) + (uint64_t)leap_days(1970, (int)yr);

  days += (uint64_t)mdays_cumulative[mo];
  days += is_leap_yr & (mo > 1);

  days += dy - 1;
  uint64_t time_in_second64 = seconds + days * 60 * 60 * 24;
  *time_in_second = (uint32_t)time_in_second64;
  return time_in_second64 == (uint32_t)time_in_second64;
}


static const int mdays_minus_one[] = {30, 27, 30, 29, 30, 29, 30, 30, 29, 30, 29, 30};

// uses more instructions than sse_parse_time but might be slightly faster.
bool sse_parse_time_alt(const char *date_string, uint32_t *time_in_second) {
  // We load the block of digits. We subtract 0x30 (the code point value of the
  // character '0'), and all bytes values should be between 0 and 9,
  // inclusively. We know that some character must be smaller that 9, for
  // example, we cannot have more than 59 seconds and never 60 seconds, in the
  // time stamp string. So one character must be between 0 and 5. Similarly, we
  // start the hours at 00 and end at 23, so one character must be between 0
  // and 2. We do a saturating subtraction of the maximum: the result of such a
  // subtraction should be zero if the value is no larger. We then use a special
  // instruction to multiply one byte by 10, and sum it up with the next byte,
  // getting a 16-bit value. We then repeat the same approach as before,
  // checking that the result is not too large.
  //
  // We compute the month the good old ways, as an integer in [0,11], we
  // check for overflows later.
  uint64_t mo = (uint64_t)((date_string[4]-0x30)*10 + (date_string[5]-0x30) - 1);
  __m128i v = _mm_loadu_si128((const __m128i *)date_string);
  // loaded YYYYMMDDHHmmSS.....
  v = _mm_xor_si128(v, _mm_set1_epi8(0x30));
  // W can use _mm_sub_epi8 or _mm_xor_si128 for the subtraction above.
  // subtracting by 0x30 (or '0'), turns all values into a byte value between 0
  // and 9 if the initial input was made of digits.
  __m128i limit =
      _mm_setr_epi8(9, 9, 9, 9, 1, 9, 3, 9, 2, 9, 5, 9, 5, 9, -1, -1);
  // credit @aqrit
  // overflows are still possible, if hours are in the range 24 to 29
  // of if days are in the range 32 to 39
  // or if months are in the range 12 to 19.
  __m128i abide_by_limits = _mm_subs_epu8(v, limit); // must be all zero

  __m128i byteflip = _mm_setr_epi64((__m64)0x0607040502030001ULL,
                                    (__m64)0x0e0f0c0d0a0b0809ULL);

  __m128i little_endian = _mm_shuffle_epi8(v, byteflip);
  __m128i limit16 = _mm_setr_epi16(0x0909, 0x0909, 0x0102, 0x0301, 0x0203,
                                   0x0509, 0x0509, -1);
  __m128i abide_by_limits16 =
      _mm_subs_epu16(little_endian, limit16); // must be all zero

  __m128i combined_limits =
      _mm_or_si128(abide_by_limits16, abide_by_limits); // must be all zero

  if (!_mm_test_all_zeros(combined_limits, combined_limits)) {
    return false;
  }
  // 0x000000SS0mmm0HHH`00DD00MM00YY00YY
  //////////////////////////////////////////////////////
  // pmaddubsw has a high latency (e.g., 5 cycles) and is
  // likely a performance bottleneck.
  /////////////////////////////////////////////////////
  const __m128i weights = _mm_setr_epi8(
      //     Y   Y   Y   Y   m   m   d   d   H   H   M   M   S   S   -   -
      10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 0, 0);
  v = _mm_maddubs_epi16(v, weights);

  uint64_t hi = (uint64_t)_mm_extract_epi64(v, 1);
  uint64_t seconds = (hi * 0x0384000F00004000) >> 46;
  uint64_t lo = (uint64_t)_mm_extract_epi64(v, 0);
  uint64_t yr = (lo * 0x64000100000000) >> 48;

  // We compute the day (starting at zero). We implicitly 
  // check for overflows later.
  uint64_t dy = (uint64_t)_mm_extract_epi8(v, 6) - 1;

  bool is_leap_yr = is_leap_year((int)yr);
  if(mo > 11) { return false; } // unlikely branch
  if (dy > (uint64_t)mdays_minus_one[mo]) { // unlikely branch
    if (mo == 1 && is_leap_yr) {
      if (dy != 29 - 1) {
        return false;
      }
    } else {
      return false;
    }
  }
  uint64_t days = 365 * (yr - 1970) + (uint64_t)leap_days(1970, (int)yr);

  days += (uint64_t)mdays_cumulative[mo];
  days += is_leap_yr & (mo > 1);

  days += dy;
  uint64_t time_in_second64 = seconds + days * 60 * 60 * 24;
  *time_in_second = (uint32_t)time_in_second64;
  return time_in_second64 == (uint32_t)time_in_second64;
}
