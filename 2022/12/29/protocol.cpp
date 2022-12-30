
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>
#include <unordered_set>
#include <string_view>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

static inline uint64_t string_to_uint64(std::string_view view) {
  uint64_t val;
  std::memcpy(&val, view.data(), sizeof(uint64_t));
  return val;
}

uint32_t string_to_uint32(const char *data) {
  uint32_t val;
  std::memcpy(&val, data, sizeof(uint32_t));
  return val;
}

bool branchless_is_special(std::string_view input) {
  uint64_t inputu = string_to_uint64(input);
  uint64_t https = string_to_uint64("https\0\0\0");
  uint32_t http = string_to_uint32("https");
  uint32_t file = string_to_uint32("file");
  uint32_t ftp = string_to_uint32("ftp\0");
  uint32_t wss = string_to_uint32("wss\0");
  uint32_t ws = string_to_uint32("ws\0\0");

  return (((inputu & 0xffffffffff) == https) & (input.size() == 5)) |
         ((((inputu & 0xffffffff) == http) | ((inputu & 0xffffffff) == file)) &
          (input.size() == 4))

         | ((((inputu & 0xffffff) == ftp) | ((inputu & 0xffffff) == wss)) &
            (input.size() == 3)) |
         (((inputu & 0xffff) == ws) & (input.size() == 2));
}

bool fast_is_special(std::string_view input) {
  uint64_t inputu = string_to_uint64(input);
  if ((inputu & 0xffffffffff) == string_to_uint64("https\0\0\0")) {
    return input.size() == 5;
  }
  if ((inputu & 0xffffffff) == string_to_uint64("http\0\0\0\0")) {
    return input.size() == 4;
  }
  if (uint32_t(inputu) == string_to_uint32("file")) {
    return input.size() == 4;
  }
  if ((inputu & 0xffffff) == string_to_uint32("ftp\0")) {
    return input.size() == 3;
  }
  if ((inputu & 0xffffff) == string_to_uint32("wss\0")) {
    return input.size() == 3;
  }
  if ((inputu & 0xffff) == string_to_uint32("ws\0\0")) {
    return input.size() == 2;
  }
  return false;
}
bool default_is_special(std::string_view input) {
  return (input == "https") | (input == "http") | (input == "ftp") |
         (input == "file") | (input == "ws") | (input == "wss");
}

static const std::unordered_set<std::string_view> special_set = {
    "ftp", "file", "http", "https", "ws", "wss"};
bool hash_is_special(std::string_view input) {
  return special_set.find(input) != special_set.end();
}

std::vector<std::string_view> populate(size_t length) {
  std::mt19937 gen;
  // we generate a distribution where https is more common
  std::discrete_distribution<> d({50, 20, 10, 10, 5, 5, 5, 5});
  const static std::string options[] = {
      "https\0\0\0",    "http\0\0\0\0",  "ftp\0\0\0\0\0", "file\0\0\0\0",
      "ws\0\0\0\0\0\0", "wss\0\0\0\0\0", "garbage\0",     "fake\0\0\0\0"};
  std::vector<std::string_view> answer;
  answer.reserve(length);
  for (size_t pos = 0; pos < length; pos++) {
    const std::string &picked = options[d(gen)];
    answer.emplace_back(std::string_view(picked.data(), strlen(picked.data())));
  }
  return answer;
}

void simulation(size_t N) {

  std::vector<std::string_view> data = populate(N);

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    size_t matches{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      matches = 0;
      for (auto v : data) {
        matches += hash_is_special(v);
      }
      finish = nano();
    }
    double t = double(finish - start) / (N * count);

    printf("hash_is_special %f ns/string, matches = %zu \n", t, matches);
  }

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    size_t matches{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      matches = 0;
      for (auto v : data) {
        matches += default_is_special(v);
      }
      finish = nano();
    }
    double t = double(finish - start) / (N * count);

    printf("default_is_special %f ns/string, matches = %zu \n", t, matches);
  }

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    size_t matches{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      matches = 0;
      for (auto v : data) {
        matches += fast_is_special(v);
      }
      finish = nano();
    }
    double t = double(finish - start) / (N * count);

    printf("fast_is_special %f ns/string, matches = %zu \n", t, matches);
  }

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    size_t matches{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      matches = 0;
      for (auto v : data) {
        matches += branchless_is_special(v);
      }
      finish = nano();
    }
    double t = double(finish - start) / (N * count);

    printf("branchless_is_special %f ns/string, matches = %zu \n", t, matches);
  }
}

int main() { simulation(8192); }
