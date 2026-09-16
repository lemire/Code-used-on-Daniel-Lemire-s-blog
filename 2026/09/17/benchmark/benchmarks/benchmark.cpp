// Comparing C++23's std::flat_map with std::map (a red-black tree) and
// std::unordered_map (a hash table) on the operations that matter in
// practice: building, lookup, iteration, erasure, and memory usage.
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <flat_map>
#include <format>
#include <iterator>
#include <map>
#include <new>
#include <numeric>
#include <print>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "counters/bench.h"

using key_type = uint64_t;
using mapped_type = uint64_t;

using tree_map = std::map<key_type, mapped_type>;
using hash_map = std::unordered_map<key_type, mapped_type>;
using flat_map = std::flat_map<key_type, mapped_type>;

// Returns the mapped value, or 0 if the key is absent.
template <class C> inline mapped_type lookup(const C &c, key_type k) {
  auto it = c.find(k);
  return it == c.end() ? mapped_type(0) : mapped_type(it->second);
}

// Sum of the mapped values, in iteration order.
template <class C> inline uint64_t sum_all(const C &c) {
  uint64_t s = 0;
  for (const auto &kv : c) {
    s += kv.second;
  }
  return s;
}

// Elements are removed if this returns true: about half of them, scattered
// pseudo-randomly through the container.
static inline bool doomed(key_type k) {
  return ((k * 0x9E3779B97F4A7C15ull) >> 63) != 0;
}

template <class C> inline void erase_half_by_key(C &c, const std::vector<key_type> &keys) {
  for (key_type k : keys) {
    if (doomed(k)) {
      c.erase(k);
    }
  }
}

template <class C> inline void erase_half_if(C &c) {
  std::erase_if(c, [](const auto &kv) { return doomed(kv.first); });
}

// ---------------------------------------------------------------------------
// Input data
// ---------------------------------------------------------------------------
struct splitmix64 {
  uint64_t x;
  uint64_t operator()() {
    uint64_t z = (x += 0x9E3779B97F4A7C15ull);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
    return z ^ (z >> 31);
  }
};

struct dataset {
  std::vector<key_type> keys;          // N distinct random keys, random order
  std::vector<key_type> sorted_keys;   // the same keys, increasing
  std::vector<mapped_type> values;     // values[i] goes with keys[i]
  std::vector<std::pair<key_type, mapped_type>> pairs;        // (keys[i], values[i])
  std::vector<std::pair<key_type, mapped_type>> sorted_pairs; // same, sorted by key
  std::vector<key_type> hits;          // the keys, in another random order
  std::vector<key_type> misses;        // random keys that are not in the map
  size_t first;                        // starting index for the dependent chain
};

// The mapped value of keys[i] is the index of the next key to look up, so
// that following the values visits every key exactly once, in random order.
// That gives us a lookup benchmark where each lookup depends on the previous.
static dataset make_dataset(size_t N) {
  splitmix64 rng{0x1234567ull + N};
  dataset d;
  d.keys.resize(N);
  for (;;) {
    for (auto &k : d.keys) {
      k = rng();
    }
    std::vector<key_type> tmp = d.keys;
    std::sort(tmp.begin(), tmp.end());
    if (std::unique(tmp.begin(), tmp.end()) == tmp.end()) {
      d.sorted_keys = std::move(tmp);
      break;
    }
  }
  std::vector<size_t> order(N);
  std::iota(order.begin(), order.end(), size_t(0));
  for (size_t i = N - 1; i > 0; i--) {
    std::swap(order[i], order[rng() % (i + 1)]);
  }
  d.values.resize(N);
  for (size_t j = 0; j < N; j++) {
    d.values[order[j]] = mapped_type(order[(j + 1) % N]);
  }
  d.first = order[0];
  d.pairs.resize(N);
  for (size_t i = 0; i < N; i++) {
    d.pairs[i] = {d.keys[i], d.values[i]};
  }
  d.sorted_pairs = d.pairs;
  std::sort(d.sorted_pairs.begin(), d.sorted_pairs.end());
  d.hits = d.keys;
  for (size_t i = N - 1; i > 0; i--) {
    std::swap(d.hits[i], d.hits[rng() % (i + 1)]);
  }
  d.misses.resize(N);
  for (auto &k : d.misses) {
    do {
      k = rng();
    } while (std::binary_search(d.sorted_keys.begin(), d.sorted_keys.end(), k));
  }
  return d;
}

