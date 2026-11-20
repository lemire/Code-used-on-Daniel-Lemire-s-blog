# Decoding base64 email attachments: a speed ranking of programming languages

Email attachments are base64. So are cryptographic PEM certificates. In both cases the text is broken into lines, 76 characters for email and 64 for PEM.

Skipping line endings sounds trivial. Yet many fast decoders only work on uninterrupted base64. As soon as they meet a line break, they fall back to a slow path.

How fast can we decode MIME base64 in popular programming languages? I wrote a small benchmark. I generate one megabyte of random bytes, encode it as base64 with 76-character lines ending in CRLF (1.37 MB of text) and decode it again with the standard or most popular function of each language. I check that each decoder returns the right bytes.

I use an Intel Xeon Gold 6548N (Emerald Rapids) with Linux, pinned to one core. I report speeds in gigabytes of input per second (GB/s). Each number is the median of three runs, and each run keeps the best of ten timed batches. I use the latest version of each tool.

| rank | system | function | GB/s |
|---:|---|---|---:|
| 1 | C++ (GCC 16) with simdutf 9.4 | `simdutf::base64_to_binary` | 18.4 |
| 2 | PHP 8.7-dev with my pull request | `base64_decode` | 17.1 |
| 3 | Python 3.15 with fastpybase64 | `fastpybase64.b64decode` | 16.4 |
| 4 | Bun 1.4.4 | `Buffer.from(s, 'base64')` | 14.2 |
| 5 | Node.js 26.13 | `Buffer.from(s, 'base64')` | 13.3 |
| 6 | Deno 2.9.8 | `Uint8Array.fromBase64(s)` | 8.4 |
| 7 | .NET 11 | `Base64.DecodeFromUtf8` | 4.9 |
| 8 | Go 1.27.4 | `base64.StdEncoding.Decode` | 1.6 |
| 9 | Java 27 | `Base64.getMimeDecoder().decode` | 1.2 |
| 10 | Zig 0.17.1 | `std.base64` decoder ignoring `\r\n` | 0.85 |
| 11 | PHP 8.5.12 | `base64_decode` | 0.61 |

![Bar chart of MIME base64 decoding speed on an Intel Xeon Gold 6548N. The six systems that use simdutf decode at 8.4 to 18.4 GB/s; the others decode at 0.61 to 4.9 GB/s.](speed.webp)

There is a factor of thirty between the top and the bottom of the table.

The top six systems all rely on the simdutf library. The simdutf library removes the line endings inside its vector loop. A clean block of 64 characters is decoded directly. A block that contains line endings is compacted first, and decoding proceeds from a small buffer. The line endings never send the decoder to a slow path. See Lemire (2027) for the details.

PHP is the most dramatic case. With the current release, PHP is last at 0.61 GB/s, even though it decodes base64 without line endings at more than 20 GB/s on this machine. My [pull request](https://github.com/php/php-src/pull/25476) ports the simdutf decoders to PHP. It makes MIME decoding 28 times faster, and PHP moves to second place.

[My source code is available](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/11/javashell/mimebench).

## References

Lemire, D. (2027). [Fast forgiving base64 decoding with SIMD instructions](https://doi.org/10.1145/3859286). ACM Transactions on Architecture and Code Optimization, 24(1). ([arXiv](https://arxiv.org/abs/2610.17406))

Muła, W., & Lemire, D. (2020). [Base64 encoding and decoding at almost the speed of a memory copy](https://doi.org/10.1002/spe.2777). Software: Practice and Experience, 50(2), 89–97. ([arXiv](https://arxiv.org/abs/1910.05109))
