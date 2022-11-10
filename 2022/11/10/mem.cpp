
#include "nadeau.h"
#include <vector>
#include <iostream>

void print_pages() {
    static size_t pagesize = sysconf(_SC_PAGESIZE);
    int64_t bytes = getCurrentRSS();
    assert((bytes % pagesize) == 0);
    size_t pages = bytes / pagesize;
    std::cout << "page size: " << pagesize << "\t";
    std::cout << "bytes: " << bytes << "\t";
    std::cout << "pages: " << pages << std::endl;
}

int main() {
  print_pages();
  std::vector<uint8_t> v1(10);
  print_pages();
  std::vector<uint8_t> v2(1000000);
  print_pages();

  return EXIT_SUCCESS;
}