// ---------------------------------------------------------------------------
// Harness
// ---------------------------------------------------------------------------
static void pretty_print(const std::string &name, size_t num_values,
                         counters::event_aggregate agg) {
  std::print("{:<48} : ", name);
  std::print(" {:8.2f} ns/op ", agg.fastest_elapsed_ns() / double(num_values));
  if (counters::has_performance_counters()) {
    std::print(" {:5.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
    std::print(" {:8.2f} c ", agg.fastest_cycles() / double(num_values));
    std::print(" {:8.2f} i ", agg.fastest_instructions() / double(num_values));
    std::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / double(agg.fastest_cycles()));
  }
  std::print("\n");
}

static counters::bench_parameter params() {
  counters::bench_parameter p;
  p.min_repeat = 3;
  p.min_time_ns = 200'000'000;
  // std::steady_clock only has microsecond resolution on macOS: make each
  // timed block long enough that this does not matter.
  p.min_time_per_inner_ns = 1'000'000;
  return p;
}

// One-at-a-time insertion into a flat container shifts everything after the
// insertion point, so N random insertions cost O(N^2). Past this size we skip
// the quadratic cases rather than wait for hours.
static constexpr size_t quadratic_limit = 100'000;

template <class C> constexpr bool is_flat = std::is_same_v<C, flat_map>;

static void skip(const std::string &name) {
  std::print("{:<48} :   (skipped: quadratic)\n", name);
}

// 1. Building. We start from an empty container, insert N elements one at a
//    time (in random or in increasing key order), and destroy it. Then we
//    build from a range of pairs in one go, which lets a flat container sort
//    once instead of shifting on every insertion.
template <class C>
static void bench_insert(const std::string &name, const dataset &d,
                         const std::vector<key_type> &keys) {
  size_t N = keys.size();
  if (is_flat<C> && N > quadratic_limit && &keys == &d.keys) {
    skip(name);
    return;
  }
  volatile uint64_t sink = 0;
  auto f = [&]() {
    C c;
    for (size_t i = 0; i < N; i++) {
      c.emplace(keys[i], d.values[i]);
    }
    sink += c.size();
  };
  pretty_print(name, N, counters::bench(f, params()));
}

template <class C> static C from_range(const dataset &d) {
  return C(d.pairs.begin(), d.pairs.end());
}

template <class C>
static void bench_from_range(const std::string &name, const dataset &d) {
  size_t N = d.keys.size();
  volatile uint64_t sink = 0;
  auto f = [&]() {
    C c = from_range<C>(d);
    sink += c.size();
  };
  pretty_print(name, N, counters::bench(f, params()));
}

template <class C>
static void bench_from_sorted_range(const std::string &name, const dataset &d) {
  size_t N = d.keys.size();
  volatile uint64_t sink = 0;
  auto f = [&]() {
    if constexpr (is_flat<C>) {
      C c(std::sorted_unique, d.sorted_pairs.begin(), d.sorted_pairs.end());
      sink += c.size();
    } else {
      C c(d.sorted_pairs.begin(), d.sorted_pairs.end());
      sink += c.size();
    }
  };
  pretty_print(name, N, counters::bench(f, params()));
}

// 2. Lookups. Independent lookups let the processor overlap several of them;
//    the dependent chain (each key comes from the previous lookup's value)
//    measures the latency of a single lookup.
template <class C>
static void bench_lookup(const std::string &name, const C &c,
                         const std::vector<key_type> &queries) {
  volatile uint64_t sink = 0;
  auto f = [&]() {
    uint64_t s = 0;
    for (key_type k : queries) {
      s += lookup(c, k);
    }
    sink += s;
  };
  pretty_print(name, queries.size(), counters::bench(f, params()));
}

template <class C>
static void bench_lookup_chain(const std::string &name, const C &c,
                               const dataset &d) {
  size_t N = d.keys.size();
  volatile uint64_t sink = 0;
  auto f = [&]() {
    size_t idx = d.first;
    for (size_t i = 0; i < N; i++) {
      idx = size_t(c.find(d.keys[idx])->second);
    }
    sink += idx;
  };
  pretty_print(name, N, counters::bench(f, params()));
}

// 3. Iteration: walk the whole container in its natural order.
template <class C>
static void bench_iterate(const std::string &name, const C &c) {
  volatile uint64_t sink = 0;
  auto f = [&]() { sink += sum_all(c); };
  pretty_print(name, c.size(), counters::bench(f, params()));
}

// 4. Erasing half the elements at scattered positions. Erasure is destructive
//    so we must rebuild the container before each run. Building a node
//    container is expensive and noisy, so rather than subtracting a separately
//    measured build cost, we time the erase phase alone with a fine clock and
//    report the fastest run.
static uint64_t now_ns() {
  timespec ts;
#if defined(__APPLE__)
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
#else
  clock_gettime(CLOCK_MONOTONIC, &ts);
#endif
  return uint64_t(ts.tv_sec) * 1'000'000'000ull + uint64_t(ts.tv_nsec);
}

