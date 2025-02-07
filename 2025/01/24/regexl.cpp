#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <regex>
#include <string>
#include <thread>
#include <vector>
// Function to generate a random string
void generate_random_string(std::mt19937 &gen, int max_length,
                            std::string &output) {
  output.clear();
  std::uniform_int_distribution<> dis(1, max_length);
  std::uniform_int_distribution<> char_dis(35, 126); // ASCII without '"'

  int length = dis(gen);
  for (int i = 0; i < length; ++i) {
    output += static_cast<char>(char_dis(gen));
  }
}

// Thread function to perform matches
void thread_function(size_t number, std::atomic<int64_t> &worst_duration,
                     std::atomic<bool> &done, std::mutex &mtx,
                     std::string &worst_str1, std::string &worst_str2) {
  // Each thread has its own local random number generator
  std::random_device rd;
  std::mt19937 gen(rd());

  int64_t local_worst_duration = 0;
  std::string local_worst_str1, local_worst_str2;
  std::string str1, str2;
  str2 = "Everyone loves Lucy.";
  for (size_t i = 0; i < number && !done; ++i) {
    generate_random_string(gen, 16, str1);

    auto start = std::chrono::steady_clock::now();
    try {
      std::regex pattern(str1);
      std::smatch match;
      // generate_random_string(gen, 32, str2);
      std::regex_search(str2, match, pattern);
    } catch (...) {
      // If the pattern is invalid, skip it
      continue;
    }
    auto end = std::chrono::steady_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count();

    if (duration > local_worst_duration) {
      local_worst_duration = duration;
      local_worst_str1 = str1;
      local_worst_str2 = str2;

      // Critical section to update global worst duration and strings
      std::lock_guard<std::mutex> lock(mtx);
      if (duration > worst_duration) {
        worst_duration = duration;
        worst_str1 = str1;
        worst_str2 = str2;
        std::cout << "New worst time taken: " << duration << " nanoseconds ("
                  << duration / 1000000000.0 << " seconds)\n";
        std::cout << "String 1 (used as regex pattern): \"" << str1 << "\" ("
                  << str1.size() << " bytes)\n";
        std::cout << "String 2 (to match against): \"" << str2 << "\" ("
                  << str2.size() << " bytes)\n";
      }
    }
  }
}

int main() {
  const size_t processor_count = std::thread::hardware_concurrency();
  const size_t num_threads = (processor_count == 0) ? 1 : processor_count / 2;
  std::cout << "using " << num_threads << " threads" << std::endl;
  std::vector<std::thread> threads;
  std::atomic<int64_t> worst_duration{0};
  std::atomic<bool> done{false};
  std::mutex mtx;
  std::string worst_str1, worst_str2;
  size_t number_per_thread = 500000000 / num_threads;

  // Launch threads, each with its own independent random generator
  // initialization
  for (size_t i = 0; i < num_threads; ++i) {
    threads.emplace_back(thread_function, number_per_thread,
                         std::ref(worst_duration), std::ref(done),
                         std::ref(mtx), std::ref(worst_str1),
                         std::ref(worst_str2));
  }

  // Join threads
  for (auto &t : threads) {
    t.join();
  }

  std::cout << "Worst duration found: " << worst_duration.load()
            << " nanoseconds\n";
  std::cout << "Worst pair of strings:\n";
  std::cout << "String 1 (used as regex pattern): " << worst_str1 << "\n";
  std::cout << "String 2 (to match against): " << worst_str2 << "\n";

  return EXIT_SUCCESS;
}
