#pragma once

#include "performancecounters/event_counter.h"
#include <atomic>
#include <cstdio>

event_collector collector;

template <class function_type>
event_aggregate bench(const function_type &function, size_t min_repeat = 1,
                      size_t min_time_ns = 1000000000,
                      size_t max_repeat = 1000000, double tolerance = 2.0) {
  // run it a few times to warm up the cache
  for (size_t i = 0; i < 10; i++) {
    function();
  }

  size_t N = min_repeat;
  if (N == 0) {
    N = 1;
  }
  size_t max_trials = 30;
  size_t trial = 0;
  std::pair<double, event_aggregate> best{std::numeric_limits<double>::max(),
                                     event_aggregate{}};
  do {
    event_aggregate aggregate{};
    for (size_t i = 0; i < N; i++) {
      std::atomic_thread_fence(std::memory_order_acquire);
      collector.start();
      function();
      std::atomic_thread_fence(std::memory_order_release);
      event_count allocate_count = collector.end();
      aggregate << allocate_count;
      if ((i + 1 == N) && (aggregate.total_elapsed_ns() < min_time_ns) &&
          (N < max_repeat)) {
        N *= 10;
      }
    }
    double ratio = aggregate.elapsed_ns() / aggregate.fastest_elapsed_ns();
    trial++;
    if(ratio < tolerance) {
      return aggregate;
    }
    if(ratio < best.first) {
      best = {ratio, aggregate};
    }
    if(trial >= max_trials) {
      //fprintf(stderr, "Warning: failed to converge after %zu trials got %f \n", max_trials, best.first);
      return best.second;
    }
  } while(true);
}
