#include <unordered_map>
#include <map>
#include <string_view>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>


constexpr size_t volume = 1000000;

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

template <typename PROCEDURE>
double bench(PROCEDURE f, uint64_t threshold = 200'000'000) {
  uint64_t start = nano();
  uint64_t finish = start;
  size_t times{0};
  for (; finish - start < threshold; times++) {
    auto output = f();
    if(output.result() == 0) { std::cerr << "bug" << std::endl; }
    finish = nano();
  }
  return double(finish - start) / times / volume;
}

struct naive_accumulator {
    std::vector<std::string> content;
    void consume(std::string_view view) {
        content.emplace_back(view.begin(), view.end());
    }

    size_t result() {
        return content.size();
    }
};

struct view_accumulator {
    std::map<std::string_view,size_t> content; // keys point at string_buffer
    std::vector<char> string_buffer; // owns the string content

    void consume(std::string_view view) {
        auto i = content.find(view);
        if(i == content.end()) { // key is not found
          size_t where_at = string_buffer.size();
          string_buffer.insert(string_buffer.end(), view.begin(), view.end());
          std::string_view permanent_view(&string_buffer[where_at], view.size());
          content.emplace(permanent_view, 1);
        } else {
          i->second++;
        }
    }

    size_t result() {
        return content.size();
    }
};


struct basic_accumulator {
    std::map<std::string,size_t> content; 

    void consume(std::string_view view) {
        std::string copy(view);
        auto i = content.find(copy);
        if(i == content.end()) { // key is not found
          content.emplace(copy, 1);
        } else {
          i->second++;
        }
    }

    size_t result() {
        return content.size();
    }
};

template <class ACCUMULATOR>
ACCUMULATOR accumulate() {
    ACCUMULATOR a;
    std::vector<std::string> master = {"Toyota from Tokyo, Japan", "Honda", "General Motors", "Ford Motor Company"};
    for(size_t i = 0; i < volume;i++) {
        auto & m = master[i%master.size()];
        std::string_view view(m.data(), m.size());
        a.consume(view);
    }
    return a;
}

template <class ACCUMULATOR>
ACCUMULATOR accumulate_short_string() {
    ACCUMULATOR a;
    std::vector<std::string> master = {"Toyota", "Honda", "GM", "Ford"};
    for(size_t i = 0; i < volume;i++) {
        auto & m = master[i%master.size()];
        std::string_view view(m.data(), m.size());
        a.consume(view);
    }
    return a;
}


int main() {
    std::cout << "Using relatively long strings" << std::endl;

    std::cout << "time (ns) with basic     accumulator " << bench(accumulate<basic_accumulator>)
              << std::endl;
    std::cout << "time (ns) with view      accumulator " << bench(accumulate<view_accumulator>)
              << std::endl;
    std::cout << "time (ns) with vector    accumulator " << bench(accumulate<naive_accumulator>)
              << std::endl;

    std::cout << "Switching to short strings" << std::endl;

    std::cout << "time (ns) with basic     accumulator " << bench(accumulate_short_string<basic_accumulator>)
              << std::endl;
    std::cout << "time (ns) with view      accumulator " << bench(accumulate_short_string<view_accumulator>)
              << std::endl;
    std::cout << "time (ns) with vector    accumulator " << bench(accumulate_short_string<naive_accumulator>)
              << std::endl;
}