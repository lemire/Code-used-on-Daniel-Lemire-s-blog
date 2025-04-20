#ifndef SUMVALUES_H
#define SUMVALUES_H

#include <algorithm>
#include <numeric>
#include <ranges>

template <typename map_type> auto sum_values(const map_type &map) {
  auto values = map | std::ranges::views::values;
  return std::accumulate(values.begin(), values.end(),
                         typename map_type::mapped_type{});
}

template <typename map_type>
size_t count_keys_with_prefix(const map_type &map, std::string_view prefix) {
  auto keys = map | std::ranges::views::keys;
  return std::ranges::count_if(
      keys, [prefix](std::string_view key) { return key.starts_with(prefix); });
}

template <typename map_type> auto sum_values_daniel(const map_type &map) {
  typename map_type::mapped_type sum{};
  for (const auto &value : map | std::ranges::views::values) {
    sum += value;
  }
  return sum;
}

template <typename map_type>
size_t count_keys_with_prefix_daniel(const map_type &map,
                                     std::string_view prefix) {
  size_t count = 0;
  for (const auto &key : map | std::ranges::views::keys) {
    if (key.starts_with(prefix)) {
      ++count;
    }
  }
  return count;
}

template <typename map_type>
typename map_type::mapped_type sum_values_cpp11(const map_type &map) {
  typename map_type::mapped_type sum = typename map_type::mapped_type();
  for (typename map_type::const_iterator it = map.begin(); it != map.end();
       ++it) {
    sum += it->second;
  }
  return sum;
}

template <typename map_type>
size_t count_keys_with_prefix_cpp11(const map_type &map,
                                    const std::string &prefix) {
  size_t count = 0;
  for (typename map_type::const_iterator it = map.begin(); it != map.end();
       ++it) {
    const std::string &key = it->first;
    if (key.size() >= prefix.size() &&
        key.compare(0, prefix.size(), prefix) == 0) {
      ++count;
    }
  }
  return count;
}

template <typename map_type>
size_t count_keys_with_prefix_cpp11_starts_with(const map_type &map,
                                                const std::string &prefix) {
  size_t count = 0;
  for (typename map_type::const_iterator it = map.begin(); it != map.end();
       ++it) {
    const std::string &key = it->first;
    if (key.starts_with(prefix)) {
      ++count;
    }
  }
  return count;
}

#endif