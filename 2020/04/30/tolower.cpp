// Adapted from Muła
// credit : Wojciech Muła
// http://0x80.pl/notesen/2016-01-06-swar-swap-case.html
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <float.h>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>
using clk = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<clk>;
using dur_double = std::chrono::duration<double>;
using std::chrono::duration_cast;

class Timer {
public:
  Timer(const std::string &cmd) : _cmd{cmd}, _start{clk::now()} {};

  double time_ns() {
    auto duration = clk::now() - _start;
    auto elapsed_s = duration_cast<dur_double>(duration).count();
    return elapsed_s * 1000 * 1000 * 1000;
  }

  ~Timer(){};

private:
  std::string _cmd;
  time_point _start;
};

constexpr uint64_t packed_byte(uint8_t b) {
  // replicate the byte 8 times
  return uint64_t(b) * uint64_t(0x0101010101010101);
}

uint64_t to_lower_ascii(uint64_t chars) {
  const uint64_t ascii_chars = chars & packed_byte(0x7f);
  const uint64_t is_not_ascii_chars = chars & packed_byte(0x80);
  const uint64_t A =
      ascii_chars +
      packed_byte(
          128 - 'A'); // MSB[i] is set if chars[i] is greater or equal than 'A'
  const uint64_t Z =
      ascii_chars +
      packed_byte(128 - 'Z' -
                  1); // MSB[i] is set if chars[i] is greater than 'Z' - 1
  uint64_t mask_lower = (A ^ Z) & packed_byte(0x80);
  mask_lower &= ~is_not_ascii_chars; // only transform when ASCII
  return chars ^ (mask_lower >> 2);
}

/**
 * This function compares the bytes in input1 and input2. Whenever, at a given
 * index, we have ASCII bytes in each of the string (most significant bit is 0),
 * then we compare them while ignoring the case. For other bytes (where the most
 * significant bit is 1), we make no comparison.
 */
bool is_ascii_equal_ignoring_case(const char *input1, const char *input2,
                                  size_t len) {
  size_t i = 0;
  for (; i + sizeof(uint64_t) <= len; i += sizeof(uint64_t)) {
    uint64_t w1, w2;
    memcpy(&w1, input1 + i, sizeof(w1));
    memcpy(&w2, input2 + i, sizeof(w2));
    if (to_lower_ascii(w1) != to_lower_ascii(w2)) {
      return false;
    }
  }
  // handle tail
  uint64_t w1 = 0;
  uint64_t w2 = 0;
  memcpy(&w1, input1 + i, len - i);
  memcpy(&w2, input2 + i, len - i);
  if (to_lower_ascii(w1) != to_lower_ascii(w2)) {
    return false;
  }
  return true;
}

// compare two 8-byte strings. If any non-ASCII, return false.
// Otherwise, compare as
bool compare_ascii(uint64_t w1, uint64_t w2) {
  const uint64_t diff = w1 ^ w2;          // only valid diff at 0x20
  if ((diff & 0xdfdfdfdfdfdfdfdf) != 0) { //  ~0x20 = 0xdf
    return false;
  }
  // check if any non-ascii
  const uint64_t any_non_ascii = (w1 | w2) & packed_byte(0x80);
  if (any_non_ascii) {
    return false;
  }

  // validate if w1 contains only A-Z/a-z chars (then w2 must also be in this
  // range)
  w1 |= packed_byte(0x20);

  const uint64_t A = w1 + packed_byte(128 - 'A');
  const uint64_t Z = w1 + packed_byte(128 - 'Z' - 1);
  const uint64_t mask_lower = (A ^ Z) & packed_byte(0x80);

  return mask_lower == packed_byte(0x80);
}

bool is_ascii_equal_ignoring_case_mula(const char *input1, const char *input2,
                                       size_t len) {
  size_t i = 0;
  for (; i + sizeof(uint64_t) <= len; i += sizeof(uint64_t)) {
    uint64_t w1, w2;
    memcpy(&w1, input1 + i, sizeof(w1));
    memcpy(&w2, input2 + i, sizeof(w2));
    if (compare_ascii(w1, w2)) {
      return false;
    }
  }
  // handle tail
  uint64_t w1 = 0;
  uint64_t w2 = 0;
  memcpy(&w1, input1 + i, len - i);
  memcpy(&w2, input2 + i, len - i);
  if (compare_ascii(w1, w2)) {
    return false;
  }
  return true;
}

