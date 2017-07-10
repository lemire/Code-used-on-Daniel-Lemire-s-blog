#ifndef DESPACER_H
#define DESPACER_H
#include <stddef.h>
#include <stdint.h>

static inline size_t despace(char *bytes, size_t howmany) {
  size_t i = 0, pos = 0;
  while (i < howmany) {
    const char c = bytes[i++];
    bytes[pos] = c;
    pos += (c > 32 ? 1 : 0);
  }
  return pos;
}

// let us go neon
#include <arm_neon.h>


static inline uint8x16_t is_white(uint8x16_t data) {
  const uint8x16_t wchar = vdupq_n_u8(' ');
  uint8x16_t isw = vcleq_u8(data, wchar);
  return isw;
}



////////////
// see https://stackoverflow.com/a/41406502/73007
// compiles to something like
//        uqxtn   v0.2s, v0.2d
//        fmov    x0, d0
//////////
static inline uint64_t is_not_zero(uint8x16_t v) {
  uint64x2_t v64 = vreinterpretq_u64_u8(v);
  uint32x2_t v32 = vqmovn_u64(v64);
  uint64x1_t result = vreinterpret_u64_u32(v32);
#ifdef __clang__
  return result[0];
#else
  return result;
#endif
}

/*
* could also have 
*
static inline uint16_t is_not_zero(uint8x16_t v) {
    return vaddlvq_u8(v);
}
* but experimentally, it appears slower.
* credit: Cyril Lashkevich
*/


static inline size_t neon_despace(char *bytes, size_t howmany) {
   size_t i = 0, pos = 0;
  const size_t chunk_size = 16 * 4 * 1;
  uint8x16_t justone = vdupq_n_u8(1);
  for (; i + chunk_size <= howmany; /*i += chunk_size*/) {
    uint8x16_t vecbytes0 = vld1q_u8((uint8_t *)bytes + i);
    uint8x16_t vecbytes1 = vld1q_u8((uint8_t *)bytes + i + 16);
    uint8x16_t vecbytes2 = vld1q_u8((uint8_t *)bytes + i + 32);
    uint8x16_t vecbytes3 = vld1q_u8((uint8_t *)bytes + i + 48);
    uint8x16_t w0 = is_white(vecbytes0);
    uint64_t haswhite0 = is_not_zero(w0);
    w0 = vaddq_u8(justone, w0);
    uint8x16_t w1 = is_white(vecbytes1);
    uint64_t haswhite1 = is_not_zero(w1);
    w1 = vaddq_u8(justone, w1);
    uint8x16_t w2 = is_white(vecbytes2);
    uint64_t haswhite2 = is_not_zero(w2);
    w2 = vaddq_u8(justone, w2);
    uint8x16_t w3 = is_white(vecbytes3);
    uint64_t haswhite3 = is_not_zero(w3);
    w3 = vaddq_u8(justone, w3);
    if(!haswhite0) {
      vst1q_u8((uint8_t *)bytes + pos,vecbytes0);
      pos += 16;
      i += 16;
     } else {
      for (int k = 0; k < 16; k++) {
        bytes[pos] = bytes[i++];
        pos += w0[k];
      }
    }
    if(!haswhite1) {
      vst1q_u8((uint8_t *)bytes + pos,vecbytes1);
      pos += 16;
      i += 16;
     } else {
       for (int k = 0; k < 16; k++) {
        bytes[pos] = bytes[i++];
        pos += w1[k];
      }
    }
    if(!haswhite2) {
      vst1q_u8((uint8_t *)bytes + pos,vecbytes2);
      pos += 16;
      i += 16;
     } else {
       for (int k = 0; k < 16; k++) {
        bytes[pos] = bytes[i++];
        pos += w2[k];
      }
    }
    if(!haswhite3) {
      vst1q_u8((uint8_t *)bytes + pos,vecbytes3);
      pos += 16;
      i += 16;
     } else {
       for (int k = 0; k < 16; k++) {
        bytes[pos] = bytes[i++];
        pos += w3[k];
      }
    }
  }
  while (i < howmany) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}





