
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <vector>

uint32_t karprabin_hash(const char *data, size_t len, uint32_t B) {
  uint32_t hash = 0;
  for (size_t i = 0; i < len; i++) {
    hash = hash * B + data[i];
  }
  return hash;
}

__attribute__ ((noinline))
size_t karprabin_naive(const char *data, size_t len, size_t N, uint32_t B, uint32_t target) {
  size_t counter = 0;
  for(size_t i = 0; i < len-N; i++) {
    uint32_t hash = 0;
    for(size_t j = 0; j < N; j++) {
      hash = hash * B + data[i+j];
    }
    if (hash == target) {
      counter++;
    }
  }
  return counter;
}

__attribute__ ((noinline))
size_t karprabin_rolling(const char *data, size_t len, size_t N, uint32_t B, uint32_t target) {
  size_t counter = 0;
  uint32_t BtoN = 1;
  for(size_t i = 0; i < N; i++) {
    BtoN *= B;
  }
  uint32_t hash = 0;
  for(size_t i = 0; i < N; i++) {
    hash = hash * B + data[i];
  }
  if (hash == target) {
    counter++;
  }
  for(size_t i = N; i < len; i++) {
    hash = hash * B + data[i] - BtoN * data[i-N];
    // karprabin_hash(data+i-N+1, N, B) == hash
    if (hash == target) {
      counter++;
    }
  }
  return counter;
}

size_t karprabin_rolling4(const char *data, size_t len, size_t N, uint32_t B, uint32_t target) {
  size_t counter = 0;
  uint32_t BtoN = 1;
  for(size_t i = 0; i < N; i++) {
    BtoN *= B;
  }
  uint32_t hash = 0;
  for(size_t i = 0; i < N; i++) {
    hash = hash * B + data[i];
  }
  if (hash == target) {
    counter++;
  }
  size_t i = N;
  for(; i + 3 < len; i+=4) {
    hash = hash * B + data[i] - BtoN * data[i-N];
    if (hash == target) {
      counter++;
    }
    hash = hash * B + data[i+1] - BtoN * data[i+1-N];
    if (hash == target) {
      counter++;
    }
    hash = hash * B + data[i+2] - BtoN * data[i+2-N];
    if (hash == target) {
      counter++;
    }
    hash = hash * B + data[i+3] - BtoN * data[i+3-N];
    if (hash == target) {
      counter++;
    }
  }

  for(; i < len; i++) {
    hash = hash * B + data[i] - BtoN * data[i-N];
    // karprabin_hash(data+i-N+1, N, B) == hash
    if (hash == target) {
      counter++;
    }
  }
  return counter;
}

size_t karprabin_rolling4_2(const char *data, size_t len, size_t N, uint32_t B, uint32_t target) {
  size_t counter = 0;
  uint32_t BtoN = 1;
  for(size_t i = 0; i < N; i++) {
    BtoN *= B;
  }

  uint32_t B2 = B * B;
  uint32_t B3 = B * B * B;

  uint32_t BtoN1 = BtoN * B;
  uint32_t BtoN2 = BtoN * B * B;

  uint32_t hash = 0;
  for(size_t i = 0; i < N; i++) {
    hash = hash * B + data[i];
  }
  if (hash == target) {
    counter++;
  }
  size_t i = N;
  for(; i + 3 < len; i+=4) {
    uint32_t hash3 = hash * B3 + data[i] * B2 + data[i+1] * B + data[i+2]
      - BtoN2 * data[i-N] - BtoN1 * data[i+1-N] - BtoN * data[i+2-N];

    uint32_t hash1 = hash * B + data[i] - BtoN * data[i-N];
    
    
    if (hash1 == target) {
      counter++;
    }

    uint32_t hash2 = hash1 * B + data[i+1] - BtoN * data[i+1-N];
    if (hash2 == target) {
      counter++;
    }
    
    
    if (hash3 == target) {
      counter++;
    }
    uint32_t hash4 = hash3 * B + data[i+3] - BtoN * data[i+3-N];
    if (hash4 == target) {
      counter++;
    }

    hash = hash4;
  }

  for(; i < len; i++) {
    hash = hash * B + data[i] - BtoN * data[i-N];
    // karprabin_hash(data+i-N+1, N, B) == hash
    if (hash == target) {
      counter++;
    }
  }
  return counter;
}

