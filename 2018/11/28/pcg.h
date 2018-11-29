#include <stdint.h>

typedef struct {
  uint64_t state;
  uint64_t inc;
} pcg32_random_t;

pcg32_random_t gstate = {0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL};

uint32_t pcg32_random_r(pcg32_random_t *rng) {
  uint64_t oldstate = rng->state;
  // Advance internal state
  rng->state = oldstate * 6364136223846793005ULL + (rng->inc | 1);
  // Calculate output function (XSH RR), uses old state for max ILP
  uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
  uint32_t rot = oldstate >> 59u;
  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

uint32_t random_bounded(uint32_t range) {
  uint64_t random32bit = pcg32_random_r(&gstate);
  uint64_t result = random32bit * range;
  return result >> 32;
}
