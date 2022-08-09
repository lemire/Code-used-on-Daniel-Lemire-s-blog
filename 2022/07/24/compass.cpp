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
  bool xneg = x < 0;
  bool yneg = y < 0;
  if (xneg) {
    x = -x;
  }
  if (yneg) {
    y = -y;
  }
  double outx = 0.7071067811865475; // 1/sqrt(2)
  double outy = 0.7071067811865475;// 1/sqrt(2)
  if (x >= 0.923879532511286) { // sin(3*pi/8)
    outx = 1;
  }
 if (y >= 0.923879532511286) { // sin(3*pi/8)
    outy = 1;
  }
  if (y >= 0.923879532511286) { // sin(3*pi/8)
    outx = 0;
  }
  if (x >= 0.923879532511286) { // sin(3*pi/8)
    outy = 0;
  }
  if (xneg) {
    outx = -outx;
  }
  if (yneg) {
    outy = -outy;
  }
  *xout = outx;
  *yout = outy;
}

void branchlessround(double x, double y, double *xout, double *yout) {
  bool xneg = x < 0;
  x = xneg ? -x : x;
  bool yneg = y < 0;
  y = yneg ? -y : y;
  double outx = (x >= 0.923879532511286) ? 1 : 0.7071067811865475;
  double outy = (y >= 0.923879532511286) ? 1 : 0.7071067811865475;
  outx = (y >= 0.923879532511286) ? 0 : outx;
  outy = (x >= 0.923879532511286) ? 0 : outy;
  outx = xneg ? -outx : outx;
  outy = yneg ? -outy : outy;
  *xout = outx;
  *yout = outy;
}

void leeround(double x, double y, double* xout, double* yout) {
    double posx = fabs(x);
    double posy = fabs(y);
    double tmpx = (posx >= 0.923879532511286) ? 1 : 0.7071067811865475;
    double tmpy = (posy >= 0.923879532511286) ? 1 : 0.7071067811865475;
    tmpx = (posy >= 0.923879532511286) ? 0 : tmpx;
    tmpy = (posx >= 0.923879532511286) ? 0 : tmpy;
    *xout = copysign(tmpx, x);
    *yout = copysign(tmpy, y);
}

void roundtan(double x, double y, double *xout, double *yout) {
  double angle = atan2(y, x);
  angle = (int(round(4 * angle / PI + 8)) % 8) * PI / 4;
  *xout = cos(angle);
  *yout = sin(angle);
}

#include <iostream>

std::tuple<double, double, double, double> benchmark(size_t size) {
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
  std::unique_ptr<double[]> databuffer3(new double[2 * size]);

  auto benchfncbranchless = [&data, &databuffer3, size]() {
    for (size_t i = 0; i < size; i++) {
      branchlessround(data.get()[2 * i], data.get()[2 * i + 1],
               databuffer3.get() + 2 * i, databuffer3.get() + 2 * i + 1);
    }
  };
  std::unique_ptr<double[]> databuffer4(new double[2 * size]);

  auto benchfnclee = [&data, &databuffer4, size]() {
    for (size_t i = 0; i < size; i++) {
      leeround(data.get()[2 * i], data.get()[2 * i + 1],
               databuffer4.get() + 2 * i, databuffer4.get() + 2 * i + 1);
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
  count = 0;
  bogus = 0;
  start = nano();
  finish = start;
  for (; finish - start < threshold;) {
    count++;
    benchfncbranchless();
    bogus += databuffer3.get()[size - 1] + databuffer3.get()[2*size - 1];
    finish = nano();
  }
  if (bogus == 0.5) {
    printf("zero\n");
  }
  for(size_t i = 0; i < 2*size; i++) {
    if(fabs(databuffer3.get()[i]- databuffer.get()[i]) > 0.001) { printf("bug\n"); }
  }
  double t3 = double(finish - start) / (count * size);
  count = 0;
  bogus = 0;
  start = nano();
  finish = start;
  for (; finish - start < threshold;) {
    count++;
    benchfnclee();
    bogus += databuffer4.get()[size - 1] + databuffer4.get()[2*size - 1];
    finish = nano();
  }
  if (bogus == 0.5) {
    printf("zero\n");
  }
  for(size_t i = 0; i < 2*size; i++) {
    if(fabs(databuffer4.get()[i]- databuffer.get()[i]) > 0.001) { printf("bug\n"); }
  }
  double t4 = double(finish - start) / (count * size);
  return std::make_tuple(t1, t2, t3, t4);
}
int main() {
  srand(time(NULL));
  for (size_t i = 0; i < 1000000; i++) {
    double x = 2 * ((double)rand() / RAND_MAX) - 1;
    double y = 2 * ((double)rand() / RAND_MAX) - 1;
    double norm = sqrt(x * x + y * y);
    x /= norm;
    y /= norm;

    double outx, outy;
    roundtan(x, y, &outx, &outy);
    double outx2, outy2;

    round(x, y, &outx2, &outy2);
    if ((fabs(outx - outx2) > 0.0001) || (fabs(outy - outy2) > 0.0001)) {
      std::cout << x << " " << y << std::endl;
      std::cout << outx << " " << outy << std::endl;
      std::cout << outx2 << " " << outy2 << std::endl;
      break;
    }
  }
  std::cout << " lemire/branches tan/sin/cos lemire/branchless copysing "<< std::endl;

  for(int trial = 0; trial < 10; trial++) {
    auto [t1, t2, t3, t4] = benchmark(100000);
    std::cout << t1 << "          " << t2  << "       " << t3 << "       " << t4 << std::endl;
  }
}
