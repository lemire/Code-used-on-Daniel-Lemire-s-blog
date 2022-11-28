
#include <fstream>
#include <random>

int main() {
  size_t N = 10000;
  int64_t *x = new int64_t[N];
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_int_distribution<int64_t> distr(-127,127);
  for (size_t i = 0; i < N; i++) {
    x[i] = distr(eng);
  }
  std::ofstream wf("testint.dat", std::ios::out | std::ios::binary);
  wf.write((char *)x, sizeof(int64_t) * N);
  wf.close();
  delete[] x;
}