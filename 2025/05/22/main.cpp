// c++ -std=c++23 main.cpp
#include <format>
#include <iostream>
#include <utility>
#include <cstdlib>

// N is the total number of elements
// M is the number of chunks
// i is the index of the chunk (0-indexed)
std::pair<size_t, size_t> get_chunk_range_simple(size_t N, size_t M, size_t i) {
    // Calculate the quotient and remainder
    size_t quotient = N / M;
    size_t remainder = N % M;
    size_t start = quotient * i + (i < remainder ? i : remainder);
    size_t length = quotient + (i < remainder ? 1 : 0);
    return {start, length};
}

int main() {
    size_t N = 20; // Total number of elements
    size_t M = 6;  // Number of chunks
    for(size_t i = 0; i < M; i++) {
        auto [begin, length] = get_chunk_range_simple(N, M, i);
        std::println("Chunk {} range: [{}, {} size = {})", i, begin, begin + length, length);
    }
        
    return EXIT_SUCCESS;
}
