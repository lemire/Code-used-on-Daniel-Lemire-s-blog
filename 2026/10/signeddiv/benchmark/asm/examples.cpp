#include "div32.h"
extern "C" {
int32_t compiler_div7(int32_t x) { return x / 7; }
int32_t div32_7(int32_t x) { return div32<7>(x); }
int32_t compiler_div1000(int32_t x) { return x / 1000; }
int32_t div32_1000(int32_t x) { return div32<1000>(x); }
}
