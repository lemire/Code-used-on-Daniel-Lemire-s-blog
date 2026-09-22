// Fixed harness for the simdutf commit history. Links against the amalgamated
// simdutf.cpp of each commit. Reports, per function, the corpus-wide throughput
// (total bytes / total best-of-N time) in GB/s. Buffers sit 16 bytes past a
// 64-byte boundary, which is what malloc hands out for large blocks.
#include "simdutf.h"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

static const int REPS = 7;
static const long BYTES_PER_REP = 100000000L;
static const int OFFSET = 16;

static std::vector<char> read_file(const std::string &path) {
  FILE *f = fopen(path.c_str(), "rb");
  if (!f) return {};
  fseek(f, 0, SEEK_END);
  long n = ftell(f);
  fseek(f, 0, SEEK_SET);
  std::vector<char> v(n);
  if (fread(v.data(), 1, n, f) != (size_t)n) { fclose(f); return {}; }
  fclose(f);
  return v;
}

// Returns best time in seconds for `iters` calls.
template <class F> static double best_time(F f, int iters) {
  for (int i = 0; i < 20; i++) f();
  double best = 1e30;
  size_t acc = 0;
  for (int rep = 0; rep < REPS; rep++) {
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < iters; i++) acc += f();
    auto t1 = std::chrono::steady_clock::now();
    best = std::min(best, std::chrono::duration<double>(t1 - t0).count());
  }
  if (acc == 1) printf(" ");
  return best;
}

static char *alloc_off(size_t bytes) {
  char *raw = (char *)aligned_alloc(64, ((bytes + OFFSET + 63) / 64) * 64 + 64);
  return raw + OFFSET;
}

struct Acc { double bytes = 0, secs = 0; };

static void report(const char *name, const Acc &a) {
  printf("%-18s %14.0f bytes %10.6f s %8.3f GB/s\n", name, a.bytes, a.secs, a.bytes / a.secs / 1e9);
}

int main(int argc, char **argv) {
  std::string dir = argc > 1 ? argv[1] : "data";
  std::vector<std::string> files;
  for (auto &e : std::filesystem::directory_iterator(dir)) files.push_back(e.path().string());
  std::sort(files.begin(), files.end());
  printf("implementation %s\n", std::string(simdutf::get_active_implementation()->name()).c_str());

  Acc v8, v16, t8to16, t16to8, ascii;
  for (auto &path : files) {
    auto data = read_file(path);
    size_t len = data.size();
    if (len < 1024) continue;
    if (!simdutf::validate_utf8(data.data(), len)) { fprintf(stderr, "invalid utf8: %s\n", path.c_str()); continue; }
    int iters = (int)std::max<long>(2, BYTES_PER_REP / (long)len);
    char *u8 = alloc_off(len);
    memcpy(u8, data.data(), len);
    size_t len16 = simdutf::utf16_length_from_utf8(u8, len);
    char16_t *u16 = (char16_t *)alloc_off(len16 * 2);
    simdutf::convert_utf8_to_utf16le(u8, len, u16);
    char16_t *u16out = (char16_t *)alloc_off(len16 * 2);
    char *u8out = alloc_off(len);
    int iters16 = (int)std::max<long>(2, BYTES_PER_REP / (long)(len16 * 2));

    double s;
    s = best_time([&] { return simdutf::validate_utf8(u8, len) ? 1 : 0; }, iters);
    v8.bytes += double(len) * iters; v8.secs += s;
    s = best_time([&] { return simdutf::validate_utf16le(u16, len16) ? 1 : 0; }, iters16);
    v16.bytes += double(len16) * 2 * iters16; v16.secs += s;
    s = best_time([&] { return simdutf::convert_utf8_to_utf16le(u8, len, u16out); }, iters);
    t8to16.bytes += double(len) * iters; t8to16.secs += s;
    s = best_time([&] { return simdutf::convert_utf16le_to_utf8(u16, len16, u8out); }, iters16);
    t16to8.bytes += double(len16) * 2 * iters16; t16to8.secs += s;
    if (simdutf::validate_ascii(u8, len)) {
      s = best_time([&] { return simdutf::validate_ascii(u8, len) ? 1 : 0; }, iters);
      ascii.bytes += double(len) * iters; ascii.secs += s;
    }
  }
  report("validate_utf8", v8);
  report("validate_ascii", ascii);
  report("validate_utf16le", v16);
  report("utf8_to_utf16le", t8to16);
  report("utf16le_to_utf8", t16to8);

  // base64 on 32 KiB of pseudo-random bytes
  const size_t N = 32768;
  char *bin = alloc_off(N);
  uint64_t x = 0x9E3779B97F4A7C15ULL;
  for (size_t i = 0; i < N; i++) { x ^= x << 13; x ^= x >> 7; x ^= x << 17; bin[i] = (char)x; }
  size_t b64len = simdutf::base64_length_from_binary(N);
  char *b64 = alloc_off(b64len);
  simdutf::binary_to_base64(bin, N, b64);
  char *b64out = alloc_off(b64len);
  char *binout = alloc_off(simdutf::maximal_binary_length_from_base64(b64, b64len));
  int itb = (int)(BYTES_PER_REP / (long)N);
  Acc enc, dec;
  enc.secs = best_time([&] { return simdutf::binary_to_base64(bin, N, b64out); }, itb);
  enc.bytes = double(N) * itb;
  dec.secs = best_time([&] { return simdutf::base64_to_binary(b64, b64len, binout).count; }, itb);
  dec.bytes = double(b64len) * itb;
  report("base64_encode", enc);
  report("base64_decode", dec);
  return 0;
}
