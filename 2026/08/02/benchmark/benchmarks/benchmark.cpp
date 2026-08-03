// Comparing std::vector, std::list and std::hive (via the plf::hive reference
// implementation) on the two operations that matter most in practice:
// building a container by appending elements, and iterating over it.
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <iterator>
#include <list>
#include <memory>
#include <memory_resource>
#include <print>
#include <string>
#include <vector>

#include "plf_hive.h"

#include "counters/bench.h"

using value_type = uint64_t;

// std::hive spells "append at the end" as insert(); the sequence containers
// spell it push_back(). One name for the three of them.
inline void append(std::vector<value_type> &c, value_type v) { c.push_back(v); }
inline void append(std::list<value_type> &c, value_type v) { c.push_back(v); }
inline void append(plf::hive<value_type> &c, value_type v) { c.insert(v); }

template <class C>
concept has_reserve = requires(C &c) { c.reserve(size_t(1)); };

static void pretty_print(const std::string &name, size_t num_values,
                         counters::event_aggregate agg) {
  std::print("{:<44} : ", name);
  std::print(" {:8.3f} ns/elem ", agg.fastest_elapsed_ns() / double(num_values));
  std::print(" {:6.3f} Gelem/s ",
             double(num_values) / agg.fastest_elapsed_ns());
  if (counters::has_performance_counters()) {
    std::print(" {:5.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
    std::print(" {:8.2f} c ", agg.fastest_cycles() / double(num_values));
    std::print(" {:8.2f} i ", agg.fastest_instructions() / double(num_values));
    std::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / double(agg.fastest_cycles()));
  }
  std::print("\n");
}

// Elements are removed if this returns true. It is a cheap hash, so the
// elements we drop are scattered pseudo-randomly through the container.
static inline bool doomed(value_type v) {
  return ((v * 0x9E3779B97F4A7C15ull) >> 63) != 0;
}

static counters::bench_parameter params() {
  counters::bench_parameter p;
  p.min_repeat = 5;
  p.min_time_ns = 300'000'000;
  return p;
}

// ---------------------------------------------------------------------------
// A std::list whose nodes come from a pre-allocated arena: the "reserve"
// counterpart to std::vector (reserve) and std::hive (reserve). We back a
// std::pmr::list with a std::pmr::monotonic_buffer_resource over a buffer
// large enough for the whole container, so push_back never touches malloc.
// ---------------------------------------------------------------------------

// How many bytes does a pmr::list of N elements ask its allocator for? We ask
// a counting memory_resource, which never releases anything, and read off the
// total. The node layout lives in the implementation, so we measure it.
static size_t pmr_list_arena_bytes(size_t N) {
  struct counting : std::pmr::memory_resource {
    size_t used = 0;
    void *do_allocate(size_t bytes, size_t align) override {
      bytes = (bytes + align - 1) / align * align;
      used += bytes;
      return std::malloc(bytes);
    }
    void do_deallocate(void *p, size_t, size_t) override { std::free(p); }
    bool do_is_equal(const std::pmr::memory_resource &o) const noexcept override {
      return this == &o;
    }
  } count;
  {
    // No monotonic resource here: we want the exact per-node cost, not the
    // power-of-two block sizes a monotonic resource would round up to.
    std::pmr::list<value_type> l(&count);
    for (size_t i = 0; i < N; i++) {
      l.push_back(value_type(i));
    }
  }
  return count.used;
}

// An arena-backed list. An upstream "alarm" trips if the pre-allocated arena
// was ever too small (i.e. some node was allocated from the default resource),
// which would silently ruin the measurement.
struct reserved_list {
  struct alarm : std::pmr::memory_resource {
    bool exhausted = false;
    void *do_allocate(size_t bytes, size_t align) override {
      exhausted = true;
      return std::pmr::get_default_resource()->allocate(bytes, align);
    }
    void do_deallocate(void *p, size_t bytes, size_t align) override {
      std::pmr::get_default_resource()->deallocate(p, bytes, align);
    }
    bool do_is_equal(const std::pmr::memory_resource &o) const noexcept override {
      return this == &o;
    }
  } alarm_;
  // new std::byte[] leaves the buffer uninitialized, like vector::reserve;
  // a std::vector<std::byte> would zero-fill the whole arena every time.
  size_t arena_bytes_ = 0;
  std::unique_ptr<std::byte[]> arena_;
  std::pmr::monotonic_buffer_resource res_;
  std::pmr::list<value_type> c_;

