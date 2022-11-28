
#include <fstream>
#include <random>

int main() {
  size_t N = 10000;
  double *x = new double[N];
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<float> distr(0.0f, 1.0f);
  for (size_t i = 0; i < N; i++) {
    x[i] = distr(eng);
  }
  std::ofstream wf("test.dat", std::ios::out | std::ios::binary);
  wf.write((char *)x, sizeof(double) * N);
  wf.close();
  delete[] x;
}