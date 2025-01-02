// c++ -std=c++20 urldomain.cpp && ./a.out

#include "ada.cpp"
#include "ada.h"
#include <iostream>
#include <iomanip>

// Function to convert a single UTF-8 character to UTF-32
char32_t utf8_to_utf32(const char* utf8, size_t& index, size_t length) {
    unsigned char byte = utf8[index++];
    if (byte < 0x80) return byte; // Single byte character

    char32_t codepoint = 0;
    unsigned int more_bytes;

    // Determine how many continuation bytes follow
    if ((byte & 0xE0) == 0xC0) {  // Two bytes
        codepoint = byte & 0x1F;
        more_bytes = 1;
    } else if ((byte & 0xF0) == 0xE0) {  // Three bytes
        codepoint = byte & 0x0F;
        more_bytes = 2;
    } else if ((byte & 0xF8) == 0xF0) {  // Four bytes
        codepoint = byte & 0x07;
        more_bytes = 3;
    } else {  // Invalid UTF-8 leading byte
        return 0xFFFD; // Replacement character
    }

    // Check if there are enough bytes left in the string
    if (index + more_bytes > length) return 0xFFFD;

    // Process continuation bytes
    for (unsigned int i = 0; i < more_bytes; ++i) {
        byte = utf8[index++];
        if ((byte & 0xC0) != 0x80) return 0xFFFD; // Not a continuation byte
        codepoint = (codepoint << 6) | (byte & 0x3F);
    }

    return codepoint;
}

int main(int, char *[]) {
   auto url = ada::parse("https://microsoft.coⅯ");
   if (!url) {
     std::cout << "failure" << std::endl;
     return EXIT_FAILURE;
   }
   auto host = url->get_host();
   for (size_t i = 0; i < host.length();) {
        char32_t code_point = utf8_to_utf32(host.data(), i, host.length());

        for (int j = 3; j >= 0; j--) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << ((code_point >> (j * 8)) & 0xFF);
        }
    }
    std::cout << std::dec << std::endl; // Reset to decimal output
    std::cout << "Note: UTF-32 encoding results in 4 bytes per character, so each character will appear as 8 hexadecimal digits." << std::endl;
   return EXIT_SUCCESS;
 }
