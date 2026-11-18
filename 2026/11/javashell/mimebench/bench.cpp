#include "simdutf.h"
#include <chrono>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static std::string slurp(const char *p) {
  std::ifstream f(p, std::ios::binary); std::stringstream ss; ss << f.rdbuf(); return ss.str();
}
int main() {
  size_t expLen; unsigned long long expSum;
  { std::ifstream f("expected.txt"); f >> expLen >> expSum; }
  printf("simdutf %s, implementation %s\n", SIMDUTF_VERSION, simdutf::get_active_implementation()->name().data());
  for (const char *name : {"mime"}) {
    std::string in = slurp((std::string(name) + ".txt").c_str());
    std::vector<char> out(simdutf::maximal_binary_length_from_base64(in.data(), in.size()));
    auto decode = [&] {
      auto r = simdutf::base64_to_binary(in.data(), in.size(), out.data());
      if (r.error != simdutf::error_code::SUCCESS) { printf("error\n"); exit(1); }
      return r.count;
    };
    size_t n = decode();
    unsigned long long s = 0; for (size_t i = 0; i < n; i++) s += (unsigned char)out[i];
    if (n != expLen || s != expSum) { printf("bad decode %s\n", name); return 1; }
    const int iters = 200; volatile size_t sink = 0;
    for (int i = 0; i < 3 * iters; i++) sink = decode();
    double best = 1e300;
    for (int t = 0; t < 10; t++) {
      auto st = std::chrono::steady_clock::now();
      for (int i = 0; i < iters; i++) sink = decode();
      best = std::min(best, std::chrono::duration<double, std::nano>(std::chrono::steady_clock::now() - st).count() / iters);
    }
    printf("c++ simdutf %s base64_to_binary: %.2f GB/s\n", name, in.size() / best);
  }
}
