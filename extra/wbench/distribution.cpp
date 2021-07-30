#include <vector>
#ifdef __linux__
#include "linux-perf-events.h"
#else
#warning "Linux expected."
#endif
#include "timing.h"

template <typename PROCEDURE>
void bench(PROCEDURE f) {
  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);;
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);
  std::vector<unsigned long long> results;
  results.resize(evts.size());
  f();
  auto start_time = NowNanos();
  unified.start();
  auto time = NowNanos() - start_time;
  unified.end(results);
  printf("cycles: %5.1f, instructions: (%5.1f, %4.2f/cycle)  effective frequency %4.2f GHz\n",
    results[0]*1.0,
    results[1]*1.0,
    results[1]*1.0/results[0],
    results[0]*1.0/time);
}

int main() {
  char * data;
  auto allocate_memory = [&data]()  {
    data = new char[100000]{};
  };
  bench(allocate_memory);
}
