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
    uint8x16x4_t vecbytes = vld4q_u8((uint8_t *)bytes + i);
    uint8x16_t w0 = is_white(vecbytes.val[0]);
    uint64_t haswhite0 = is_not_zero(w0);
    w0 = vaddq_u8(justone, w0);
    uint8x16_t w1 = is_white(vecbytes.val[1]);
    uint64_t haswhite1 = is_not_zero(w1);
    w1 = vaddq_u8(justone, w1);
    uint8x16_t w2 = is_white(vecbytes.val[2]);
    uint64_t haswhite2 = is_not_zero(w2);
    w2 = vaddq_u8(justone, w2);
    uint8x16_t w3 = is_white(vecbytes.val[3]);
    uint64_t haswhite3 = is_not_zero(w3);
    w3 = vaddq_u8(justone, w3);
    if(!haswhite0) {
      vst1q_u8((uint8_t *)bytes + pos,vecbytes.val[0]);
      pos += 16;
      i += 16;
     } else {
      for (int k = 0; k < 16; k++) {
        bytes[pos] = bytes[i++];
        pos += w0[k];
      }
    }
    if(!haswhite1) {
      vst1q_u8((uint8_t *)bytes + pos,vecbytes.val[1]);
      pos += 16;
      i += 16;
     } else {
       for (int k = 0; k < 16; k++) {
        bytes[pos] = bytes[i++];
        pos += w1[k];
      }
    }
    if(!haswhite2) {
      vst1q_u8((uint8_t *)bytes + pos,vecbytes.val[2]);
      pos += 16;
      i += 16;
     } else {
       for (int k = 0; k < 16; k++) {
        bytes[pos] = bytes[i++];
        pos += w2[k];
      }
    }
    if(!haswhite3) {
      vst1q_u8((uint8_t *)bytes + pos,vecbytes.val[3]);
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


#endif // end of file
