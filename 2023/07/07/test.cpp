#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <time.h>

#if (defined(__x86_64) || defined(__x86_64__))
#include <x86intrin.h>

// From "20141103 012910", we want to get
// 0x20141103012910
uint64_t extract_nibbles(const char *c) {
  uint64_t part1, part2;
  memcpy(&part1, c, sizeof(uint64_t));
  memcpy(&part2, c + 7, sizeof(uint64_t));
  part1 = _bswap64(part1);
  part2 = _bswap64(part2);
  part1 = _pext_u64(part1, 0x0f0f0f0f0f0f0f0f);
  part2 = _pext_u64(part2, 0x0f000f0f0f0f0f0f);
  return (part1 << 24) | (part2);
}
#elif defined(__aarch64__) || defined(_M_ARM64)
#include <arm_neon.h>

// From "20141103 012910", we want to get
// 0x20141103012910
uint64_t extract_nibbles(const char *c) {
  const uint8_t *ascii = (const uint8_t *)(c);
  uint8x16_t in = vld1q_u8(ascii);
  // masking the high nibbles,
  in = vandq_u8(in, vmovq_n_u8(0x0f));
  // shuffle the bytes
  const uint8x16_t shuf = {14, 13, 12, 11, 10, 9, 7,   6,
                           5,  4,  3,  2,  1,  0, 255, 255};
  in = vqtbl1q_u8(in, shuf);
  // then shift/or
  uint16x8_t ins =
      vsraq_n_u16(vreinterpretq_u16_u8(in), 
      vreinterpretq_u16_u8(in), 4);
  // then narrow (16->8),
  int8x8_t packed = vmovn_u16(ins);
  // extract to general register.
  return vget_lane_u64(vreinterpret_u64_u16(packed), 0);
}
#else
#error "Unsupported architecture"
#endif

int main() {
  const char *c = "20141103 012917  ";
  printf("%llx\n", extract_nibbles(c));
}