# simdjson 5.0 is out

The [simdjson](https://github.com/simdjson/simdjson) library is a C++ library to parse and
generate JSON. It is used in Node.js, ClickHouse, Meta Velox, StarRocks, Apache Doris, the
Ladybird browser and many other systems. We released version 4.0 a year ago, in September 2025. Its
headline feature was C++26 static reflection: you could turn a C++ structure
into JSON, and back, without writing any glue code.

Today we are releasing
version 5.0.


1. Static reflection is no longer guarded and is an officially supported feature. When your
   compiler has reflection enabled (e.g., `g++ -std=c++26 -freflection` with
   GCC 16), simdjson detects it by itself and the reflection-based functions
   become available.

2. When deserializing a C++ structure through reflection, simdjson now uses
   *key selectors* (see below) by default: it reads the object in a single pass,
   whatever the order of the keys. You can return to the previous approach
   (one lookup per member) with `-DSIMDJSON_DISABLE_KEY_SELECTOR_REFLECTION=1`.

3. A positive integer in `[2^64, 10^20)` is now
   reported as a big integer (`BIGINT_NUMBER`), like other overflowing
   integers, instead of a malformed number. When big integers are parsed as
   strings, a token such as `123456789123456789123x` is now rejected.




One major change is the key selectors. A common task is to extract a few fields from a JSON object.  In simdjson 5.0 (C++20 or better), you can name the keys at compile time and visit the object once:

```cpp
using namespace simdjson;
auto json = R"({ "name": "Daniel", "age": 42, "city": "Montreal" })"_padded;
ondemand::parser parser;
auto doc = parser.iterate(json);

std::string_view name, city;
uint64_t age = 0;
auto result = doc.get_object().for_each<"name", "city", "age">(name, city, age);
// name == "Daniel", city == "Montreal", age == 42
```

The keys can appear in any order. At compile time, simdjson builds a perfect
hash function for your set of keys. At run time, recognizing a key takes a hash
computed from a couple of bytes and one comparison. You can also pass one
callback per key instead of variables. The iteration stops as soon as all keys
have been found.



We added annotations for the data structures for automated (C++26) serialization and deserialization: `rename`, `rename_all`,
`alias`, `skip`, `default_value`, `flatten`, `deny_unknown_fields`,
`transparent`, and so forth.

```cpp
struct [[= simdjson::rename_all<simdjson::case_style::camel_case>]] User {
  std::string first_name;
  int64_t user_id;
  [[= simdjson::rename<"KEY">]] int api_key;
};
// {"firstName":"Ann","userId":7,"KEY":8}
```

We often get many JSON documents in one file or one network message. simdjson
has long supported streams of documents separated by white space (NDJSON). In
5.0 we added:

- [RFC 7464](https://www.rfc-editor.org/rfc/rfc7464) JSON text sequences
  (each document preceded by the record separator character) and
  comma-separated documents;
- `stream_format::newline_delimited`: you promise that each document sits on
  its own line. When you only read part of a document, simdjson jumps to the
  next line instead of walking over the rest of the document.
- `simdjson::slice_at`, which cuts a stream into blocks at document
  boundaries so you can parse the blocks on as many threads as you like. The
  built-in threaded mode uses at most two threads.

We also fixed several bugs in `document_stream`, found in an
audit by Francisco Geiman Thiesen.

There are many other smaller features.

- NaN and infinity: JSON does not allow `NaN` or `Infinity`, but many
  systems produce them anyway. If you define `SIMDJSON_ENABLE_NAN_INF`, simdjson
  parses them, and serializes them.
- Narrow types: `get_uint8()`, `get_int8()`, `get_uint16()`, `get_int16()`
  check the range for you. With C++23, `get_float32()` and `get_float64()`
  return `std::float32_t` and `std::float64_t`. The binary32 value is rounded
  once, directly from the decimal string, not through a double.
- The DOM API can parse a buffer that has no padding
  (`parser.parse_unpadded(...)`). It is slower than the regular function, but
  it never reads past the end of your buffer and it does not copy your data.
- With C++17, `simdjson::padded_input` adds padding only when it is needed:
  when your string ends near a page boundary.
- C++20 ranges: you can pipe On-Demand arrays and objects into
  `std::views::transform` and other adaptors.
- DOM arrays support reverse iteration (`rbegin()`, `rend()`) with no
  allocation.
- On-Demand objects offer `get_current_position()` and `revert_position()`: if
  you miss an optional field, you can go back to where you were instead of
  rescanning the whole object.
- `char8_t` (`u8`) variants of the string accessors in C++20.
- Better pretty printing with the [FracturedJson](https://github.com/j-brooke/FracturedJson)
  style, including tables.
- Memory-mapped files under Windows.
- We support the memory-safe compiler Fil-C.

The simdjson 5.0 release improved performance compared to simdjson 4.0 in some key cases. Let me review some of them.

I built both versions with
GCC 16.1 (`-O3`, CMake Release) and ran them on an Intel Xeon Gold 6548N
(Emerald Rapids), pinned to one core.

Let us start with DOM parsing of our standard files (GB/s):

| file           | 4.0  | 5.0  | speedup |
| -------------- | ---: | ---: | ------: |
| twitter        | 4.78 | 4.82 |    1.0 |
| citm_catalog   | 4.82 | 4.79 |    1.0 |
| github_events  | 5.33 | 5.30 |    1.0 |
| canada         | 1.10 | 1.21 |    1.1 |
| marine_ik      | 1.25 | 1.38 |    1.1 |
| mesh           | 1.17 | 1.26 |    1.1 |
| numbers        | 1.13 | 1.41 |    1.3 |
| twitterescaped | 1.59 | 2.85 |    1.8 |
| update-center  | 3.96 | 3.84 |    1.0 |
| apache_builds  | 4.95 | 4.78 |    1.0 |

Files full of numbers (canada, marine_ik, mesh, numbers) are 8% to 25% faster.
And a file full of escaped Unicode characters (twitterescaped) is almost twice
as fast: among other changes, we now decode consecutive `\uXXXX` sequences without going back to the
string scanner between them.

We also serialize faster.  Printing floating-point numbers used to be a bottleneck. We replaced the ancient Grisu2
by Dragonbox, and removed calls to `memcpy` and
`memmove` from the hot path.


| file         | 4.0  | 5.0  | speedup |
| ------------ | ---: | ---: | ------: |
| twitter      | 0.94 | 0.96 |    1.0 |
| citm_catalog | 1.07 | 1.08 |    1.0 |
| gsoc-2018    | 1.10 | 1.25 |    1.1 |
| canada       | 0.31 | 0.52 |    1.7 |
| marine_ik    | 0.28 | 0.37 |    1.3 |
| mesh         | 0.34 | 0.47 |    1.4 |
| numbers      | 0.32 | 0.49 |    1.6 |



The simdjson library is a community project. Since version 4.6, contributions came from
fior512, 吴杨帆, Alecto Irene Perez, Francisco Geiman Thiesen, Max
Bachmann, MoonFlowww, Advit Arora, Jaël Champagne Gareau, Taimoor Kiani, Vasily
Pelikh, jmestwa-coder, liyinlong, AlbertoFVisconti, Aylin Dmello, Cuda Chen,
Ezra Li, Madhurendra Purbay, Makkar, Pastoray, Paul Dreik, Pavel Kruglov, Piotr
Kubaj, Yusuf İhsan Görgel, metsw24-max, neil, pratap singh, Vladimir Saraikin,
wankun, xaldarof, Riyane El Qoqui, Justin Li and others. Thank you!
