#include <chrono>
#include <cstdio>

void walk(const char *p, int maximum);
extern long counter;

int main() {
  constexpr int n = 100'000'000;
  char pad = 0;
  double best = 1e9;
  for (int r = 0; r < 5; r++) {
    auto t0 = std::chrono::steady_clock::now();
    walk(&pad, n);
    double t = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    if (t < best) { best = t; }
  }
  printf("%.2f ns per span (counter %ld)\n", best * 1e9 / n, counter);
}
