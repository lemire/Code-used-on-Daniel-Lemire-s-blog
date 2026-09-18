# SVE2 helps simdjson on AWS Graviton processors

ARM processors, like those in your phone, have instructions capable of processing several elements at once (SIMD).
These instructions are called NEON. But many newer processors have a different SIMD extension called SVE. The latest ARM processors have SVE2. Unfortunately, Apple has not yet adopted SVE, but SVE processors are available in the cloud.

In April, I wrote that the SVE2 `match` instruction might be [the fastest way to match characters on ARM processors](https://lemire.me/blog/2026/04/19/the-fastest-way-to-match-characters-on-arm-processors/). At the time, my benchmark was a toy. The question was whether the idea survives contact with a real parser. Madhurendra Purbay, an engineer at ARM, answered the question with a [pull request to the simdjson library](https://github.com/simdjson/simdjson/pull/2863). Let me go through what it does and what it buys us.

## The problem

The simdjson library includes a fast JSON parser. JSON is a ubiquitous data format online; everyone uses it. It is made of strings, numbers, arrays (`[1,2,3]`) and *objects*. An object is a key-value map where keys are strings, written as `{"key1": 1, "key2": 2}`. You can combine arrays and objects (e.g., an object can be in an array). 

When the simdjson library indexes a JSON document, it first computes, for each block of 64 bytes, a few 64-bit masks. One of them marks the JSON *structural* characters (`,`, `:`, `[`, `]`, `{`, `}`). From these masks and a few others, we derive the positions of all the JSON tokens.

The ARM NEON version of the classifier, which I designed with Geoff Langdale years ago, uses a table lookup (`tbl`). Take the byte, add 3, keep the high nibble, and look it up in a 16-byte table that returns the one structural character with that nibble (or `0xff`). If the looked-up byte is equal to the input, the input is structural. With the `vaddq`/`vshrq`/`vqtbl1q`/`vceqq` NEON intrinsics, it is four instructions per 16 bytes:

```cpp
const uint8x16_t op_table = simd8<uint8_t>(
  0xff, 0, ',', ':', 0, '[', ']', '{', '}', 0, 0, 0, 0, 0, 0, 0
);
const uint8x16_t match_op_0 = vceqq_u8(
  vqtbl1q_u8(op_table, vshrq_n_u8(vaddq_u8(d0_0, vdupq_n_u8(3)), 4)),
  d0_0);
```

We get a vector of 16 bytes that are either `0x00` or `0xff`. To turn four such vectors into one 64-bit mask, we AND each byte with a bit weight (1, 2, 4, ..., 128) and sum adjacent bytes three times with `addp`. That is another eight instructions or so per 64-byte block, and it is shared with the white-space mask.


SVE2 has an instruction, `match`, that takes a vector of bytes and a second vector that acts as a small set: it produces a predicate (a mask) with a bit set at each position where the input byte belongs to the set. Because it works within 128-bit segments, the set is at most 16 bytes, which is plenty for our six structural characters. NEON has nothing like it; on x64, the closest thing is the SSE4.2 string-comparison instructions (`pcmpistrm`), which are slow.

In C++, using intrinsics, the code might look as follows:

```cpp
// input is a set of 16 ASCII bytes we want to classify
// the whole thing compiles to little more than the match instruction
svbool_t match_operators_sve2(uint8x16_t input) {
  // The characters we care about. We use `0xff` as a
  // filler (it is an impossible byte value within a JSON document)
  const uint8x16_t operators = {
    0xff, ',', ':', '[', ']', '{', '}', 0xff,
    ',', ':', '[', ']', '{', '}', ',', ':'
  };
  // pg is a mask over the first 16 values
  const svbool_t pg = svptrue_pat_b8(SV_VL16);
  // 'move' the NEON register to SVE
  const svuint8_t data = svset_neonq_u8(svundef_u8(), input);
  // 'move' the table to SVE
  const svuint8_t table = svset_neonq_u8(svundef_u8(), operators);
  // call the match instruction
  return svmatch_u8(pg, data, table);
}
```

This function *classifies* 16 ASCII bytes with maybe just one instruction (`match`).

We use `svset_neonq_u8`, which is part of the NEON-SVE bridge. It allows you to mix and match NEON and SVE. The `uint8x16_t` type is a NEON type (16 8-bit integers). The type `svuint8_t` is an SVE type (a vector of 8-bit integers). As a convention, the first 16 bytes of SVE types are shared with NEON. Thus I expect that `svuint8_t data = svset_neonq_u8(svundef_u8(), input)` might compile to nothing.

The catch, as I explained in April, is that a predicate lives in a predicate register. My function returns `svbool_t`.
SVE gives you no cheap way to move it to a general-purpose register: the architecture does not want to assume that a mask fits in 16 bits, since the registers might be wider. So we materialize the predicate as bytes instead, with a predicated select (`svsel`). The whole thing is a bit complicated (see [Lemire (2025)](https://doi.org/10.1002/spe.3420) for an explanation of the trick).


```cpp
// We have four masks, p0, p1, p2, p3
// and we want to convert them each to a 16-bit value and then combine them to
// form a 64-bit mask.
uint64_t operator_predicates_to_bytes(
    svbool_t p0, svbool_t p1, svbool_t p2, svbool_t p3) {
  uint8x16_t bit_mask = {0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80,
                         0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
  // map the NEON register bit_mask to an SVE register
  const svuint8_t weights = svset_neonq_u8(svundef_u8(), bit_mask);
  // create a zero register
  const svuint8_t zero = svdup_n_u8(0);
  // where p0 is set, put the value from bit_mask, otherwise zero
  // The `svget_neonq_u8` function is part of the NEON-SVE bridge.
  const uint8x16_t b0 = svget_neonq_u8(svsel_u8(p0, weights, zero));
  const uint8x16_t b1 = svget_neonq_u8(svsel_u8(p1, weights, zero));
  const uint8x16_t b2 = svget_neonq_u8(svsel_u8(p2, weights, zero));
  const uint8x16_t b3 = svget_neonq_u8(svsel_u8(p3, weights, zero));
  const uint8x16_t sum = vpaddq_u8(vpaddq_u8(b0, b1), vpaddq_u8(b2, b3));
  return vgetq_lane_u64(vreinterpretq_u64_u8(vpaddq_u8(sum, sum)), 0);
}
```

Does it help?

I benchmarked the `match` classifier against the NEON classifier, using the `parse` benchmark that comes with simdjson, over the 22 JSON files that we use as our standard corpus (about 24 MB in total). The benchmark parses each file 300 times and keeps the best time. I ran each binary three times, interleaved with its counterpart, pinned to one core, and I kept the best. The run-to-run variation is under 1%. I used GCC 15 and LLVM clang 21 on Ubuntu 26.04, with `-mcpu=native`.

The `match` instruction is part of SVE2, not the original SVE. Among the AWS Graviton processors, the Graviton 3 (Neoverse V1) has SVE but not SVE2: it cannot run this code. So I used the two processors that can:

- Graviton 4 (Neoverse V2) on a `c8g.2xlarge` instance,
- Graviton 5 (Neoverse V3) on a `c9g.2xlarge` instance.

Both have 128-bit SVE registers.

Here is the gain in the indexing stage (stage 1), file by file, as the ratio of the throughput with `match` to the throughput with NEON. A value of 1.10 means 10% faster.

| file | Graviton 4, GCC | Graviton 4, clang | Graviton 5, GCC | Graviton 5, clang |
| --- | ---: | ---: | ---: | ---: |
| apache_builds | 1.09 | 1.14 | 1.02 | 1.05 |
| canada | 1.07 | 1.06 | 0.97 | 1.03 |
| citm_catalog | 1.02 | 1.14 | 1.05 | 1.06 |
| github_events | 1.08 | 1.14 | 1.05 | 1.08 |
| google_maps_api_compact_response | 1.01 | 1.01 | 1.00 | 1.02 |
| google_maps_api_response | 1.04 | 1.09 | 1.01 | 1.02 |
| gsoc-2018 | 1.09 | 1.16 | 1.11 | 1.11 |
| instruments | 1.05 | 1.08 | 1.00 | 1.01 |
| marine_ik | 1.04 | 1.03 | 1.02 | 1.02 |
| mesh | 1.02 | 1.03 | 0.98 | 1.01 |
| mesh.pretty | 1.04 | 1.09 | 1.01 | 1.02 |
| numbers | 1.06 | 1.08 | 1.01 | 1.02 |
| random | 1.14 | 1.14 | 1.05 | 1.06 |
| repeat | 1.07 | 1.12 | 1.09 | 1.08 |
| semanticscholar-corpus | 1.06 | 1.08 | 1.06 | 1.06 |
| tree-pretty | 1.05 | 1.09 | 1.01 | 1.02 |
| twitter | 1.04 | 1.11 | 1.04 | 1.05 |
| twitter_api_compact_response | 1.08 | 1.09 | 1.02 | 1.04 |
| twitter_api_response | 1.06 | 1.10 | 1.03 | 1.04 |
| twitter_timeline | 1.07 | 1.10 | 1.02 | 1.04 |
| twitterescaped | 1.07 | 1.10 | 1.05 | 1.06 |
| update-center | 1.04 | 1.11 | 1.02 | 1.05 |
| **geometric mean** | **1.06** | **1.09** | **1.03** | **1.04** |

The files that gain the least (`canada`, `mesh`, `marine_ik`) are mostly numbers, where the indexing stage is cheap to begin with. The files that gain the most (`gsoc-2018`, `random`, `github_events`) are the ones with a lot of structure. No file gets slower, except `canada` on the Graviton 5 with GCC (by 3%, at the edge of what I can measure).

In absolute terms, the indexing stage goes from 4.8 GB/s to 5.3 GB/s on the Graviton 4 with clang (5.5 GB/s to 5.8 GB/s with GCC), and from 6.3 GB/s to 6.6 GB/s on the Graviton 5 with clang (7.1 GB/s to 7.3 GB/s with GCC). The Graviton 4 benefits more than the Graviton 5.

The second stage of the parser is untouched, so the gain on the whole parse is smaller: 2% to 4% on the Graviton 4 and 1% to 2% on the Graviton 5. It is a modest gain, but it comes from replacing four NEON instructions with one, in a routine that we had already tuned carefully.

In a real parser, `match` gives 3% to 9% faster indexing on Graviton 4 and Graviton 5, with a handful of intrinsics and no assembly. The code is in [simdjson pull request 2866](https://github.com/simdjson/simdjson/pull/2866). It requires SVE2, which Apple processors and the older Graviton processors do not have, so simdjson falls back on NEON when SVE2 is not available at compile time. 

The limitation today is that the code is compiled in only if you build with `-mcpu=native` or the equivalent: a default build gets the NEON code everywhere. The next step for simdjson is to select the SVE2 code at runtime, as we do with the various x64 instruction sets, so that a default build uses it on processors that have the instruction. I am working on it.

*Credit*: The `match` classifier is the work of [Madhurendra Purbay](https://github.com/mpurbay-arm) (ARM), from his [pull request 2863](https://github.com/simdjson/simdjson/pull/2863), where he used a different (and slightly faster) technique, with inline assembly, to extract the predicates. [My benchmark results and scripts are available](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/09/svehelp).

## References

Keiser, J., & Lemire, D. (2024). [On-demand JSON: A better way to parse documents?](https://doi.org/10.1002/spe.3313). Software: Practice and Experience, 54(6), 1074-1086.

Langdale, G., & Lemire, D. (2019). [Parsing gigabytes of JSON per second](https://doi.org/10.1007/s00778-019-00578-5). The VLDB Journal, 28(6), 941-960. ([arXiv](https://arxiv.org/abs/1902.08318))

Lemire, D. (2025). [Mixing ARM NEON with SVE code for fun and profit](https://lemire.me/blog/2025/03/29/mixing-arm-neon-with-sve-code-for-fun-and-profit/).

Lemire, D. (2025). [Scanning HTML at tens of gigabytes per second on ARM processors](https://doi.org/10.1002/spe.3420). Software: Practice and Experience, 55(7), 1256-1265.
