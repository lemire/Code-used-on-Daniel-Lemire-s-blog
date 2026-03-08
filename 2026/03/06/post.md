Suppose that you have a record of your sales per day. 
You might want to get a running record 
where, for each day, you are told how many sales you have made
since the start of the year.

| day | sales per day | running sales |
|-----|---------------|---------------|
| 1   |    10$        |      10 $     |
| 2   |    15$        |      25 $     | 
| 3   |     5$        |      30 $     |

Such an operation is called a prefix sum or a scan.

Implementing it in C is not difficult. It is a simple loop.

```c
  for (size_t i = 1; i < length; i++) {
    data[i] += data[i - 1];
  }
```

How fast can this function be? We can derive a speed limit rather simply: to compute the current value, you must have computed the previous one, and so forth.

```
data[0] -> data[1] -> data[2] -> ...
```

At best, you require one CPU cycle per entry in your table. Thus, on a 4 GHz processor, you might process 4 billion integer values per second. It is an upper bound but you might be able to reach close to it in practice on many modern systems.
Of course, there are other instructions involved such as loads, stores and branching, but our processors can execute many instructions per cycle and they can predict branches effectively. So you should be able to process billions of integers per second on most processors today.

Not bad! But can we do better?

We can use SIMD instructions. SIMD instructions are special instructions that process several values at once. All 64-bit ARM processors support NEON instructions. NEON instructions can process four integers at once, if they are packed in one SIMD register.

But how do you do the prefix sum on a 4-value register? You can do it with two shifts and two additions. In theory, it scales as log(N) where N is the number 
elements in a vector register.

```
input   = [A B   C     D]
shift1  = [0 A   B     C]
sum1    = [A A+B B+C   C+D]
shift2  = [0 0   A     B+A]
result  = [A A+B A+B+C A+B+C+D]
```

You can then extract the last value (`A+B+C+D`) and broadcast it to all positions so that you can add it to the next value.

Is this faster than the scalar approach? We have 4 instructions in sequence, plus at least one instruction if you want to use the total sum in the next block of four values.

Thus the SIMD approach might be worse. It is disappointing.

A solution might be the scale up over many more integer values.

Consider ARM NEON which has interleaved load and store instructions. If you can load 16 values at once, and get all of the first values together, all of the second values together, and so forth.

```
original data : ABCD EFGH IJKL MNOP
loaded data   : AEIM BFJN CGKO DHLP
```

Then I can do a prefix sum over the four blocks in parallel. It takes three instructions. At the end of the three instructions, we have one register which contains the local sums:

```
A+B+C+D E+F+G+H I+J+K+L M+N+O+P
```

And then we can apply our prefix sum recipe on this register (4 instructions). You might end up with something like 8 sequential instructions per block of 16 values.

It is theoretically twice as fast as the scalar approach.

In C with instrinsics, you might code it as follows.

```c

void neon_prefixsum_fast(uint32_t *data, size_t length) {
  uint32x4_t zero = {0, 0, 0, 0};
  uint32x4_t prev = {0, 0, 0, 0};

  for (size_t i = 0; i < length / 16; i++) {
    uint32x4x4_t vals = vld4q_u32(data + 16 * i);

    // Prefix sum inside each transposed ("vertical") lane
    vals.val[1] = vaddq_u32(vals.val[1], vals.val[0]);
    vals.val[2] = vaddq_u32(vals.val[2], vals.val[1]);
    vals.val[3] = vaddq_u32(vals.val[3], vals.val[2]);

    // Now vals.val[3] contains the four local prefix sums:
    //   vals.val[3] = [s0=A+B+C+D, s1=E+F+G+H, 
    //                  s2=I+J+K+L, s3=M+N+O+P]

    // Compute prefix sum across the four local sums 
    uint32x4_t off = vextq_u32(zero, vals.val[3], 3);
    uint32x4_t ps = vaddq_u32(vals.val[3], off);       
    off = vextq_u32(zero, ps, 2);                      
    ps = vaddq_u32(ps, off);

    // Now ps contains cumulative sums across the four groups
    // Add the incoming carry from the previous 16-element block
    ps = vaddq_u32(ps, prev);

    // Prepare carry for next block: broadcast the last lane of ps
    prev = vdupq_laneq_u32(ps, 3);

    // The add vector to apply to the original lanes is the 
    // prefix up to previous group
    uint32x4_t add = vextq_u32(prev, ps, 3);  

    // Apply carry/offset to each of the four transposed lanes
    vals.val[0] = vaddq_u32(vals.val[0], add);
    vals.val[1] = vaddq_u32(vals.val[1], add);
    vals.val[2] = vaddq_u32(vals.val[2], add);
    vals.val[3] = vaddq_u32(vals.val[3], add);

    // Store back the four lanes (interleaved)
    vst4q_u32(data + 16 * i, vals);
  }

  scalar_prefixsum_leftover(data, length, 16);
}
```


Here is a markdown table listing **all the intrinsics** used in the `neon_prefixsum_fast` function you provided earlier, along with what they do. These are standard ARM NEON intrinsics (from `arm_neon.h`).



Let us try it out on an Apple M4 processor (4.5 GHz).

|  method   | billions of values/s |
|-----------|----------------------|
| scalar    |     3.9              |
| naive SIMD |    3.6              |
| fast SIMD  |    8.9              |

So the SIMD approach is about 2.3 times faster than the scalar approach. Not bad.

Could you do better? Maybe.

[My source code is available on GitHub](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/03/06/benchmark).



*Appendix*. Instrinsics


| Intrinsic                        | What it does                                                                                   |
|----------------------------------|------------------------------------------------------------------------------------------------|
| `vld4q_u32`                      | Loads 16 consecutive 32-bit unsigned integers from memory and deinterleaves them into 4 separate `uint32x4_t` vectors (lane 0 = elements 0,4,8,12,…; lane 1 = 1,5,9,13,… etc.). |
| `vaddq_u32`                      | Adds corresponding 32-bit unsigned integer lanes from two vectors (`a[i] + b[i]` for each of 4 lanes). |
| `vextq_u32`                      | Extracts (concatenates a and b, then takes 4 lanes starting from lane `n` of the 8-lane concatenation). Used to implement shifts/rotates by inserting zeros (when `a` is zero vector). |
| `vdupq_laneq_u32`                | Broadcasts (duplicates) the value from the specified lane (0–3) of the input vector to all 4 lanes of the result. |
| `vdupq_n_u32`    | Sets all 4 lanes of the result to the same scalar value (commonly used for zero or broadcast). |

