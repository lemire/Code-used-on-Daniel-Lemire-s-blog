# Converting data to hexadecimal outputs quickly

Given any string of bytes, you can convert it to an hexadecimal string by mapping the least significant and the most significant 4 bits of byte to characters in `01...9A...F`. There are more efficient techniques like base64, that map 3 bytes to 4 characters. However, hexadecimal outputs are easier to understand and often sufficiently concise.

A simple function to do the conversion using a short lookup table is as follows:

```cpp
static const char hex[] = "0123456789abcdef";
for (size_t i = 0, k = 0; k < dlen; i += 1, k += 2) {
    uint8_t val = src[i];
    dst[k + 0] = hex[val >> 4];
    dst[k + 1] = hex[val & 15];
}
```

This code snippet implements a straightforward byte-to-hexadecimal string conversion loop in C++. It iterates over an input byte array (`src`), processing one byte at a time using index `i`, while simultaneously building the output string in `dst` with index `k` that advances twice as fast (by 2) since each input byte produces two hexadecimal characters. For each byte, it extracts the value as an unsigned 8-bit integer (`val`), then isolates the high 4 bits (via right shift by 4) and low 4 bits (via bitwise AND with 15) to index into a static lookup table (`hex`) containing the characters '0' through '9' and 'a' through 'f'. The loop continues until `k` reaches the expected output length (`dlen`), which should be twice the input length, ensuring all bytes are converted without bounds errors.

This lookup table approach is used in the popular Node.js JavaScript runtime. Skovorora recently proposed to replace this lookup table approach with an arithmetic version.


```cpp
char nibble(uint8_t x) { return x + '0' + ((x > 9) * 39); }
for (size_t i = 0, k = 0; k < dlen; i += 1, k += 2) {
    uint8_t val = src[i];
    dst[k + 0] = nibble(val >> 4);
    dst[k + 1] = nibble(val & 15);
}
```

Surprisingly maybe, this approach is much faster and uses far fewer instructions. At first glance, this result might be puzzling. A table lookup is cheap, the new `nibble` function seemingly does more work.

The trick that Skovorora relies upon is that compilers are smart: they will 'autovectorize' such number crunching functions (if you are lucky). That is, instead of using regular instructions that process byte values, the will SIMD instructions that process 16 bytes at once or more.

Of course, instead of relying on the compiler, you can manually invoke SIMD instructions through SIMD instrinsic functions. Let us assume that you have an ARM processors (e.g., on Apple Silicon). Then you can process blocks of 32 bytes as follows.

```cpp
  size_t maxv = (slen - (slen%32));
  for (; i < maxv; i += 32) {
    uint8x16_t val1 = vld1q_u8((uint8_t*)src + i);
    uint8x16_t val2 = vld1q_u8((uint8_t*)src + i + 16);
 
    uint8x16_t high1 = vshrq_n_u8(val1, 4);
    uint8x16_t low1 = vandq_u8(val1, vdupq_n_u8(15));
    uint8x16_t high2 = vshrq_n_u8(val2, 4);
    uint8x16_t low2 = vandq_u8(val2, vdupq_n_u8(15));

    uint8x16_t high_chars1 = vqtbl1q_u8(table, high1);
    uint8x16_t low_chars1 = vqtbl1q_u8(table, low1);

    uint8x16_t high_chars2 = vqtbl1q_u8(table, high2);
    uint8x16_t low_chars2 = vqtbl1q_u8(table, low2);

    uint8x16x2_t zipped1 = vzipq_u8(high_chars1, low_chars1);
    uint8x16x2_t zipped2 = vzipq_u8(high_chars2, low_chars2);

    vst1q_u8((uint8_t*)dst + i*2, zipped1.val[0]);
    vst1q_u8((uint8_t*)dst + i*2 + 16, zipped1.val[1]);
    vst1q_u8((uint8_t*)dst + i*2 + 32, zipped2.val[0]);
    vst1q_u8((uint8_t*)dst + i*2 + 48, zipped2.val[1]);
  }
  ```

This SIMD code leverages ARM NEON intrinsics to accelerate hexadecimal encoding by processing 32 input bytes simultaneously. It begins by loading two 16-byte vectors (`val1` and `val2`) from the source array using `vld1q_u8`. For each vector, it extracts the high nibbles (via right shift by 4 with `vshrq_n_u8`) and low nibbles (via bitwise AND with 15 using `vandq_u8` and `vdupq_n_u8`). The nibbles are then used as indices into a pre-loaded hex table via `vqtbl1q_u8` to fetch the corresponding ASCII characters. The high and low character vectors are interleaved using `vzipq_u8`, producing two output vectors per input pair. Finally, the results are stored back to the destination array with `vst1q_u8`, ensuring efficient memory operations.

You could do similar work on other systems like x64. The same code with AVX-512 for recent Intel and AMD processors would probably be insanely efficient.

Benchmarking these implementations on a dataset of 10,000 random bytes reveals significant performance differences. The basic lookup table version achieves around 3 GB/s, while the arithmetic version, benefiting from compiler autovectorization, reaches 23 GB/s. The manual SIMD NEON versions push performance further: I reach 42 GB/s in my tests.

| method | speed | instructions per byte |
|--------|-------|-----------------------|
| table  | 3.1 GB/s | 9 |
| Skovorora | 23 GB/s | 0.75 |
| intrinsics | 42 GB/s | 0.69 |


One lesson is that intuition can be a poor guide when trying to assess performance. 

[My source code is available.]()