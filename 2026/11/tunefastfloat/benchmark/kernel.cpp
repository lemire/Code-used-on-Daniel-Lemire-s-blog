// One parsing loop: it sums the parsed numbers. CMake compiles this file once
// per configuration, with KERNEL naming the function and HINTS holding the
// performance hints (no hints if HINTS is not defined).
#include "kernels.h"

#include <cstdlib>
#include <system_error>

#include "fast_float/fast_float.h"

double KERNEL(const std::vector<std::string> &numbers) {
  double sum = 0;
  for (const std::string &s : numbers) {
    double value;
#ifdef HINTS
    auto answer =
        fast_float::from_chars<HINTS>(s.data(), s.data() + s.size(), value);
#else
    auto answer = fast_float::from_chars(s.data(), s.data() + s.size(), value);
#endif
    if (answer.ec != std::errc()) {
      std::abort();
    }
    sum += value;
  }
  return sum;
}
