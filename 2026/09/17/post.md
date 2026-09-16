# How fast is C++23's std::flat_map?

C++23 added a new type to the standard library: `std::flat_map`.
There is also a `std::flat_set` and other variants, but let me focus on `std::flat_map`.

A flat map is a sorted vector of keys next to a vector of values.
A query is a binary search over the sorted keys. 

You need a recent standard library: GCC 15's libstdc++ has `std::flat_map`, as
does LLVM's libc++ since LLVM 20 (Apple's clang 17 has it too).


Because the container is just two arrays, we can save it by copying
the two arrays. Take a map from 64-bit keys to a fixed-size value:

```cpp
struct point {
  double x;
  double y;
};

std::flat_map<uint64_t, point> m;
m[42] = {1.0, 2.0};
m[7] = {3.0, 4.0};
m[1000] = {5.0, 6.0};

std::vector<uint64_t> keys(m.keys().begin(), m.keys().end());
std::vector<point> values(m.values().begin(), m.values().end());
// keys   = {7, 42, 1000}
// values = {{3, 4}, {1, 2}, {5, 6}}
```

The `keys` array is `8 * keys.size()` bytes and the `values` array is
`16 * values.size()` bytes: you can write them to disk with two `memcpy` or
`write` calls, as they are.

If you loaded both arrays from a network or the disk, you can then move them straight
into your `std::flat_map` like so.


```cpp
std::flat_map<uint64_t, point> back(std::sorted_unique,
                                    std::move(keys), std::move(values));
// back == m
```

With the `std::sorted_unique` tag, you promise that the keys are
sorted and distinct. In practice, if the data comes from the network or some untrusted
source, you should do some sanity testing.

Like the good old `std::map`, a flat map keeps its keys in sorted order, so you can iterate
over it in key order. But the  `std::map` is a red-black tree so you have significant storage
overhead and possibly poor memory locality.


The downside of a `std::flat_map` is that it might be slower if you need to mutate it.

Let us examine the speed.

I use random 64-bit keys mapped to 64-bit values, GCC 16.1 with `-O3
-march=native`, on an Intel Xeon Gold 6548N (Emerald Rapids), pinned to one
core. I report nanoseconds per operation

Let us start with inserting
elements one at a time, in random order, into an initially empty container.

| container       |    1K |  100K |    1M |   10M |
| --------------- | ----: | ----: | ----: | ----: |
| `std::map`      |    93 |   253 |   465 |  1118 |
| `std::flat_map` |    83 |  7149 |       |       |


Up to maybe a thousand elements or so, the `std::flat_map` is fine and maybe faster
than the `std::map`. But as the size grows, the time goes up quadratically. Thus, do not
use an `std::flat_map` if you need to insert millions of keys in random order. It is bad.

If the keys arrive in increasing order, then it is entirely different. The `std::flat_map` is
much faster.

| container       |    1K |  100K |    1M |   10M |
| --------------- | ----: | ----: | ----: | ----: |
| `std::map`      |    31 |    67 |   125 |   202 |
| `std::flat_map` |   6.8 |    12 |    15 |    19 |


You can also do bulk inserts. Given a batch of new pairs, in any order, the
map sorts the batch and merges it with its arrays in one pass, instead of
shifting the arrays once per element:

```cpp
std::vector<std::pair<uint64_t, point>> more = {{500, {7.0, 8.0}},
                                                {3, {9.0, 10.0}}};
m.insert_range(more);
// keys = {3, 7, 42, 500, 1000}
```

Constructing a map from a range of random key-value pairs works the same way,
and it is much faster with a `std::flat_map`:


| container       |    1K |  100K |    1M |   10M |
| --------------- | ----: | ----: | ----: | ----: |
| `std::map`      |    69 |   176 |   316 |   825 |
| `std::flat_map` |    23 |    62 |    72 |    87 |



Random lookups are also much faster for large maps in part because
the `std::flat_map` uses less memory.


| container       |    1K |  100K |    1M |   10M |
| --------------- | ----: | ----: | ----: | ----: |
| `std::map`      |    52 |   192 |   386 |   980 |
| `std::flat_map` |    54 |    96 |   152 |   239 |


In many practical cases, the new `std::flat_map` is a better alternative to 
the `std::map`. It is somewhat amusing considering that you are replacing a fancy
textbook data structure (red-black tree) with a trivial one.

[My source code is available](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/09/17).
