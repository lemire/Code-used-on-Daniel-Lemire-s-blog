#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <iostream>
#include <print>
#include <random>
#include <string>
#include <vector>

#include "counters/bench.h"

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

#ifdef __SSE2__
#include <emmintrin.h>
#endif


struct array_container_t {
    const uint16_t *array;
    size_t cardinality;
};


__attribute__((always_inline)) inline bool simd_quad(const array_container_t *arr, uint16_t pos) {
    constexpr int32_t gap = 16;
    const uint16_t *carr = arr->array;
    int32_t cardinality = arr->cardinality;
    if (cardinality < gap) {
        // two overlapping loads cover the array: no data-dependent branch
#ifdef __ARM_NEON
        if (cardinality >= 8) {
            uint16x8_t needle = vdupq_n_u16(pos);
            uint16x8_t v0 = vld1q_u16(carr);
            uint16x8_t v1 = vld1q_u16(carr + cardinality - 8);
            uint16x8_t hit = vorrq_u16(vceqq_u16(v0, needle), vceqq_u16(v1, needle));
            return vmaxvq_u16(hit) != 0;
        }
        if (cardinality >= 4) {
            uint16x4_t needle = vdup_n_u16(pos);
            uint16x4_t v0 = vld1_u16(carr);
            uint16x4_t v1 = vld1_u16(carr + cardinality - 4);
            uint16x4_t hit = vorr_u16(vceq_u16(v0, needle), vceq_u16(v1, needle));
            return vmaxv_u16(hit) != 0;
        }
#else
        if (cardinality >= 8) {
            __m128i needle = _mm_set1_epi16((short)pos);
            __m128i v0 = _mm_loadu_si128((const __m128i *)carr);
            __m128i v1 = _mm_loadu_si128((const __m128i *)(carr + cardinality - 8));
            __m128i hit = _mm_or_si128(_mm_cmpeq_epi16(v0, needle),
                                       _mm_cmpeq_epi16(v1, needle));
            return _mm_movemask_epi8(hit) != 0;
        }
        if (cardinality >= 4) {
            __m128i needle = _mm_set1_epi16((short)pos);
            __m128i v0 = _mm_loadl_epi64((const __m128i *)carr);
            __m128i v1 = _mm_loadl_epi64((const __m128i *)(carr + cardinality - 4));
            __m128i hit = _mm_or_si128(_mm_cmpeq_epi16(v0, needle),
                                       _mm_cmpeq_epi16(v1, needle));
            return (_mm_movemask_epi8(hit) & 0xFF) != 0;
        }
#endif
        if (cardinality == 0) return false;
        return (carr[0] == pos) | (carr[cardinality / 2] == pos) |
               (carr[cardinality - 1] == pos);
    }
    int32_t num_blocks = cardinality / gap;
    int32_t base = 0;
    int32_t n = num_blocks;
    while (n > 3) {
      int32_t quarter = n >> 2;

      int32_t k1 = carr[(base + quarter + 1) * gap - 1];
      int32_t k2 = carr[(base + 2 * quarter + 1) * gap - 1];
      int32_t k3 = carr[(base + 3 * quarter + 1) * gap - 1];

      int32_t c1 = (k1 < pos);
      int32_t c2 = (k2 < pos);
      int32_t c3 = (k3 < pos);

      base += (c1 + c2 + c3) * quarter;
      n -= 3 * quarter;
    }
    while (n > 1) {
        int32_t half = n >> 1;
        base = (carr[(base + half + 1) * gap - 1] < pos) ? base + half : base;
        n -= half;
    }
    int32_t lo = (carr[(base + 1) * gap - 1] < pos) ? base + 1 : base;

    if (lo < num_blocks) {
        const uint16_t *blk = carr + lo * gap;
#ifdef __ARM_NEON
        uint16x8_t needle = vdupq_n_u16(pos);
        uint16x8_t v0 = vld1q_u16(blk);
        uint16x8_t v1 = vld1q_u16(blk + 8);
        uint16x8_t hit = vorrq_u16(vceqq_u16(v0, needle), vceqq_u16(v1, needle));
        return vmaxvq_u16(hit) != 0;
#else
        __m128i needle = _mm_set1_epi16((short)pos);
        __m128i v0 = _mm_loadu_si128((const __m128i *)blk);
        __m128i v1 = _mm_loadu_si128((const __m128i *)(blk + 8));
        __m128i hit = _mm_or_si128(_mm_cmpeq_epi16(v0, needle),
                                   _mm_cmpeq_epi16(v1, needle));
        return _mm_movemask_epi8(hit) != 0;
#endif
    }

    for (int32_t j = num_blocks * gap; j < cardinality; j++) {
        uint16_t v = carr[j];
        if (v >= pos) return (v == pos);
    }
    return false;
}


