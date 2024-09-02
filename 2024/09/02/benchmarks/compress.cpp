
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <charconv>
#include <curl/curl.h>
#include <fast_float/fast_float.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <stdlib.h>
#include <string_view>
#include <vector>

#include "avx512compress.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string loadFileFromURL(const std::string &url) {
  printf("loadFileFromURL %s \n", url.c_str());

  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  // Initialize curl
  curl = curl_easy_init();
  if (curl) {
    // Set URL to download
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    // Pass our 'chunk' to the callback function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    // Perform the request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    // Clean up
    curl_easy_cleanup(curl);
  }
  printf("Downloaded %zu bytes\n", readBuffer.size());
  // Print the content of the downloaded file

  return readBuffer;
}

std::vector<double> loadDataFromURL(const std::string_view url) {
  std::string data = loadFileFromURL(std::string(url));
  std::vector<double> values;
  std::string_view sv(data);
  while (!sv.empty()) {
    auto line_end = sv.find('\n');
    std::string_view line = sv.substr(0, line_end);
    double value;
    fast_float::from_chars(line.data(), line.data() + line.size(), value);
    values.push_back(value);
    if (line_end == std::string_view::npos)
      break; // Handle last line without newline
    sv.remove_prefix(line_end + 1);
  }
  return values;
}

double pretty_print(size_t volume, size_t bytes, std::string name,
                    event_aggregate agg) {
  printf("\t%-45s: %5.1f GB/s %5.1f billion floats/s \n ", name.c_str(), bytes / agg.elapsed_ns(), volume / agg.elapsed_ns());
  return agg.elapsed_ns() / volume;
}

void bench(std::vector<double> &data) {
  size_t volume = data.size();
  std::vector<uint16_t> compressed(data.size());
  std::vector<double> decompressed(data.size());
  to_float16(compressed.data(), data.data(), data.size());
  from_float16(decompressed.data(), compressed.data(), data.size());
  double max_error = 0;
  for (size_t i = 0; i < data.size(); i++) {
    double error = std::abs(data[i] - decompressed[i]);
    if (error > max_error) {
      max_error = error;
      printf("approximating %f with %f \n", data[i], decompressed[i]);
    }
  }
  printf("max error: %f\n", max_error);
  printf("number of floats: %zu\n", volume);
  pretty_print(volume, volume * sizeof(uint64_t), "to_float16",
               bench([&data, &compressed]() {
                 to_float16(compressed.data(), data.data(), data.size());
               }));
  pretty_print(volume, volume * sizeof(uint64_t), "from_float16",
               bench([&decompressed, &compressed]() {
                 from_float16(decompressed.data(), compressed.data(),
                              decompressed.size());
               }));
  pretty_print(volume, volume * sizeof(uint64_t), "round-trip",
               bench([&decompressed, &compressed, &data]() {
                 to_float16(compressed.data(), data.data(), data.size());
                 from_float16(decompressed.data(), compressed.data(),
                              decompressed.size());
               }));
}

int main(int, char **) {
  auto data =
      loadDataFromURL("https://raw.githubusercontent.com/lemire/"
                      "simple_fastfloat_benchmark/master/data/canada.txt");
  printf("loadDataFromURL() succeeded %zu\n", data.size());

  bench(data);
  return EXIT_SUCCESS;
}
