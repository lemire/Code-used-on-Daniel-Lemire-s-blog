#include <arpa/inet.h>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <omp.h>
#include <string>
#include <string_view>
#include <vector>
#include <x86intrin.h>

extern "C" {
#include "base16.h"
}

std::vector<std::string> strings = {"f",    "fo",    "foo",
                                    "foob", "fooba", "foobar"};
std::vector<std::string> encoded = {"66",       "666F",       "666F6F",
                                    "666F6F62", "666F6F6261", "666F6F626172"};

bool simple_test() {
  for (size_t i = 0; i < strings.size(); i++) {
    std::cout << " =----= " << std::endl;
    std::string coded = encoded[i];
    size_t input_size = coded.size();
    size_t blocks = input_size / 8;
    coded.resize((blocks + 1) * 8);
    std::string output;
    output.resize((blocks + 1) * 5);

    std::cout << "testing: " << coded << std::endl;
    size_t decoded = base16hex_simple((uint8_t *)output.data(),
                                      (const uint8_t *)coded.data());
    output.resize(decoded / 2);
    std::cout << "got: " << output << "[" << output.size() << " bytes]..."
              << std::endl;
    if (output != strings[i]) {
      std::cout << "expected: " << strings[i] << " [" << strings[i].size()
                << " bytes]..." << std::endl;

      abort();
    }
    size_t fast_decoded =
        base16hex_simd((uint8_t *)output.data(), (const uint8_t *)coded.data());
    output.resize(fast_decoded / 2);
    std::cout << "got: " << output << "[" << output.size() << " bytes]..."
              << std::endl;
    if (output != strings[i]) {
      std::cout << "expected: " << strings[i] << " [" << strings[i].size()
                << " bytes]..." << std::endl;

      abort();
    }
    size_t fast_decoded_geof = base16hex_simd_geoff(
        (uint8_t *)output.data(), (const uint8_t *)coded.data());
    output.resize(fast_decoded_geof / 2);
    std::cout << "got: " << output << "[" << output.size() << " bytes]..."
              << std::endl;
    if (output != strings[i]) {
      std::cout << "expected: " << strings[i] << " [" << strings[i].size()
                << " bytes]..." << std::endl;

      abort();
    }

    size_t fast_decoded_fallback = base16hex_simdzone_fallback(
        (uint8_t *)output.data(), (const uint8_t *)coded.data());
    output.resize(fast_decoded_fallback / 2);
    std::cout << "got: " << output << "[" << output.size() << " bytes]..."
              << std::endl;
    if (output != strings[i]) {
      std::cout << "expected: " << strings[i] << " [" << strings[i].size()
                << " bytes]..." << std::endl;

      abort();
    }

    size_t output_size;

    base16hex_simd_skipspace(
        (uint8_t *)output.data(), (const uint8_t *)coded.data(), &output_size);
    output.resize(output_size);
    std::cout << "got: " << output << "[" << output.size() << " bytes]..."
              << std::endl;
    if (output != strings[i]) {
      std::cout << "expected: " << strings[i] << " [" << strings[i].size()
                << " bytes]..." << std::endl;

      abort();
    }
  }
  printf("SUCCESS\n");
  return true;
}
bool space_separated_test() {
  std::string encoded = "666 F 6 F6 261 72";
  std::string string = "foobar";
  std::string output;
  output.resize(32);
  size_t output_size;

  base16hex_simd_skipspace(
      (uint8_t *)output.data(), (const uint8_t *)encoded.data(), &output_size);
  output.resize(output_size);
  std::cout << "got: " << output << "[" << output.size() << " bytes]..."
            << std::endl;
  if (output != string) {
    std::cout << "expected: " << string << " [" << string.size() << " bytes]..."
              << std::endl;

    abort();
  }
  printf("SUCCESS\n");
  return true;
}
int main() {
  return (simple_test() && space_separated_test()) ? EXIT_SUCCESS
                                                   : EXIT_FAILURE;
}
