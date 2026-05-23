# Parsing IPv6 addresses faster with AVX-512


Every machine connected to the Internet has an address called an IP address. Originally, these addresses were 32-bit
integers (IPv4), giving a theoretical maximum of about four billion distinct addresses. We are all familiar with these addresses (e.g., `192.168.0.0`). There was a big fuss about how we would run out of addresses. It never happened because we don't actually need every device to have its own unique address. Your home router needs an address, but every device in your home does not need a worldwide unique address.

Nevertheless, the range was extended to cover 128 bits (IPv6). An IPv6 address is conventionally written as eight groups of four hexadecimal digits separated bycolons. For example:

```
2001:0db8:85a3:0000:0000:8a2e:0370:7334
```

Because addresses often contain runs of zeroes, the format allows two shortcuts:

- Leading zeroes within a group may be omitted: `2001:db8:85a3:0:0:8a2e:370:7334`.
- A single run of all-zero groups may be replaced by `::`: `2001:db8:85a3::8a2e:370:7334`.

The double-colon trick can appear only once in an address, and can match one or more zero groups.
Hence `::1` is the loopback address (all zeros except the last group), and `::` is the
unspecified address (all zeros).

IPv6 also accepts an embedded IPv4 address in the last 32 bits, written in the usual
dotted-decimal form. This is mostly used for IPv4-mapped IPv6 addresses such as
`::ffff:192.168.1.1`. The longest possible textual form is 45 characters:

```
0000:0000:0000:0000:0000:ffff:255.255.255.255
```

So a parser must accept hexadecimal groups, the compressed form, and an optional IPv4 tail. It is
more involved than parsing IPv4.

The standard C function for the job is `inet_pton`, available on essentially every system.

Can we do better?

[A few years ago, I showed that you could parse IPv4 addresses really fast](https://lemire.me/blog/2023/06/08/parsing-ip-addresses-crazily-fast/). Can we do the same with IPv6?

The trick is to use data parallelism: we invoke the so-called SIMD instructions that all our processors support. These instructions can process potentially dozens of bytes at once.

Shreesh Adiga gave it a try with AVX-512, the powerful instruction set supported by recent Intel server processors and all new AMD CPUs.
The idea is to load the entire string into a 512-bit register, find the colons with a single
comparison, compute the spacing between them to drive a byte-level expand, translate hex digits
via a permute, and finish with a multiply-accumulate that combines the hex digits into bytes.
Almost the whole parser is branch-free, meaning that there are few `if` clauses.

I put together a small benchmark that generates random IPv6 addresses with `inet_ntop` (so the
addresses are written in their canonical, compressed form) and parses each one with both
`inet_pton` and the AVX-512 routine. The benchmark runs on an Intel Xeon Gold 6548N CPU @ 2.8 GHz
(Emerald Rapids) with GCC, compiled with `-march=native -O3`.

| function    |     ns/addr |    speed (Mv/s) |   cycles/addr |   instr/addr |   i/c |
|-------------|------------:|----------------:|--------------:|-------------:|------:|
| `inet_pton` |       175.3 |             5.7 |           611 |          954 |  1.56 |
| AVX-512     |        14.0 |            71.3 |            49 |          120 |  2.45 |

The AVX-512 routine is about **12 times faster** than `inet_pton`, parsing more than 70 million
addresses per second on a single core. It uses eight times fewer  instructions, and runs them at
a higher throughput (2.45 instructions per cycle versus 1.56).

The source code used for this benchmark is available
[on my blog repository](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/2026/05/22).
