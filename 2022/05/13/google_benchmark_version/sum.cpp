#include <benchmark/benchmark.h>

#include <stdexcept>
#include <vector>

#include <random>

int get_positive_value(int x) {
    if (x < 0) {
        throw std::runtime_error("it is not positive!");
    }
    return x;
}
__attribute__((noinline)) int with_exception(std::vector<int>& a) {
    int sum = 0;
    for (int x : a) {
        try {
            sum += get_positive_value(x);
        } catch (...) {
            sum += -x;
        }
    }
    return sum;
}

__attribute__((noinline)) int without_exception(
    std::vector<int>& a) noexcept {
    int sum = 0;
    for (int x : a) {
        if (x < 0) {
            sum += -x;
        } else {
            sum += x;
        }
    }
    return sum;
}

struct MyFixture : public benchmark::Fixture {
    std::vector<int> data;

    void SetUp(const ::benchmark::State& state) {
        for (size_t i = 0; i < 10000; i++) {
            data.push_back((rand() % 1000) - 500);
        }
    }
};

BENCHMARK_F(MyFixture, WithException)(benchmark::State& st) {
   for (auto _ : st) {
       int result = with_exception(data);
       benchmark::DoNotOptimize(result);
  }
}
BENCHMARK_F(MyFixture, WithoutException)(benchmark::State& st) {
   for (auto _ : st) {
       int result = without_exception(data);
       benchmark::DoNotOptimize(result);
  }
}
