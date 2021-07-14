
#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

template <typename PROCEDURE>
double bench(PROCEDURE f, uint64_t threshold = 200'000'000) {
  uint64_t start = nano();
  uint64_t finish = start;
  size_t times{0};
  size_t output_size;
  for (; finish - start < threshold; times++) {
    output_size = f();
    if (output_size == 0) {
      throw std::runtime_error("bad input?");
    }
    finish = nano();
  }
  return double(finish - start) / times / output_size;
}

template <typename T>
size_t union2by2(T *input1, size_t size1, T *input2, size_t size2,
                 T *output_buffer) {
  if (size1 == 0) {
    for (size_t i = 0; i < size2; i++) {
      output_buffer[i] = input2[i];
    }
    return size2;
  }
  if (size2 == 0) {
    for (size_t i = 0; i < size1; i++) {
      output_buffer[i] = input1[i];
    }
    return size1;
  }
  size_t pos1{0};
  size_t pos2{0};
  T v1 = input1[pos1];
  T v2 = input2[pos2];
  size_t pos{0};
  while (true) {
    if (v1 < v2) {
      output_buffer[pos++] = v1;
      pos1++;
      if (pos1 == size1) {
        break;
      }
      v1 = input1[pos1];
    } else if (v1 > v2) {
      output_buffer[pos++] = v2;
      pos2++;
      if (pos2 == size2) {
        break;
      }
      v2 = input2[pos2];
    } else {
      output_buffer[pos++] = input1[pos1];
      pos1++;
      pos2++;
      if ((pos1 == size1) || (pos2 == size2)) {
        break;
      }
      v1 = input1[pos1];
      v2 = input2[pos2];
    }
  }
  while (pos1 < size1) {
    output_buffer[pos++] = input1[pos1++];
  }
  while (pos2 < size2) {
    output_buffer[pos++] = input2[pos2++];
  }
  return pos;
}

/**
 * Inspired by work by Andrey Pechkurov
 */
template <typename T>
size_t union2by2_branchless(T *input1, size_t size1, T *input2, size_t size2,
                            T *output_buffer) {
  size_t pos1{0};
  size_t pos2{0};
  size_t pos{0};
  while ((pos1 < size1) & (pos2 < size2)) {
    T v1 = input1[pos1];
    T v2 = input2[pos2];
    output_buffer[pos++] = (v1 <= v2) ? v1 : v2;
    pos1 = (v1 <= v2) ? pos1 + 1 : pos1;
    pos2 = (v2 <= v1) ? pos2 + 1 : pos2;
  }
  while (pos1 < size1) {
    output_buffer[pos++] = input1[pos1++];
  }
  while (pos2 < size2) {
    output_buffer[pos++] = input2[pos2++];
  }
  return pos;
}

size_t union2by2_branchless_ptr(uint32_t *input1, size_t size1,
                                uint32_t *input2, size_t size2,
                                uint32_t *output_buffer) {

  uint32_t *end1 = input1 + size1;
  uint32_t *end2 = input2 + size2;
  uint32_t *begin = output_buffer;

  while ((input1 < end1) & (input2 < end2)) {
    uint32_t v1 = *input1;
    uint32_t v2 = *input2;

    *(output_buffer++) = v1 <= v2 ? *input1 : *input2;
    input1 += v1 <= v2;
    input2 += v2 <= v1;
  }

  while (input1 < end1) {
    *(output_buffer++) = *(input1++);
  }

  while (input2 < end2) {
    *(output_buffer++) = *(input2++);
  }
  return output_buffer - begin;
}

struct scenario {
  scenario(size_t s1, size_t s2)
      : input1(new uint32_t[s1]), size1(s1), input2(new uint32_t[s2]),
        size2(s2), united(new uint32_t[s1 + s2]) {}
  std::unique_ptr<uint32_t[]> input1;
  size_t size1;
  std::unique_ptr<uint32_t[]> input2;
  size_t size2;
  std::unique_ptr<uint32_t[]> united;
};

inline scenario create_scenario(std::mt19937 &gen) {
  std::uniform_int_distribution<size_t> sizes(0, 1000000);
  std::uniform_int_distribution<uint32_t> values;
  scenario ans(sizes(gen), sizes(gen));
  for (size_t i = 0; i < ans.size1; i++) {
    ans.input1.get()[i] = values(gen);
  }
  for (size_t i = 0; i < ans.size2; i++) {
    ans.input2.get()[i] = values(gen);
  }
  std::sort(ans.input1.get(), ans.input1.get() + ans.size1);
  std::sort(ans.input2.get(), ans.input2.get() + ans.size2);
  return ans;
}

std::vector<scenario> create_many(size_t N) {
  std::mt19937 gen(1234);
  std::vector<scenario> ans;
  for (size_t i = 0; i < N; i++) {
    ans.emplace_back(create_scenario(gen));
  }
  return ans;
}

int main() {
  std::cout << "generating data...";
  std::cout.flush();
  std::vector<scenario> data = create_many(100);
  std::cout << " done." << std::endl;
  std::cout << "checking...";
  std::cout.flush();
  for (auto &sce : data) {
    std::vector<uint32_t> output1(sce.size1 + sce.size2);
    std::vector<uint32_t> output2(sce.size1 + sce.size2);
    std::vector<uint32_t> output3(sce.size1 + sce.size2);
    size_t size1 = union2by2(sce.input1.get(), sce.size1, sce.input2.get(),
                             sce.size2, output1.data());
    output1.resize(size1);
    size_t size2 =
        union2by2_branchless(sce.input1.get(), sce.size1, sce.input2.get(),
                             sce.size2, output2.data());
    output2.resize(size2);
    if (output1 != output2) {
      throw std::runtime_error("bug");
    }
    size_t size3 =
        union2by2_branchless_ptr(sce.input1.get(), sce.size1, sce.input2.get(),
                                 sce.size2, output3.data());
    output3.resize(size3);
    if (output1 != output3) {
      throw std::runtime_error("bug");
    }
  }
  std::cout << " done." << std::endl;

  auto naive_procedure = [&data]() -> size_t {
    size_t total{0};
    for (auto &sce : data) {
      total += union2by2(sce.input1.get(), sce.size1, sce.input2.get(),
                         sce.size2, sce.united.get());
    }
    return total;
  };
  auto branchless_procedure = [&data]() -> size_t {
    size_t total{0};
    for (auto &sce : data) {
      total +=
          union2by2_branchless(sce.input1.get(), sce.size1, sce.input2.get(),
                               sce.size2, sce.united.get());
    }
    return total;
  };
  auto branchless_procedure_ptr = [&data]() -> size_t {
    size_t total{0};
    for (auto &sce : data) {
      total += union2by2_branchless_ptr(sce.input1.get(), sce.size1,
                                        sce.input2.get(), sce.size2,
                                        sce.united.get());
    }
    return total;
  };
  std::cout << "warming cache...";
  std::cout.flush();
  bench(naive_procedure);
  bench(branchless_procedure);
  bench(branchless_procedure_ptr);

  std::cout << " done." << std::endl;

  for (size_t trial = 0; trial < 10; trial++) {
    std::cout << "naive          " << bench(naive_procedure) << std::endl;
    std::cout << "branchless     " << bench(branchless_procedure) << std::endl;
    std::cout << "branchless_ptr " << bench(branchless_procedure_ptr)
              << std::endl;

    std::cout << "====" << std::endl;
  }

  return EXIT_SUCCESS;
}