template <class C>
static void bench_erase(const std::string &name, const dataset &d, bool by_key) {
  size_t N = d.keys.size();
  if (by_key && is_flat<C> && N > quadratic_limit) {
    skip(name);
    return;
  }
  volatile uint64_t sink = 0;
  uint64_t best = UINT64_MAX;
  uint64_t total = 0;
  size_t erased = 0;
  for (size_t rep = 0; rep < 3 || total < 200'000'000ull; rep++) {
    C c = from_range<C>(d);
    uint64_t t0 = now_ns();
    if (by_key) {
      erase_half_by_key(c, d.hits);
    } else {
      erase_half_if(c);
    }
    uint64_t t1 = now_ns();
    sink += c.size();
    erased = N - c.size();
    best = std::min(best, t1 - t0);
    total += t1 - t0;
  }
  std::print("{:<48} :  {:8.2f} ns/elem  ({:8.2f} ns per erased element)\n", name,
             double(best) / double(N), double(best) / double(erased));
}

// 5. Memory: how many bytes does the container really hold per element? We
//    replace the global operator new/delete and ask the allocator how big each
//    block it handed out really is (a 40-byte request costs 48 bytes on macOS,
//    64 bytes with glibc). Counting is switched on only around the measurement
//    so that the timed benchmarks are not disturbed.
#if defined(__GLIBC__)
#include <malloc.h>
// malloc_usable_size excludes glibc's 8-byte chunk header; add it back so
// that the numbers match what mallinfo2() reports.
static size_t block_size(void *p) { return malloc_usable_size(p) + sizeof(size_t); }
static constexpr bool can_measure_heap = true;
#elif defined(__APPLE__)
#include <malloc/malloc.h>
static size_t block_size(void *p) { return malloc_size(p); }
static constexpr bool can_measure_heap = true;
#else
static size_t block_size(void *) { return 0; }
static constexpr bool can_measure_heap = false;
#endif

static bool heap_counting = false;
static size_t heap_bytes = 0;

// For a large block, the allocator may hand out a recycled region that is
// bigger than requested (macOS does this: ask for 80 MB, get 97 MB). That is
// not memory the container needs, so large blocks are charged their requested
// size, rounded up to a page. We remember what we charged so that the matching
// delete subtracts the same amount.
static constexpr size_t large_threshold = 1 << 20;
static constexpr size_t page = 16384;
struct large_block {
  void *p;
  size_t charged;
};
static large_block large_blocks[4096];
static size_t large_count = 0;

static size_t charge_new(void *p, size_t n) {
  size_t actual = block_size(p);
  if (actual < large_threshold) {
    return actual;
  }
  size_t charged = std::min(actual, (n + page - 1) / page * page);
  if (large_count < std::size(large_blocks)) {
    large_blocks[large_count++] = {p, charged};
    return charged;
  }
  return actual;
}

static size_t charge_delete(void *p) {
  size_t actual = block_size(p);
  if (actual >= large_threshold) {
    for (size_t i = 0; i < large_count; i++) {
      if (large_blocks[i].p == p) {
        size_t charged = large_blocks[i].charged;
        large_blocks[i] = large_blocks[--large_count];
        return charged;
      }
    }
  }
  return actual;
}

void *operator new(size_t n) {
  void *p = std::malloc(n ? n : 1);
  if (!p) {
    throw std::bad_alloc();
  }
  if (heap_counting) {
    heap_bytes += charge_new(p, n);
  }
  return p;
}
void *operator new[](size_t n) { return operator new(n); }
void operator delete(void *p) noexcept {
  if (p) {
    if (heap_counting) {
      heap_bytes -= charge_delete(p);
    }
    std::free(p);
  }
}
void operator delete(void *p, size_t) noexcept { operator delete(p); }
void operator delete[](void *p) noexcept { operator delete(p); }
void operator delete[](void *p, size_t) noexcept { operator delete(p); }

template <class C> static double bytes_per_element_once(const dataset &d) {
  heap_bytes = 0;
  large_count = 0;
  heap_counting = true;
  C *c = new C(from_range<C>(d));
  size_t held = heap_bytes;
  heap_counting = false;
  double result = double(held) / double(c->size());
  delete c;
  return result;
}

// Measured twice, keeping the smaller value: a one-time lazy allocation made
// by the runtime inside the measurement window only affects the first run.
template <class C> static double bytes_per_element(const dataset &d) {
  return std::min(bytes_per_element_once<C>(d), bytes_per_element_once<C>(d));
}

