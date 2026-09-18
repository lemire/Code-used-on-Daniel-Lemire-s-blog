// Converting four SVE predicates (16 active byte lanes each) into one 64-bit mask.
// Compile: g++ -O3 -march=armv9-a+sve2 pred.cpp
#include <arm_neon.h>
#include <arm_neon_sve_bridge.h>
#include <arm_sve.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>

#define INLINE __attribute__((always_inline)) inline

INLINE svbool_t match_operators_sve2(uint8x16_t input) {
  const uint8x16_t operators = {0xff, ',', ':', '[', ']', '{', '}', 0xff,
                                ',',  ':', '[', ']', '{', '}', ',', ':'};
  const svbool_t pg = svptrue_pat_b8(SV_VL16);
  const svuint8_t data = svset_neonq_u8(svundef_u8(), input);
  const svuint8_t table = svset_neonq_u8(svundef_u8(), operators);
  return svmatch_u8(pg, data, table);
}

// ---- Version A: the pull request (inline asm, store predicate to the stack) ----
#define STORE_PREDICATE(PREDICATE, DESTINATION)                                 \
  __asm__ volatile("str %0, [%1]" : : "Upl"(PREDICATE), "r"(DESTINATION) : "memory")

INLINE uint64_t predicates_to_mask_asm(svbool_t p0, svbool_t p1, svbool_t p2,
                                       svbool_t p3) {
  alignas(16) uint8_t stored[4][32];
  STORE_PREDICATE(p0, stored[0]);
  STORE_PREDICATE(p1, stored[1]);
  STORE_PREDICATE(p2, stored[2]);
  STORE_PREDICATE(p3, stored[3]);
  uint16_t m0, m1, m2, m3;
  memcpy(&m0, stored[0], 2);
  memcpy(&m1, stored[1], 2);
  memcpy(&m2, stored[2], 2);
  memcpy(&m3, stored[3], 2);
  return uint64_t(m0) | (uint64_t(m1) << 16) | (uint64_t(m2) << 32) |
         (uint64_t(m3) << 48);
}

// ---- Version B: portable intrinsics (NEON-SVE bridge) ----
// svsel picks the bit weight where the predicate is true and 0 elsewhere:
// it does the "materialize + AND with weights" in one instruction. The rest
// is the usual NEON addp tree that simdjson already uses for its NEON masks.
INLINE uint64_t predicates_to_mask_bridge(svbool_t p0, svbool_t p1, svbool_t p2,
                                          svbool_t p3) {
  const svuint8_t w = svdupq_n_u8(1, 2, 4, 8, 16, 32, 64, 128,
                                  1, 2, 4, 8, 16, 32, 64, 128);
  const svuint8_t zero = svdup_n_u8(0);
  uint8x16_t b0 = svget_neonq_u8(svsel_u8(p0, w, zero));
  uint8x16_t b1 = svget_neonq_u8(svsel_u8(p1, w, zero));
  uint8x16_t b2 = svget_neonq_u8(svsel_u8(p2, w, zero));
  uint8x16_t b3 = svget_neonq_u8(svsel_u8(p3, w, zero));
  uint8x16_t s0 = vpaddq_u8(b0, b1);
  uint8x16_t s1 = vpaddq_u8(b2, b3);
  uint8x16_t s = vpaddq_u8(s0, s1);
  s = vpaddq_u8(s, s);
  return vgetq_lane_u64(vreinterpretq_u64_u8(s), 0);
}