// If there is any change, it must be [a-z]<=>[A-Z]. Yes? If so return true.
bool equal_ascii_ignore_case(uint64_t w1, uint64_t w2) {
  const uint64_t diff = w1 ^ w2; // only valid diff at 0x20
  // is_diff is where we have a difference, any bit set outside 0x80 will get a
  // false return
  const uint64_t is_diff = (diff << 2) | (diff >> (64 - 2)); // rotate
  // check if any non-ascii
  const uint64_t non_ascii = (w1 | w2) & packed_byte(0x80);
  const uint64_t ascii = ~non_ascii;
  w1 |= packed_byte(
      0x20); // if it were in A-Z, it becomes a-z, otherwise, a-z is unchanged
  w1 &= packed_byte(0x7f); // set high bits to zero so the following work
  const uint64_t A = w1 + packed_byte(128 - 'a');
  const uint64_t Z = w1 + packed_byte(128 - 'z' - 1);
  // mask_lower indicate [a-zA-Z], check that it matches our diff
  const uint64_t mask_lower = ((A ^ Z) & packed_byte(0x80)) & ascii;
  return (mask_lower & is_diff) == is_diff;
}

bool is_ascii_equal_ignoring_case_mula2(const char *input1, const char *input2,
                                        size_t len) {
  size_t i = 0;
  for (; i + sizeof(uint64_t) <= len; i += sizeof(uint64_t)) {
    uint64_t w1, w2;
    memcpy(&w1, input1 + i, sizeof(w1));
    memcpy(&w2, input2 + i, sizeof(w2));
    if (!equal_ascii_ignore_case(w1, w2)) {
      return false;
    }
  }
  // handle tail
  uint64_t w1 = 0;
  uint64_t w2 = 0;
  memcpy(&w1, input1 + i, len - i);
  memcpy(&w2, input2 + i, len - i);
  if (!equal_ascii_ignore_case(w1, w2)) {
    return false;
  }
  return true;
}

void demo(size_t N) {
  std::cout << " N = " << N << std::endl;
  srand(time(NULL));
  double t1 = DBL_MAX;
  double t2 = DBL_MAX;
  double t3 = DBL_MAX;
  double t4 = DBL_MAX;
  double t5 = DBL_MAX;

  double tz;

  for (size_t z = 0; z < 10; z++) {
    std::vector<uint8_t> random;
    for (size_t z = 0; z < N; z++) {
      uint8_t c = (rand() % 94) + 32;
      random.push_back(c);
    }
    std::vector<uint8_t> randomlower(random);
    for (size_t z = 0; z + sizeof(uint64_t) <= N; z += sizeof(uint64_t)) {
      uint64_t w;
      memcpy(&w, randomlower.data() + z, sizeof(w));
      uint64_t w2 = to_lower_ascii(w);
      memcpy(randomlower.data() + z, &w2, sizeof(w2));
    }
    for (size_t trial = 0; trial < 5; trial++) {
      auto t = Timer{"fast"};
      bool is_the_same = is_ascii_equal_ignoring_case(
          (const char *)random.data(), (const char *)randomlower.data(), N);
      if (!is_the_same) {
        std::cerr << "bug" << std::endl;
        abort();
      }
      tz = t.time_ns() / N;
      if (tz < t1)
        t1 = tz;
      t = Timer{"slow"};
      is_the_same = (strncasecmp((const char *)random.data(),
                                 (const char *)randomlower.data(), N) == 0);
      if (!is_the_same) {
        std::cerr << "bug" << std::endl;
        abort();
      }
      tz = t.time_ns() / N;
      if (tz < t2)
        t2 = tz;
      t = Timer{"slow!"};
      for (size_t i = 0; i < N; i++) {
        if (tolower(random[i]) != tolower(randomlower[i])) {
          is_the_same = false;
          break;
        }
      }
      if (!is_the_same) {
        std::cerr << "bug" << std::endl;
        abort();
      }
      tz = t.time_ns() / N;
      if (tz < t3)
        t3 = tz;
      t = Timer{"mula"};
      is_the_same = (is_ascii_equal_ignoring_case_mula(
          (const char *)random.data(), (const char *)randomlower.data(), N));
      if (!is_the_same) {
        std::cerr << "bug" << std::endl;
        abort();
      }
      if (tz < t4)
        t4 = tz;
      t = Timer{"mula (2) "};
      is_the_same = (is_ascii_equal_ignoring_case_mula2(
          (const char *)random.data(), (const char *)randomlower.data(), N));
      if (!is_the_same) {
        std::cerr << "bug" << std::endl;
        abort();
      }
      tz = t.time_ns() / N;
      if (tz < t5)
        t5 = tz;
    }
  }
  std::cout << "hand rolled " << t1 << std::endl;
  std::cout << "strncasecmp " << t2 << std::endl;
  std::cout << "tolower " << t3 << std::endl;
  std::cout << "mula " << t4 << std::endl;
  std::cout << "mula (2) " << t5 << std::endl;
  std::cout << std::endl;
}

int main() {
  demo(12);
  demo(10000);
}
