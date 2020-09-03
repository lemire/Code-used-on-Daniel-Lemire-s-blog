#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <float.h>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>
using clk = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<clk>;
using dur_double = std::chrono::duration<double>;
using std::chrono::duration_cast;

class Timer {
public:
  Timer(const std::string &cmd) : _cmd{cmd}, _start{clk::now()} {};

  double time_ns() {
    auto duration = clk::now() - _start;
    auto elapsed_s = duration_cast<dur_double>(duration).count();
    return elapsed_s * 1000 * 1000 * 1000;
  }

  ~Timer(){};

private:
  std::string _cmd;
  time_point _start;
};


bool is_space(uint8_t c) {
    static const bool table[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    return table[c];
}

const char * skip_leading_spaces(const char * start, const char * end) {
  while((start != end) && (is_space(*start))) {
    start++;
  }
  return start;
}

const char * skip_leading_spaces(const char * start) {
  while(is_space(*start)) {
    start++;
  }
  return start;
}

std::vector<std::string> generate(size_t N, size_t max_space) {
  std::vector<std::string> answer;
  srand(time(NULL));
  for(size_t i = 0; i < N; i++) {
    std::string newstring(max_space, 'B');
    size_t leading  = rand() % max_space;
    for(size_t k = 0; k < leading; k++ ) {
      newstring[k] = ' ';
    }
    answer.push_back(newstring);
  }
  return answer;
}

void demo(size_t N,  size_t max_space) {
  std::cout << " N = " << N << std::endl;
  
  srand(time(NULL));
  char bogus{};
  double best_z1 = DBL_MAX;
  double best_z2 = DBL_MAX;
  for(size_t trial = 0; trial < 5; trial ++) {
    auto data = generate(N, max_space);
    for(std::string & s : data) {
     if(skip_leading_spaces(s.data(), s.data() + s.size()) != skip_leading_spaces(s.data())) { abort(); }
    }
    auto t1 = Timer{"regular"};
    for(std::string & s : data) {
    bogus += *skip_leading_spaces(s.data(), s.data() + s.size());
    }
    double z1 = t1.time_ns() / N;
    if(z1 < best_z1) { best_z1 = z1; }
    auto t2 = Timer{"sentinel"};
    for(std::string & s : data) {
      bogus += *skip_leading_spaces(s.data());
    }
    double z2 = t2.time_ns() / N;
    if(z2 < best_z2) { best_z2 = z2; }
  }

  std::cout << "range " << best_z1 << std::endl;
  std::cout << "sentinel " << best_z2 << std::endl;
  std::cout << "ratio " << best_z1 / best_z2 << std::endl;

  std::cout << bogus << std::endl;
}

int main() {
  demo(10000, 4096);
}
