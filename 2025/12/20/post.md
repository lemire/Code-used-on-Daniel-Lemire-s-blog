# Performance trick : optimistic vs pessimistic checks

Strings in programming are often represented as arrays of 8-bit words. The string is ASCII if and only if all 8-bit words have their most significant bit unset. In other words, the byte values must be no larger than 127 (or `0x7F` in hexadecimal). 

A decent C function to check that the string is ASCII is as follows.

```C
bool is_ascii_pessimistic(const char *data, size_t length) {
  for (size_t i = 0; i < length; i++) {
    if (static_cast<unsigned char>(data[i]) > 0x7F) {
      return false;
    }
  }
  return true;
}
```

We go over each character, we compare it with `0x7F` and continue if the value is no larger than `0x7F`. If you have scanned the entire string and all tests have passed, you know that your string is ASCII.

Notice how I called this function `pessimistic`. What do I mean? I mean that it expects, in some sense, that it will find some non-ASCII character. If so, the best option is to immediately return and not scan the whole string.

What if you expect the string to almost always be ASCII? An alternative then is to effectively do a bitwise OR reduction of the string: you OR all characters together and you check just once that the result is bounded by `0x7F`. If any character has its most significant bit set, then the bitwise OR of all characters will also have its most significant bit set. So you might write your function as follows.

```C
bool is_ascii_optimistic(const char *data, size_t length) {
  unsigned char result = 0;
  for (size_t i = 0; i < length; i++) {
    result |= static_cast<unsigned char>(data[i]);
  }
  return result <= 0x7F;
}
```

If you have strings that are all pure ASCII, which function will be fastest? Maybe surprisingly,
the optimistic might be several times faster. [I wrote a benchmark](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2025/12/20/optimistic) and ran it with GCC 15 on an Intel Ice Lake processor. I get the following results.

| function |  speed |
|----------|--------|
| pessimistic | 1.8 GB/s |
| optimistic | 13 GB/s |

Why is the optimistic faster? Mostly because the compiler is better able to optimize it. Among other possibilities, it can use autovectorization to automatically use data-level parallelization (e.g., SIMD instructions).

Which function is best depends on your use case. 



What if you would prefer a pessimistic function, that is, one that returns early when non-ASCII characters are encountered, but you still want high speed? Then you can use [a dedicated library like simdutf](https://github.com/simdutf/simdutf) where we have hand-coded the logic. In simdutf,
the pessimistic function is called `validate_ascii_with_errors`. Your results will vary but I got that it has the same speed as optimistic function.


| function |  speed |
|----------|--------|
| pessimistic | 1.8 GB/s |
| pessimistic (simdutf) | 14 GB/s |
| optimistic | 13 GB/s |

So it is possible to combine the benefits of pessimism and optimism although it requires a bit of care.