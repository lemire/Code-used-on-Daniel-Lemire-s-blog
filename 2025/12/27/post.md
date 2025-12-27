# Parsing IP addresses quickly (portably, without SIMD magic)


Most programmers are familiar with IP addresses. They take the form
of four numbers between 0 and 255 separated by dots: `192.168.0.1`. 
In some sense, it is a convoluted way to represent a 32-bit integer.
The *modern* version of an IP address is IPv6 which is usually surrounded
by square brackets. It is less common in my experience.

Using fancy techniques, [you can parse IP addresses with as little as 50 instructions](https://lemire.me/blog/2023/06/08/parsing-ip-addresses-crazily-fast/).
It is a bit complicated and not necessarily portable.

What if you want high speed without too much work or a specialized library? You can try 
to roll your own. But since I am civilized programmer, I just asked my favorite AI to write it for me.

```cpp
// Parse an IPv4 address starting at 'p'.
// p : start pointer, pend: end of the string
std::expected<uint32_t, parse_error> parse_manual(const char *p, const char *pend) {
uint32_t ip = 0;
    int octets = 0;
    while (p < pend && octets < 4) {
        uint32_t val = 0;
        const char *start = p;
        while (p < pend && *p >= '0' && *p <= '9') {
            val = val * 10 + (*p - '0');
            if (val > 255) {
                return std::unexpected(parse_error::invalid_format);
            }
            p++;
        }
        if (p == start || (p - start > 1 && *start == '0')) {
            return std::unexpected(parse_error::invalid_format);
        }
        ip = (ip << 8) | val;
        octets++;

        if (octets < 4) {
            if (p == pend || *p != '.') {
                return std::unexpected(parse_error::invalid_format);
            }
            p++; // Skip dot
        }
    }
    if (octets == 4 && p == pend) {
        return ip;
    } else {
        return std::unexpected(parse_error::invalid_format);
    }
}
```

It was immediately clear to me that this function was not as fast as it could be.
I then asked the AI to improve the result by using the fact that each number is made of between
one and three digits. I got the following reasonable function.

```cpp
std::expected<uint32_t, parse_error> parse_manual_unrolled(const char *p, const char *pend) {
    uint32_t ip = 0;
    int octets = 0;
    while (p < pend && octets < 4) {
        uint32_t val = 0;
        if (p < pend && *p >= '0' && *p <= '9') {
            val = (*p++ - '0');
            if (p < pend && *p >= '0' && *p <= '9') {
                if (val == 0) { return std::unexpected(parse_error::invalid_format); }
                val = val * 10 + (*p++ - '0');
                if (p < pend && *p >= '0' && *p <= '9') {
                    val = val * 10 + (*p++ - '0');
                    if (val > 255) { return std::unexpected(parse_error::invalid_format); }
                }
            }
        } else {
            return std::unexpected(parse_error::invalid_format);
        }
        ip = (ip << 8) | val;
        octets++;
        if (octets < 4) {
            if (p == pend || *p != '.') {
              return std::unexpected(parse_error::invalid_format);
            }
            p++; // Skip the dot
        }
    }
    if (octets == 4 && p == pend) {
        return ip;
    } else {
        return std::unexpected(parse_error::invalid_format);
    }
}
```

In C++, we have standard functions to parse numbers (`std::from_chars`) which can significantly simplify the code.

```cpp
std::expected<uint32_t, parse_error> parse_ip(const char *p, const char *pend) {
  const char *current = p;
  uint32_t ip = 0;
  for (int i = 0; i < 4; ++i) {
    uint8_t value;
    auto r = std::from_chars(current, pend, value);
    if (r.ec != std::errc()) {
      return std::unexpected(parse_error::invalid_format);
    }
    current = r.ptr;
    ip = (ip << 8) | value;
    if (i < 3) {
      if (current == pend || *current++ != '.') {
        return std::unexpected(parse_error::invalid_format);
      }
    }
  }
  return ip;
}
```

You can also use the [fast_float library](https://github.com/fastfloat/fast_float)  as a substitute for `std::from_chars`. The latest version of fast_float
has faster 8-bit integer parsing thanks to Shikhar Soni (with a fix by Pavel Novikov).

[I wrote a benchmark for this problem](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2025/12/27/benchmark_ip_parsing). Let us first consider the results using an Apple M4 processors (4.5 GHz)
with LLVM 17.

| function  | instructions/ip | ns/ip |
|-----------|-----------------|-------|
| manual    |     312         |  14   |
| manual (unrolled)    |     244         |  8.9   |
| from_chars    |     381         |  14   |
| fast_float    |     181         |  7.2   |

Let us try with GCC 12 and an Intel Ice Lake processor (3.2 GHz) using GCC 12.


| function  | instructions/ip | ns/ip |
|-----------|-----------------|-------|
| manual    |     310         |  35   |
| manual (unrolled)    |     244         |  32  |
| from_chars    |     220         |  29   |
| fast_float    |     211         |  18   |


And finally, let us try with a Chinese Longsoon 3A6000 processor (2.5 GHz) using LLVM 21.


| function  | instructions/ip | ns/ip |
|-----------|-----------------|-------|
| manual    |      287         |  39   |
| manual (unrolled)    |     215         |  32  |
| from_chars    |     191         |  39   |
| fast_float    |     193         |  27   |



The optimization work on the fast_float library paid off. The difference is especially striking on the x64 processor. Yet observe how there are no gains on the Longsoon processor: this illustrates that software performance is often empirical. It helps to have actual hardware and run actual code.


What is also interesting in my little experiment is that I was able to get the AI to produce faster code with relatively little effort on my part. I did have to 'guide' the AI. Does that mean that I can retire? Not yet. But I am happy that I can more quickly get good reference baselines, which allows me to better focus my work where it matters.




*Reference*: [The fast_float C++ library](https://github.com/fastfloat/fast_float) is a fast number parsing library part of GCC and major web browsers.
