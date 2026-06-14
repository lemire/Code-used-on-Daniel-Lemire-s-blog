# Parsing JSON at compile time with C++26 static reflection

Suppose that you have a configuration file in JSON. Something like this:

```json
{ "width": 1920, "height": 1080, "fullscreen": true,
  "title": "My Game", "volume": 0.8 }
```

Normally you ship this file alongside your program, open it at startup, read it,
and parse it. That is a lot of work for data that never changes. What if the file
is fixed at build time? Could the compiler read it, parse it, and bake the result
directly into the executable as a constant?

With C++26, the answer is yes. We need two new ingredients, all of which are
usable right now with the latest version of the GCC compiler (16).

1. `#embed` to pull the file into the program at compile time,
2. A software library supporting *static reflection* like simdjson.


Let me show you how far we can take this.

The new `#embed` directive reads a file and expands it into a comma-separated
list of byte values. To read the file `data.json` at compile time and keep it
around as a constant, we write:

```cpp
constexpr const char json_data[] = {
#embed "data.json"
    , 0
};
```

I use `constexpr` because I want the compiler to be allowed to inspect these
bytes during constant evaluation. The trailing `, 0` simply appends a null terminator, so the array can be treated as an ordinary C string.

There is  no run-time input/output of any kind. The bytes
are part of the program.

But embedded bytes are not yet useful by themselves. What I really want is a
typed C++ object. In my example, the target type is this configuration struct:

```cpp
struct Window {
  int         width;
  int         height;
  bool        fullscreen;
  std::string title;
  double      volume;
};
```

The traditional way to populate such a struct from JSON is to write, by hand, one
line per field: read `"width"`, store it into `width`, read `"height"`, store it
into `height`, and so on. It is tedious. And because it runs at startup, a
malformed file becomes a run-time error, discovered by your users rather than by
you.


Recent versions of [simdjson](https://github.com/simdjson/simdjson) can parse
JSON *at compile time* using C++26 static reflection. The entry point is
`simdjson::compile_time::parse_json`, and it does something I still find slightly
magical: it reads the JSON and, from the keys it finds, and synthesises the struct
type for you.

```cpp
#define SIMDJSON_STATIC_REFLECTION 1
#include "simdjson.h"

constexpr const char json_data[] = {
#embed "data.json"
    , 0
};

constexpr auto window = simdjson::compile_time::parse_json<json_data>();
```

The variable `window` is a value computed entirely by the compiler. Its type is
generated from the document: it has a `width` and a `height` (both 64-bit
integers), a `bool` `fullscreen`, a `double` `volume`, and a `title`. From here
on I write `window.width` and it behaves like any ordinary field.


How do I know the parsing really happened at compile time? Because I can assert
things about the result that the compiler must check before the program even
exists:

```cpp
static_assert(window.width      == 1920);
static_assert(window.height     == 1080);
static_assert(window.fullscreen == true);
```


If I corrupt the JSON — delete a brace, misspell `true`, leave a trailing comma —
the program no longer compiles, and the error points at the `parse_json` line.
The broken file is caught at build time, on my machine, instead of at startup on
someone else's.


Because `window` is a genuine compile-time constant, any computation over it is a
constant too. Consider this function:

```cpp
int  screen_area()   { return window.width * window.height; }
```

Compiled with `-O3`, there is no multiplication, no field access, and certainly
no parsing left — only the answers, as immediate values (here on my macBook):

```asm
screen_area:    mov  w0, #0xa400        // 0x1fa400 = 2073600
                movk w0, #0x1f, lsl #16
                ret
```

The JSON has vanished from the binary. It was read and parsed exactly once, by
the compiler, and all that survives is the number 2073600.


Because static reflection is so new, when building with GCC 16, you need to pass the flags `-std=c++26 -freflection`: the `-freflection` flag is necessary to activate compile-time reflection You must also set the simdjson macro `SIMDJSON_STATIC_REFLECTION=1` before importing the `simdjson.h`. It is a temporary safeguard.

The [source code to reproduce these examples is available](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/06/16).

*Reference*: [P2996 — Reflection for C++26](https://wg21.link/p2996) and
[the simdjson library](https://simdjson.org).

*Credit*: The simdjson implementation is joint work with 
Francisco Geiman Thiesen.
