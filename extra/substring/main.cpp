#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <chrono>

#include "murmur3.h"

bool matching_8bytes(const char * c1, const char * c2) {
      uint64_t x,y;
      memcpy(&x, c1, sizeof(uint64_t) );
      memcpy(&y, c2, sizeof(uint64_t) );
      return x == y;
}

bool matching_4bytes(const char * c1, const char * c2) {
      uint32_t x,y;
      memcpy(&x, c1, sizeof(uint32_t) );
      memcpy(&y, c2, sizeof(uint32_t) );
      return x == y;
}

// First two baselines taken from the Lemire blog source
uint64_t matching_bytes_in_word(uint64_t x, uint64_t y) {
  uint64_t xor_xy = x ^ y;
  // credit: mula
  // 7th bit set if lower 7 bits are zero
  const uint64_t t0 = (~xor_xy & 0x7f7f7f7f7f7f7f7fllu) + 0x0101010101010101llu;
  // 7th bit set if 7th bit is zero
  const uint64_t t1 = (~xor_xy & 0x8080808080808080llu);
  uint64_t zeros = t0 & t1;
  return ((zeros >> 7) * 0x0101010101010101ULL) >> 56;
}
uint64_t matching_bytes(const char * c1, const char * c2, size_t n) {
    size_t count = 0;
    size_t i = 0;
    uint64_t x, y;
    for(; i + sizeof(uint64_t) <= n; i+= sizeof(uint64_t)) {
      memcpy(&x, c1 + i, sizeof(uint64_t) );
      memcpy(&y, c2 + i, sizeof(uint64_t) );
      count += matching_bytes_in_word(x,y);
    }
    for(; i < n; i++) {
        if(c1[i]  == c2[i]) { count++; }
    }
    return count;
}
uint64_t standard_matching_bytes(const char * c1, const char * c2, size_t n) {
    size_t count = 0;
    size_t i = 0;
    for(; i < n; i++) {
        if(c1[i]  == c2[i]) { count++; }
    }
    return count;
}

// Third baseline is strncmp



// Uses prefix-preferring masks for 16-byte strings -- (0xFFFFFFFFFFFFFFFFull >> (4*i))
#define char_mask(i) (0xFFFFFFFFFFFFFFFFull >> ((i) << 2))
#define prefix_mask(i) (0xFFFFFFFFFFFFFFFFull << (63-((i) << 2))) // i is max char index = length-1

uint64_t prefixhash(const std::string s) {
    uint64_t hash = 0;

    unsigned char temp[2];
    uint64_t hashout[2]; // Need 64-bit, will discard half.

    for(int i = 0; i < s.size(); i++) {
        //MurmurHash3_x64_128(&s[i], 1, i, hashout);
        temp[0] = s[i];
        temp[1] = i;
        MurmurHash3_x64_128(temp, 2, 0, hashout);
        hash ^= (hashout[0] & char_mask(i));
    }

    return hash;
}

struct HashedString {
    std::string s;
    uint64_t hash;
    HashedString(const std::string & s_): s(s_) {
        hash = prefixhash(s_);
    }
};

// Generates a heavily biased first 16 bits
// which I believe is more representative of real IPs in this scenario.
std::string generate_ip_address() {
    std::string s = std::to_string(188 + 4*(rand()%3)); // Actual DNS blocks
    s += "."+std::to_string(190 + (rand()%30));
    s += "."+std::to_string(rand() % 256);
    s += "."+std::to_string(rand() % 256);
    return s;
}