  explicit reserved_list(size_t N)
      : arena_bytes_(pmr_list_arena_bytes(N)),
        arena_(new std::byte[arena_bytes_]), res_(arena_.get(), arena_bytes_, &alarm_),
        c_(&res_) {}

  void push_back(value_type v) { c_.push_back(v); }
  void remove_if_doomed() { c_.remove_if(doomed); }
  auto begin() { return c_.begin(); }
  auto end() { return c_.end(); }
  auto begin() const { return c_.begin(); }
  auto end() const { return c_.end(); }
  size_t size() const { return c_.size(); }
  void check_arena() const {
    if (alarm_.exhausted) {
      std::fputs("warning: reserved-list arena was too small\n", stderr);
    }
  }
};

static double bench_build_reserved_list(const std::string &name, size_t N) {
  volatile uint64_t sink = 0;
  size_t arena_bytes = pmr_list_arena_bytes(N);
  auto f = [&]() {
    std::unique_ptr<std::byte[]> arena(new std::byte[arena_bytes]);
    reserved_list::alarm a;
    std::pmr::monotonic_buffer_resource res(arena.get(), arena_bytes, &a);
    std::pmr::list<value_type> c(&res);
    for (size_t i = 0; i < N; i++) {
      c.push_back(value_type(i));
    }
    sink += c.size();
    if (a.exhausted) {
      std::fputs("warning: reserved-list arena was too small\n", stderr);
    }
  };
  auto agg = counters::bench(f, params());
  pretty_print(name, N, agg);
  return agg.fastest_elapsed_ns() / double(N);
}

// ---------------------------------------------------------------------------
// 1. Building: start from an empty container, append N elements, destroy it.
//    The allocation and the teardown are inside the measurement because that
//    is what you actually pay for.
// ---------------------------------------------------------------------------
template <class C>
static double bench_build(const std::string &name, size_t N, bool reserve) {
  volatile uint64_t sink = 0;
  auto f = [&]() {
    C c;
    if constexpr (has_reserve<C>) {
      if (reserve) {
        c.reserve(N);
      }
    }
    for (size_t i = 0; i < N; i++) {
      append(c, value_type(i));
    }
    sink += c.size();
  };
  auto agg = counters::bench(f, params());
  pretty_print(name, N, agg);
  return agg.fastest_elapsed_ns() / double(N);
}

// ---------------------------------------------------------------------------
// 2. Iterating: walk the whole container and sum the values.
// ---------------------------------------------------------------------------
template <class C> static double bench_sum(const std::string &name, const C &c) {
  volatile uint64_t sink = 0;
  size_t N = c.size();
  auto f = [&]() {
    uint64_t s = 0;
    for (value_type v : c) {
      s += v;
    }
    sink += s;
  };
  auto agg = counters::bench(f, params());
  pretty_print(name, N, agg);
  return agg.fastest_elapsed_ns() / double(N);
}

// Is the traversal latency-bound? Walk the container with two independent
// iterators at once, one starting halfway in. If the cost per element visited
// drops, the single-iterator loop was mostly waiting on load latency.
template <class C>
static void bench_sum_two_streams(const std::string &name, const C &c) {
  volatile uint64_t sink = 0;
  size_t N = c.size();
  size_t half = N / 2;
  auto mid = c.begin();
  std::advance(mid, half);
  auto f = [&]() {
    uint64_t s1 = 0, s2 = 0;
    auto a = c.begin();
    auto b = mid;
    for (size_t k = 0; k < half; k++) {
      s1 += *a;
      ++a;
      s2 += *b;
      ++b;
    }
    sink += s1 + s2;
  };
  pretty_print(name, 2 * half, counters::bench(f, params()));
}

// ---------------------------------------------------------------------------
// 3. Erasing half the elements, at scattered positions. Erasing is destructive
//    so we must rebuild the container each time: we report build+erase and
//    subtract the build-only cost measured in step 1.
// ---------------------------------------------------------------------------
static void erase_half(std::vector<value_type> &c) {
  c.erase(std::remove_if(c.begin(), c.end(), doomed), c.end());
}
static void erase_half(std::list<value_type> &c) { c.remove_if(doomed); }
static void erase_half(plf::hive<value_type> &c) {
  for (auto it = c.begin(); it != c.end();) {
    it = doomed(*it) ? c.erase(it) : std::next(it);
  }
}

