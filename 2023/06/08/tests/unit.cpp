#include <arpa/inet.h>
#include <charconv>
#include <cstring>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <string_view>
extern "C" {
#include "sse_inet_aton.h"
}
// https://lemire.me/blog/2023/02/01/serializing-ips-quickly-in-c/
std::string_view ipv4(std::string &output, const uint64_t address) {
  char *point = output.data();
  char *point_end = output.data() + output.size();
  point = std::to_chars(point, point_end, uint8_t(address)).ptr;
  for (int i = 1; i <= 3; i++) {
    *point++ = '.';
    point = std::to_chars(point, point_end, uint8_t(address >> (i * 8))).ptr;
  }
  return {output.data(), size_t(point - output.data())};
}

bool test_exhaustive() {
  int err;
  std::string output(16, '\0');
  for (uint64_t x = 0; x <= 0xffffffff; x++) {
    if ((x & 0xffffff) == 0) {
      printf(".");
      fflush(NULL);
    }
    std::string_view view = ipv4(output, x);
    output[view.size()] = '\0';
    uint32_t ipv4;
    err = sse_inet_aton(view.data(), view.size(), &ipv4);
    if (err != 1) {
      printf("[sse_inet_aton] non-one error code\n");
      printf(" value %x \n", err);
      return false;
    }
    if (ipv4 != x) {
      printf("[sse_inet_aton] bad value %x %x \n", ipv4, uint32_t(x));
      return false;
    }
    err = inet_pton(AF_INET, view.data(), &ipv4);
    if (err != 1) {
      printf("[inet_pton] non-one error code\n");
      printf(" value %x \n", err);
      return false;
    }
    if (ipv4 != x) {
      printf("[inet_pton] bad value %x %x \n", ipv4, uint32_t(x));
      return false;
    }
  }
  printf("\n");
  printf("SUCCESS\n");
  return true;
}
int main() { return test_exhaustive() ? EXIT_SUCCESS : EXIT_FAILURE; }