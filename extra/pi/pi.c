#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
const uint64_t RR = (uint64_t)RAND_MAX * RAND_MAX;

struct pcg_state_setseq_64 {
  uint64_t
      state;    // RNG state.  All values are possible.  Will change over time.
  uint64_t inc; // Controls which RNG sequence (stream) is
  // selected. Must *always* be odd. Might be changed by the user, never by our
  // code.
};

typedef struct pcg_state_setseq_64 pcg32_random_t;

static pcg32_random_t pcg32_global; // global state

// call this once before calling pcg32_random_r
static inline void pcg32_seed(uint64_t seed1, uint64_t seed2) {
  pcg32_global.state = seed1;
  pcg32_global.inc = seed2 | 1;
}

static inline uint32_t pcg32_random_r(pcg32_random_t *rng) {
  uint64_t oldstate = rng->state;
  rng->state = oldstate * UINT64_C(0x5851f42d4c957f2d) + rng->inc;
  uint32_t xorshifted = ((oldstate >> 18) ^ oldstate) >> 27;
  uint32_t rot = oldstate >> 59;
  return (xorshifted >> rot) | (xorshifted << (32 - rot));
}

static inline uint32_t pcg32(void) { return pcg32_random_r(&pcg32_global); }

double MonteCarloPi(size_t n) {
  size_t k = 0;
  for (size_t i = 0; i < n; ++i) {
    uint64_t x = pcg32(), y = pcg32();
    if (1 - x * x >= y * y)
      k++;
  }
  return 4.0 * k / n;
}

int main() {
  pcg32_seed(4312321, 4321531415);
  printf("%f\n", MonteCarloPi(500000000));
}