template <class C>
static void bench_build_and_erase(const std::string &name, size_t N,
                                  double build_ns_per_elem) {
  volatile uint64_t sink = 0;
  auto f = [&]() {
    C c;
    for (size_t i = 0; i < N; i++) {
      append(c, value_type(i));
    }
    erase_half(c);
    sink += c.size();
  };
  auto agg = counters::bench(f, params());
  double total = agg.fastest_elapsed_ns() / double(N);
  std::print("{:<44} :  {:8.3f} ns/elem  (build {:8.3f} + erase {:8.3f})\n",
             name, total, build_ns_per_elem, total - build_ns_per_elem);
}

static void bench_build_and_erase_reserved_list(const std::string &name,
                                                size_t N,
                                                double build_ns_per_elem) {
  volatile uint64_t sink = 0;
  size_t arena_bytes = pmr_list_arena_bytes(N);
  auto f = [&]() {
    std::unique_ptr<std::byte[]> arena(new std::byte[arena_bytes]);
    reserved_list::alarm a;
    std::pmr::monotonic_buffer_resource res(arena.get(), arena_bytes, &a);
    std::pmr::list<value_type> c(&res);
    for (size_t i = 0; i < N; i++) {
      c.push_back(value_type(i));
    }
    c.remove_if(doomed);
    sink += c.size();
  };
  auto agg = counters::bench(f, params());
  double total = agg.fastest_elapsed_ns() / double(N);
  std::print("{:<44} :  {:8.3f} ns/elem  (build {:8.3f} + erase {:8.3f})\n",
             name, total, build_ns_per_elem, total - build_ns_per_elem);
}

template <class C> static C make_container(size_t N) {
  C c;
  for (size_t i = 0; i < N; i++) {
    append(c, value_type(i));
  }
  return c;
}

template <class C> static C make_aged_container(size_t N) {
  C c = make_container<C>(N);
  erase_half(c);
  return c;
}

// How much memory does the container really hold? We ask the allocator instead
// of guessing: mallinfo2().uordblks is the number of bytes currently handed out
// by malloc.
#ifdef __GLIBC__
#include <malloc.h>
static size_t heap_in_use() {
  auto info = mallinfo2();
  // uordblks covers the ordinary heap; hblkhd covers the large blocks that
  // glibc satisfies with mmap. A 80 MB vector lives entirely in the latter.
  return info.uordblks + info.hblkhd;
}
static constexpr bool can_measure_heap = true;
#else
static size_t heap_in_use() { return 0; }
static constexpr bool can_measure_heap = false;
#endif

static double bytes_per_reserved_list(size_t N, bool aged) {
  size_t before = heap_in_use();
  size_t arena_bytes = pmr_list_arena_bytes(N);
  size_t after, live;
  {
    std::unique_ptr<std::byte[]> arena(new std::byte[arena_bytes]);
    std::pmr::monotonic_buffer_resource res(arena.get(), arena_bytes);
    std::pmr::list<value_type> c(&res);
    for (size_t i = 0; i < N; i++) {
      c.push_back(value_type(i));
    }
    if (aged) {
      c.remove_if(doomed);
    }
    after = heap_in_use();
    live = c.size();
  }
  return double(after - before) / double(live);
}

template <class C> static double bytes_per_element(size_t N, bool aged) {
  size_t before = heap_in_use();
  C *c = new C;
  for (size_t i = 0; i < N; i++) {
    append(*c, value_type(i));
  }
  if (aged) {
    erase_half(*c);
  }
  size_t after = heap_in_use();
  double result = double(after - before) / double(c->size());
  delete c;
  return result;
}

