// https://gist.github.com/mhroth/2b494cc18b1c6eabf4e4
uint32_t _mm_movemask_ps(float32x4_t x) {
  uint32x4_t mmA = vandq_u32(
    vreinterpretq_u32_f32(x), (uint32x4_t) {0x1, 0x2, 0x4, 0x8}); // [0 1 2 3]
  uint32x4_t mmB = vextq_u32(mmA, mmA, 2);                        // [2 3 0 1]
  uint32x4_t mmC = vorrq_u32(mmA, mmB);                           // [0+2 1+3 0+2 1+3]
  uint32x4_t mmD = vextq_u32(mmC, mmC, 3);                        // [1+3 0+2 1+3 0+2]
  uint32x4_t mmE = vorrq_u32(mmC, mmD);                           // [0+1+2+3 ...]
  return vgetq_lane_u32(mmE, 0);
}

// https://www.snip2code.com/Snippet/716850/A-basic-NEON-implementation-of-SSE-_mm_m
uint32_t _mm_movemask_ps(float32x4_t x) {
  uint32x4_t mmA = vandq_u32(
    vreinterpretq_u32_f32(x), (uint32x4_t) {0x1, 0x2, 0x4, 0x8}); // [0 1 2 3]
  uint32x4_t mmB = vextq_u32(mmA, mmA, 2);                        // [2 3 0 1]
  uint32x4_t mmC = vorrq_u32(mmA, mmB);                           // [0+2 1+3 0+2 1+3]
  uint32x4_t mmD = vextq_u32(mmC, mmC, 3);                        // [1+3 0+2 1+3 0+2]
  uint32x4_t mmE = vorrq_u32(mmC, mmD);                           // [0+1+2+3 ...]
  return vgetq_lane_u32(mmE, 0);
}

//http://techqa.info/programming/question/11870910/SSE--mm-movemask-epi8-equivalent-method-for-ARM-NEON
int32_t _mm_movemask_epi8_neon(uint8x16_t input)
{
    const int8_t __attribute__ ((aligned (16))) xr[8] = {-7,-6,-5,-4,-3,-2,-1,0};
    uint8x8_t mask_and = vdup_n_u8(0x80);
    int8x8_t mask_shift = vld1_s8(xr);

    uint8x8_t lo = vget_low_u8(input);
    uint8x8_t hi = vget_high_u8(input);

    lo = vand_u8(lo, mask_and);
    lo = vshl_u8(lo, mask_shift);

    hi = vand_u8(hi, mask_and);
    hi = vshl_u8(hi, mask_shift);

    lo = vpadd_u8(lo,lo);
    lo = vpadd_u8(lo,lo);
    lo = vpadd_u8(lo,lo);

    hi = vpadd_u8(hi,hi);
    hi = vpadd_u8(hi,hi);
    hi = vpadd_u8(hi,hi);

    return ((hi[0] << 8) | (lo[0] & 0xFF));
}
