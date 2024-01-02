
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <vector>

#include "parseuint8.h"

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

int main(int argc, char **argv) {
  make_lut();
  srandom(1234);
  for (size_t z = 0; z < 2; z++) {
    std::vector<std::string> input;

    bool randomflag = (z == 0);
    printf("random: %d\n", randomflag);

    size_t volume = 0;
    for (size_t i = 0; i < 20000; i++) {

      uint8_t val = randomflag ? (random() % 256): (i % 256);
      auto s = std::to_string(val);
      s.reserve(4);
      input.push_back(s);
      volume += s.size();
      uint8_t result;
      parse_uint8_swar(s.data(), s.size(), &result);
      if (result != val) {
        printf("parse_uint8_swar %d %d\n", result, val);
        abort();
      }
      parse_uint8_fastswar(s.data(), s.size(), &result);
      if (result != val) {
        printf("parse_uint8_fastswar %d %d\n", result, val);
        abort();
      }
      parse_uint8_fromchars(s.data(), s.size(), &result);
      if (result != val) {
        printf("parse_uint8_fromchars %d %d\n", result, val);
        abort();
      }
      parse_uint8_naive(s.data(), s.size(), &result);
      if (result != val) {
        printf("parse_uint8_naive %d %d\n", result, val);
        abort();
      }
      parse_uint8_switch_case(s.data(), s.size(), &result);
      if (result != val) {
        printf("parse_uint8_switch_case %d %d\n", result, val);
        abort();
      }
    }

    std::cout << "volume " << volume << " bytes" << std::endl;

    size_t sum = 0;
    pretty_print(
        input.size(), volume, "parse_uint8_switch_case", bench([&input, &sum]() {
          for (const std::string &s : input) {
            uint8_t result;
            int r = parse_uint8_switch_case(s.data(), s.size(),
                                 &result); // technically, should check error
            if(!r) { abort(); }
            sum += result;
          }
        }));
    pretty_print(
        input.size(), volume, "parse_uint8_nocheck", bench([&input, &sum]() {
          for (const std::string &s : input) {
            uint8_t result;
            parse_uint8_nocheck(s.data(), s.size(),
                                 &result); // technically, should check error
            sum += result;
          }
        }));
    pretty_print(
        input.size(), volume, "parse_uint8_fastswar_bob", bench([&input, &sum]() {
          for (const std::string &s : input) {
            uint8_t result;
            int r = parse_uint8_fastswar_bob(s.data(), s.size(),
                                 &result); // technically, should check error
            if(!r) { abort(); }
            sum += result;
          }
        }));
     pretty_print(
        input.size(), volume, "parse_uint8_fastswar", bench([&input, &sum]() {
          for (const std::string &s : input) {
            uint8_t result;
            int r = parse_uint8_fastswar(s.data(), s.size(),
                                 &result); // technically, should check error
            if(!r) { abort(); }
            sum += result;
          }
        }));
    pretty_print(
        input.size(), volume, "parse_uint8_lut", bench([&input, &sum]() {
          for (const std::string &s : input) {
            uint8_t result;
            int r = parse_uint8_lut(s.data(), s.size(),
                                 &result); // technically, should check error
            if(!r) { abort(); }
            sum += result;
          }
        }));
    pretty_print(
        input.size(), volume, "parse_uint8_swar", bench([&input, &sum]() {
          for (const std::string &s : input) {
            uint8_t result;
            int r = parse_uint8_swar(s.data(), s.size(),
                             &result); // technically, should check error
            if(!r) { abort(); }
            sum += result;
          }
        }));
    pretty_print(
        input.size(), volume, "parse_uint8_fromchars", bench([&input, &sum]() {
          for (const std::string &s : input) {
            uint8_t result;
            int r = parse_uint8_fromchars(s.data(), s.size(),
                                  &result); // technically, should check error
            if(!r) { abort(); }
            sum += result;
          }
        }));
    pretty_print(
        input.size(), volume, "parse_uint8_naive", bench([&input, &sum]() {
          for (const std::string &s : input) {
            uint8_t result;
            int r = parse_uint8_naive(s.data(), s.size(),
                              &result); // technically, should check error
            if(!r) { abort(); }
            sum += result;
          }
        }));
  }
}
