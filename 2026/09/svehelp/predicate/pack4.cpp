#include <arm_sve.h>
#include <arm_neon.h>
#include <arm_neon_sve_bridge.h>
#include <cstdint>
#include <cstdio>
#include <cstring>

// Daniel's version: store each predicate through a pointer cast, at 2-byte stride.
static inline uint64_t pack4_masks(svbool_t a, svbool_t b, svbool_t c, svbool_t d) {
    alignas(2) uint16_t w[4];
    *(svbool_t *)&w[0] = a;
    *(svbool_t *)&w[1] = b;
    *(svbool_t *)&w[2] = c;
    *(svbool_t *)&w[3] = d;
    uint64_t x;
    memcpy(&x, w, 8);
    return x;
}

// Same, but with room for the widest predicate (32 bytes at VL 2048) after w[3].
static inline uint64_t pack4_masks_padded(svbool_t a, svbool_t b, svbool_t c, svbool_t d) {
    alignas(2) uint16_t w[4 + 16];
    *(svbool_t *)&w[0] = a;
    *(svbool_t *)&w[1] = b;
    *(svbool_t *)&w[2] = c;
    *(svbool_t *)&w[3] = d;
    uint64_t x;
    memcpy(&x, w, 8);
    return x;
}

static inline svbool_t match_ops(uint8x16_t in) {
  const uint8x16_t operators = {0xff, ',', ':', '[', ']', '{', '}', 0xff,
                                ',',  ':', '[', ']', '{', '}', ',', ':'};
  return svmatch_u8(svptrue_pat_b8(SV_VL16), svset_neonq_u8(svundef_u8(), in),
                    svset_neonq_u8(svundef_u8(), operators));
}

#define CLASSIFY(NAME, PACK)                                                        \
  __attribute__((noinline)) uint64_t NAME(const uint8_t *in) {                      \
    return PACK(match_ops(vld1q_u8(in)), match_ops(vld1q_u8(in + 16)),              \
                match_ops(vld1q_u8(in + 32)), match_ops(vld1q_u8(in + 48)));        \
  }                                                                                 \
  __attribute__((noinline)) uint64_t NAME##_all(const uint8_t *in, size_t n) {      \
    uint64_t acc = 0;                                                               \
    for (size_t i = 0; i + 64 <= n; i += 64) {                                      \
      acc = (acc * 31) ^ PACK(match_ops(vld1q_u8(in + i)), match_ops(vld1q_u8(in + i + 16)), \
                              match_ops(vld1q_u8(in + i + 32)), match_ops(vld1q_u8(in + i + 48))); \
    }                                                                               \
    return acc;                                                                     \
  }
CLASSIFY(classify, pack4_masks)
CLASSIFY(classify_padded, pack4_masks_padded)

// Overflow probe: canary right after w in a struct with fixed layout.
__attribute__((noinline)) int overflow_probe(svbool_t a, svbool_t b, svbool_t c, svbool_t d) {
  struct { uint16_t w[4]; uint16_t canary[16]; } s;
  memset(&s, 0, sizeof(s));
  *(svbool_t *)&s.w[0] = a;
  *(svbool_t *)&s.w[1] = b;
  *(svbool_t *)&s.w[2] = c;
  *(svbool_t *)&s.w[3] = d;
  int clobbered = 0;
  for (int i = 0; i < 16; i++) { if (s.canary[i]) { clobbered = i + 1; } }
  return clobbered;  // number of canary uint16s written past w[3]
}

int main() {
  printf("running at VL %u bits (predicate = %u bytes)\n", unsigned(svcntb() * 8), unsigned(svcntb() / 8));
  alignas(64) uint8_t buf[256];
  for (int i = 0; i < 256; i++) { buf[i] = "ab,c:d[e]f{g}h  x,y:z[w]v{u}t \"\\"[i % 32]; }
  uint64_t expect[4] = {0, 0, 0, 0};
  for (int i = 0; i < 256; i++) { if (strchr(",:[]{}", buf[i]) && buf[i]) { expect[i / 64] |= uint64_t(1) << (i % 64); } }
  uint64_t acc = 0;
  for (int k = 0; k < 4; k++) { acc = (acc * 31) ^ expect[k]; }
  printf("classify:        %s\n", classify(buf) == expect[0] && classify_all(buf, 256) == acc ? "OK" : "WRONG");
  printf("classify_padded: %s\n", classify_padded(buf) == expect[0] && classify_padded_all(buf, 256) == acc ? "OK" : "WRONG");
  svbool_t all = svptrue_b8();
  printf("overflow past w[3]: %d uint16 slots\n", overflow_probe(all, all, all, all));
  return 0;
}
