#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}
#define PI 3.14159265358979323846

void round(double x, double y, double *xout, double *yout) {
  const double oneoversqrt2 = 0.7071067811865475;
  bool xneg = x < 0;
  x = xneg ? -x : x;
  bool yneg = y < 0;
  y = yneg ? -y : y;
  double tmpx = oneoversqrt2;
  double tmpy = oneoversqrt2;
  tmpx = (x >= 0.923879532511286) ? 1 : tmpx;
  tmpy = (y >= 0.923879532511286) ? 1 : tmpy;
  tmpx = (x < 0.3826834323650898) ? 0 : tmpx;
  tmpy = (y < 0.3826834323650898) ? 0 : tmpy;
  tmpx = xneg ? -tmpx : tmpx;
  tmpy = yneg ? -tmpy : tmpy;
  *xout = tmpx;
  *yout = tmpy;
}

void roundtan(double x, double y, double *xout, double *yout) {
  double angle = atan2(y, x);
  angle = (int(round(4 * angle / PI + 8)) % 8) * PI / 4;
  *xout = cos(angle);
  *yout = sin(angle);
}

#include <iostream>

std::pair<double, double> benchmark(size_t size) {
  std::unique_ptr<double[]> data(new double[2 * size]);
  for (size_t i = 0; i < 2 * size; i++) {
    data.get()[i] = 2 * ((double)rand() / RAND_MAX) - 1;
  }
  for (size_t i = 0; i < size; i++) {
    double norm = sqrt(data.get()[2 * i] * data.get()[2 * i] +
                       data.get()[2 * i + 1] * data.get()[2 * i + 1]);
    data.get()[2 * i] /= norm;
    data.get()[2 * i + 1] /= norm;
  }
  std::unique_ptr<double[]> databuffer(new double[2 * size]);
  auto benchfnc = [&data, &databuffer, size]() {
    for (size_t i = 0; i < size; i++) {
      round(data.get()[2 * i], data.get()[2 * i + 1], databuffer.get() + 2 * i,
            databuffer.get() + 2 * i + 1);
    }
  };
  std::unique_ptr<double[]> databuffer2(new double[2 * size]);

  auto benchfnctan = [&data, &databuffer2, size]() {
    for (size_t i = 0; i < size; i++) {
      roundtan(data.get()[2 * i], data.get()[2 * i + 1],
               databuffer2.get() + 2 * i, databuffer2.get() + 2 * i + 1);
    }
  };
  uint64_t start = nano();
  uint64_t finish = start;
  size_t count{0};
  uint64_t threshold = 500'000'000;
  double bogus = 0;
  for (; finish - start < threshold;) {
    count++;
    benchfnc();
    bogus += databuffer.get()[size - 1] + databuffer.get()[2*size - 1];
    finish = nano();
  }
  if (bogus == 0.5) {
    printf("zero\n");
  }
  double t1 = double(finish - start) / (count * size);
  count = 0;
  bogus = 0;
  start = nano();
  finish = start;
  for (; finish - start < threshold;) {
    count++;
    benchfnctan();
    bogus += databuffer2.get()[size - 1] + databuffer2.get()[2*size - 1];
    finish = nano();
  }
  if (bogus == 0.5) {
    printf("zero\n");
  }
  for(size_t i = 0; i < 2*size; i++) {
    if(fabs(databuffer2.get()[i]- databuffer.get()[i]) > 0.001) { printf("bug\n"); }
  }
  double t2 = double(finish - start) / (count * size);
  return std::make_pair(t1, t2);
}
int main() {
  srand(time(NULL));
  for (size_t i = 0; i < 1000000; i++) {
    double x = 2 * ((double)rand() / RAND_MAX) - 1;
    double y = 2 * ((double)rand() / RAND_MAX) - 1;
    double norm = sqrt(x * x + y * y);
    x /= norm;
    y /= norm;

    double tmpx, tmpy;
    roundtan(x, y, &tmpx, &tmpy);
    double tmpx2, tmpy2;

    round(x, y, &tmpx2, &tmpy2);
    if ((fabs(tmpx - tmpx2) > 0.0001) || (fabs(tmpy - tmpy2) > 0.0001)) {
      std::cout << x << " " << y << std::endl;
      std::cout << tmpx << " " << tmpy << std::endl;
      std::cout << tmpx2 << " " << tmpy2 << std::endl;
      break;
    }
  }

  for(int trial = 0; trial < 10; trial++) {
    auto b = benchmark(100000);
    std::cout << b.first << " " << b.second << std::endl;
  }
}
