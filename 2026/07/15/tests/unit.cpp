#include <arpa/inet.h>
#include <algorithm>
#include <atomic>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <print>
#include <thread>
#include <vector>
#include "serialize_ipv4.h"


bool run_ipv4_serialize_tests() {
    constexpr uint64_t total = (1ULL << 32);
    const uint64_t thread_count = std::max(1u, std::thread::hardware_concurrency());
    const uint64_t chunk = total / thread_count;
    std::atomic<bool> all_ok{true};
    std::atomic<bool> error_reported{false};
    std::mutex print_mutex;
    std::vector<std::thread> workers;
    workers.reserve(thread_count);

    for (uint64_t t = 0; t < thread_count; ++t) {
        const uint64_t start = t * chunk;
        const uint64_t end = (t + 1 == thread_count) ? total : (t + 1) * chunk;

        workers.emplace_back([start, end, &all_ok, &error_reported, &print_mutex]() {
            for (uint64_t i = start; i < end && all_ok.load(std::memory_order_relaxed); ++i) {
                uint32_t ip = static_cast<uint32_t>(i);
                char buf[16];
                size_t len = ipv4_to_string(ip, buf);
                buf[len] = '\0';

                struct in_addr addr{};
                addr.s_addr = ip;
                char ref[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &addr, ref, sizeof(ref));

                if (std::strcmp(buf, ref) != 0) {
                    all_ok.store(false, std::memory_order_relaxed);
                    if (!error_reported.exchange(true, std::memory_order_relaxed)) {
                        std::scoped_lock lock(print_mutex);
                        std::print("FAIL  ipv4 serialize: SIMD '{}' want '{}'\n", buf, ref);
                    }
                    return;
                }
            }
        });
    }

    for (auto &worker : workers) {
        worker.join();
    }

    return all_ok.load(std::memory_order_relaxed);
}


int main() {
    bool all_ok = true;
    all_ok &= run_ipv4_serialize_tests();
    if (all_ok) {
        std::print("All tests passed.\n");
        return EXIT_SUCCESS;
    } else {
        std::print("Some tests failed.\n");
        return EXIT_FAILURE;
    }
}