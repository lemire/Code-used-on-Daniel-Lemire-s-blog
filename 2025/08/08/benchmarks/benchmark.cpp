#include <algorithm>
#include <string>
#include <iostream>
#include <chrono>
#include <random>

#include "counters/event_counter.h"
#include "benchmarker.h"

#include "simdutf.h"
#include "fmt/core.h"
#include "fmt/format.h"


double pretty_print(const std::string &name, size_t num_values,
                  std::pair<event_aggregate,size_t> result) {
  const auto& agg = result.first;
  size_t N = result.second;
  num_values *= N; // Adjust num_values to account for repetitions
  fmt::print("{:<50} : ", name);
  fmt::print(" {:5.2f} ns ", agg.fastest_elapsed_ns()/num_values);
  fmt::print(" {:5.2f} GB/s ", num_values / agg.fastest_elapsed_ns());
  if (collector.has_events()) {
    fmt::print(" {:5.2f} GHz ",
               agg.cycles() / agg.elapsed_ns());
    fmt::print(" {:5.2f} c ", agg.fastest_cycles() / num_values);
    fmt::print(" {:5.2f} i ", agg.fastest_instructions() / num_values);
    fmt::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / agg.fastest_cycles());
  }
  fmt::print("\n");
  return num_values / agg.fastest_elapsed_ns();
}



std::string generate_random_ascii_string(size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t max_index = sizeof(charset) - 1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, max_index - 1);

    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        result += charset[dis(gen)];
    }
    result += '='; // Add '=' at the end
    return result;
}


// Naive implementation of find
const char* naive_find(const char* start, const char* end, char character) {
    while (start != end) {
        if (*start == character) {
            return start;
        }
        ++start;
    }
    return end;
}


const char* find64(const char* start, 
                       const char* end, 
                       char character) {
    const char* p = start;
    // Replicated constants for the byte-wise check
    const uint64_t repeat_one = 0x0101010101010101ULL;
    const uint64_t repeat_char = (uint64_t)(uint8_t)character * repeat_one;
    const uint64_t high_bits = 0x8080808080808080ULL;
    // Handle alignment to 8-byte boundary for safe unaligned access avoidance
    // (though on x86 it's fine, this ensures portability)
    uintptr_t misalignment = (uintptr_t)p % 8;
    if (misalignment != 0) {
        size_t align_steps = 8 - misalignment;
        for (size_t i = 0; i < align_steps && p < end; ++i) {
            if (*p == character) {
                return p;
            }
            ++p;
        }
    }
    // Main loop: process 8 bytes at a time
    while (p + 8 <= end) {
        uint64_t word = *(const uint64_t*)p;
        uint64_t xor_val = word ^ repeat_char;
        uint64_t has_match = (xor_val - repeat_one) & ~xor_val & high_bits;
        if (has_match != 0) {
            // A match in this 8-byte chunk; scan to find the first one
            for (int i = 0; i < 8; ++i) {
                if (p[i] == character) {
                    return p + i;
                }
            }
        }
        p += 8;
    }
    // Handle remaining bytes (0 to 7)
    while (p < end) {
        if (*p == character) {
            return p;
        }
        ++p;
    }
    return end;
}


// Define 256-bit vector type for four int64_t (32 bytes)
typedef uint64_t vec4x64 __attribute__((vector_size(32)));

const char* findvec4x64(const char* start, 
                       const char* end, 
                       char character) {
    const char* p = start;
    
    // Broadcast character to all bytes in a 32-byte vector
    const uint64_t char_broadcast = (uint64_t)(uint8_t)character * 0x0101010101010101ULL;
    const vec4x64 target = {char_broadcast, char_broadcast, char_broadcast, char_broadcast};
    
    // Handle alignment to 32-byte boundary for vector loads
    uintptr_t misalignment = (uintptr_t)p % 32;
    if (misalignment != 0) {
        size_t align_steps = 32 - misalignment;
        for (size_t i = 0; i < align_steps && p < end; ++i) {
            if (*p == character) {
                return p;
            }
            ++p;
        }
    }
    
    // Main loop: process 32 bytes (4x int64_t) at a time
    while (p + 32 <= end) {
        // Load 32 bytes as four int64_t
        vec4x64 data = *(const vec4x64*)p;
        
        // Compare for equality (produces 0xFF in matching bytes, 0x00 elsewhere)
        vec4x64 cmp = data == target;
        
        // Combine results into a single 256-bit mask
        // Non-zero indicates at least one matching byte
        uint64_t mask = 0;
        for (int i = 0; i < 4; ++i) {
            mask |= (uint64_t)cmp[i];
        }
        
        if (mask != 0) {
            // Found a match; scan the 32 bytes to find the first one
            for (int i = 0; i < 32; ++i) {
                if (p[i] == character) {
                    return p + i;
                }
            }
        }
        
        p += 32;
    }
    
    // Handle remaining bytes (0 to 31)
    while (p < end) {
        if (*p == character) {
            return p;
        }
        ++p;
    }
    
    return end;
}


