
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstdio>
#include <numeric>
#include <random>
#include <string>

#include "needsescaping.h"

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

int main(int argc, char **argv) {
  std::string justquotes(75,'\"');
  std::string justbackslash(75,'\\');

  std::vector<std::string> data = {"Hello, World!",
                                   "La vie est belle",
                                   "C++ is fun",
                                   "I love programming",
                                   "I love algorithms",
                                   "I love data structures",
                                   "I love mathematics",
                                   "I love computer science",
                                   "I love software engineering",
                                   "I love machine learning",
                                   "I love artificial intelligence",
                                   "I love deep learning",
                                   "I love neural networks",
                                   "I love convolutional neural networks",
                                   "I love recurrent neural networks",
                                   "I love long short-term memory networks",
                                   "I love generative adversarial networks",
                                   "I love reinforcement learning",
                                   "I love unsupervised learning",
                                   "I love supervised learning",
                                   "I love semi-supervised learning",
                                   "I love self-supervised learning",
                                   "I love transfer learning",
                                   "I love multi-task learning",
                                   "I love meta-learning",
                                   "I love few-shot learning",
                                   "I love zero-shot learning",
                                   "I love one shot",
                                   "John went to town",
                                   "Melville",
                                   "Johanna",
                                   "Can't we just get along",
                                   "What about China?",
                                   "Hello, World!",
                                   "La vie est belle",
                                   "C++ is fun",
                                   "I love programming",
                                   "I love algorithms",
                                   "I love data structures",
                                   "I love mathematics",
                                   "I love computer science",
                                   "I love software engineering",
                                   "I love machine learning",
                                   "I love artificial intelligence",
                                   "I love deep learning",
                                   "I love neural networks",
                                   "I love convolutional neural networks",
                                   "I love recurrent neural networks",
                                   "I love long short-term memory networks",
                                   "I love generative adversarial networks",
                                   "I love reinforcement learning",
                                   "I love unsupervised learning",
                                   "I love supervised learning",
                                   "I love semi-supervised learning",
                                   "I love self-supervised learning",
                                   "I love transfer learning",
                                   "I love multi-task learning",
                                   "I love meta-learning",
                                   "I love few-shot learning",
                                   "I love zero-shot learning",
                                   "I love one shot",
                                   "John went to town",
                                   "Melville",
                                   "Johanna",
                                   "Can't we just get along",
                                   "What about China?",
                                   "Hello, World!",
                                   "La vie est belle",
                                   "C++ is fun",
                                   "I love programming",
                                   "I love algorithms",
                                   "I love data structures",
                                   "I love mathematics",
                                   "I love philosophy",
                                   "I love software engineering",
                                   "I dislike machine learning",
                                   "I love artificial intelligence",
                                   "I love deep learning",
                                   "I love neural networks",
                                   "I love convolutional neural networks",
                                   "I love recurrent neural networks",
                                   "I love long short-term memory networks",
                                   "I love generative adversarial networks",
                                   "I love reinforcement learning",
                                   "I love unsupervised learning",
                                   "I love supervised learning",
                                   "I love semi-supervised learning",
                                   "I love self-supervised learning",
                                   "I love transfer learning",
                                   "I love multi-task learning",
                                   "I love meta-learning",
                                   "I love few-shot learning",
                                   "I love zero-shot learning",
                                   "I dislike one shot",
                                   "Peter went to town",
                                   "Marie",
                                   "Johanna",
                                   "Can't we just get fight?",
                                   "What about Tokyo?"};
  size_t volume = 0;
  for (const std::string &s : data) {
    volume += s.size();
  }
  volatile uint64_t counter = 0;
  if(!simple_needs_escaping(justquotes)|| !simple_needs_escaping(justbackslash)) {
    printf("simple implementation is not correct\n");
    return 1;
  }
  pretty_print(data.size(), volume, "simple_needs_escaping",
               bench([&data, &counter]() {
                 for (const std::string &s : data) {
                   counter += simple_needs_escaping(s);
                 }
               }));
  if(!branchless_needs_escaping(justquotes) || !branchless_needs_escaping(justbackslash)) {
    printf("branchless implementation is not correct\n");
    return 1;
  }
  pretty_print(data.size(), volume, "branchless_needs_escaping",
               bench([&data, &counter]() {
                 for (const std::string &s : data) {
                   counter += branchless_needs_escaping(s);
                 }
               }));
  if(!table_needs_escaping(justquotes) || !table_needs_escaping(justbackslash)) {
    printf("table implementation is not correct\n");
    return 1;
  }
  pretty_print(data.size(), volume, "table_needs_escaping",
               bench([&data, &counter]() {
                 for (const std::string &s : data) {
                   counter += table_needs_escaping(s);
                 }
               }));
#if HAS_NEON || HAS_SSE2
  if(!simd_needs_escaping(justquotes) || !simd_needs_escaping(justbackslash)) {
    printf("SIMD implementation is not correct\n");
    return 1;
  } else {
    printf("SIMD implementation is correct: %d %d\n", simd_needs_escaping(justquotes), simd_needs_escaping(justbackslash));
  }
  pretty_print(data.size(), volume, "simd_needs_escaping",
               bench([&data, &counter]() {
                 for (const std::string &s : data) {
                   counter += simd_needs_escaping(s);
                 }
               }));
#endif
}