static const uint8_t __attribute__((aligned(16))) mask_shuffle[256*8] = {
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0, 0,1,0,0,0,0,0,0,
    2,0,0,0,0,0,0,0, 0,2,0,0,0,0,0,0, 1,2,0,0,0,0,0,0, 0,1,2,0,0,0,0,0,
    3,0,0,0,0,0,0,0, 0,3,0,0,0,0,0,0, 1,3,0,0,0,0,0,0, 0,1,3,0,0,0,0,0,
    2,3,0,0,0,0,0,0, 0,2,3,0,0,0,0,0, 1,2,3,0,0,0,0,0, 0,1,2,3,0,0,0,0,
    4,0,0,0,0,0,0,0, 0,4,0,0,0,0,0,0, 1,4,0,0,0,0,0,0, 0,1,4,0,0,0,0,0,
    2,4,0,0,0,0,0,0, 0,2,4,0,0,0,0,0, 1,2,4,0,0,0,0,0, 0,1,2,4,0,0,0,0,
    3,4,0,0,0,0,0,0, 0,3,4,0,0,0,0,0, 1,3,4,0,0,0,0,0, 0,1,3,4,0,0,0,0,
    2,3,4,0,0,0,0,0, 0,2,3,4,0,0,0,0, 1,2,3,4,0,0,0,0, 0,1,2,3,4,0,0,0,
    5,0,0,0,0,0,0,0, 0,5,0,0,0,0,0,0, 1,5,0,0,0,0,0,0, 0,1,5,0,0,0,0,0,
    2,5,0,0,0,0,0,0, 0,2,5,0,0,0,0,0, 1,2,5,0,0,0,0,0, 0,1,2,5,0,0,0,0,
    3,5,0,0,0,0,0,0, 0,3,5,0,0,0,0,0, 1,3,5,0,0,0,0,0, 0,1,3,5,0,0,0,0,
    2,3,5,0,0,0,0,0, 0,2,3,5,0,0,0,0, 1,2,3,5,0,0,0,0, 0,1,2,3,5,0,0,0,
    4,5,0,0,0,0,0,0, 0,4,5,0,0,0,0,0, 1,4,5,0,0,0,0,0, 0,1,4,5,0,0,0,0,
    2,4,5,0,0,0,0,0, 0,2,4,5,0,0,0,0, 1,2,4,5,0,0,0,0, 0,1,2,4,5,0,0,0,
    3,4,5,0,0,0,0,0, 0,3,4,5,0,0,0,0, 1,3,4,5,0,0,0,0, 0,1,3,4,5,0,0,0,
    2,3,4,5,0,0,0,0, 0,2,3,4,5,0,0,0, 1,2,3,4,5,0,0,0, 0,1,2,3,4,5,0,0,
    6,0,0,0,0,0,0,0, 0,6,0,0,0,0,0,0, 1,6,0,0,0,0,0,0, 0,1,6,0,0,0,0,0,
    2,6,0,0,0,0,0,0, 0,2,6,0,0,0,0,0, 1,2,6,0,0,0,0,0, 0,1,2,6,0,0,0,0,
    3,6,0,0,0,0,0,0, 0,3,6,0,0,0,0,0, 1,3,6,0,0,0,0,0, 0,1,3,6,0,0,0,0,
    2,3,6,0,0,0,0,0, 0,2,3,6,0,0,0,0, 1,2,3,6,0,0,0,0, 0,1,2,3,6,0,0,0,
    4,6,0,0,0,0,0,0, 0,4,6,0,0,0,0,0, 1,4,6,0,0,0,0,0, 0,1,4,6,0,0,0,0,
    2,4,6,0,0,0,0,0, 0,2,4,6,0,0,0,0, 1,2,4,6,0,0,0,0, 0,1,2,4,6,0,0,0,
    3,4,6,0,0,0,0,0, 0,3,4,6,0,0,0,0, 1,3,4,6,0,0,0,0, 0,1,3,4,6,0,0,0,
    2,3,4,6,0,0,0,0, 0,2,3,4,6,0,0,0, 1,2,3,4,6,0,0,0, 0,1,2,3,4,6,0,0,
    5,6,0,0,0,0,0,0, 0,5,6,0,0,0,0,0, 1,5,6,0,0,0,0,0, 0,1,5,6,0,0,0,0,
    2,5,6,0,0,0,0,0, 0,2,5,6,0,0,0,0, 1,2,5,6,0,0,0,0, 0,1,2,5,6,0,0,0,
    3,5,6,0,0,0,0,0, 0,3,5,6,0,0,0,0, 1,3,5,6,0,0,0,0, 0,1,3,5,6,0,0,0,
    2,3,5,6,0,0,0,0, 0,2,3,5,6,0,0,0, 1,2,3,5,6,0,0,0, 0,1,2,3,5,6,0,0,
    4,5,6,0,0,0,0,0, 0,4,5,6,0,0,0,0, 1,4,5,6,0,0,0,0, 0,1,4,5,6,0,0,0,
    2,4,5,6,0,0,0,0, 0,2,4,5,6,0,0,0, 1,2,4,5,6,0,0,0, 0,1,2,4,5,6,0,0,
    3,4,5,6,0,0,0,0, 0,3,4,5,6,0,0,0, 1,3,4,5,6,0,0,0, 0,1,3,4,5,6,0,0,
    2,3,4,5,6,0,0,0, 0,2,3,4,5,6,0,0, 1,2,3,4,5,6,0,0, 0,1,2,3,4,5,6,0,
    7,0,0,0,0,0,0,0, 0,7,0,0,0,0,0,0, 1,7,0,0,0,0,0,0, 0,1,7,0,0,0,0,0,
    2,7,0,0,0,0,0,0, 0,2,7,0,0,0,0,0, 1,2,7,0,0,0,0,0, 0,1,2,7,0,0,0,0,
    3,7,0,0,0,0,0,0, 0,3,7,0,0,0,0,0, 1,3,7,0,0,0,0,0, 0,1,3,7,0,0,0,0,
    2,3,7,0,0,0,0,0, 0,2,3,7,0,0,0,0, 1,2,3,7,0,0,0,0, 0,1,2,3,7,0,0,0,
    4,7,0,0,0,0,0,0, 0,4,7,0,0,0,0,0, 1,4,7,0,0,0,0,0, 0,1,4,7,0,0,0,0,
    2,4,7,0,0,0,0,0, 0,2,4,7,0,0,0,0, 1,2,4,7,0,0,0,0, 0,1,2,4,7,0,0,0,
    3,4,7,0,0,0,0,0, 0,3,4,7,0,0,0,0, 1,3,4,7,0,0,0,0, 0,1,3,4,7,0,0,0,
    2,3,4,7,0,0,0,0, 0,2,3,4,7,0,0,0, 1,2,3,4,7,0,0,0, 0,1,2,3,4,7,0,0,
    5,7,0,0,0,0,0,0, 0,5,7,0,0,0,0,0, 1,5,7,0,0,0,0,0, 0,1,5,7,0,0,0,0,
    2,5,7,0,0,0,0,0, 0,2,5,7,0,0,0,0, 1,2,5,7,0,0,0,0, 0,1,2,5,7,0,0,0,
    3,5,7,0,0,0,0,0, 0,3,5,7,0,0,0,0, 1,3,5,7,0,0,0,0, 0,1,3,5,7,0,0,0,
    2,3,5,7,0,0,0,0, 0,2,3,5,7,0,0,0, 1,2,3,5,7,0,0,0, 0,1,2,3,5,7,0,0,
    4,5,7,0,0,0,0,0, 0,4,5,7,0,0,0,0, 1,4,5,7,0,0,0,0, 0,1,4,5,7,0,0,0,
    2,4,5,7,0,0,0,0, 0,2,4,5,7,0,0,0, 1,2,4,5,7,0,0,0, 0,1,2,4,5,7,0,0,
    3,4,5,7,0,0,0,0, 0,3,4,5,7,0,0,0, 1,3,4,5,7,0,0,0, 0,1,3,4,5,7,0,0,
    2,3,4,5,7,0,0,0, 0,2,3,4,5,7,0,0, 1,2,3,4,5,7,0,0, 0,1,2,3,4,5,7,0,
    6,7,0,0,0,0,0,0, 0,6,7,0,0,0,0,0, 1,6,7,0,0,0,0,0, 0,1,6,7,0,0,0,0,
    2,6,7,0,0,0,0,0, 0,2,6,7,0,0,0,0, 1,2,6,7,0,0,0,0, 0,1,2,6,7,0,0,0,
    3,6,7,0,0,0,0,0, 0,3,6,7,0,0,0,0, 1,3,6,7,0,0,0,0, 0,1,3,6,7,0,0,0,
    2,3,6,7,0,0,0,0, 0,2,3,6,7,0,0,0, 1,2,3,6,7,0,0,0, 0,1,2,3,6,7,0,0,
    4,6,7,0,0,0,0,0, 0,4,6,7,0,0,0,0, 1,4,6,7,0,0,0,0, 0,1,4,6,7,0,0,0,
    2,4,6,7,0,0,0,0, 0,2,4,6,7,0,0,0, 1,2,4,6,7,0,0,0, 0,1,2,4,6,7,0,0,
    3,4,6,7,0,0,0,0, 0,3,4,6,7,0,0,0, 1,3,4,6,7,0,0,0, 0,1,3,4,6,7,0,0,
    2,3,4,6,7,0,0,0, 0,2,3,4,6,7,0,0, 1,2,3,4,6,7,0,0, 0,1,2,3,4,6,7,0,
    5,6,7,0,0,0,0,0, 0,5,6,7,0,0,0,0, 1,5,6,7,0,0,0,0, 0,1,5,6,7,0,0,0,
    2,5,6,7,0,0,0,0, 0,2,5,6,7,0,0,0, 1,2,5,6,7,0,0,0, 0,1,2,5,6,7,0,0,
    3,5,6,7,0,0,0,0, 0,3,5,6,7,0,0,0, 1,3,5,6,7,0,0,0, 0,1,3,5,6,7,0,0,
    2,3,5,6,7,0,0,0, 0,2,3,5,6,7,0,0, 1,2,3,5,6,7,0,0, 0,1,2,3,5,6,7,0,
    4,5,6,7,0,0,0,0, 0,4,5,6,7,0,0,0, 1,4,5,6,7,0,0,0, 0,1,4,5,6,7,0,0,
    2,4,5,6,7,0,0,0, 0,2,4,5,6,7,0,0, 1,2,4,5,6,7,0,0, 0,1,2,4,5,6,7,0,
    3,4,5,6,7,0,0,0, 0,3,4,5,6,7,0,0, 1,3,4,5,6,7,0,0, 0,1,3,4,5,6,7,0,
    2,3,4,5,6,7,0,0, 0,2,3,4,5,6,7,0, 1,2,3,4,5,6,7,0, 0,1,2,3,4,5,6,7,
};

