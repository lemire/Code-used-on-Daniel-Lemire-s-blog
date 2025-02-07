#include <cstddef>
#include <iostream>
#include <chrono>
#include <vector>
#include <cstring>
#include <atomic>

// Your safe_memcpy function
void safe_memcpy(char *dest, const char *src, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        char input =
            std::atomic_ref<const char>(src[i]).load(std::memory_order_relaxed);
        std::atomic_ref<char>(dest[i]).store(input, std::memory_order_relaxed);
    }
}
void benchmark_memcpy(size_t size) {
    size_t repeat = 1000000000/size;
    std::vector<char> src(size, 'a');
    std::vector<char> dest1(size, 'b');
    std::vector<char> dest2(size, 'b');

    // Benchmarking std::memcpy
    auto start = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < repeat; ++i) {
      std::memcpy(dest1.data(), src.data(), size);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_std = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    // Benchmarking safe_memcpy
    start = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < repeat; ++i) {
    safe_memcpy(dest2.data(), src.data(), size);
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration_safe = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "For " << size << " bytes:" << std::endl;
    std::cout << "std::memcpy took " << duration_std << " nanoseconds" << std::endl;
    std::cout << "safe_memcpy took " << duration_safe << " nanoseconds" << std::endl;
    std::cout << "Speed ratio (safe_memcpy/std::memcpy): " << static_cast<double>(duration_safe) / duration_std << std::endl;
    std::cout << std::endl;
}


int main() {
    // Benchmark with different sizes
    std::vector<size_t> sizes = { 1024, 1024*1024, 1024*1024*10}; // 1MB, 10MB
    for (auto size : sizes) {
        for(size_t i = 0; i < 3; ++i) {
            benchmark_memcpy(size);
        }
    }
    return 0;
}