__attribute__((always_inline)) inline bool simd_binary(const array_container_t *arr, uint16_t pos) {
    constexpr int32_t gap = 16;
    const uint16_t *carr = arr->array;
    int32_t cardinality = arr->cardinality;
    if (cardinality < gap) {
        // two overlapping loads cover the array: no data-dependent branch
#ifdef __ARM_NEON
        if (cardinality >= 8) {
            uint16x8_t needle = vdupq_n_u16(pos);
            uint16x8_t v0 = vld1q_u16(carr);
            uint16x8_t v1 = vld1q_u16(carr + cardinality - 8);
            uint16x8_t hit = vorrq_u16(vceqq_u16(v0, needle), vceqq_u16(v1, needle));
            return vmaxvq_u16(hit) != 0;
        }
        if (cardinality >= 4) {
            uint16x4_t needle = vdup_n_u16(pos);
            uint16x4_t v0 = vld1_u16(carr);
            uint16x4_t v1 = vld1_u16(carr + cardinality - 4);
            uint16x4_t hit = vorr_u16(vceq_u16(v0, needle), vceq_u16(v1, needle));
            return vmaxv_u16(hit) != 0;
        }
#else
        if (cardinality >= 8) {
            __m128i needle = _mm_set1_epi16((short)pos);
            __m128i v0 = _mm_loadu_si128((const __m128i *)carr);
            __m128i v1 = _mm_loadu_si128((const __m128i *)(carr + cardinality - 8));
            __m128i hit = _mm_or_si128(_mm_cmpeq_epi16(v0, needle),
                                       _mm_cmpeq_epi16(v1, needle));
            return _mm_movemask_epi8(hit) != 0;
        }
        if (cardinality >= 4) {
            __m128i needle = _mm_set1_epi16((short)pos);
            __m128i v0 = _mm_loadl_epi64((const __m128i *)carr);
            __m128i v1 = _mm_loadl_epi64((const __m128i *)(carr + cardinality - 4));
            __m128i hit = _mm_or_si128(_mm_cmpeq_epi16(v0, needle),
                                       _mm_cmpeq_epi16(v1, needle));
            return (_mm_movemask_epi8(hit) & 0xFF) != 0;
        }
#endif
        if (cardinality == 0) return false;
        return (carr[0] == pos) | (carr[cardinality / 2] == pos) |
               (carr[cardinality - 1] == pos);
    }
    int32_t num_blocks = cardinality / gap;
    int32_t base = 0;
    int32_t n = num_blocks;
    while (n > 1) {
        int32_t half = n >> 1;
        base = (carr[(base + half + 1) * gap - 1] < pos) ? base + half : base;
        n -= half;
    }
    int32_t lo = (carr[(base + 1) * gap - 1] < pos) ? base + 1 : base;

    if (lo < num_blocks) {
        const uint16_t *blk = carr + lo * gap;
#ifdef __ARM_NEON
        uint16x8_t needle = vdupq_n_u16(pos);
        uint16x8_t v0 = vld1q_u16(blk);
        uint16x8_t v1 = vld1q_u16(blk + 8);
        uint16x8_t hit = vorrq_u16(vceqq_u16(v0, needle), vceqq_u16(v1, needle));
        return vmaxvq_u16(hit) != 0;
#else
        __m128i needle = _mm_set1_epi16((short)pos);
        __m128i v0 = _mm_loadu_si128((const __m128i *)blk);
        __m128i v1 = _mm_loadu_si128((const __m128i *)(blk + 8));
        __m128i hit = _mm_or_si128(_mm_cmpeq_epi16(v0, needle),
                                   _mm_cmpeq_epi16(v1, needle));
        return _mm_movemask_epi8(hit) != 0;
#endif
    }

    for (int32_t j = num_blocks * gap; j < cardinality; j++) {
        uint16_t v = carr[j];
        if (v >= pos) return (v == pos);
    }
    return false;
}


double pretty_print(const std::string &name, const std::string &mode,
                    size_t num_values, counters::event_aggregate agg) {
  std::print("{:<40} {:<4} : ", name, mode);
  std::print(" {:5.3f} ns ", agg.fastest_elapsed_ns() / double(num_values));
  std::print(" {:5.2f} Gv/s ", double(num_values) / agg.fastest_elapsed_ns());
  if (counters::has_performance_counters()) {
    std::print(" {:5.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
    std::print(" {:5.2f} c ", agg.fastest_cycles() / double(num_values));
    std::print(" {:5.2f} i ", agg.fastest_instructions() / double(num_values));
    std::print(" {:5.2f} bm ", agg.branch_misses() / double(num_values));
    std::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / double(agg.fastest_cycles()));
  }
  std::print("\n");
  return double(num_values) / agg.fastest_elapsed_ns();
}

