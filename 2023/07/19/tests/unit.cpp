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
#include "base32.h"
}

std::vector<std::string> strings = {
    "I am so so tired", "The quick brown fox jumps over the lazy dog. ",
    "The simdzone parser is the fastest in the world!! ",
    "I have never understood why it is greed to want to keep the money you "
    "have earned but not greed to want to take somebody else's money.  (Thomas "
    "Sowel)"};
std::vector<std::string> encoded = {
    "94G62R90EDNI0SRF41Q6ISJ5CG======",
    "AHK6A83HELKM6QP0C9P6UTRE41J6UU10D9QMQS3J41NNCPBI41Q6GP90DHGNKU90CHNMEBH0",
    "AHK6A83JD5MM8UJFDPII0S31E9PMASH0D5PI0T38CKG6COBJEHIN6T10D5N20T38CKG7ERRIDH"
    "I22890",
    "94G6GOBMCKG6SPBMCLP20TBECHIN4SRKDTNM883ND1SI0QBK41KN6837E9IMAP10EHNI0TR1DP"
    "Q20T3F41LMAPBG41Q6GP90DLNMSPBP41SMUT90D1GNCP90CLGN4RJ5CGG64TBK41N6UT10CTP6"
    "APB441Q6U83NC5N7883KDSG78OBBCKG76RRDCLH6UP3P41IMOSR54TPI0RBFDPINIBH040K58Q"
    "3FDLGN682JDTRMAR19"};
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
    size_t decoded = base32hex_simple((uint8_t *)output.data(),
                                      (const uint8_t *)coded.data());
    output.resize((decoded * 5) / 8);
    std::cout << "got: " << output << "[" << output.size() << " bytes]..."
              << std::endl;
    if (output != strings[i]) {
      std::cout << "expected: " << strings[i] << " [" << strings[i].size()
                << " bytes]..." << std::endl;

      abort();
    }
    size_t fast_decoded = base32hex_fast((uint8_t *)output.data(),
                                      (const uint8_t *)coded.data());
    output.resize((fast_decoded * 5) / 8);
    std::cout << "got: " << output << "[" << output.size() << " bytes]..."
              << std::endl;
    if (output != strings[i]) {
      std::cout << "expected: " << strings[i] << " [" << strings[i].size()
                << " bytes]..." << std::endl;

      abort();
    }
    size_t simd_decoded =
        base32hex_simd((uint8_t *)output.data(), (const uint8_t *)coded.data());
    std::cout << "simd_decoded = " << simd_decoded << std::endl;
    output.resize((simd_decoded * 5) / 8);
    std::cout << "got: " << output << "[" << output.size() << " bytes]..."
              << std::endl;
    if (output != strings[i]) {
      std::cout << "expected: " << strings[i] << " [" << strings[i].size()
                << " bytes]..." << std::endl;

      abort();
    }

    size_t swar_decoded =
        base32hex_swar((uint8_t *)output.data(), (const uint8_t *)coded.data());
    std::cout << "swar_decoded = " << swar_decoded << std::endl;

    output.resize((swar_decoded * 5) / 8);
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

int main() { return (simple_test()) ? EXIT_SUCCESS : EXIT_FAILURE; }
