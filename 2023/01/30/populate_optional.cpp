
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <vector>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

template <bool shortstrings> std::vector<std::string> populate(size_t length) {
  const static char *short_options[] = {"https", "http", "ftp",    "file",
                                        "ws",    "wss",  "garbae", "fake",
                                        "httpr", "filer"};
  const static char *long_options[] = {
      "Let me not to the marriage of true minds",
      "Love is not love Which alters when it alteration finds",
      " And every fair from fair sometimes declines",
      "Wisely and slow; they stumble that run fast."};
  std::vector<std::string> answer;
  answer.reserve(length);
  for (size_t pos = 0; pos < length; pos++) {
    const char *picked = shortstrings
                             ? short_options[rand() % std::size(short_options)]
                             : long_options[rand() % std::size(long_options)];
    answer.emplace_back(picked);
  }
  return answer;
}

std::vector<std::string> data;
std::vector<std::optional<std::string>> outdata;
std::vector<std::string*> outdataptr;

template <bool shortstrings> std::tuple<double, double, double> simulation(size_t N) {
  double t1, t2, t3;

  {
    data = populate<shortstrings>(N);
    outdata = std::vector<std::optional<std::string>>(N);

    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      outdata[i] = data[i];
    }
    uint64_t finish = nano();
    t1 = double(finish - start) / N;
  }
  {
    data = populate<shortstrings>(N);
    outdata = std::vector<std::optional<std::string>>(N);

    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      outdata[i] = std::move(data[i]);
    }
    uint64_t finish = nano();
    t2 = double(finish - start) / N;
  }
  {
    data = populate<shortstrings>(N);
    outdataptr = std::vector<std::string*>(N);

    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      outdataptr[i] = &data[i];
    }
    uint64_t finish = nano();
    t3 = double(finish - start) / N;
  }
  return {t1, t2, t3};
}

template <bool shortstrings> void demo() {
  double avg1 = 0;
  double avg2 = 0;
  double avg3 = 0;

  size_t times = 100;

  for (size_t i = 0; i < times; i++) {
    auto [t1, t2, t3] = simulation<shortstrings>(131072);
    avg1 += t1;
    avg2 += t2;
    avg3 += t3;
  }
  avg1 /= times;
  avg2 /= times;
  avg3 /= times;

  std::cout << avg1 << " " << avg2 << " " << avg3 << std::endl;
}

int main() {
  puts("We report ns/string (first copy, then move, then pointer).\n");
  puts("short strings:");
  demo<true>();
  puts("long strings:");
  demo<false>();
}
