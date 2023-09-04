#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

extern "C" {
#include "identifiers.h"
}

bool basic_test() {
  printf("basic_test\n");
  std::string input = "hello world I am a string I love Lucy 1234567890. Geralda has my colon.";
  size_t ci = count_identifiers(input.c_str(), input.size());
  if(ci !=  13) {
    printf("count_identifiers FAIL %zu\n", ci);
    return false;
  }
  ci = count_identifiers_neon(input.c_str(), input.size());
  if(ci !=  13) {
    printf("count_identifiers_neon FAIL %zu\n", ci);
    return false;
  }
  printf("SUCCESS\n");
  return true; 
}

bool funky_test() {
  printf("funky\n");
  std::string input = "\x7c\xc4\xdd\xd5\x33\x61\x45\x74\x48\x30\x63\x55\x4e\x02\x64\x56\x46\x76\x6a\x62";
  size_t ci = count_identifiers(input.c_str(), input.size());
  if(ci !=  1) {
    printf("count_identifiers FAIL %zu\n", ci);
    return false;
  }
  ci = count_identifiers_neon(input.c_str(), input.size());
  if(ci !=  1) {
    printf("count_identifiers_neon FAIL %zu\n", ci);
    return false;
  }
  printf("SUCCESS\n");
  return true; 
}

int main() {
 return (basic_test() && funky_test()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
