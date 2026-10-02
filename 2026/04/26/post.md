# You can beat the binary search

We sometimes have to look for a value in a sorted array. The simplest algorithm consists in just going through the values one by one, until we encounter the value, or exhaust the array. We sometimes call this algorithm a linear search. In C++, you can get the desired effect with the `std::find` function.

For large arrays, you can do better with a binary search. Binary search is a classic algorithm that efficiently locates a target value in a sorted array by repeatedly dividing the search interval in half. Starting with the entire array, it compares the target to the middle element: if the target is smaller, it discards the upper half; if larger, it discards the lower half. This process continues until the target is found or the interval is empty. It is much faster than linear search for large datasets. In C++, this is implemented by the `std::binary_search` function, which returns a boolean indicating whether the value is present.

The popular Roaring Bitmap format uses arrays of 16-bit integers of size ranging from 1 to 4096. We sometimes have to check whether a value is present. We use a binary search.

I wanted a faster approach. I had two insights.

1. Virtually all processors today have data parallel instructions (sometimes called SIMD) that can check several values at once. Both 64-bit ARM and x64 processors (Intel/AMD) always support comparing eight 16-bit integers with a target value using a single instruction. This suggests that you should not bother going down in the binary search to blocks that are smaller than eight elements. And you may also want to cheaply compare sixteen elements or more.

2. The binary search checks one value at a time. However, recent processors can load and check more than one value at once. They have excellent memory-level parllelism. This suggest that instead of a binary search, we might want to try a quaternary search: instead of splitting arrays in halves, we might split them in quarters. The net result might generate a few more instructions but the number of instructions is likely not the limiting factor.


Thus, I created something I call the SIMD Quad algorithm. It is an efficient search algorithm for sorted arrays of 16-bit unsigned integers, combining a quaternary interpolation search with SIMD (Single Instruction, Multiple Data). The algorithm divides the array into fixed-size blocks of 16 elements (except maybe for the last block) and uses the last element of each block as interpolation keys to quickly narrow down the search to a single block, then employs SIMD instructions to check all 16 elements in that block simultaneously.

The core idea is to perform a hierarchical search: first, use interpolation search on a coarser level (block boundaries) to find the likely block containing the target value, then switch to SIMD for fine-grained parallel checking within the block. This hybrid approach leverages the strengths of both algorithmic optimization (interpolation search reduces comparisons logarithmically) and hardware acceleration (SIMD checks multiple elements at once).


1. Initial Check: If the array has fewer than 16 elements, compare the target against two overlapping SIMD loads that together cover the array (below four elements, compare it against the first, middle and last elements). There is no data-dependent branch.

2. Block Division: Divide the array into blocks of 16 consecutive elements. For an array of size `cardinality`, there are `num_blocks = cardinality / 16` full blocks.

3. Quaternary Interpolation Search: Use the last element of each block (at positions `16-1`, `32-1`, etc.) as keys for interpolation. The search performs a quaternary (base-4) interpolation to find the block where the target `pos` is likely located. This involves comparing the target against quarter-points of the current search range and adjusting the base accordingly.

4. Block Selection: After narrowing down, select the appropriate block index `lo` based on the interpolation results.

5. SIMD Check: If a valid block is found, load the 16 elements into SIMD registers (using NEON on ARM or SSE2 on x64) and perform parallel equality comparisons with the target value. If any match is found, return true.

6. Remainder Check: For any elements not in full blocks (remainder), perform a linear search.


How does it do? I wrote a benchmark. The benchmark works as follows. For each array size from 2 to 4096 elements, it generates 100,000 sorted arrays of 16-bit unsigned integers. For each size, it performs 10 million membership queries in "cold" mode (each query searches a different array, simulating cache misses) and 10 million queries in "warm" mode (queries are grouped by array, with each array being searched 100 times consecutively, simulating cache hits). The benchmark measures the average time per query for three algorithms: linear search (`std::find`), binary search (`std::binary_search`), and the new SIMD Quad algorithm. 

I use two systems. An Apple M4 with Apple LLVM and an Intel Emeral Rapids processor with GCC.

Firstly, let us compare the linear search with the binary search.

Intel/GCC:

![Benchmark comparison on Intel](comparison_plot_intel.png)

Apple/LLVM

![Benchmark comparison on Apple M4](comparison_plot_apple.png)


The result is clear. The binary search beats the linear search as soon as the arrays get large. That is to be expected.

On a cold cache, the linear search is relatively worse. That is to be expected because it accesses more data, causing more cache faults.


We have established that the binary search is the net winner over the linear search. Let us now compare with the SIMD Quad algorithm.


Intel/GCC:

![Benchmark comparison on Intel](binary_vs_simd_intel.png)

Apple/LLVM

![Benchmark comparison on Apple M4](binary_vs_simd_apple.png)


The results  differ markedly between the Intel and AMD platform. On the Intel platform the SIMD Quad is more than twice as fast as the binary search on the warm cache. The benefits are lesser on the cold cache. On the Apple platform, the reverse is true, it is with the code cache that the SIMD Quad  is more than twice as fast, whereas the benefits are more marginal on the cold cache.

But the important point is that, in all instances,  SIMD Quad  is faster than the binary search.

The SIMD component of the algorithm is rather straightforward: we use specialized instructions that save work. So it is easy to see why it might make things faster. There are few instructions, fewer branches.

