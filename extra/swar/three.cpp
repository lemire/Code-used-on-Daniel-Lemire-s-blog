// Type your code here, or load an example.
#include <cstdint>
#include <utility>
#include <array>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <cstdint>
#include <utility>
#include <array>
#include <cstring>


bool has_special_bytes(uint64_t v) { 
    const uint64_t ones = 0x0101010101010101U; 
    const uint64_t high = 0x8080808080808080U; 
    uint64_t mask10 = ((v ^ 0x1010101010101010U) | high) - ones; 
    uint64_t mask23 = ((v ^ 0x2323232323232323U) | high) - ones; 
    uint64_t mask45 = ((v ^ 0x4545454545454545U) | high) - ones; 
    uint64_t mask = high & ~(mask10 & mask23 & mask45) & ~v; 
    return mask != 0; 
}

bool has_special_bytes2(uint64_t v) { 
    const uint64_t ones = 0x0101010101010101U; 
    uint64_t v8 = v | 0x8080808080808080; 
    uint64_t mask10 = (v8 ^ 0x1010101010101010U) - ones; 
    uint64_t mask23 = (v8 ^ 0x2323232323232323U) - ones; 
    uint64_t mask45 = (v8 ^ 0x4545454545454545U) - ones; 
    uint64_t mask = (0x8080808080808080 ^ (mask10 & mask23 & mask45)) & ~v; 
    return mask != 0;
}

// Returns zero if no byte is 0x10, 0x23, or 0x45; nonzero otherwise.
// https://stackoverflow.com/questions/79764782/how-to-clear-specific-byte-values-inside-a-64-bit-value-without-looping/79782258#79782258
uint64_t aqrit (uint64_t v) {
    const uint64_t x7F = 0x7F7F7F7F7F7F7F7F;
    uint64_t m = v & x7F; // clear msb of each byte

    // detect unsigned byte values
    // if xor doesn't produce `0x00` then msb becomes set
    m = ((m ^ 0x1010101010101010) + x7F) &
        ((m ^ 0x2323232323232323) + x7F) &
        ((m ^ 0x4545454545454545) + x7F);
    return (m ^ 0x8080808080808080) & ~v;
}

// Test harness: iterate over three detector functions and verify behavior.
using detector_fn = bool(*)(uint64_t);

bool test_all_detectors() {
    // detectors to test
    detector_fn detectors[] = { [](uint64_t x){ return aqrit(x) != 0; }, has_special_bytes2, has_special_bytes };
    const char* detector_names[] = { "aqrit", "has_special_bytes2", "has_special_bytes" };

    const uint8_t targets[] = { 0x10, 0x23, 0x45 };

    // For each detector, each byte position, set one byte to each target (with and without MSB)
    // and expect true. Also ensure a non-target byte (0x00) yields false.
    for (size_t di = 0; di < (sizeof(detectors)/sizeof(detectors[0])); ++di) {
        auto det = detectors[di];
        const char* name = detector_names[di];
        for (int pos = 0; pos < 8; ++pos) {
            for (uint8_t t : targets) {
                // plain target
                uint64_t v = 0;
                v |= (uint64_t)t << (pos * 8);
                if (!det(v)) {
                    std::printf("[FAIL] %s: pos=%d byte=0x%02X expected true but got false\n", name, pos, t);
                    return false; // expected true but got false
                }

                // target with MSB set (e.g., 0x10 | 0x80)
                uint64_t v_msb = 0;
                uint8_t t_msb = t | 0x80;
                v_msb |= (uint64_t)t_msb << (pos * 8);
                if (det(v_msb)) {
                    std::printf("[FAIL] %s: pos=%d byte=0x%02X (MSB set) expected false but got %s\n", name, pos, t_msb, det(v_msb) ? "true" : "false");
                    return false; // expected true but got false for MSB variant
                }
            }

            // Now test that a non-target byte (e.g., 0x00) yields false for this single-byte value
            uint64_t v0 = 0;
            // Put a byte different from targets (choose 0x00 which isn't in targets)
            v0 |= (uint64_t)0x00 << (pos * 8);
            if (det(v0)) {
                std::printf("[FAIL] %s: pos=%d byte=0x00 expected false but got true\n", name, pos);
                return false; // expected false but got true
            }
        }
    }
    return true;
}


int main() {
    if(!test_all_detectors()) {
        printf("Self-test failed!\n");
        return EXIT_FAILURE;
    }
    printf("Self-test passed.\n");
    return EXIT_SUCCESS;
}