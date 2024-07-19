
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <string_view>

#include "vectorclassification.h"
std::string load_file_content(std::string filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return "";
  }

  std::string content;
  std::string line;
  while (std::getline(file, line)) {
    content += line + "\n";
  }
  file.close();
  return content;
}

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.2f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

#if defined(__aarch64__)

struct neon_match {
  neon_match(const char *start, const char *end) : start(start), end(end) {
    low_nibble_mask = {0, 0, 0, 0, 0, 0, 0x26, 0, 0, 0, 0, 0, 0x3c, 0xd, 0, 0};
    v0f = vmovq_n_u8(0xf);
    offset = 0;
    if (start + 16 >= end) {
      char buffer[16]{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
      memcpy(buffer, start, end - start);
      update(buffer);
    } else {
      update();
    }
  }
  const char *get() const { return start + offset; }
  // Call consume after you have called advance() to move on.
  void consume() {
    offset++;
    matches >>= 4;
  }

  // move to the next match, when starting out, it moves you to the first value
  // (if there is one), otherwise it moves you to the next value. If you are already
  // at a match, you will remain at that match. You need to call consume to move on.
  // It returns false if there are no more matches.
  bool advance() {
    while (matches == 0) {
      start += 16;
      if (start >= end) {
        return false;
      }
      if (start + 16 >= end) {
        char buffer[16]{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        memcpy(buffer, start, end - start);
        update(buffer);
        if (matches == 0) {
          return false;
        }
      } else {
        update();
      }
    }
    int off = __builtin_ctzll(matches);
    matches >>= off;
    offset += off >> 2;
    return true;
  }

private:
  inline void update() { update(start); }
  inline void update(const char *buffer) {
    uint8x16_t data = vld1q_u8(reinterpret_cast<const uint8_t *>(buffer));
    uint8x16_t lowpart = vqtbl1q_u8(low_nibble_mask, vandq_u8(data, v0f));
    uint8x16_t matchesones = vceqq_u8(lowpart, data);
    const uint8x8_t res = vshrn_n_u16(matchesones, 4);
    matches = vget_lane_u64(vreinterpret_u64_u8(res), 0);
    offset = 0;
  }

  const char *start;
  const char *end;
  size_t offset;
  uint64_t matches{};
  uint8x16_t low_nibble_mask;
  uint8x16_t v0f;
};

bool check(const char *start, const char *end) {
  const char *targets = "<&\r\0";
  const char *expected = start;
  expected =
      std::find_first_of(expected, end, targets, targets + strlen(targets));

  neon_match m(start, end);
  while (m.advance()) {
    if (m.get() != expected) {
      return false;
    }
    m.consume();
    if (expected < end)
      expected++;
    expected =
        std::find_first_of(expected, end, targets, targets + strlen(targets));
  }
  if (expected != end) {
    return false;
  }
  return true;
}
#endif

int main(int argc, char **argv) {
  std::string data = load_file_content("data.html");
  size_t volume = data.size();
  size_t size = 1;
  volatile uint64_t count = 0;
  const char *targets = "<&\r\0";
  pretty_print(size, volume, "std", bench([&data, &count, targets]() {
                 const char *start = data.data();
                 const char *end = start + data.size();
                 while (start < end) {
                   count = *start;
                   start = std::find_first_of(start, end, targets,
                                              targets + strlen(targets));
                   if (start < end)
                     start++;
                 }
               }));

#if defined(__aarch64__)
  {
    const char *start = data.data();
    const char *end = start + data.size();
    check(start, end);
  }

  pretty_print(size, volume, "neon", bench([&data, &count]() {
                 const char *start = data.data();
                 const char *end = start + data.size();
                 neon_match m(start, end);
                 while (m.advance()) {
                   count = *m.get();
                   m.consume();
                 }
               }));

#endif
  pretty_print(size, volume, "NaiveAdvanceString", bench([&data, &count]() {
                 const char *start = data.data();
                 const char *end = start + data.size();
                 while (start < end) {
                   count = *start;
                   NaiveAdvanceString(start, end);
                   if (start < end)
                     start++;
                 }
               }));
  pretty_print(size, volume, "AdvanceString", bench([&data, &count]() {
                 const char *start = data.data();
                 const char *end = start + data.size();
                 while (start < end) {
                   count = *start;
                   AdvanceString(start, end);
                   if (start < end)
                     start++;
                 }
               }));
  pretty_print(size, volume, "AdvanceStringTable", bench([&data, &count]() {
                 const char *start = data.data();
                 const char *end = start + data.size();
                 while (start < end) {
                   count = *start;
                   AdvanceStringTable(start, end);
                   if (start < end)
                     start++;
                 }
               }));
  pretty_print(size, volume, "AdvanceStringTableSimpler",
               bench([&data, &count]() {
                 const char *start = data.data();
                 const char *end = start + data.size();
                 while (start < end) {
                   count = *start;
                   AdvanceStringTableSimpler(start, end);
                   if (start < end)
                     start++;
                 }
               }));
  pretty_print(size, volume, "AdvanceStringTableSimpler2",
               bench([&data, &count]() {
                 const char *start = data.data();
                 const char *end = start + data.size();
                 while (start < end) {
                   count = *start;
                   AdvanceStringTableSimpler2(start, end);
                   if (start < end)
                     start++;
                 }
               }));
}
