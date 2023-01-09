
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>
#include <unordered_set>
#include <string_view>
#include <regex>
#include "gperf.c"

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

static inline uint64_t string_to_uint64(const uint8_t * data) {
  uint64_t val;
  std::memcpy(&val, data, sizeof(uint64_t));
  return val;
}

uint32_t string_to_uint32(const char *data) {
  uint32_t val;
  std::memcpy(&val, data, sizeof(uint32_t));
  return val;
}



bool direct_is_special(std::string_view input) {
  return (input == "https") | (input == "http") | (input == "ftp") |
         (input == "file") | (input == "ws") | (input == "wss");
}

bool directb_is_special(std::string_view input) {
  return (input == "https") || (input == "http") || (input == "ftp") ||
         (input == "file") || (input == "ws") || (input == "wss");
}

static const std::unordered_set<std::string_view> special_set = {
    "ftp", "file", "http", "https", "ws", "wss"};
bool hash_is_special(std::string_view input) {
  return special_set.find(input) != special_set.end();
}



bool switch_is_special(std::string_view input) {
  if(input.empty()) { return false; }
  int hash_value = (2*input.size() + (unsigned)(input[0])) & 7;
  switch (hash_value) {
    case 0:
      return input == "http";
    case 2:
        return input == "https";
    case 3:
        return input == "ws";
    case 4:
        return input == "ftp";
    case 5:
        return input == "wss";
    case 6:
        return input == "file";
    default:
        return false;
  }
}
constexpr std::string_view table_craftedhash_is_special[] = {"http", "", "https", 
  "ws", "ftp", "wss", "file", ""};

constexpr bool craftedhash_is_special(std::string_view input) {
  if(input.empty()) { return false; }
  int hash_value = (2*input.size() + (unsigned)(input[0])) & 7;
  const std::string_view target = table_craftedhash_is_special[hash_value];
  return (target[0] == input[0]) && (target.substr(1) == input.substr(1));
}

std::vector<std::string_view> populate(size_t length) {
  std::mt19937 gen;
  // we generate a distribution where https is more common
  std::discrete_distribution<> d({50, 20, 10, 10, 5, 5, 5, 5, 5, 5});
  const static char* options[] = {
      "https\0\0\0",    "http\0\0\0\0",  "ftp\0\0\0\0\0", "file\0\0\0\0",
      "ws\0\0\0\0\0\0", "wss\0\0\0\0\0", "garbage\0",     "fake\0\0\0\0",
      "httpr\0\0\0", "filer\0\0\0"};
  std::vector<std::string_view> answer;
  answer.reserve(length);
  for (size_t pos = 0; pos < length; pos++) {
    const char* picked{options[d(gen)]};
    answer.emplace_back(std::string_view(picked, strlen(picked)));
  }
  return answer;
}

void simulation(size_t N) {
  puts("=======\n");
  printf("Simulation with N = %zu \n", N);


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
        matches += craftedhash_is_special(v);
      }
      finish = nano();
    }
    double t = double(finish - start) / (N * count);

    printf("hash %f ns/string, matches = %zu \n", t, matches);
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
        matches += (Perfect_Hash::in_word_set(v.data(), v.size()) != nullptr);
      }
      finish = nano();
    }
    double t = double(finish - start) / (N * count);

    printf("gperf %f ns/string, matches = %zu \n", t, matches);
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
        matches += hash_is_special(v);
      }
      finish = nano();
    }
    double t = double(finish - start) / (N * count);

    printf("unordered_set %f ns/string, matches = %zu \n", t, matches);
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
        matches += direct_is_special(v);
      }
      finish = nano();
    }
    double t = double(finish - start) / (N * count);

    printf("direct_is_special %f ns/string, matches = %zu \n", t, matches);
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
        matches += directb_is_special(v);
      }
      finish = nano();
    }
    double t = double(finish - start) / (N * count);

    printf("directb_is_special %f ns/string, matches = %zu \n", t, matches);
  }
  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    size_t matches{0};
    uint64_t threshold = 500000000;
    const std::regex txt_regex("https?|ftp|file|wss?");
    for (; finish - start < threshold;) {
      count++;
      matches = 0;
      for (auto v : data) {
        matches += std::regex_match(v.begin(), v.end(), txt_regex);
      }
      finish = nano();
    }
    double t = double(finish - start) / (N * count);

    printf("regex %f ns/string, matches = %zu \n", t, matches);
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
        matches += switch_is_special(v);
      }
      finish = nano();
    }
    double t = double(finish - start) / (N * count);

    printf("switch %f ns/string, matches = %zu \n", t, matches);
  }
  
}
int main() { simulation(8192); simulation(8192); simulation(65536); simulation(65536); }
