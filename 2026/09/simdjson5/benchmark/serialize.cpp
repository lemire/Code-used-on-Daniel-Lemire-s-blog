// Times DOM serialization (simdjson::minify(dom::element)) and DOM parsing.
#include "simdjson.h"
#include <chrono>
#include <cstdio>
#include <string>

int main(int argc, char **argv) {
  for (int a = 1; a < argc; a++) {
    simdjson::padded_string json;
    if (simdjson::padded_string::load(argv[a]).get(json)) { return EXIT_FAILURE; }
    simdjson::dom::parser parser;
    simdjson::dom::element doc;
    if (parser.parse(json).get(doc)) { return EXIT_FAILURE; }
    std::string out = simdjson::minify(doc);
    double best = 1e300;
    size_t sink = 0;
    for (int i = 0; i < 400; i++) {
      auto t0 = std::chrono::steady_clock::now();
      std::string s = simdjson::minify(doc);
      auto t1 = std::chrono::steady_clock::now();
      sink += s.size();
      double d = std::chrono::duration<double>(t1 - t0).count();
      if (d < best) { best = d; }
    }
    std::string name = argv[a];
    name = name.substr(name.find_last_of('/') + 1);
    printf("%-22s serialize %8.3f GB/s (output %zu bytes) %zu\n", name.c_str(),
           double(out.size()) / best / 1e9, out.size(), sink % 7);
  }
  return EXIT_SUCCESS;
}