static void run(size_t N) {
  std::print("\n===== {} elements ({} bytes of payload) =====\n\n", N,
             N * sizeof(value_type));

  std::print("-- appending {} elements to an empty container --\n", N);
  double v_build =
      bench_build<std::vector<value_type>>("std::vector push_back", N, false);
  bench_build<std::vector<value_type>>("std::vector push_back (reserve)", N,
                                       true);
  double h_build =
      bench_build<plf::hive<value_type>>("std::hive insert", N, false);
  bench_build<plf::hive<value_type>>("std::hive insert (reserve)", N, true);
  double l_build =
      bench_build<std::list<value_type>>("std::list push_back", N, false);
  double lr_build =
      bench_build_reserved_list("std::list push_back (arena)", N);

  auto v = make_container<std::vector<value_type>>(N);
  auto h = make_container<plf::hive<value_type>>(N);
  auto l = make_container<std::list<value_type>>(N);
  reserved_list lr(N);
  for (size_t i = 0; i < N; i++) {
    lr.push_back(value_type(i));
  }
  lr.check_arena();

  std::print("\n-- summing all the values --\n");
  bench_sum("std::vector sum", v);
  bench_sum("std::hive sum", h);
  bench_sum("std::list sum", l);
  bench_sum("std::list sum (arena)", lr);

  std::print("\n-- same sum, but two interleaved traversals --\n");
  bench_sum_two_streams("std::vector sum x2", v);
  bench_sum_two_streams("std::hive sum x2", h);
  bench_sum_two_streams("std::list sum x2", l);
  bench_sum_two_streams("std::list sum x2 (arena)", lr);

  std::print("\n-- erasing half the elements (scattered) --\n");
  bench_build_and_erase<std::vector<value_type>>("std::vector build+erase", N,
                                                 v_build);
  bench_build_and_erase<plf::hive<value_type>>("std::hive build+erase", N,
                                               h_build);
  bench_build_and_erase<std::list<value_type>>("std::list build+erase", N,
                                               l_build);
  bench_build_and_erase_reserved_list("std::list build+erase (arena)", N,
                                      lr_build);

  auto va = make_aged_container<std::vector<value_type>>(N);
  auto ha = make_aged_container<plf::hive<value_type>>(N);
  auto la = make_aged_container<std::list<value_type>>(N);
  reserved_list lra(N);
  for (size_t i = 0; i < N; i++) {
    lra.push_back(value_type(i));
  }
  lra.remove_if_doomed();
  lra.check_arena();

  std::print("\n-- summing the values after half were erased --\n");
  bench_sum("std::vector sum (aged)", va);
  bench_sum("std::hive sum (aged)", ha);
  bench_sum("std::list sum (aged)", la);
  bench_sum("std::list sum (aged, arena)", lra);

  if constexpr (can_measure_heap) {
    std::print("\n-- heap held, bytes per live element --\n");
    std::print("{:<44} :  {:8.2f}\n", "std::vector",
               bytes_per_element<std::vector<value_type>>(N, false));
    std::print("{:<44} :  {:8.2f}\n", "std::hive",
               bytes_per_element<plf::hive<value_type>>(N, false));
    std::print("{:<44} :  {:8.2f}\n", "std::list",
               bytes_per_element<std::list<value_type>>(N, false));
    std::print("{:<44} :  {:8.2f}\n", "std::list (arena)",
               bytes_per_reserved_list(N, false));
    std::print("{:<44} :  {:8.2f}\n", "std::vector (aged)",
               bytes_per_element<std::vector<value_type>>(N, true));
    std::print("{:<44} :  {:8.2f}\n", "std::hive (aged)",
               bytes_per_element<plf::hive<value_type>>(N, true));
    std::print("{:<44} :  {:8.2f}\n", "std::list (aged)",
               bytes_per_element<std::list<value_type>>(N, true));
    std::print("{:<44} :  {:8.2f}\n", "std::list (aged, arena)",
               bytes_per_reserved_list(N, true));
  }
}

int main(int argc, char **argv) {
  std::print("element type: {} bytes\n", sizeof(value_type));
  std::print("hive block capacity: {} .. {} elements\n",
             plf::hive<value_type>::block_capacity_default_limits().min,
             plf::hive<value_type>::block_capacity_default_limits().max);
  std::print("performance counters: {}\n",
             counters::has_performance_counters() ? "yes" : "no");
  if (argc > 1) {
    run(size_t(std::stoull(argv[1])));
    return 0;
  }
  for (size_t N : {size_t(100'000), size_t(1'000'000), size_t(10'000'000)}) {
    run(N);
  }
  return 0;
}
