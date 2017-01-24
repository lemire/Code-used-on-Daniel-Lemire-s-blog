// g++ -O3 -o lock lock.cpp -std=c++11
#include <cassert>
#include <math.h>
#include <iostream>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <vector>

constexpr int CacheLineSize = 64;

using namespace std;

class WallClockTimer {
public:
  struct timeval t1, t2;
  WallClockTimer() : t1(), t2() {
    gettimeofday(&t1, 0);
    t2 = t1;
  }
  void reset() {
    gettimeofday(&t1, 0);
    t2 = t1;
  }
  int elapsed() {
    return (t2.tv_sec * 1000 * 1000 + t2.tv_usec ) -
           (t1.tv_sec * 1000 * 1000 + t1.tv_usec );
  }
  int split() {
    gettimeofday(&t2, 0);
    return elapsed();
  }
};

template <class T> void locked_add(T *out, T in) {
  asm volatile("lock add %0, %1" : "+r"(in), "+m"(*out) : : "memory", "cc");
}

template <class T> void runtest() {
  size_t N = 100 * 1000;
  int repeat = 100;
  WallClockTimer timer;
  const bool paranoid = false;
  cout << " processing word of size " << sizeof(T) << endl;
  for (unsigned int offset = 0; offset < CacheLineSize + sizeof(T); ++offset) {
    char cache[CacheLineSize + sizeof(T)];
    unsigned long long absolute_offset = reinterpret_cast<unsigned long long>(&cache[offset]);
    cout << " absolute offset = " << (absolute_offset % CacheLineSize) << endl;
    if (absolute_offset % CacheLineSize > (absolute_offset + sizeof(T) - 1) % CacheLineSize) {
      cout << " spans cache line" << endl;
    }
    int sumt = 0;
    size_t sumofsquares = 0;
    int mintime = 0xFFFFFF;
    int maxtime = 0;
    for (int k = 0; k < repeat; ++k) {
      timer.reset();
      T *val = reinterpret_cast<T *>(&cache[offset]);
      *val = 1;
      for (size_t i = 0; i < N; ++i) {
        locked_add(val, static_cast<T>(i) * (*val) + 33);
      }
      int time = timer.split();
      if(mintime > time) mintime = time;
      if(maxtime < time) maxtime = time;
      sumt += time;
      sumofsquares += time * time;
    }
    double average = sumt * 1.0 / repeat;
    double averagesquare = sumofsquares * 1.0 / repeat;
    double error = sqrt(averagesquare - average * average);
    cout << " average time for offset " << (absolute_offset % CacheLineSize) << " is "
         << average << " (+/- "<< error <<", range : ["<<mintime<<"-"<<maxtime<<"] ) " << endl;
    cout << endl;
  }
}

int main() {
  runtest<int>();
  cout << sizeof(int) << endl;
  runtest<long>();
  cout << sizeof(long) << endl;
  runtest<long long>();
  cout << sizeof(long long) << endl;
  return 0;
}


