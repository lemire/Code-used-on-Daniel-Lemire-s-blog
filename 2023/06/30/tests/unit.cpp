#include <arpa/inet.h>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <x86intrin.h>
#include <omp.h>

extern "C" {
#include "sse_date.h"
}
static inline std::string to_date_string(time_t rawtime) {
  std::string output(16, '\0');
  struct tm timeinfo;
  gmtime_r(&rawtime, &timeinfo);
  size_t len = strftime(output.data(), output.size(), "%Y%m%d%H%M%S", &timeinfo);
  if (len == 0) {
    printf("len = %zu rawtime = %ld \n", len, rawtime);
    abort();
  }
  return output;
}

bool test_exhaustive() {
  printf("testing exhaustive.\n");
  printf("number of threads: %u\n", omp_get_max_threads());
  int errors = 0;
  int count = 0;
  #pragma omp parallel for reduction(+ : errors,count)
  for (uint64_t x64 = 0; x64 <= 0xfffffff; x64++) {
    count++;
    const uint32_t x = (uint32_t)x64;
    if ((count & 0xfffff) == 0) {
      printf(".");
      fflush(NULL);
    }
    const std::string view = to_date_string(x);
    uint32_t time = 0;
    if (!parse_time(view.data(), &time)) {
      printf("[parse_time] bad parsing\n");
      printf(" value %s \n", view.data());
      abort();
      errors++;
    }

    if (time != x) {
      printf("[parse_time] bad value %x %x from %s %s %s\n", time, uint32_t(x),
             view.data(), to_date_string(time).data(), to_date_string(x).data());
      errors++;
    }
    uint32_t time2 = 0;
    if (!sse_parse_time(view.data(), &time2)) {
      printf("[sse_parse_time] bad parsing\n");
      printf(" value %s \n", view.data());
      abort();
      errors++;
    }
    if (time2 != x) {
      printf("[sse_parse_time] bad value %x %x from %s %s %s\n", time, uint32_t(x),
             view.data(), to_date_string(time).data(), to_date_string(x).data());
      errors++;
    }
  }
  printf("\n");
  if (errors) {
    return false;
  }
  printf("testing exhaustive: SUCCESS\n");
  return true;
}

bool test_bad() {
  printf("testing bad examples.\n");
  std::string bad_examples[] = {
      "20230630236159", "20230630245959", "20230631235959", "20231330000060",
      "20231330235959", "20231330000060", "20231330006000", "20231330240000"};
  for (std::string &bad : bad_examples) {
    bad.reserve(16);
    uint32_t time;
    bool err = sse_parse_time(bad.data(), &time);
    if (err == 1) {
      std::cout << " it should not allow this string to be valid " << bad
                << std::endl;
      return false;
    }
  }
  printf("Ok.\n");
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
  return (test_good1() && test_bad() && test_exhaustive()) ? EXIT_SUCCESS
                                                           : EXIT_FAILURE;
}