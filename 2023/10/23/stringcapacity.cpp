#include "performancecounters/benchmarker.h"
#include <iostream>
#include <string>

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
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
int main() {
  std::string my_string;
  size_t capacity = size_t(-1);
  while (my_string.size() <= 10'000'000) {
    my_string += "a";
    if (my_string.capacity() != capacity) {
      std::cout << "Length: " << my_string.length()
                << ", Capacity: " << my_string.capacity() << std::endl;
      capacity = my_string.capacity();
    }
  }
  size_t sum = 0;
  for (size_t volume = 100; volume <= 100'000'000; volume *= 10) {
    pretty_print(volume, volume, std::to_string(volume), bench([&sum,volume]() {
                   std::string my_string;
                   while (my_string.size() <= volume) {
                     my_string += "a";
                   }
                   sum += my_string.capacity();
                 }));
  }
  return EXIT_SUCCESS;
}