// Define 256-bit vector type for 32 uint8_t (32 bytes)
typedef uint8_t vec32x8 __attribute__((vector_size(32)));

const char* findvec32x8(const char* start, 
                       const char* end, 
                       char character) {
    const char* p = start;
    
    // Broadcast character to all 32 bytes in a vector
    vec32x8 target = {(uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character, 
                   (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
                   (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
                   (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
                   (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
                   (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
                   (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
                   (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character};
    
    // Handle alignment to 32-byte boundary for vector loads
    uintptr_t misalignment = (uintptr_t)p % 32;
    if (misalignment != 0) {
        size_t align_steps = 32 - misalignment;
        for (size_t i = 0; i < align_steps && p < end; ++i) {
            if (*p == character) {
                return p;
            }
            ++p;
        }
    }
    
    // Main loop: process 32 bytes at a time
    while (p + 32 <= end) {
        // Load 32 bytes as a vector
        vec32x8 data = *(const vec32x8*)p;
        
        // Compare for equality (produces 0xFF in matching bytes, 0x00 elsewhere)
        vec32x8 cmp = data == target;
        
        // Combine results into a mask (non-zero indicates a match)
        uint32_t mask = 0;
        for (int i = 0; i < 32; ++i) {
            mask |= (uint32_t)cmp[i];
        }
        
        if (mask != 0) {
            // Found a match; scan the 32 bytes to find the first one
            for (int i = 0; i < 32; ++i) {
                if (p[i] == character) {
                    return p + i;
                }
            }
        }
        
        p += 32;
    }
    
    // Handle remaining bytes (0 to 31)
    while (p < end) {
        if (*p == character) {
            return p;
        }
        ++p;
    }
    
    return end;
}


// Define 512-bit vector type for 64 uint8_t (64 bytes)
typedef uint8_t vec64x8 __attribute__((vector_size(64)));

const char* findvec64x8(const char* start, 
                       const char* end, 
                       char character) {
    const char* p = start;
    
    // Broadcast character to all 64 bytes in a vector
    vec64x8 target = {
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character,
        (uint8_t)character, (uint8_t)character, (uint8_t)character, (uint8_t)character
    };
    
    // Handle alignment to 64-byte boundary for vector loads
    uintptr_t misalignment = (uintptr_t)p % 64;
    if (misalignment != 0) {
        size_t align_steps = 64 - misalignment;
        for (size_t i = 0; i < align_steps && p < end; ++i) {
            if (*p == character) {
                return p;
            }
            ++p;
        }
    }
    
    // Main loop: process 64 bytes at a time
    while (p + 64 <= end) {
        // Load 64 bytes as a vector
        vec64x8 data = *(const vec64x8*)p;
        
        // Compare for equality (produces 0xFF in matching bytes, 0x00 elsewhere)
        vec64x8 cmp = data == target;
        
        // Combine results into a mask (non-zero indicates a match)
        uint64_t mask = 0;
        for (int i = 0; i < 64; ++i) {
            mask |= (uint64_t)cmp[i];
        }
        
        if (mask != 0) {
            // Found a match; scan the 64 bytes to find the first one
            for (int i = 0; i < 64; ++i) {
                if (p[i] == character) {
                    return p + i;
                }
            }
        }
        
        p += 64;
    }
    
    // Handle remaining bytes (0 to 63)
    while (p < end) {
        if (*p == character) {
            return p;
        }
        ++p;
    }
    
    return end;
}

std::vector<std::tuple<size_t, double, double, double, double, double, double, double, double>> benchmark_results;

void collect_benchmark_results(size_t input_size) {
    std::string input = generate_random_ascii_string(input_size);
    volatile uint64_t counter = 0;

            // Benchmark 64x8 bits
    auto findvec64x8_result = pretty_print("findvec64x8", input.size(), 
                                   bench([&input, &counter]() {
                                       char* ptr = (char*)findvec64x8(input.data(),input.data()+input.size(), '=');
                                       counter = counter + size_t(ptr - input.data());
                                   }));

        // Benchmark 32x8 bits
    auto findvec32x8_result = pretty_print("findvec32x8", input.size(), 
                                   bench([&input, &counter]() {
                                       char* ptr = (char*)findvec32x8(input.data(),input.data()+input.size(), '=');
                                       counter = counter + size_t(ptr - input.data());
                                   }));

    // Benchmark 4x64 bits
    auto findvec4x64_result = pretty_print("findvec4x64", input.size(), 
                                   bench([&input, &counter]() {
                                       char* ptr = (char*)findvec4x64(input.data(),input.data()+input.size(), '=');
                                       counter = counter + size_t(ptr - input.data());
                                   }));

    // Benchmark 64 bits
    auto find64_result = pretty_print("find64", input.size(), 
                                   bench([&input, &counter]() {
                                       char* ptr = (char*)find64(input.data(),input.data()+input.size(), '=');
                                       counter = counter + size_t(ptr - input.data());
                                   }));

    // Benchmark strchr
    auto memchr_result = pretty_print("memchr", input.size(), 
                                   bench([&input, &counter]() {
                                       char* ptr = (char*)memchr(input.data(),'=',input.size());
                                       counter = counter + size_t(ptr - input.data());
                                   }));

    // Benchmark std::find
    auto std_result = pretty_print("std::find", input.size(), 
                                   bench([&input, &counter]() {
                                       auto it = std::find(input.data(), input.data() + input.size(), '=');
                                       counter = counter + size_t(it - input.data());
                                   }));

    // Benchmark simdutf::find
    auto simdutf_result = pretty_print("simdutf::find", input.size(), 
                                       bench([&input, &counter]() {
                                           auto it = simdutf::find(input.data(), input.data() + input.size(), '=');
                                           counter = counter + size_t(it - input.data());
                                       }));

    // Benchmark naive_find
    auto naive_result = pretty_print("naive_find", input.size(), 
                                      bench([&input, &counter]() {
                                          auto it = naive_find(input.data(), input.data() + input.size(), '=');
                                          counter = counter + size_t(it - input.data());
                                      }));

    benchmark_results.emplace_back(input_size, findvec64x8_result, findvec32x8_result, findvec4x64_result, find64_result, memchr_result, std_result, simdutf_result, naive_result);
}

void print_markdown_table() {
    fmt::print("| Input Size (bytes) |findvec64x8(GB/s) |findvec32x8 (GB/s)  | findvec4x64(GB/s) | find64 (GB/s)      | memchr (GB/s)    | std::find (GB/s) |  simdutf::find (GB/s) | naive_find (GB/s) |\n");
    fmt::print("|--------------------|------------------|--------------------|-------------------|--------------------|------------------|------------------|-----------------------|-------------------|\n");

    for (const auto& [size, findvec64x8_gbps, findvec32x8_gbps, findvec4x64_gbps, find64_gbps, memchr_gbps, std_gbps, simdutf_gbps, naive_gbps] : benchmark_results) {
        fmt::print("| {:<18} | {:16.2f} |  {:16.2f} |  {:16.2f} | {:16.2f} | {:<16.2f} | {:<16.2f} | {:<21.2f} | {:<17.2f} |\n", size, findvec64x8_gbps, findvec32x8_gbps, findvec4x64_gbps, find64_gbps, memchr_gbps, std_gbps, simdutf_gbps, naive_gbps);
    }
}

int main(int argc, char **argv) {
    std::vector<size_t> input_sizes = {4, 16, 128, 1024, 8192, 65536, 524288, 2097152}; // Sizes from cache-friendly to RAM-intensive

    for (size_t size : input_sizes) {
        collect_benchmark_results(size);
    }

    // Print markdown table
    print_markdown_table();
}

