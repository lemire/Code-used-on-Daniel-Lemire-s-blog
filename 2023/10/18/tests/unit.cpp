#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "escape.h"

bool basic_test() {
  printf("basic_test\n");
  std::string input = "fdsfds%fdsfds";
  std::string expected = "fdsfds%25fdsfds";
  if (find_string_escape(input) == expected && string_escape(input) == expected && stream_escape(input) == expected) {
    printf("SUCCESS\n");
    return true;
  }
  printf("FAILURE\n");
  return false;
}

int main() { return (basic_test()) ? EXIT_SUCCESS : EXIT_FAILURE; }