size_t karprabin_rolling4_3(const char *data, size_t len, size_t N, uint32_t B, uint32_t target) {
  size_t counter = 0;
  uint32_t BtoN = 1;
  for(size_t i = 0; i < N; i++) {
    BtoN *= B;
  }

  uint32_t B2 = B * B;
  uint32_t B3 = B * B * B;

  uint32_t BtoN1 = BtoN * B;
  uint32_t BtoN2 = BtoN * B * B;

  uint32_t hash = 0;
  for(size_t i = 0; i < N; i++) {
    hash = hash * B + data[i];
  }

  uint32_t hash1 = hash * B + data[N+1] - BtoN * data[0];
  

  if (hash == target) {
    counter++;
  }
  if (hash1 == target) {
    counter++;
  }


  size_t i = N;
  for(; i + 1 < len; i+=2) {
    hash = hash * B2 + data[i] * B + data[i+1] 
      - BtoN1 * data[i-N] - BtoN * data[i+1-N];

    hash1 = hash1 * B2 + data[i+1] * B + data[i+2] 
      - BtoN1 * data[i+1-N] - BtoN * data[i+2-N];      
    
    if (hash == target) {
      counter++;
    }
    
    if (hash1 == target) {
      counter++;
    }
  }

  for(; i < len; i++) {
    hash = hash * B + data[i] - BtoN * data[i-N];
    // karprabin_hash(data+i-N+1, N, B) == hash
    if (hash == target) {
      counter++;
    }
  }
  return counter;
}

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  printf(" %d iterations ", agg.iterations);

  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.2f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}
void init(float *v, size_t N) {
  std::mt19937 gen(0);
  std::uniform_real_distribution<float> dis(0, 1);
  for (size_t i = 0; i < N; i++) {
    v[i] = dis(gen);
  }
}
int main(int argc, char **argv) {
  printf("please be patient, this will take a few seconds...\n");
  const size_t N = 1024 * 1024;
  std::unique_ptr<char[]> data(new char[N]);
  for(size_t i = 0; i < N; i++) {
    data.get()[i] = i % 256;
  }
  size_t volume = N;
  volatile size_t counter = 0;
  for(size_t window = 64; window <= 256; window *= 2) {
    printf("window = %zu\n", window);
    pretty_print(1, volume, "karprabin_rolling4", bench([&data, &counter, &window]() {
      data.get()[0] = counter;
      counter += karprabin_rolling4(data.get(), N, window, 31, 0);
    }));
    pretty_print(1, volume, "karprabin_rolling4_2", bench([&data, &counter, &window]() {
      data.get()[0] = counter;
      counter += karprabin_rolling4_2(data.get(), N, window, 31, 0);
    }));
    pretty_print(1, volume, "karprabin_rolling4_3", bench([&data, &counter, &window]() {
            data.get()[0] = counter;
      counter += karprabin_rolling4_3(data.get(), N, window, 31, 0);
    }));
    pretty_print(1, volume, "karprabin_rolling", bench([&data, &counter, &window]() {
      data.get()[0] = counter;
      counter += karprabin_rolling(data.get(), N, window, 31, 0);
    }));
    // pretty_print(1, volume, "karprabin_naive", bench([&data, &counter, &window]() {
    //   counter += karprabin_naive(data.get(), N, window, 31, 0);
    // }));
  }
}

#include <chrono>
#include <cstdint>
#include <iostream>

uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main2(int argc, char **argv) {

    printf("please be patient, this will take a few seconds...\n");

    const size_t N = 1024 * 1024;

    std::unique_ptr<char[]> data(new char[N]);

    for(size_t i = 0; i < N; i++) {

        data.get()[i] = i % 256;

    }

    size_t volume = N;

    volatile size_t counter = 0;




    // Warmup

    for (int i = 0; i < 2000; i++) {

        counter += karprabin_rolling4_3(data.get(), N, 128, 31, 0);

    }




    long start = timeSinceEpochMillisec();

    for (int i = 0; i < 2000; i++) {
        data.get()[0] = i;

        counter += karprabin_rolling4_3(data.get(), N, 128, 31, 0);

    }

    long end = timeSinceEpochMillisec();

    printf("karprabin_rolling4_3: %d\n", (int)(end - start));







    // Warmup

    for (int i = 0; i < 2000; i++) {

        counter += karprabin_rolling4_2(data.get(), N, 128, 31, 0);

    }




    start = timeSinceEpochMillisec();

    for (int i = 0; i < 2000; i++) {
        data.get()[0] = i;
        counter += karprabin_rolling4_2(data.get(), N, 128, 31, 0);

    }

    end = timeSinceEpochMillisec();

    printf("karprabin_rolling4_2: %d\n",(int) (end - start));

}