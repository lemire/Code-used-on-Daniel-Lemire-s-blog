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

using clk = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<clk>;
using dur_double = std::chrono::duration<double>;
using std::chrono::duration_cast;

#include "isa.h"

template <typename T> class atomic_ptr {
public:
  atomic_ptr(T *_ptr) : ptr{_ptr} {}

  operator const T *() const { return ptr.load(); }
  const T &operator*() const { return *ptr; }
  const T *operator->() const { return ptr.load(); }

  operator T *() { return ptr.load(); }
  T &operator*() { return *ptr; }
  T *operator->() { return ptr.load(); }
  atomic_ptr &operator=(T *_ptr) {
    ptr = _ptr;
    return *this;
  }

private:
  std::atomic<T *> ptr;
};

class implementation {
public:
  implementation(std::string s, uint32_t u)
      : _name(s), _required_instruction_sets(u) {}
  const std::string &name() const { return _name; }
  uint32_t required_instruction_sets() const {
    return _required_instruction_sets;
  };

  virtual void operate(int &x) const = 0;
  std::string _name;
  uint32_t _required_instruction_sets;
};

class avx_implementation : public implementation {
public:
  avx_implementation() : implementation("avx", instruction_set::AVX2) {}
  void operate(int &x) const { x += 3; }
};

class sse_implementation : public implementation {
public:
  sse_implementation() : implementation("sse", instruction_set::SSE42) {}
  void operate(int &x) const { x += 2; }
};

class legacy_implementation : public implementation {
public:
  legacy_implementation() : implementation("legacy", 0) {}
  void operate(int &x) const { x += 1; }
};

const avx_implementation avx_singleton;
const sse_implementation sse_singleton;
const legacy_implementation legacy_singleton;

const std::initializer_list<const implementation *>
    available_implementation_pointers{
        &avx_singleton, &sse_singleton,
        &legacy_singleton}; // available_implementation_pointers

atomic_ptr<const implementation> active_implementation{&legacy_singleton};

const implementation *detect_best_supported() noexcept {
  // They are prelisted in priority order, so we just go down the list
  uint32_t supported_instruction_sets = detect_supported_architectures();
  for (const implementation *impl : available_implementation_pointers) {
    uint32_t required_instruction_sets = impl->required_instruction_sets();
    if ((supported_instruction_sets & required_instruction_sets) ==
        required_instruction_sets) {
      return impl;
    }
  }
  return &legacy_singleton;
}

void reset() {
  auto start = clk::now();
  active_implementation = detect_best_supported();
  auto duration = clk::now() - start;
  auto elapsed_s = duration_cast<dur_double>(duration).count();
  std::cout << "reset:" << elapsed_s * 1000 * 1000 * 1000 << " ns" << std::endl;
}

void nothing() {
  auto start = clk::now();
  auto duration = clk::now() - start;
  auto elapsed_s = duration_cast<dur_double>(duration).count();
  std::cout << "nothing:" << elapsed_s * 1000 * 1000 * 1000 << " ns"
            << std::endl;
}


void justquery(uint32_t &counter) {
  auto start = clk::now();
  counter += detect_supported_architectures();
  auto duration = clk::now() - start;
  auto elapsed_s = duration_cast<dur_double>(duration).count();
  std::cout << "justquery:" << elapsed_s * 1000 * 1000 * 1000 << " ns"
            << std::endl;
}

void direct(int &counter) {
  auto start = clk::now();
  active_implementation->operate(counter);
  auto duration = clk::now() - start;
  auto elapsed_s = duration_cast<dur_double>(duration).count();
  std::cout << "direct:" << elapsed_s * 1000 * 1000 * 1000 << " ns" << std::endl;
}

int main() {
  printf("%s\n", active_implementation->name().c_str());
  for (size_t i = 0; i < 10; i++)
    nothing();
  uint32_t c;
  for (size_t i = 0; i < 10; i++)
    justquery(c);
  for (size_t i = 0; i < 10; i++)
    reset();
  int counter;
  for (size_t i = 0; i < 10; i++)
    direct(counter);
  printf("%s\n", active_implementation->name().c_str());
  printf("%d %u\n", counter, c);
}