void toy_ip_example(int N) {
    // We suppose we are given an unsorted list of IPv4 addresses.
    // (Generated below.)

    std::cout << "Generating and hashing " << N << " strings. This will take a second.\n";

    std::vector<HashedString> ips;
    ips.reserve(N);
    for(int i = 0; i < N; i++) {
        ips.emplace_back(generate_ip_address());
    }

    std::vector<int> matching;
    matching.reserve(500);


    // Now we are wondering how many of them point towards 192.210.14.*
    const char * ref = "192.210.14.";


    std::cout << "Finished setup. Looking for matches!\n\n";
    
    using namespace std::chrono;

    // Using matching_bytes baseline -----------------------
    auto start = high_resolution_clock::now(); 

    for(int i = 0; i < N; i++) {
        if(matching_bytes(&ips[i].s[0], ref, 11) == 11) {
            matching.push_back(i);
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "using matching_bytes:\n";
    std::cout << "Counted " << int(matching.size()) << " matches in " << duration.count() << " ms\n\n";

    matching.clear();

    // Using standard_matching_bytes comparison
    start = high_resolution_clock::now(); 

    for(int i = 0; i < N; i++) {
        if(standard_matching_bytes(&ips[i].s[0], ref, 11) == 11) {
            matching.push_back(i);
        }
    }

    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);

    std::cout << "using standard_matching_bytes:\n";
    std::cout << "Counted " << int(matching.size()) << " matches in " << duration.count() << " ms\n\n";

    matching.clear();


    // Using traditional string comparison
    start = high_resolution_clock::now(); 

    for(int i = 0; i < N; i++) {
        if(!memcmp(&ips[i].s[0], ref, 11)) {
            matching.push_back(i);
        }
    }

    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);

    std::cout << "using memcmp:\n";
    std::cout << "Counted " << int(matching.size()) << " matches in " << duration.count() << " ms\n\n";

    matching.clear();


    // Using simple filter
    start = high_resolution_clock::now(); 

    for(int i = 0; i < N; i++) {
        if(matching_8bytes(&ips[i].s[0], ref) && matching_4bytes(&ips[i].s[0] + 7, ref + 7)) {
            matching.push_back(i);
        }
    }

    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);

    std::cout << "using simple filter:\n";
    std::cout << "Counted " << int(matching.size()) << " matches in " << duration.count() << " ms\n\n";

    matching.clear();
    // Using prefix hash filter comparison -------------------------------
    start = high_resolution_clock::now();

    std::string refstr(ref);
    uint64_t mask = prefix_mask((refstr.size()-1));
    uint64_t refhash = prefixhash(refstr) & mask;

    int counter = 0;

    for(int i = 0; i < N; i++) {
        if(refhash == (ips[i].hash & mask)) {
            counter++;
            if(!strncmp(&ips[i].s[0], ref, 11)) {
                matching.push_back(i);
            }
        }
    }

    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);

    std::cout << "strncmp with prefixhash filter:\n";
    std::cout << "Counted " << int(matching.size()) << " matches in " << duration.count() << " ms\n";
    std::cout << "# hash matches: " << counter << " / " << N << "\n\n";
}

int main() {
    srand(1337);

    std::cout << "First, an extremely simplistic example to illustrate the idea. We have two strings:\n";
    std::cout << "                         \"Hello, world!!!\"     \"Hello, wordl!!!\"\n";

    std::string s1 = "Hello, world!!!";
    std::string s2 = "Hello, wordl!!!";

    uint64_t h1 = prefixhash(s1), h2 = prefixhash(s2);

    std::cout << "Raw hashes:              " << h1 << ", " << h2 << '\n';
    uint64_t mask = prefix_mask(10-1);
    std::cout << "(10 byte) Masked hashes: " << (h1&mask) << ", " << (h2&mask) << '\n';

    std::cout << "\n\n\nNow for a more realistic scenario, involving IP matching.\n";
    std::cout << "Here, we are given a ton of IPv4 addresses. (Generated for the toy problem.)\n";
    std::cout << "Then, we want to find all of the form 192.210.14.*\n";
    std::cout << "We demonstrate a (tuned) substring hash can provide a 2x speed boost.\n";
    std::cout << "See the code for a few more comments on the exact setup.\n\n\n";

    toy_ip_example(50000000);
}