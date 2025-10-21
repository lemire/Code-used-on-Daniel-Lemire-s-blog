In large code bases, we are often stuck with unpleasant designs that are harming our performance. We might be looking for a non-intrusive method to improve the performance. For example, you may not want to change the function signatures.

Let us consider a concrete example. Maybe someone designed the programming interface so that you have to access the values from a map using an index. They may have code like so:

```cpp
auto at_index(map_like auto& index_map, size_t idx) {
  size_t count = 0;
  for (const auto &[key, value] : index_map) {
    if(count == idx)
      return value;
    count++;
  }
  throw std::out_of_range("Index out of range");
}
```

This code goes through the keys of the map idx times. Typictally, it implies some kind of linked list traversal. If you are stuck with this interface, going through the values might imply repeated calls to the at_indexfunction:

```
for (size_t i = 0; i < input_size; ++i) {
    at_index(index_map, i);
}
```

If you took any kind of computer science, you will immediately see the problem: my code has quadratic complexity. If you double the map size, you may quadruple the running time. It is likely fine if you have 2 or 4 elements in the map, but definitively not fine if you have 400 elements.

The proper solution is to avoid such a design. If you can have access directly to the map, you can just iterate through it directly:

```cpp
for (auto& [key, value] : index_map) {
   sum += value;
}
```

But what if you are stuck? Then you can use a a static or thread_localcache. The key inisght is to keep in cache your location in the map, and start from there on the next query. If the user is typictally querying in seqpence, then your cache should speed up tremendously the function.

```cpp
auto 
at_index_thread_local_cache(map_like auto& index_map,
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
```

In C++, a thread_local variable is such that there is just one instance of the variable (shared by all function calls) within the same thread. If you wish to have just one instance of the variable for the entire program, you can use static instead, but thread_local is the best choice in our case. You might be worried about the performance implication of a thread_local variable, but it is generally quite cheap: we only add a few instructions when acceasing it or modifying it.

Our cache variable remembers the last accessed iterator and index per thread. If the next index is requested, we just increment the iterator and return. If the access is non-sequential or the first call, it falls back to a linear scan from the beginning, rebuilding the cache along the way.

The code is more complicated, and if you are not accessing the key in sequence, it might be slower. However, the performance gains can be enormous. [I wrote a benchmark to test it out with maps containing 400 elements](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2025/10/19/).

| Method | ns/key | instructions/key |
| -------- | -------- | -------- |
| original | 300 | 2000 |
| cache | 2 | 17 |

In my case, the cache multiplied the performance by 150. Not bad.
