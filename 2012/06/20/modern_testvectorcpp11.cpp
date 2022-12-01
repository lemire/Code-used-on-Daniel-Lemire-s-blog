#include <cassert>
#include <chrono>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <vector>

typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef unsigned long long ticks;

using namespace std;

class CPUBenchmark {
public:
  CPUBenchmark() : ticktime() { start(); }

  std::chrono::high_resolution_clock::time_point ticktime;

  void start() { ticktime = std::chrono::high_resolution_clock::now(); }

  unsigned long long stop() {
    auto finish = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(finish -
                                                                ticktime)
        .count();
  }
};
template <class C> 
__attribute__((noinline)) 
vector<C> runtestbasic(size_t N) {
  vector<C> bigarray(N);
  for (unsigned int k = 0; k < N; ++k)
    bigarray[k] = k;
  return bigarray;
}
template <class C> 
__attribute__((noinline)) 
vector<C> runtestnice(size_t N) {
  vector<C> bigarray;
  for (unsigned int k = 0; k < N; ++k)
    bigarray.push_back(k);
  return bigarray;
}
template <class C> 
__attribute__((noinline)) 
vector<C> runtestnicewreserve(size_t N) {
  vector<C> bigarray;
  bigarray.reserve(N);
  for (unsigned int k = 0; k < N; ++k)
    bigarray.push_back(k);
  return bigarray;
}



template <class C> 
__attribute__((noinline)) 
vector<int> runtestinsert(size_t N, C* prealloc) {
  vector<int> bigarray;
  bigarray.insert(bigarray.begin(), prealloc, prealloc + N);
  return bigarray;
}

template <class C> 
__attribute__((noinline)) 
vector<C> runtestemplacewreserve(size_t N) {
  vector<C> bigarray;
  bigarray.reserve(N);
  for (unsigned int k = 0; k < N; ++k)
    bigarray.emplace_back(k);
  return bigarray;
}
template <class C> 
__attribute__((noinline)) 
vector<C> runtestinit(size_t N) {
  vector<C> bigarray(N);
  for (unsigned int k = 0; k < N; ++k)
    bigarray[k] = k;
  return bigarray;
}

// bogus class
class BenchmarkTester {
public:
  BenchmarkTester()
      : m_myVec(100), m_myPair(24.5, "Happy days are here again"), X(0)

  {}

  BenchmarkTester(int x)
      : m_myVec(100),
        m_myPair(24.5, "Happy days are here again"), X(x)

  {}

  BenchmarkTester &operator+=(const BenchmarkTester &o) {
    X += o.X;
    return *this;
  }

  operator int() { return X; }

private:
  vector<int> m_myVec;
  pair<double, string> m_myPair;
  int X;
};

template <class C> void demo() {
  CPUBenchmark time;
  const size_t N = 2 * 1000 * 1000;
  C * prealloc = new C[N];
  for(size_t i = 0; i < N; i++) { prealloc[i] = i; }
  double x, y;
  size_t trials = 1000;

  cout.precision(3);
  cout << " report speed indicator in nanoseconds per integer" << endl;
  volatile C hole;
  x = 1e300;
  y = 0;
  for (size_t i = 0; i < trials; i++) {
    time.start();
    auto z = runtestinit<C>(N);
    auto result = time.stop() * 1.0 / N;
    y += result;
    if (result < x) {
      x = result;
    }
    hole = z[0];
  }
  y = y / trials;
  cout << "with init                    : " << x << " (" << (y - x) * 100 / x << " %)"
       << endl;

  x = 1e300;
  y = 0;
  for (size_t i = 0; i < trials; i++) {
    time.start();
    auto z = runtestbasic<C>(N);
    auto result = time.stop() * 1.0 / N;
    y += result;
    if (result < x) {
      x = result;
    }
    hole = z[0];
  }
  y = y / trials;
  cout << "basic               : " << x << " (" << (y - x) * 100 / x << " %)"
       << endl;
   x = 1e300;
  y = 0;
  for (size_t i = 0; i < trials; i++) {
    time.start();
    auto z = runtestnice<C>(N);
    auto result = time.stop() * 1.0 / N;
    y += result;
    if (result < x) {
      x = result;
    }
    hole = z[0];
  }
  y = y / trials;
  cout << "with push_back               : " << x << " (" << (y - x) * 100 / x << " %)"
       << endl;
  x = 1e300;
  y = 0;
  for (size_t i = 0; i < trials; i++) {
    time.start();
    auto z = runtestnicewreserve<C>(N);
    auto result = time.stop() * 1.0 / N;
    y += result;
    if (result < x) {
      x = result;
    }
    hole = z[0];
  }
  y = y / trials;
  cout << "with push_back and reserve   : " << x << " (" << (y  - x) * 100 / x
       << " %)" << endl;

  x = 1e300;
  y = 0;

  for (size_t i = 0; i < trials; i++) {
    time.start();
    auto z = runtestemplacewreserve<C>(N);
    auto result = time.stop() * 1.0 / N;
    y += result;
    if (result < x) {
      x = result;
    }
    hole = z[0];
  }
  y = y / trials;
  cout << "with emplace_back and reserve: " << x << " ("
       << (y - x) * 100 / x << " %)" << endl;
  x = 1e300;
  y = 0;

  for (size_t i = 0; i < trials; i++) {
    time.start();
    auto z = runtestinsert(N, prealloc);
    auto result = time.stop() * 1.0 / N;
    y += result;
    if (result < x) {
      x = result;
    }
    hole = z[0];
  }
  y = y / trials;
  cout << "with insert: " << x << " ("
       << (y - x) * 100 / x << " %)" << endl;
  if(hole == 0) { printf("zero"); }
}

int main() {
  cout << "**With int primitive" << endl;
  demo<int>();
  cout << endl;
  return 0;
}
