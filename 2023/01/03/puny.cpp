#include "puny.h"


#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}


int main() {
  char buffer[64];
  std::string_view words[] = {"a", "A", "3", "-", "--", "London", "Lloyd-Atkinson", "This has spaces", "-> $1.00 <-", "а", "ü", "α", "例", "😉", "αβγ", "München", "Mnchen-3ya", "München-Ost", "Bahnhof München-Ost", "abæcdöef", "правда", "ยจฆฟคฏข", "도메인", "ドメイン名例", "MajiでKoiする5秒前", "「bücher」"};
  for(std::string_view v : words) {
    int length = utf8_to_punycode(v.data(), v.size(), buffer);
    if(length < 0) { return EXIT_FAILURE; }
    // std::cout << v << " becomes " << std::string_view(buffer, length) << std::endl;
  }
  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    volatile size_t total_output_size{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      for(std::string_view v : words) {
        count++;
        total_output_size += utf8_to_punycode(v.data(), v.size(), buffer);
      }
      finish = nano();
    }

    printf(" %f ns/string, output speed = %f GB/s \n", double(finish - start) / count, total_output_size / double(finish - start));
  }
  return EXIT_SUCCESS;
}