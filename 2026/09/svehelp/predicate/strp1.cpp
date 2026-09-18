#include <arm_sve.h>
#include <cstdint>
#include <cstring>
// Attempt 1: treat the predicate as an ordinary object.
uint16_t to_mask(svbool_t p) {
  uint16_t m;
  memcpy(&m, &p, sizeof(m));   // needs the address and the layout of p
  return m;
}
uint16_t to_mask2(svbool_t p) {
  union { svbool_t p; uint16_t m; } u;  // predicate in a union
  u.p = p;
  return u.m;
}