But what about the 'quad' part. Does it matter? So I tried a binary version of the same algorithm. It has the same SIMD optimization, but I am dropping the  quaternary interpolation search and replacing it with a standard binary search.



Intel/GCC:

![Benchmark comparison on Intel](three_way_comparison_intel.png)

Apple/LLVM

![Benchmark comparison on Apple M4](three_way_comparison_apple.png)


To put it in simple terms, the quad approach has little effect on the Apple platform, but it is a decent optimization on the Intel platform for large arrays in the cold case. The quaternary search better exploits the memory-level parallelism on my Intel server.

[My source code is available](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/04/26/benchmark).


**Conclusion**. What my results suggest is that while a textbook binary search is a decent algorithm, you can do better in ways that matter. Standard algorithms were often not designed for computers that have so much parallelism. The SIMD Quad algorithm tries to leverage both the memory-level and data parallelism. Further, I suspect that we can do even better than my algorithm. Let us get creative!

## Appendix (source code)


```cpp
bool simd_quad(const uint16_t *carr, int32_t cardinality, uint16_t pos) {
    constexpr int32_t gap = 16;
    if (cardinality < gap) {
        // two overlapping loads cover the array: no data-dependent branch
#ifdef __ARM_NEON
        if (cardinality >= 8) {
            uint16x8_t needle = vdupq_n_u16(pos);
            uint16x8_t v0 = vld1q_u16(carr);
            uint16x8_t v1 = vld1q_u16(carr + cardinality - 8);
            uint16x8_t hit = vorrq_u16(vceqq_u16(v0, needle), vceqq_u16(v1, needle));
            return vmaxvq_u16(hit) != 0;
        }
        if (cardinality >= 4) {
            uint16x4_t needle = vdup_n_u16(pos);
            uint16x4_t v0 = vld1_u16(carr);
            uint16x4_t v1 = vld1_u16(carr + cardinality - 4);
            uint16x4_t hit = vorr_u16(vceq_u16(v0, needle), vceq_u16(v1, needle));
            return vmaxv_u16(hit) != 0;
        }
#else
        if (cardinality >= 8) {
            __m128i needle = _mm_set1_epi16((short)pos);
            __m128i v0 = _mm_loadu_si128((const __m128i *)carr);
            __m128i v1 = _mm_loadu_si128((const __m128i *)(carr + cardinality - 8));
            __m128i hit = _mm_or_si128(_mm_cmpeq_epi16(v0, needle),
                                       _mm_cmpeq_epi16(v1, needle));
            return _mm_movemask_epi8(hit) != 0;
        }
        if (cardinality >= 4) {
            __m128i needle = _mm_set1_epi16((short)pos);
            __m128i v0 = _mm_loadl_epi64((const __m128i *)carr);
            __m128i v1 = _mm_loadl_epi64((const __m128i *)(carr + cardinality - 4));
            __m128i hit = _mm_or_si128(_mm_cmpeq_epi16(v0, needle),
                                       _mm_cmpeq_epi16(v1, needle));
            return (_mm_movemask_epi8(hit) & 0xFF) != 0;
        }
#endif
        if (cardinality == 0) return false;
        return (carr[0] == pos) | (carr[cardinality / 2] == pos) |
               (carr[cardinality - 1] == pos);
    }
    int32_t num_blocks = cardinality / gap;
    int32_t base = 0;
    int32_t n = num_blocks;
    while (n > 3) {
      int32_t quarter = n >> 2;

      int32_t k1 = carr[(base + quarter + 1) * gap - 1];
      int32_t k2 = carr[(base + 2 * quarter + 1) * gap - 1];
      int32_t k3 = carr[(base + 3 * quarter + 1) * gap - 1];

      int32_t c1 = (k1 < pos);
      int32_t c2 = (k2 < pos);
      int32_t c3 = (k3 < pos);

      base += (c1 + c2 + c3) * quarter;
      n -= 3 * quarter;
    }
    while (n > 1) {
        int32_t half = n >> 1;
        base = (carr[(base + half + 1) * gap - 1] < pos) ? base + half : base;
        n -= half;
    }
    int32_t lo = (carr[(base + 1) * gap - 1] < pos) ? base + 1 : base;

    if (lo < num_blocks) {
        const uint16_t *blk = carr + lo * gap;
#ifdef __ARM_NEON
        uint16x8_t needle = vdupq_n_u16(pos);
        uint16x8_t v0 = vld1q_u16(blk);
        uint16x8_t v1 = vld1q_u16(blk + 8);
        uint16x8_t hit = vorrq_u16(vceqq_u16(v0, needle), vceqq_u16(v1, needle));
        return vmaxvq_u16(hit) != 0;
#else
        __m128i needle = _mm_set1_epi16((short)pos);
        __m128i v0 = _mm_loadu_si128((const __m128i *)blk);
        __m128i v1 = _mm_loadu_si128((const __m128i *)(blk + 8));
        __m128i hit = _mm_or_si128(_mm_cmpeq_epi16(v0, needle),
                                   _mm_cmpeq_epi16(v1, needle));
        return _mm_movemask_epi8(hit) != 0;
#endif
    }

    for (int32_t j = num_blocks * gap; j < cardinality; j++) {
        uint16_t v = carr[j];
        if (v >= pos) return (v == pos);
    }
    return false;
}

```