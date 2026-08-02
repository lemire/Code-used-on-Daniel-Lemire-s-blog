# How fast is std::hive?

C++26 adds a new container to the standard library: `std::hive`. It is meant to
occupy the ground between `std::vector` and `std::list`. Like a vector, it keeps
its elements in contiguous blocks of memory, so scanning it does not require you
to chase a pointer for every element. Like a list, it never moves an element
once it has been inserted: your pointers, references and iterators stay valid,
and you may erase any element in constant time without disturbing the others.

Internally, a hive is a linked list of blocks. Each block carries a *skipfield*:
a small integer per slot that tells the iterator how many erased slots to jump
over.

No standard library ships `std::hive` yet to my knowledge.  Fortunately there is an implementation
([plf::hive](https://github.com/mattreecebentley/plf_hive) by Matt Bentley) as a
single header file that you can use today.

I use elements of type `uint64_t`, GCC 16.1 with `-O3 -march=native`, on an
Intel Xeon Gold 6548N (Emerald Rapids), pinned to one core. Numbers are
nanoseconds per element, along with the cycles and instructions retired per
element.


We start from an empty container and append a million values. The container is
then destroyed. 

| container                  | ns/element | instructions/element |
| -------------------------- | ---------: | -------------------: |
| `std::vector` (`reserve`)  |       0.29 |                  8.0 |
| `std::vector`              |       0.81 |                  8.0 |
| `std::hive`                |       1.57 |                 16.2 |
| `std::hive` (`reserve`)    |       1.76 |                 17.0 |
| `std::list`                |      14.22 |                220.0 |

A `std::list` needs one allocation per element, and glibc's malloc and free
together cost over 200 instructions per element. It is an order of magnitude
behind everyone else. That is not news.

The interesting comparison is vector against hive. A hive is about twice the
cost of a vector, and it needs twice the instructions. This is the price of the
skipfield: every insertion writes an element *and* a skipfield entry, and
maintains the block bookkeeping. Note that calling `reserve` on a hive does not
help in my experiments.

Next we iterate over the the container and sum the values.

| container     | ns/element | cycles/element | instructions/element |
| ------------- | ---------: | -------------: | -------------------: |
| `std::vector` |       0.22 |           0.78 |                  1.0 |
| `std::list`   |       1.51 |           5.27 |                  4.0 |
| `std::hive`   |       1.77 |           6.18 |                  9.0 |

A hive iterates no faster than a linked list here, slightly
slower, in fact, and about eight times slower than a vector.

The vector loop retires one instruction per element
and finishes in 0.78 cycles: the processor is executing several elements at
once. This is possible because the `std::vector` implementation benefits
from autovectorization: the compiler recognizes that it can load several
words at once in wide (SIMD). Further, it does not have to check the bitfield
like the  `std::hive` data structure.

We can check this. Walk the same container with two independent iterators, one
starting halfway in, and count the cost per element visited:

| container     | one traversal | two interleaved traversals |
| ------------- | ------------: | -------------------------: |
| `std::vector` |   0.78 cycles |                0.79 cycles |
| `std::list`   |   5.27 cycles |                3.02 cycles |
| `std::hive`   |   6.18 cycles |                3.10 cycles |

The vector does not care: it was already throughput-bound. The hive and the list
get nearly twice as fast per element, because two independent chains can be in
flight at once. Hive iteration is latency-bound, exactly like list iteration. It
merely has better locality.

That locality does show up when the data gets big. At ten million elements the
list falls apart while the hive holds steady:

| container     | 100K | 1M   | 10M  |
| ------------- | ---: | ---: | ---: |
| `std::vector` | 0.08 | 0.22 | 0.32 |
| `std::list`   | 1.48 | 1.51 | 3.51 |
| `std::hive`   | 1.76 | 1.77 | 1.96 |


Erasing is what a hive is for, so it would be unfair not to look. I erase half
the elements at scattered positions:

| container     | ns per original element |
| ------------- | ----------------------: |
| `std::hive`   |                     2.1 |
| `std::vector` |                     3.0 |
| `std::list`   |                    77.4 |

The hive wins, but by less than you might expect, and at ten million elements
the ordering reverses (1.3 ns for the vector against 2.5 for the hive).
`std::remove_if` is a single streaming pass, and streaming passes are cheap. Of
course the vector moved every surviving element and invalidated every pointer
into it, which is precisely what a hive promises not to do. 

Memory, measured by asking glibc how many bytes it has handed out, per live
element:

| container     | after building | after `shrink_to_fit` |
| ------------- | -------------: | --------------------: |
| `std::vector` |            8.4 |                   8.0 |
| `std::hive`   |            9.4 |                   9.4 |
| `std::list`   |           32.0 |                      |

A hive costs about a byte per element over a vector, for a payload of eight
bytes, when the vector is packed tight. A list costs more due to the overhead of the linked list.

A vector built by `push_back` has a capacity that typically exceeds its
size. Thus even if you have 8-byte entries, you will use, on average,
more than 8 bytes per entry even for large vectors. You can recover the
excess capacity with the `shrink_to_fit` method.

What should we conclude?

The `std::hive` data structure is not a faster vector. But it is a much better `std::list`. It gives you
the same guarantees that make people reach for a list, stable references,
cheap erasure anywhere, while using less memory.

[My source code is available](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/08/02).
