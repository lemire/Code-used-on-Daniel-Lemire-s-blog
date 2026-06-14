// main.cpp
//
// Load a JSON file at compile time with #embed, then parse it at compile time
// with simdjson's static-reflection parser. The result is a constexpr value
// whose fields the optimiser folds into immediates.
//
// Build (inside the gcc16 station):
//   g++ -std=c++26 -freflection -DSIMDJSON_STATIC_REFLECTION=1 --embed-dir=. \
//       -O3 -c main.cpp -o main.o
//   g++ -std=c++26 -O3 -c simdjson.cpp -o simdjson.o
//   g++ main.o simdjson.o -o demo

#define SIMDJSON_STATIC_REFLECTION 1
#include "simdjson.h"

#include <cstdio>
#include <string_view>

// 1. The file becomes part of the program: no run-time I/O.
constexpr const char json_data[] = {
#embed "data.json"
    , 0
};

// 2. simdjson parses the bytes at compile time and synthesises a struct whose
//    members are exactly the JSON keys (width, height, fullscreen, title,
//    volume). We neither write a parser nor declare the struct.
constexpr auto window = simdjson::compile_time::parse_json<json_data>();

// 3. The result is a genuine compile-time constant.
static_assert(window.width      == 1920);
static_assert(window.height     == 1080);
static_assert(window.fullscreen == true);
// simdjson gives a string field the type `const char*`, so we wrap it in a
// string_view to compare by content rather than by pointer.
static_assert(std::string_view(window.title) == "My Game");
static_assert(window.volume == 0.8);
static_assert(window.width * window.height == 2073600);

// The optimiser therefore turns any computation over it into a constant.
// Inspect with `objdump -d`: no loads, no parsing, just immediates.
extern "C" int  screen_area()   { return window.width * window.height; }
extern "C" bool is_fullscreen() { return window.fullscreen; }

int main() {
  std::printf("title      : %s\n", window.title);
  std::printf("resolution : %lld x %lld\n", (long long)window.width,
              (long long)window.height);
  std::printf("fullscreen : %s\n", window.fullscreen ? "true" : "false");
  std::printf("volume     : %g\n", window.volume);
  std::printf("screen_area()   = %d\n", screen_area());
  std::printf("is_fullscreen() = %d\n", is_fullscreen());
}