void collect_benchmark_results(size_t array_size, size_t number_arrays,
                               size_t warmth, bool run_cold, bool run_warm) {
  if (array_size > 65536) {
    array_size = 65536;
  }
  std::mt19937_64 rng(42);

  std::vector<std::vector<uint16_t>> arrays;
  arrays.reserve(number_arrays);
  std::vector<uint32_t> pool(65536);
  for (uint32_t i = 0; i < 65536; ++i) {
    pool[i] = i;
  }
  for (size_t a = 0; a < number_arrays; ++a) {
    for (size_t i = 0; i < array_size; ++i) {
      std::uniform_int_distribution<size_t> dist(i, pool.size() - 1);
      size_t j = dist(rng);
      std::swap(pool[i], pool[j]);
    }
    std::vector<uint16_t> arr(array_size);
    for (size_t i = 0; i < array_size; ++i) {
      arr[i] = uint16_t(pool[i]);
    }
    std::sort(arr.begin(), arr.end());
    arrays.push_back(std::move(arr));
  }

  size_t total = number_arrays * warmth;

  std::vector<size_t> indices_cold(total);
  for (size_t i = 0; i < total; ++i) {
    indices_cold[i] = i % number_arrays;
  }
  std::vector<size_t> indices_warm(total);
  for (size_t i = 0; i < total; ++i) {
    indices_warm[i] = i / warmth;
  }

  std::vector<uint16_t> keys(total);
  std::uniform_int_distribution<uint32_t> key_dist(0, 65535);
  for (size_t i = 0; i < total; ++i) {
    keys[i] = uint16_t(key_dist(rng));
  }

  volatile uint64_t counter = 0;

  std::print("arrays: {}  size per array: {}  warmth: {}  total queries per mode: {}\n",
             number_arrays, array_size, warmth, total);

  {
    size_t check_count = std::min<size_t>(total, size_t(10000));
    size_t mismatches = 0;
    auto check_one = [&](const char *name, bool got, bool expected,
                         const std::vector<uint16_t> &arr, uint16_t k) {
      if (got != expected) {
        if (mismatches < 5) {
          std::print("MISMATCH: {} arr_size={} key={} expected={} got={}\n",
                     name, arr.size(), k, expected, got);
        }
        ++mismatches;
      }
    };
    for (size_t i = 0; i < check_count; ++i) {
      const auto &arr = arrays[indices_cold[i]];
      uint16_t k = keys[i];
      array_container_t container{arr.data(), arr.size()};
      bool expected = std::binary_search(arr.begin(), arr.end(), k);
      check_one("simd_quad", simd_quad(&container, k), expected, arr, k);
      check_one("simd_binary", simd_binary(&container, k), expected, arr, k);
    }
    if (mismatches > 0) {
      std::print("FAIL: {} mismatches across {} queries — aborting\n", mismatches, check_count);
      std::exit(1);
    }
    std::print("verification passed: {} queries × all algorithms agree with std::binary_search\n",
               check_count);
  }

  auto bench_inline = [&](const char *base, auto fn) {
    auto make = [&](const std::vector<size_t> &indices) {
      return [&arrays, &indices, &keys, &counter, fn]() {
        size_t c = 0;
        size_t n = indices.size();
        for (size_t i = 0; i < n; ++i) {
          if (fn(arrays[indices[i]], keys[i])) ++c;
        }
        counter += c;
      };
    };
    if (run_cold) {
      pretty_print(base, "cold", total, counters::bench(make(indices_cold)));
    }
    if (run_warm) {
      pretty_print(base, "warm", total, counters::bench(make(indices_warm)));
    }
  };

  bench_inline("simd_quad", [](const std::vector<uint16_t> &arr, uint16_t k) {
    array_container_t container{arr.data(), arr.size()};
    return simd_quad(&container, k);
  });
  bench_inline("simd_binary", [](const std::vector<uint16_t> &arr, uint16_t k) {
    array_container_t container{arr.data(), arr.size()};
    return simd_binary(&container, k);
  });
  bench_inline("binary_search", [](const std::vector<uint16_t> &arr, uint16_t k) {
    return std::binary_search(arr.begin(), arr.end(), k);
  });
  bench_inline("find", [](const std::vector<uint16_t> &arr, uint16_t k) {
    return std::find(arr.begin(), arr.end(), k) != arr.end();
  });
}

int main(int argc, char **argv) {
  size_t number_arrays = 100000;
  size_t warmth = 100;
  enum class Mode { Both, ColdOnly, WarmOnly };
  Mode mode = Mode::Both;
  std::vector<size_t> array_sizes = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--number" && i + 1 < argc) {
      number_arrays = std::stoul(argv[++i]);
    } else if (arg == "--warmth" && i + 1 < argc) {
      warmth = std::stoul(argv[++i]);
    } else if (arg == "--cold") {
      mode = Mode::ColdOnly;
    } else if (arg == "--warm") {
      mode = Mode::WarmOnly;
    } else if (arg == "--lengths" && i + 1 < argc) {
      std::string lengths_str = argv[++i];
      array_sizes.clear();
      size_t pos = 0;
      while ((pos = lengths_str.find(',')) != std::string::npos) {
        array_sizes.push_back(std::stoul(lengths_str.substr(0, pos)));
        lengths_str.erase(0, pos + 1);
      }
      array_sizes.push_back(std::stoul(lengths_str));
    }
  }
  bool run_cold = (mode != Mode::WarmOnly);
  bool run_warm = (mode != Mode::ColdOnly);
  for (size_t array_size : array_sizes) {
    collect_benchmark_results(array_size, number_arrays, warmth, run_cold, run_warm);
  }
}
