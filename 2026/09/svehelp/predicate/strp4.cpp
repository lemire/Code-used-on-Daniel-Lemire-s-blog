#include <arm_sve.h>
#include <cstdint>
#include <cstring>
// memcpy from the address of a sizeless predicate object
uint16_t to_mask(svbool_t p) {
  uint16_t m;
  memcpy(&m, &p, sizeof(m));
  return m;
}
