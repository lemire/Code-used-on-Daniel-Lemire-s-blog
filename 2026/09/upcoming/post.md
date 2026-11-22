# Python sets and dictionaries can have quadratic-time performance


In Python, the `dict` data structure is the conventional key-value structure. E.g., you might store a list of names as keys and have their phone numbers as values. Valentin Ignatev wrote this amusing post on X:

![](xpost.webp)

It is indeed widely believed that, in the strict sense, the `dict` data structure and its companion, the set data structure, are O(1), meaning that as you increase the size of the data structure, the time to insert or query a key remains constant.

Let us examine the claim.

A hash function is a function from objects (like strings, integers, etc.)
to integer values. We typically expect hash functions to be random-like,
although they should always map the same object to the same integer within
the current program execution.
From hash functions, we construct hash tables:

1. Create an array of buckets.
2. Given an object, apply the hash function to map it to a bucket.
3. Store the object in the bucket. When the bucket is already occupied, use some other trick (such as using a nearby bucket).


If everything goes well, access and insertion in a hash table take nearly constant time, meaning that the time they take is independent of the size of the hash table.

This can be almost true in many instances.  However, it is not formally true. There are many reasons why it is false. For example, if your data structure grows, it might be necessary to reallocate, which will typically take time proportional to the size of the data structure. But we also have the issue of collisions. A collision is what happens when two objects have the same hash value. When we use hash tables, we assume that collisions are uncommon. But it is not difficult to create many of them by picking our objects carefully.


In Python, `set` and `dict` are hash tables. I can 'easily' make my version of Python crumble:

```python
M = (1 << 61) - 1
values = [i * M for i in range(1, n + 1)]
s = set(values)                       # insertions
count = sum(v in s for v in values)   # checks
```

If the insertions and the checks are constant-time operations, then the whole construction and the entire check should take linear time. I ran this on an Apple M4 Max with Python 3.14, reporting the median of three
runs. 

| n | time|
| ---: |  ---: |
| 1000 | 4.8 ms |
| 2000 |15.5 ms |
| 4000 | 65.5 ms |
| 8000 | 257 ms |
| 16000 | 1072 ms |

The time roughly quadruples each time *n* doubles. That is quadratic time, not linear time. The membership checks behave the same way: 1066 ms at *n* = 16000. At a hundred thousand elements, building the set takes 45 seconds.

But could we create a hash table that would be truly constant-time? No. As the size of your data structure grows, it requires progressively slower memory. If you have a small hash table, it can reside in the CPU cache and be fast. Once it reaches megabytes in size, the data structure tends to live in RAM, which is much slower. And then, eventually, you have to store it on disk, which is even slower. And so forth.

To put it differently, saying that a hash table is O(1) or constant time is a model. It can be true, maybe even often, but it is not reality. Models are great teaching tools: they present a simplified model that you can quickly learn. But models can also introduce biases in how we think.

For example, even though you have read my paragraph that says that the dict data structure gets slower, you may not believe it. You may also believe that it is typically going to be the fastest approach you can use.

Let us consider another practical case. Suppose that you have a large map from
strings to integers, that you build once and then only query. That is a common
situation: a dictionary of words to identifiers, a lookup table of country codes,
a table of feature names.

The [fastconstmap](https://pypi.org/project/fastconstmap/) library builds an
immutable map from a `dict[str, int]`. It is suitable when your keys are known in advance.

I build a map from a million random sixteen-character strings to integers, and
then look up every key in a shuffled order. With a `dict`, I write the obvious
loop:

```python
total = 0
for k in probes:
    total += d[k]
```

With fastconstmap, I ask for all the keys at once, writing the values into a
buffer that I own, so that no Python object is allocated per key:

```python
out = array("Q", bytes(8 * n))
cm.get_many_into(probes, out)
```

I am being generous to the `dict`. I reuse the same string objects for the
lookups, and a Python string caches its hash value the first time it is
computed. So the `dict` does not pay for hashing at all, while fastconstmap
hashes every key every time. Here are the results, in nanoseconds per key.

| n | dict | get_many_into |
| ---: | ---: | ---: |
| 1000 | 21.8 | 4.3 |
| 10000 | 31.9 | 4.8 |
| 100000 | 48.1 | 5.2 |
| 1000000 | 201.9 | 11.8 |

The `dict` is not constant time. It goes from 22 ns to 202 ns per key as the map
grows, a factor of nine, and it is not because the algorithm changed or because of collisions. It is
because a million keys, their string objects, and their integer objects occupy
about 116 bytes per key, so the lookups miss in the cache. The fastconstmap version
needs 9 bytes per key: it stays in the cache much longer. Pay attention to how the numbers scale: the dict becomes 10 times slower as the size grows.

The lesson is always the same. Some models are useful but none of them is reality. Be mindful of cognitive biases.


[The code is available.](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/09/upcoming)
