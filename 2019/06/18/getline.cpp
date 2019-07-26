
#include "benchmark.h"
#include <istream>
#include <string>
#include <random>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


void rewind(std::istream & is) {
  is.clear();
  is.seekg(0,std::ios::beg);
}

__attribute__ ((noinline))
size_t sum_line_lengths(std::istream & is) {
  std::string line;
  size_t sumofalllinelengths{0};
  while(getline(is, line)) {
    sumofalllinelengths += line.size();
  }
  return sumofalllinelengths;
}

void get_line_benchmark(size_t size) {
    printf(" %s\n", __PRETTY_FUNCTION__);
    bool verbose = true;
    printf("generating and parsing input having %zu bytes.\n", size);
    int repeat = 10;
    std::vector<char> blob(size + 1, 'a');
    blob [size] = '\0';
    // add break lines at random
    // average line length should be 80
    for(size_t br = 0; br < size / 80; br++) {
      blob[rand() % size] = '\n';
    }
    std::istringstream ss(std::string(blob.data(),size + 1));
    BEST_TIME_NS(sum_line_lengths(ss), rewind(ss), repeat, size, verbose);
}
void get_line_predictable_benchmark(size_t size) {
    printf(" %s\n", __PRETTY_FUNCTION__);
    bool verbose = true;
    printf("generating and parsing input having %zu bytes.\n", size);
    int repeat = 10;
    std::vector<char> blob(size + 1, 'a');
    blob [size] = '\0';
    // average line length should be 80
    for(size_t br = 0; br < size; br+= 80) {
      blob[br] = '\n';
    }
    std::istringstream ss(std::string(blob.data(),size + 1));
    BEST_TIME_NS(sum_line_lengths(ss), rewind(ss), repeat, size, verbose);
}

__attribute__ ((noinline))
size_t count_returns(char * data, size_t length) {
    size_t answer = 0;
    for(size_t i = 0; i < length; i++) {
      if (data[i] == '\n') answer++;
    }
    return answer;
}

void count_lines_benchmark(size_t size) {
    printf(" %s\n", __PRETTY_FUNCTION__);
    bool verbose = true;
    printf("generating and parsing input having %zu bytes.\n", size);
    int repeat = 10;
    std::vector<char> blob(size + 1, 'a');
    blob [size] = '\0';
    // average line length should be 80
    for(size_t br = 0; br < size / 80; br++) {
      blob[rand() % size] = '\n';
    }
    size_t c = 0;
    BEST_TIME_NS((c = count_returns(blob.data(), blob.size())), , repeat, size, verbose);
    printf("bogus = %zu \n", c);
}



int main(int argc, char** argv) {
  count_lines_benchmark(10000000);
  get_line_benchmark(10000000);
  get_line_predictable_benchmark(10000000);
  return EXIT_SUCCESS;
}
