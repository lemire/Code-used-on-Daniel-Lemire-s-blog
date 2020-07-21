#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdlib>
#include <cstring>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <string_view>
#include <cstring>

using clk = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<clk>;
using dur_double = std::chrono::duration<double>;
using std::chrono::duration_cast;

inline bool is_ascii_branchy(const std::string_view v) {
   for (size_t i = 0; i < v.size(); i++) {
     if (uint8_t(v[i]) >= 128) {
       return false;
     }
   }
   return true;
}

inline bool is_ascii_branchless(const std::string_view v) {
  uint64_t running = 0;
  size_t i = 0;
  for(; i + 8 <= v.size(); i+=8) {
    uint64_t payload;
    memcpy(&payload, v.data() + i, 8);
    running |= payload;
  }
  for(; i < v.size(); i++) {
      running |= v[i];
  }
  return (running & 0x8080808080808080) == 0;  
}


inline bool is_ascii_hybrid(const std::string_view v) {

  size_t i = 0;
  for(; i + 8 <= v.size(); i+=8) {
    uint64_t payload;
    memcpy(&payload, v.data() + i, 8);
    if((payload & 0x8080808080808080) != 0) return false;
  }
  for(; i < v.size(); i++) {
      if((v[i] & 0x80) != 0) return false;
  }
  return true;
}
size_t volume(const std::vector<std::string_view> & source) {
  size_t vol = 0;
  for(auto sv : source) {
    vol += sv.size();
  }
  return vol;
}
void branchy(const std::vector<std::string_view> & source) {
  size_t counter = 0;
  auto start = clk::now();
  for(auto sv : source) {
    if(is_ascii_branchy(sv)) counter ++;
  }
  auto duration = clk::now() - start;
  auto elapsed_s = duration_cast<dur_double>(duration).count();
  std::cout << "branchy:" <<  volume(source) / (1000 * 1000 * 1000.0) /  elapsed_s << " GB/s" << std::endl;
  std::cout << counter << std::endl;
}

void branchless(const std::vector<std::string_view> & source) {
  size_t counter = 0;
  auto start = clk::now();
  for(auto sv : source) {
    if(is_ascii_branchless(sv)) counter ++;
  }
  auto duration = clk::now() - start;
  auto elapsed_s = duration_cast<dur_double>(duration).count();
  std::cout << "branchless:" << volume(source) / (1000 * 1000 * 1000.0) /  elapsed_s << " GB/s"
            << std::endl;
  std::cout << counter << std::endl;
}

void hybrid(const std::vector<std::string_view> & source) {
  size_t counter = 0;
  auto start = clk::now();
  for(auto sv : source) {
    if(is_ascii_hybrid(sv)) counter ++;
  }
  auto duration = clk::now() - start;
  auto elapsed_s = duration_cast<dur_double>(duration).count();
  std::cout << "nearly branchless:" << volume(source) / (1000 * 1000 * 1000.0) /  elapsed_s << " GB/s"
            << std::endl;
  std::cout << counter << std::endl;
}

std::vector<char> build_large_random(size_t N) {
  std::vector<char> buffer(N);
  for(size_t i = 0; i < buffer.size(); i++) {
    buffer[i] = (rand() % 128);
  }
  return buffer;
}

std::vector<std::string_view> build_string_view_stream(size_t N, std::vector<char> & source) {
  size_t safeN = source.size() - 128;
  std::vector<std::string_view> answer(N);
  std::cout << "Creating " <<N << " strings" << std::endl; 
  for(size_t i = 0; i < N; i++) {
    std::string_view sv(source.data() + (rand() % safeN), rand() % 128);
    answer[i] = sv;
  }
  return answer;
}
int main() {
  size_t N = 1000000;
  auto random_source = build_large_random(N);
  auto sv_source = build_string_view_stream(N, random_source);
  branchy(sv_source);
  branchless(sv_source);
  hybrid(sv_source);
}