// credit: Martins Mozeiko
static inline size_t neon_despace_branchless(char *bytes, size_t howmany) {
  size_t i = 0, pos = 0;
  const size_t chunk_size = 16;
  uint8x16_t bitmask = { 1, 2, 4, 8, 16, 32, 64, 128, 1, 2, 4, 8, 16, 32, 64, 128 };
  uint8x16_t space = vdupq_n_u8(' ');
  for (; i + chunk_size <= howmany; i += chunk_size) {
    uint8x16_t vec = vld1q_u8((uint8_t*)bytes + i);

    uint8x16_t cmp = vcgtq_u8(vec, space);
    uint64x2_t mask = vpaddlq_u32(vpaddlq_u16(vpaddlq_u8(vandq_u8(cmp, bitmask))));

    uint8_t mlow = vgetq_lane_u8(vreinterpretq_u8_u64(mask), 0);
    uint8_t mhigh = vgetq_lane_u8(vreinterpretq_u8_u64(mask), 8);

    uint8x8_t low = vtbl1_u8(vget_low_u8(vec), vld1_u8(mask_shuffle + mlow*8));
    uint8x8_t high = vtbl1_u8(vget_high_u8(vec), vld1_u8(mask_shuffle + mhigh*8));

    vst1_u8((uint8_t*)bytes + pos, low);
    pos += __builtin_popcount(mlow);

    vst1_u8((uint8_t*)bytes + pos, high);
    pos += __builtin_popcount(mhigh);
  }
  while (i < howmany) {
    char c = bytes[i];
    if (c == '\r' || c == '\n' || c == ' ') {
      continue;
    }
    bytes[pos++] = c;
  }
  return pos;
}



