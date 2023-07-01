#include <arpa/inet.h>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <x86intrin.h>

extern "C" {
#include "sse_date.h"
}
std::string to_date_string(time_t rawtime) {
  std::string output(16, '\0');
  struct tm *timeinfo = gmtime(&rawtime);
  size_t len = strftime(output.data(), output.size(), "%Y%m%d%H%M%S", timeinfo);
  if (len == 0) {
    printf("len = %zu rawtime = %ld \n", len, rawtime);
    abort();
  }
  output.resize(len);
  return output;
}

bool test_exhaustive() {
  int errors = 0;

  for (uint64_t x = 0; x <= 0xfffffff; x++) {
    if ((x & 0xffffff) == 0) {
      printf(".");
      fflush(NULL);
    }
    std::string view = to_date_string(x);
    uint32_t time;
    if (!parse_time(view.c_str(), &time)) {
      printf("[parse_time] bad parsing\n");
      printf(" value %s \n", view.data());
      errors++;
    }

    if (time != x) {
      printf("[parse_time] bad value %x %x from %s \n", time, uint32_t(x),
             view.c_str());
      std::cout << to_date_string(x) << std::endl;
      std::cout << to_date_string(time) << std::endl;
      errors++;
    }
    if (!sse_parse_time(view.c_str(), &time)) {
      printf("[sse_parse_time] bad parsing\n");
      printf(" value %s \n", view.data());
      errors++;
    }
    if (time != x) {
      printf("[sse_parse_time] bad value %x %x from %s\n", time, uint32_t(x),
             view.c_str());
      errors++;
    }
  }
  printf("\n");
  if (errors) {
    return false;
  }
  printf("SUCCESS\n");
  return true;
}

bool test_bad() {
  std::string bad = "20230630236159";
  bad.reserve(16);
  uint32_t time;
  bool err = sse_parse_time(bad.data(), &time);
  if (err == 1) {
    std::cout << " it should not allow this string to be valid " << bad
              << std::endl;
    return false;
  }
  return true;
}

bool test_bad2() {
  std::string bad = "20230630245959";
  bad.reserve(16);
  uint32_t time;
  bool err = sse_parse_time(bad.data(), &time);
  if (err == 1) {
    std::cout << " it should not allow this string to be valid " << bad
              << std::endl;
    return false;
  }
  return true;
}

bool test_bad3() {
  std::string bad = "20230631235959";
  bad.reserve(16);
  uint32_t time;
  bool err = sse_parse_time(bad.data(), &time);
  if (err == 1) {
    std::cout << " it should not allow this string to be valid " << bad
              << std::endl;
    return false;
  }
  return true;
}

bool test_bad4() {
  std::string bad = "20231330235959";
  bad.reserve(16);
  uint32_t time;
  bool err = sse_parse_time(bad.data(), &time);
  if (err == 1) {
    std::cout << " it should not allow this string to be valid " << bad
              << std::endl;
    return false;
  }
  return true;
}
bool test_good1() {
  std::string bad = "20380119031408";
  bad.reserve(16);
  uint32_t time1;
  bool err = sse_parse_time(bad.data(), &time1);
  if (!err) {
    std::cout << "it should be valid " << bad << std::endl;
    return false;
  }
  uint32_t time2;
  err = parse_time(bad.data(), &time2);
  if (!err) {
    std::cout << "it should be valid " << bad << std::endl;
    return false;
  }
  if (time1 != time2) {
    std::cout << "they differ " << bad << std::endl;
    return false;
  }
  return true;
}

int main() {
  return (test_good1() && test_bad2() && test_bad3() && test_bad4() &&
          test_bad() && test_exhaustive())
             ? EXIT_SUCCESS
             : EXIT_FAILURE;
}