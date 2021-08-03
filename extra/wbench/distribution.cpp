#include <vector>
#include <algorithm>
#include <random>

#ifdef __linux__
#include "linux-perf-events.h"
#else
#warning "Linux expected."
#endif
#include "timing.h"

template <typename PROCEDURE>
__attribute__((noinline))
uint64_t bench(PROCEDURE f, char *& data, size_t N) {
  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);;
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);
  std::vector<unsigned long long> results;
  results.resize(evts.size());

  auto start_time = NowNanos();
  unified.start();
  f(data, N);
  auto time = NowNanos() - start_time;
  unified.end(results);
  printf("nanoseconds: %5.3f/byte, cycles/byte: %5.1f, instructions: (%5.1f/byte, %4.2f/cycle)  effective frequency %4.2f GHz\n",
    time*1.0/N,
    results[0]*1.0/N,
    results[1]*1.0/N,
    results[1]*1.0/results[0],
    results[0]*1.0/time);
  return time;
}

int main(int argc, char** argv) {
  char * data;
  size_t N = argc == 1 ? 100000000: atol(argv[1]);
  printf("allocate-init\n");
  auto allocate_memory = [](char *& data, size_t N)  {
    data = new char[N];
    for(size_t i = 0; i < N; i++) { data[i] = i; }

  };
  uint64_t m0{99999999999999999};
  uint64_t m1{0};
  for(size_t z = 0; z < 20; z++) {
    uint64_t m = bench(allocate_memory, data, N);
    if(m > m1) { m1 = m; }
    if(m < m0) { m0 = m; }
    delete[] data;
  }
  std::cout << (double(m1) / double(m0) - 1 ) * 100 << " % " << std::endl;
  
  return EXIT_SUCCESS;
}