// ---- Version C: SVE2 BEXT (bit permute) + tbl gather ----
// svbext on 64-bit lanes extracts the top bit of each byte into an 8-bit value,
// one per 64-bit lane; a 4-register NEON tbl then gathers the 8 result bytes.
INLINE uint64_t predicates_to_mask_bext(svbool_t p0, svbool_t p1, svbool_t p2,
                                        svbool_t p3) {
  const svuint64_t msb = svdup_n_u64(0x8080808080808080ULL);
  svuint64_t e0 = svbext_u64(svreinterpret_u64_u8(svdup_n_u8_z(p0, 0xff)), msb);
  svuint64_t e1 = svbext_u64(svreinterpret_u64_u8(svdup_n_u8_z(p1, 0xff)), msb);
  svuint64_t e2 = svbext_u64(svreinterpret_u64_u8(svdup_n_u8_z(p2, 0xff)), msb);
  svuint64_t e3 = svbext_u64(svreinterpret_u64_u8(svdup_n_u8_z(p3, 0xff)), msb);
  uint8x16x4_t t = {svget_neonq_u8(svreinterpret_u8_u64(e0)),
                    svget_neonq_u8(svreinterpret_u8_u64(e1)),
                    svget_neonq_u8(svreinterpret_u8_u64(e2)),
                    svget_neonq_u8(svreinterpret_u8_u64(e3))};
  const uint8x16_t idx = {0, 8, 16, 24, 32, 40, 48, 56, 0, 0, 0, 0, 0, 0, 0, 0};
  return vgetq_lane_u64(vreinterpretq_u64_u8(vqtbl4q_u8(t, idx)), 0);
}

// ---- Version D: memcpy from the predicate object (no asm) ----
// A predicate is a sizeless object, but it has an address, and reading its
// first two bytes with memcpy gives the first 16 lanes (STR P layout: one bit
// per byte lane). Compilers implement the copy with str p + ldrh.
INLINE uint64_t predicates_to_mask_memcpy(svbool_t p0, svbool_t p1, svbool_t p2,
                                          svbool_t p3) {
  uint16_t m0, m1, m2, m3;
  memcpy(&m0, &p0, sizeof(m0));
  memcpy(&m1, &p1, sizeof(m1));
  memcpy(&m2, &p2, sizeof(m2));
  memcpy(&m3, &p3, sizeof(m3));
  return uint64_t(m0) | (uint64_t(m1) << 16) | (uint64_t(m2) << 32) |
         (uint64_t(m3) << 48);
}

// ---- the three classifiers over a 64-byte block ----
#define CLASSIFY(NAME, CONVERT)                                                \
  __attribute__((noinline)) uint64_t NAME(const uint8_t *in) {                 \
    uint8x16_t d0 = vld1q_u8(in), d1 = vld1q_u8(in + 16);                      \
    uint8x16_t d2 = vld1q_u8(in + 32), d3 = vld1q_u8(in + 48);                 \
    return CONVERT(match_operators_sve2(d0), match_operators_sve2(d1),         \
                   match_operators_sve2(d2), match_operators_sve2(d3));         \
  }
CLASSIFY(classify_asm, predicates_to_mask_asm)
CLASSIFY(classify_bridge, predicates_to_mask_bridge)
CLASSIFY(classify_bext, predicates_to_mask_bext)
CLASSIFY(classify_memcpy, predicates_to_mask_memcpy)

uint64_t classify_scalar(const uint8_t *in) {
  uint64_t m = 0;
  for (int i = 0; i < 64; i++) {
    // 0xff is the padding byte of the operator table, so it matches too
    // (it cannot occur in valid UTF-8).
    if ((strchr(",:[]{}", in[i]) && in[i] != 0) || in[i] == 0xff) { m |= uint64_t(1) << i; }
  }
  return m;
}

int main() {
  printf("SVE vector length: %u bits\n", unsigned(svcntb() * 8));
  std::mt19937_64 rng(42);
  const char alphabet[] = "abc ,:[]{}\"\\\n0123456789\xff\x80";
  alignas(64) uint8_t buf[64];
  for (int iter = 0; iter < 200000; iter++) {
    for (auto &c : buf) { c = alphabet[rng() % (sizeof(alphabet) - 1)]; }
    uint64_t r = classify_scalar(buf);
    uint64_t a = classify_asm(buf), b = classify_bridge(buf), c = classify_bext(buf), d = classify_memcpy(buf);
    if (a != r || b != r || c != r || d != r) {
      printf("MISMATCH iter %d: ref %016llx asm %016llx bridge %016llx bext %016llx memcpy %016llx\n",
             iter, (unsigned long long)r, (unsigned long long)a,
             (unsigned long long)b, (unsigned long long)c, (unsigned long long)d);
      return 1;
    }
  }
  printf("all four agree with the scalar reference\n");
  return 0;
}