// ---------------------------------------------------------------------------
static void report_memory(const dataset &d) {
  if constexpr (can_measure_heap) {
    std::print("\n-- heap held, bytes per element (payload: {} bytes) --\n",
               sizeof(key_type) + sizeof(mapped_type));
    std::print("{:<48} :  {:8.2f}\n", "std::map", bytes_per_element<tree_map>(d));
    std::print("{:<48} :  {:8.2f}\n", "std::unordered_map", bytes_per_element<hash_map>(d));
    std::print("{:<48} :  {:8.2f}\n", "std::flat_map", bytes_per_element<flat_map>(d));
  }
}

static void run(size_t N, bool memory_only) {
  dataset d = make_dataset(N);
  std::print("\n===== {} elements =====\n", N);
  if (memory_only) {
    report_memory(d);
    return;
  }
  std::print("\n");

  std::print("-- inserting {} elements one at a time, random key order --\n", N);
  bench_insert<tree_map>("std::map insert", d, d.keys);
  bench_insert<hash_map>("std::unordered_map insert", d, d.keys);
  bench_insert<flat_map>("std::flat_map insert", d, d.keys);

  std::print("\n-- inserting {} elements one at a time, increasing key order --\n", N);
  bench_insert<tree_map>("std::map insert (sorted)", d, d.sorted_keys);
  bench_insert<hash_map>("std::unordered_map insert (sorted)", d, d.sorted_keys);
  bench_insert<flat_map>("std::flat_map insert (sorted)", d, d.sorted_keys);

  std::print("\n-- constructing from a range of {} elements, random key order --\n", N);
  bench_from_range<tree_map>("std::map from range", d);
  bench_from_range<hash_map>("std::unordered_map from range", d);
  bench_from_range<flat_map>("std::flat_map from range", d);

  std::print("\n-- constructing from a sorted range of {} elements --\n", N);
  bench_from_sorted_range<tree_map>("std::map from sorted range", d);
  bench_from_sorted_range<hash_map>("std::unordered_map from sorted range", d);
  bench_from_sorted_range<flat_map>("std::flat_map from sorted range", d);

  auto tm = from_range<tree_map>(d);
  auto hm = from_range<hash_map>(d);
  auto fm = from_range<flat_map>(d);

  std::print("\n-- {} independent lookups, all keys present --\n", N);
  bench_lookup("std::map find (hit)", tm, d.hits);
  bench_lookup("std::unordered_map find (hit)", hm, d.hits);
  bench_lookup("std::flat_map find (hit)", fm, d.hits);

  std::print("\n-- {} independent lookups, no key present --\n", N);
  bench_lookup("std::map find (miss)", tm, d.misses);
  bench_lookup("std::unordered_map find (miss)", hm, d.misses);
  bench_lookup("std::flat_map find (miss)", fm, d.misses);

  std::print("\n-- {} dependent lookups (each key comes from the previous value) --\n", N);
  bench_lookup_chain("std::map find (chain)", tm, d);
  bench_lookup_chain("std::unordered_map find (chain)", hm, d);
  bench_lookup_chain("std::flat_map find (chain)", fm, d);

  std::print("\n-- iterating over all {} elements --\n", N);
  bench_iterate("std::map iterate", tm);
  bench_iterate("std::unordered_map iterate", hm);
  bench_iterate("std::flat_map iterate", fm);

  std::print("\n-- erasing half the elements, one erase(key) call at a time --\n");
  bench_erase<tree_map>("std::map erase(key)", d, true);
  bench_erase<hash_map>("std::unordered_map erase(key)", d, true);
  bench_erase<flat_map>("std::flat_map erase(key)", d, true);

  std::print("\n-- erasing half the elements with std::erase_if --\n");
  bench_erase<tree_map>("std::map erase_if", d, false);
  bench_erase<hash_map>("std::unordered_map erase_if", d, false);
  bench_erase<flat_map>("std::flat_map erase_if", d, false);

  report_memory(d);
}

int main(int argc, char **argv) {
  std::print("key type: {} bytes, mapped type: {} bytes\n", sizeof(key_type),
             sizeof(mapped_type));
#if defined(_LIBCPP_VERSION)
  std::print("standard library: libc++ {}\n", _LIBCPP_VERSION);
#elif defined(__GLIBCXX__)
  std::print("standard library: libstdc++ {}\n", __GLIBCXX__);
#endif
  std::print("performance counters: {}\n",
             counters::has_performance_counters() ? "yes" : "no");
  // Usage: benchmark [mem] [N ...]. "mem" reports only the memory usage.
  bool memory_only = false;
  std::vector<size_t> sizes;
  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == "mem") {
      memory_only = true;
    } else {
      sizes.push_back(size_t(std::stoull(argv[i])));
    }
  }
  if (sizes.empty()) {
    sizes = {1'000, 100'000, 1'000'000, 10'000'000};
  }
  for (size_t N : sizes) {
    run(N, memory_only);
  }
  return 0;
}
