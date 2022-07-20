#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

struct padded_struct {
  uint64_t key;
  uint64_t value;
  bool is_valid;
};

void init(double *s, size_t size) {
  // Initialize the structs with random values + prefault
  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_real_distribution<double> dist{0, 1};
  for (uint64_t i = 0; i < size; ++i) {
    s[i] = dist(gen);
  }
}



uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

double benchmark(size_t size) {
  std::unique_ptr<double[]> data(new double[size]);
  double * datain = data.get();
  init(data.get(), size);
  std::unique_ptr<float[]> out(new float[size]);
  float * dataout = out.get();
  auto benchfnc = [datain, dataout, size]() {
    for (size_t i = 0; i < size; i++) {
      datain[i] = float(dataout[i]);
    }
  };
  uint64_t start = nano();
  uint64_t finish = start;
  size_t count{0};
  uint64_t threshold = 500'000'000;
  float bogus = 0;
  for (; finish - start < threshold;) {
    count++;
    benchfnc();
    bogus += dataout[size-1];
    finish = nano();
  }
  if(bogus == 0) { printf("zero\n"); }
  return double(finish - start) / (count*size);
}

int main() {
  for (size_t i = 0; i < 3; i++) {
    std::cout << 1/benchmark(100000) << " GB/s" << std::endl;
  }
}