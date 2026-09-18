#include <arm_sve.h>
#include <arm_neon.h>
#include <arm_neon_sve_bridge.h>
#include <cstdint>
#include <cstdio>
#include <cstring>

typedef svbool_t fixed_pred __attribute__((arm_sve_vector_bits(__ARM_FEATURE_SVE_BITS)));

static inline uint16_t mask16(svbool_t p) {
    fixed_pred fp = p;
    uint16_t bits;
    __builtin_memcpy(&bits, &fp, 2);   // first 16 byte-lanes
    return bits;
}

__attribute__((noinline)) uint16_t mask16_noinline(svbool_t p) { return mask16(p); }

static inline svbool_t match_ops(uint8x16_t in) {
  const uint8x16_t operators = {0xff, ',', ':', '[', ']', '{', '}', 0xff,
                                ',',  ':', '[', ']', '{', '}', ',', ':'};
  return svmatch_u8(svptrue_pat_b8(SV_VL16), svset_neonq_u8(svundef_u8(), in),
                    svset_neonq_u8(svundef_u8(), operators));
}

__attribute__((noinline)) uint64_t classify(const uint8_t *in) {
  uint64_t m0 = mask16(match_ops(vld1q_u8(in)));
  uint64_t m1 = mask16(match_ops(vld1q_u8(in + 16)));
  uint64_t m2 = mask16(match_ops(vld1q_u8(in + 32)));
  uint64_t m3 = mask16(match_ops(vld1q_u8(in + 48)));
  return m0 | (m1 << 16) | (m2 << 32) | (m3 << 48);
}

// a loop, to see whether the stack-slot addresses are hoisted
__attribute__((noinline)) uint64_t classify_all(const uint8_t *in, size_t n) {
  uint64_t acc = 0;
  for (size_t i = 0; i + 64 <= n; i += 64) {
    uint64_t m0 = mask16(match_ops(vld1q_u8(in + i)));
    uint64_t m1 = mask16(match_ops(vld1q_u8(in + i + 16)));
    uint64_t m2 = mask16(match_ops(vld1q_u8(in + i + 32)));
    uint64_t m3 = mask16(match_ops(vld1q_u8(in + i + 48)));
    acc ^= m0 | (m1 << 16) | (m2 << 32) | (m3 << 48);
  }
  return acc;
}

int main() {
  printf("compiled for %d-bit SVE, running at %u bits, sizeof(fixed_pred)=%zu\n",
         __ARM_FEATURE_SVE_BITS, unsigned(svcntb() * 8), sizeof(fixed_pred));
  alignas(64) uint8_t buf[128];
  for (int i = 0; i < 128; i++) { buf[i] = "ab,c:d[e]f{g}h  "[i % 16]; }
  uint64_t expect = 0;
  for (int i = 0; i < 64; i++) { if (strchr(",:[]{}", buf[i]) && buf[i]) { expect |= uint64_t(1) << i; } }
  uint64_t got = classify(buf), got2 = classify_all(buf, 128);
  printf("classify: %016llx expected %016llx %s\n", (unsigned long long)got, (unsigned long long)expect, got == expect ? "OK" : "WRONG");
  printf("classify_all (two identical blocks, xor): %016llx expected 0 %s\n", (unsigned long long)got2, got2 == 0 ? "OK" : "WRONG");
  return 0;
}
