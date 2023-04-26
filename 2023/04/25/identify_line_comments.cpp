#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#if __ARM_NEON__
#include "bigtable.h"
#include <arm_neon.h>
#endif

struct bitmask {
  explicit bitmask(size_t capacity)
      : line_end((capacity + 63) / 64), hash((capacity + 63) / 64),
        comment((capacity + 63) / 64) {}
  std::vector<uint64_t> line_end;
  std::vector<uint64_t> hash;
  std::vector<uint64_t> comment;
};

#if __ARM_NEON__
void compress(uint8x16_t v, uint16_t mask, uint8_t *output) {
  uint8_t mask1 = uint8_t(mask);      // least significant 8 bits
  uint8_t mask2 = uint8_t(mask >> 8); // most significant 8 bits
  uint64x2_t shufmask64 = {thintable_epi8[mask1], thintable_epi8[mask2]};
  uint8x16_t shufmask = vreinterpretq_u8_u64(shufmask64);
  uint8x16_t inc = {0,    0,    0,    0,    0,    0,    0,    0,
                    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08};
  shufmask = vaddq_u8(shufmask, inc);
  uint8x16_t pruned = vqtbl1q_u8(v, shufmask);
  int pop1 = BitsSetTable256mul2[mask1];
  uint8x16_t compactmask = vld1q_u8(
      reinterpret_cast<const uint8_t *>(pshufb_combine_table + pop1 * 8));
  uint8x16_t answer = vqtbl1q_u8(pruned, compactmask);
  vst1q_u8(output, answer);
}
uint16_t to_bitmask(uint8x16_t v) {
  const uint8x16_t bit_mask = {0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80,
                               0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
  uint8x16_t minput = vandq_u8(v, bit_mask);
  uint8x16_t tmp = vpaddq_u8(minput, minput);
  tmp = vpaddq_u8(tmp, tmp);
  tmp = vpaddq_u8(tmp, tmp);
  return vgetq_lane_u16(vreinterpretq_u16_u8(tmp), 0);
}
bitmask compute_bitmask(std::string_view input) {
  bitmask answer(input.size() + 1);
  if (input.empty()) {
    return answer;
  }
  // beginning of the file.
  size_t i = 0;
  uint8x16_t endofline = vdupq_n_u8('\n');
  uint8x16_t hash = vdupq_n_u8('#');

  for (; i + 16 <= input.size(); i += 16) {
    uint8x16_t data = vld1q_u8((const uint8_t *)input.data() + i);
    uint16_t endoflinemask = to_bitmask(vceqq_u8(data, endofline));
    uint16_t hashmask = to_bitmask(vceqq_u8(data, hash));
    memcpy(reinterpret_cast<uint8_t *>(answer.line_end.data()) + i / 8,
           &endoflinemask, sizeof(uint16_t));
    memcpy(reinterpret_cast<uint8_t *>(answer.hash.data()) + i / 8, &hashmask,
           sizeof(uint16_t));
  }
  for (; i < input.size(); i++) {
    if (input[i] ==
        '\n') { // we consider that right after '\n', we start a new line.
      answer.line_end[i / 64] |= uint64_t(1) << (i % 64);
    }
    if (input[i] == '#') {
      answer.hash[i / 64] |= uint64_t(1) << (i % 64);
    }
  }
  // We end with a virtual '#' at the end of the file.
  answer.hash[input.size() / 64] |= uint64_t(1) << (input.size() % 64);
  return answer;
}
#else
// scalar version
bitmask compute_bitmask(std::string_view input) {
  bitmask answer(input.size() + 1);
  if (input.empty()) {
    return answer;
  }
  // beginning of the file.
  for (size_t i = 0; i < input.size(); i++) {
    if (input[i] ==
        '\n') { // we consider that right after '\n', we start a new line.
      answer.line_end[i / 64] |= uint64_t(1) << (i % 64);
    }
    if (input[i] == '#') {
      answer.hash[i / 64] |= uint64_t(1) << (i % 64);
    }
  }
  // We end with a virtual '#' at the end of the file.
  answer.hash[input.size() / 64] |= uint64_t(1) << (input.size() % 64);
  return answer;
}
#endif

void comment_trimmer(bitmask &b) {
  bool overflow = 1;
  for (size_t i = 0; i < b.line_end.size(); i++) {
    overflow = __builtin_usubll_overflow(b.hash[i], b.line_end[i] + overflow,
                                         &b.comment[i]);
    b.comment[i] &=
        ~b.hash[i]; // when there is more than one #, we want to remove it.
    b.comment[i] |= b.line_end[i]; // we want to keep the line start bits.
  }
}

size_t compress(bitmask &b, std::string_view view, uint8_t *out) {
  size_t pos = 0;
  size_t i = 0;
#if __ARM_NEON__
  for (; i + 16 <= view.size(); i += 16) {
    uint8x16_t data = vld1q_u8((const uint8_t *)view.data() + i);
    uint16_t mask = reinterpret_cast<uint16_t *>(b.comment.data())[i / 16];
    compress(data, mask ^ 0xffff, out + pos);
    pos += __builtin_popcount(mask);
  }
#endif

  for (; i < view.size(); i++) {
    if (b.comment[i / 64] & (uint64_t(1) << (i % 64))) {
      out[pos++] = view[i];
    }
  }
  return pos;
}

void process_and_print(std::string_view view) {
  bitmask b = compute_bitmask(view);
  comment_trimmer(b);
  std::string compressed(view);
  size_t hm = compress(b, view, reinterpret_cast<uint8_t *>(compressed.data()));
  compressed.resize(hm);
  std::cout << compressed << std::endl;
}

std::string test_input = R"(




[build-system] # use this to indicate the name of the project
something # hello
fdsfds
fdsfds

requires = ["setuptools>=61.0"] # # # fdsfsd fds# fdsfds fdsfds
)";

std::string test_input2 =
    R"([build-system] # use this to indicate the name of the project
requires = ["setuptools>=61.0"]
build-backend = "setuptools.build_meta"

[project]
name = "gyp-next"
version = "0.14.0"
authors = [
  { name="Node.js contributors", email="ryzokuken@disroot.org" },
]
description = "A fork of the GYP build system for use in the Node.js projects"
readme = "README.md"
license = { file="LICENSE" }
requires-python = ">=3.6"
classifiers = [
    "Development Status :: 3 - Alpha",
    "Environment :: Console",
    "Intended Audience :: Developers",
    "License :: OSI Approved :: BSD License",
    "Natural Language :: English",
    "Programming Language :: Python",
    "Programming Language :: Python :: 3",
    "Programming Language :: Python :: 3.6",
    "Programming Language :: Python :: 3.7",
    "Programming Language :: Python :: 3.8",
    "Programming Language :: Python :: 3.9",
    "Programming Language :: Python :: 3.10",
]

[project.optional-dependencies]
dev = ["flake8", "pytest"]

[project.scripts]
gyp = "gyp:script_main"

[project.urls]
"Homepage" = "https://github.com/nodejs/gyp-next"

[tool.setuptools]
package-dir = {"" = "pylib"}
packages = ["gyp", "gyp.generator"]    # looks good

# end of the file
)";

int main() { process_and_print(test_input2); }