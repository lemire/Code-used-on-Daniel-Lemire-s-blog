#include <arm_sve.h>
#include <cstdint>
// Attempt 2: a predicate store intrinsic (there is svst1 for vectors; is there one for predicates?)
void store(svbool_t p, uint8_t* dst) {
  svst1_b8(p, dst);
}