static inline uint8x16_t is_nonwhite(uint8x16_t data) {
  const uint8x16_t wchar = vdupq_n_u8(' '+1);
  uint8x16_t isw = vcgeq_u8(data, wchar);
  return isw;
}

static inline uint16_t neonmovemask_addv(uint8x16_t input8) {
  uint16x8_t input = vreinterpretq_u16_u8(input8);
  const uint16x8_t bitmask = { 0x0101 , 0x0202, 0x0404, 0x0808, 0x1010, 0x2020, 0x4040, 0x8080 };
  uint16x8_t minput = vandq_u16(input, bitmask);
  return vaddvq_u16(minput);
}

static inline uint8_t bytepopcount(uint8x16_t v) {
  return vaddvq_u8(vshrq_n_u8(v,7));
}

#include "bigtable.h"


static inline size_t neontbl_despace(char *bytes, size_t howmany) {
   size_t i = 0, pos = 0;
  const size_t chunk_size = 16 * 4 * 1;
  for (; i + chunk_size <= howmany; i += chunk_size) {
    uint8x16_t vecbytes0 = vld1q_u8((uint8_t *)bytes + i);
    uint8x16_t vecbytes1 = vld1q_u8((uint8_t *)bytes + i + 16);
    uint8x16_t vecbytes2 = vld1q_u8((uint8_t *)bytes + i + 32);
    uint8x16_t vecbytes3 = vld1q_u8((uint8_t *)bytes + i + 48);
    // as early as possible, we compute the population counts
    uint8x16_t w0 = is_nonwhite(vecbytes0);
    uint8_t numberofkeptchars0 = bytepopcount(w0);
    uint8x16_t shuf0 = vld1q_u8(shufmask + 16 * neonmovemask_addv(w0));
    uint8x16_t reshuf0 = vqtbl1q_u8(vecbytes0,shuf0);
    uint8x16_t w1 = is_nonwhite(vecbytes1);
    uint8_t numberofkeptchars1 = bytepopcount(w1);
    uint8x16_t shuf1 = vld1q_u8(shufmask + 16 * neonmovemask_addv(w1));
    uint8x16_t reshuf1 = vqtbl1q_u8(vecbytes1,shuf1);
    uint8x16_t w2 = is_nonwhite(vecbytes2);
    uint8_t numberofkeptchars2 = bytepopcount(w2);
    uint8x16_t shuf2 = vld1q_u8(shufmask + 16 * neonmovemask_addv(w2));
    uint8x16_t reshuf2 = vqtbl1q_u8(vecbytes2,shuf2);
    uint8x16_t w3 = is_nonwhite(vecbytes3);
    uint8_t numberofkeptchars3 = bytepopcount(w3);
    uint8x16_t shuf3 = vld1q_u8(shufmask + 16 * neonmovemask_addv(w3));
    uint8x16_t reshuf3 = vqtbl1q_u8(vecbytes3,shuf3);

    vst1q_u8((uint8_t *)bytes + pos,reshuf0);
    pos += numberofkeptchars0;

    vst1q_u8((uint8_t *)bytes + pos,reshuf1);
    pos += numberofkeptchars1;

    vst1q_u8((uint8_t *)bytes + pos,reshuf2);
    pos += numberofkeptchars2;

    vst1q_u8((uint8_t *)bytes + pos,reshuf3);
    pos += numberofkeptchars3;

  }
  while (i < howmany) {
    const unsigned char c = bytes[i];
    bytes[pos] = c;
    pos += (c > 32) ? 1 : 0;
  }
  return pos;
}



#endif // end of file
