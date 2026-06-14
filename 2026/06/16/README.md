# Compile-time JSON with `#embed` and simdjson (GCC 16)

This program loads a JSON file **at compile time** with `#embed`, then parses it
**at compile time** with simdjson's static-reflection parser
(`simdjson::compile_time::parse_json`). The parser *synthesises a struct type*
from the JSON keys, so there is no parser and no struct to write by hand. The
result is a `constexpr` value that the optimiser folds into immediates.

## Files

| file         | role |
|--------------|------|
| `data.json`  | the input, brought in with `#embed` (no run-time I/O) |
| `main.cpp`   | embeds the file, parses it at compile time, proves it with `static_assert`, and exposes accessors that fold to constants |
| `Makefile`   | build / run / disassemble |
| `simdjson.h`, `simdjson.cpp` | the simdjson 4.6.1 amalgamation |

## Build & run (gcc16 station)

```sh
# from this directory, on the host:
../../../../docker_programming_station/gcc16/run-docker-station 'make run'
../../../../docker_programming_station/gcc16/run-docker-station 'make asm'
```

The compile command is:

```sh
g++ -std=c++26 -freflection -DSIMDJSON_STATIC_REFLECTION=1 --embed-dir=. -O3 \
    -c main.cpp -o main.o
g++ -std=c++26 -O3 -c simdjson.cpp -o simdjson.o
g++ main.o simdjson.o -o demo
```

## Proof it is compile time

1. `static_assert`s on the parsed value compile, e.g.

   ```cpp
   static_assert(window.width * window.height == 2073600);
   ```

2. Functions that read the `constexpr window` compile to immediates — no parser,
   no memory loads (aarch64, `-O3`):

   ```
   <screen_area>:    mov w0, #0xa400 ; movk w0, #0x1f, lsl #16 ; ret   // 1920*1080 = 2073600
   <is_fullscreen>:  mov w0, #0x1 ; ret
   ```

If you corrupt `data.json` (delete a brace, misspell `true`), the build fails at
the `parse_json` line — the JSON is validated by the compiler.

## Note

simdjson's *run-time* parser is the usual SIMD-accelerated one. The compile-time
path is a separate, simpler parser that runs during constant evaluation (where
SIMD and heap allocation are unavailable); it is meant for build-time data such
as configuration and test files. Requires GCC 16 (`-freflection`, experimental
C++26 reflection / P2996) and simdjson built with `SIMDJSON_STATIC_REFLECTION=1`.
