#ifndef SEARCH_H
#define SEARCH_H

#include <map>
#include <ranges>

template <typename T>
concept map_like = std::ranges::range<T> && requires {
  typename T::key_type;
  typename T::mapped_type;
  typename T::value_type;
} && std::same_as<typename T::value_type,
          std::pair<const typename T::key_type, typename T::mapped_type>>
   && std::convertible_to<typename T::mapped_type, size_t>;

auto at_index(map_like auto& index_map, size_t idx) {
  size_t count = 0;
  for (const auto &[_, value] : index_map) {
    if(count == idx)
      return value;
    count++;
  }
  throw std::out_of_range("Index out of range");
}


auto at_index_cache(map_like auto const& index_map, size_t idx) {
  using Iterator = decltype(index_map.begin());
  struct Cache {
    Iterator last_iterator;
    size_t last_index = static_cast<size_t>(-1);
    decltype(&index_map) map_ptr = nullptr;
  };
  static Cache cache;
  if (cache.map_ptr == &index_map && idx == cache.last_index + 1
      && cache.last_iterator != index_map.end()) {
    ++cache.last_iterator;
    cache.last_index = idx;
    if (cache.last_iterator != index_map.end()) {
      return cache.last_iterator->second;
    } else {
      throw std::out_of_range("Index out of range");
    }
  } else {
    cache.last_iterator = index_map.begin();
    cache.last_index = static_cast<size_t>(-1);
    cache.map_ptr = &index_map;
    size_t count = 0;
    for (auto it = index_map.begin(); it != index_map.end(); ++it) {
      if (count == idx) {
        cache.last_iterator = it;
        cache.last_index = idx;
        return it->second;
      }
      count++;
    }
    throw std::out_of_range("Index out of range");
  }
}


auto at_index_thread_local_cache(map_like auto& index_map,
    size_t idx) {
  using iterator = decltype(index_map.begin());
  struct Cache {
    iterator last_iterator;
    size_t last_index = -1;
    decltype(&index_map) map_ptr = nullptr;
  };
  thread_local Cache cache;
  if (cache.map_ptr == &index_map
      && idx == cache.last_index + 1
      && cache.last_iterator != index_map.end()) {
    cache.last_iterator++;
    cache.last_index = idx;
    if (cache.last_iterator != index_map.end()) {
      return cache.last_iterator->second;
    } else {
      throw std::out_of_range("Index out of range");
    }
  } else {
    cache.last_iterator = index_map.begin();
    cache.last_index = -1;
    cache.map_ptr = &index_map;
    size_t count = 0;
    for (auto it = index_map.begin();
        it != index_map.end(); ++it) {
      if (count == idx) {
        cache.last_iterator = it;
        cache.last_index = idx;
        return it->second;
      }
      count++;
    }
    throw std::out_of_range("Index out of range");
  }
}


#endif // SEARCH_H