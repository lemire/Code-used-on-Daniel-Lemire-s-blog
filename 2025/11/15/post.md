# AMD vs. Intel: a Unicode benchmark


Roughly speaking, our processors come in two types, the ARM processors found in your phone and the x64 processors made by Intel and AMD. The best server processors used to be made by Intel. Increasingly, Intel is struggling to keep up. 

Recently, Amazon has made available the latest AMD microarchitecture (Zen 5). Specifically, if you start an `r8a` instance, you get an AMD EPYC 9R45 processor. The Intel counterpart (`r8i`) has an Intel Xeon 6975P-C processor. This Intel processor is from the Granite Rapids family (2024). 

 Michael Larabel at Phoronix has a couple of articles on the new AMD processors. One of them is entitled [AMD EPYC 9005 Brings Incredible Performance](https://www.phoronix.com/review/ec2-m8a-amd-epyc-turin). The article is well worth reading. He finds that, compared with the prior AMD processor (with a Zen 4 microarchitecture), the AMD EPYC 9R4 is 1.6 times faster. In a [second article](https://www.phoronix.com/review/aws-m8a-m8g-m8i-benchmarks), Michael compares the AMD processor with the corresponding Intel processor. He finds that the AMD processor is 1.6 times faster than the Intel processor.

I decided to take them out for a spin. I happened to be working on a new release of the [simdutf library](https://github.com/simdutf/simdutf). The simdutf library allows fast transcoding between UTF-8, UTF-16, and UTF-32 encodings, among other features. It is used by major browsers and JavaScript runtimes like Node.js or Bun. A common operation that matters is the conversion from UTF-16 to UTF-8. Internally, JavaScript relies on UTF-16, thus most characters use 2 bytes, whereas the Internet defaults on UTF-8 where characters can use between 1 and 4 bytes.


UTF-16 is a variable-length Unicode encoding that represents most common characters using a single 16-bit code unit (values from `0x0000` to `0xd7ff` and `0xe000` to `0xffff`), but extends to the full Unicode range beyond `U+FFFF` by using surrogate pairs: a high surrogate (`0xd800` to `0xdbff`) followed by a low surrogate (`0xdc00` to `0xdfff`), which together encode a single supplementary character which maps to four UTF-8 bytes. Thus we may consider that each element of a surrogate pair counts for two bytes in UTF-8. A non-surrogate code unit in the range `0x0000` to `0x007f` (ASCII) becomes one byte, `0x0080` to `0x07ff` becomes two bytes, and `0x0800` to `0xffff` (excluding surrogates) becomes three bytes.

My benchmark code first determines how much output memory is required and then it does the transcoding.

```cpp
size_t utf8_length = simdutf::utf8_length_from_utf16(str.data(), str.size());
if(buffer.size() < utf8_length) {
  buffer.resize(utf8_length);
}
simdutf::convert_utf16_to_utf8(str.data(), str.size(), buffer.data());
```
The transcoding code on a recent processor is not trivial ([Clausecker and Lemire, 2023](https://arxiv.org/pdf/2212.05098)). However, the computation of the UTF-8 length from the UTF-16 data is a bit simpler.


These Intel and AMD processors support AVX-512 instructions: they are instructions that can operate on up to 64-byte registers compared to the 64-bit registers we normally use. It is an instance of SIMD: single instruction on multiple data. With AVX-512, you can load and process 32 UTF-16 units at once. Our main routine looks as follows.

```cpp
__m512i input =
_mm512_loadu_si512(in);
__mmask32 is_surrogate = _mm512_cmpeq_epi16_mask(
_mm512_and_si512(input, _mm512_set1_epi16(0xf800)),
_mm512_set1_epi16(0xd800));
__mmask32 c0 =
_mm512_test_epi16_mask(input, _mm512_set1_epi16(0xff80));
__mmask32 c1 =
_mm512_test_epi16_mask(input, _mm512_set1_epi16(0xf800));
count += count_ones32(c0);
count += count_ones32(c1);
count -= count_ones32(is_surrogate);
```

The code processes a 512-bit vector of UTF-16 code units loaded from memory using `_mm512_loadu_si512`. It then identifies surrogate code units by first applying a bitwise AND (`_mm512_and_si512`) to mask each code unit with `0xf800`, retaining only the top five bits, and comparing the result (`_mm512_cmpeq_epi16_mask`) against `0xd800`; this produces a 32-bit mask where bits are set for any code unit in the surrogate range (`0xd800` to `0xdfff`), indicating potential UTF-16 surrogate pairs that should not contribute extra length in UTF-8. Next, we check (`_mm512_test_epi16_mask`) each code unit against a mask of `0xff80` using a bitwise test, setting bits in `c0` for any code unit that is not ASCII. Similarly, another `_mm512_test_epi16_mask` function against `0xf800` sets bits in `c1` for code units that require 3 bytes in UTF-8 (except for surrogate pairs). Finally, the code accumulates into a counter the number of set bits in `c0` and `c1`, then subtracts the popcount of the surrogate mask. Overall, we can process about 32 UTF-16 units using a dozen instructions. (Credit to Wojciech Muła for the insightful design and also to Yagiz Nizipli for helping me with related optimizations.)

A large Amazon instance with the AMD processor was 0.13892$/hour, while the Intel processor was 0.15976$/hour. I initiated both instances with Amazon Linux. I then ran the following commands in the shell.

```shell
sudo yum install cmake git gcc
sudo dnf install gcc14 gcc14-c++
git clone https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog.git
cd Code-used-on-Daniel-Lemire-s-blog/2025/11/15
CXX=gcc14-g++ cmake -B build
cmake --build build
./build/benchmark
```

I get the following results.

| Processor | GB/s | GHz  | Instructions/Byte | Instructions/Cycle |
|-----------|-------------------|-----------------|-------------|-------------------|
| AMD | 11 | 4.5  | 1.7 | 4.0 |
| Intel | 6 | 3.9 | 1.7 | 2.6 |

The benchmark results show that the AMD processor delivers nearly double the throughput of the Intel processor in UTF-16 to UTF-8 transcoding (10.53 GB/s versus 5.96 GB/s), aided in part by its higher operating frequency. Both systems require the same 1.71 instructions per byte, but AMD achieves markedly higher instructions per cycle (3.98 i/c versus 2.64 i/c), demonstrating superior execution efficiency within the AVX-512 pipeline. One of the reasons has to do with the number of execution units. The AMD processor has four units capable of doing compute on 512-bit registers while
the Intel is typically limited to only two such execution units.

My benchmark is more narrow than Larabel's and they help show that AMD has a large advantage over Intel when using AVX-512 instructions. It is especially remarkable given that Intel invented AVX-512 and AMD was late in supporting it.
One might say that AMD is beating Intel at its own game.

[My benchmarking code is available.](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2025/11/15)

*Further reading*: Robert Clausecker, Daniel Lemire, [Transcoding Unicode Characters with AVX-512 Instructions](https://arxiv.org/pdf/2212.05098), Software: Practice and Experience 53 (12), 2023.