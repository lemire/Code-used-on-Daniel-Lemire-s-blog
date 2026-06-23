#pragma once

#include "performancecounters/event_counter.h"
#include  <atomic>
event_collector collector;

template <class function_type> 
event_aggregate bench(const function_type& function, size_t min_repeat = 1000, size_t min_time_ns = 200, size_t max_repeat = 1000000) {

    std::chrono::time_point<std::chrono::steady_clock> benchmark_start = std::chrono::steady_clock::now();

    event_aggregate aggregate{};
    size_t N = min_repeat;
    if(N == 0) { N = 1; }
    for (size_t i = 0; i < N; i++) {
      std::atomic_thread_fence(std::memory_order_acquire);
      collector.start();
      function();
      std::atomic_thread_fence(std::memory_order_release);
      event_count allocate_count = collector.end();
      aggregate << allocate_count;
      // if((i+1 == N) && (aggregate.total_elapsed_ns() < min_time_ns) && (N<max_repeat)) {
      //   N *= 10;
      // }
    }

    std::chrono::time_point<std::chrono::steady_clock> benchmark_end = std::chrono::steady_clock::now();

    printf("\n\nTotal benchmark time: %d\n", (benchmark_end - benchmark_start));

    return aggregate;
}
