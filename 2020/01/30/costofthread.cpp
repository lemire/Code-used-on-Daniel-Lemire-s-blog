#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>


using clk        = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<clk>;
using dur_double = std::chrono::duration<double>;
using std::chrono::duration_cast;

class Timer {
public:
  Timer(const std::string& cmd) :  _cmd{cmd}, _start{clk::now()} {};

  double time_ns() {
    auto duration   = clk::now() - _start;
    auto elapsed_s  = duration_cast<dur_double>(duration).count();
    return elapsed_s * 1000 * 1000 * 1000;
  }

  ~Timer() {};

private:
  std::string _cmd;
  time_point  _start;
};

size_t counter{0};

double startthread() {
    auto t = Timer{ __FUNCTION__};
    auto mythread = std::thread([] {counter++;
        });
    mythread.join();
    return t.time_ns();
}


#include <cmath>

double mean(const std::vector<double> &v) {
    if(v.size() == 0) return 0;// right.
    double sum = 0;
    for (auto &x: v) {
        sum += x;
    }
    return sum / v.size();
}

double std_error(const std::vector<double> &v, double meanval) {
  if(v.size() == 0) return 0;// right
    double s {0};

    for (auto &x: v) {
        s += (x - meanval) * (x - meanval);
    }

    return std::sqrt(s / v.size());
}
int main() {
  std::vector<double> timings;
  for (size_t i = 0; i <= 1000; i += 1) {
    timings.push_back(startthread());
  }
  double meanval = mean(timings);
  double stde = std_error(timings, meanval);
  std::cout.precision(6);
  std::cout << meanval << " ns +/- " << stde << std::endl;
  std::cout << "min: " << *min_element(timings.begin(), timings.end()) << std::endl;
  std::cout << "max: " << *max_element(timings.begin(), timings.end()) << std::endl;

}
