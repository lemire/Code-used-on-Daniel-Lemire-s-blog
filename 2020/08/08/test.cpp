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
#include <string_view>
#include <thread>
#include <vector>

using clk = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<clk>;
using dur_double = std::chrono::duration<double>;
using std::chrono::duration_cast;

template <class T> size_t volume(const T &source) {
  size_t vol = 0;
  for (auto sv : source) {
    vol += sv.size();
  }
  return vol;
}

size_t counter = 0;

template <class T>
__attribute__((noinline)) void do_not_optimize(const std::vector<T> &source) {
  counter += source.size();
  asm volatile("" ::: "memory");
}

template <class T>
void cost_of_copy(const std::vector<T> &source, bool verbose = true) {
  auto start = clk::now();
  std::vector<T> my_copy(source);
  do_not_optimize(my_copy);
  auto duration = clk::now() - start;
  auto elapsed_s = duration_cast<dur_double>(duration).count();
  if (verbose) {
    std::cout << "speed:" << volume(source) / (1000 * 1000 * 1000.0) / elapsed_s
              << " GB/s" << std::endl;
  }
}

std::vector<char> build_large_random(size_t N) {
  std::vector<char> buffer(N);
  for (size_t i = 0; i < buffer.size(); i++) {
    buffer[i] = (rand() % 128);
  }
  return buffer;
}

std::vector<std::string_view>
build_string_view_stream(size_t N, std::vector<char> &source) {
  size_t safeN = source.size() - 128;
  std::vector<std::string_view> answer(N);
  std::cout << "Creating " << N << " strings" << std::endl;
  for (size_t i = 0; i < N; i++) {
    std::string_view sv(source.data() + (rand() % safeN), rand() % 128);
    answer[i] = sv;
  }
  return answer;
}
int main() {
  size_t N = 10000;
  auto random_source = build_large_random(N);
  std::vector<std::string_view> sv_source =
      build_string_view_stream(N, random_source);
  std::vector<std::string> string_source;
  for (auto sv : sv_source) {
    string_source.emplace_back(sv);
  }
  std::cout << "Number of strings: " << string_source.size() << std::endl;
  std::cout << "Volume (KB)      : " << volume(string_source)/1024 << std::endl;


  // warming up
  cost_of_copy(sv_source, false);
  cost_of_copy(string_source, false);
  for (size_t i = 0; i < 5; i++) {

    std::cout << "string_view :";
    cost_of_copy(sv_source, false); // warm up
    cost_of_copy(sv_source);

    std::cout << "string      :";
    cost_of_copy(string_source, false);  // warm up
    cost_of_copy(string_source);
    std::cout << std::endl;
  }
}
