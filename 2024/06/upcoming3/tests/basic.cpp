#include "vectorclassification.h"
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string_view>
using namespace std::literals;
bool run_basic(std::function<void(const char *&start, const char *end)> f) {
  std::string_view view =
      "Hello, World! I think that you have to be fair: < baba > &lt; \r   \0 "
      "Hello, World! I think that you have to be fair:"sv;
  const char *start = view.data();
  const char *end = start + view.size();
  f(start, end);
  if (*start != '<') {
    printf("expected '<'\n");
    return false;
  }
  start++;
  f(start, end);
  if (*start != '&') {
    printf("expected '&'\n");
    return false;
  }
  start++;
  f(start, end);
  if (*start != '\r') {
    printf("expected '\\r'\n");
    return false;
  }
  start++;
  f(start, end);
  if (*start != '\0') {
    printf("expected '\\0'\n");

    return false;
  }
  start++;

  f(start, end);
  if (start != end) {
    printf("expected end of string\n");
    return false;
  }
  printf("test completed\n");

  return true;
}

int main() {
  if (!run_basic(AdvanceString)) {
    std::cerr << "AdvanceString failed" << std::endl;
    return EXIT_FAILURE;
  } else {
    std::cout << "AdvanceString passed" << std::endl;
  }
  if (!run_basic(AdvanceStringTable)) {
    std::cerr << "AdvanceStringTable failed" << std::endl;
    return EXIT_FAILURE;
  } else {
    std::cout << "AdvanceStringTable passed" << std::endl;
  }
  return EXIT_SUCCESS;
}
