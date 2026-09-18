#include <arm_sve.h>
#include <cstdint>
#include <cstring>
// Attempt 3: a fixed-length predicate type (requires -msve-vector-bits=N).
typedef svbool_t fixed_pred __attribute__((arm_sve_vector_bits(128)));
uint16_t to_mask(svbool_t p) {
  fixed_pred fp = p;
  uint16_t m;
  memcpy(&m, &fp, sizeof(m));
  return m;
}
