#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>

static void escape (void *p) {
  asm volatile ("" : : "g"(p) : "memory");
}

static void clobber () {
  asm volatile ("" : : : "memory");
}

constexpr std::size_t MB        = 1024 * 1024;
constexpr std::size_t page_size = 4096;

using clk        = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<clk>;
using dur_double = std::chrono::duration<double>;
using std::chrono::duration_cast;

class Timer {
public:
  Timer(size_t size, const std::string& cmd) : _size{size}, _cmd{cmd}, _start{clk::now()} {};

  ~Timer() {
    auto duration  = clk::now() - _start;
    auto elapsed_s = duration_cast<dur_double>(duration).count();
    std::printf("%7lu pages %5lu MB   %-30s %9.3f ms  %7.1f GB/s \n", _size / page_size, _size / MB,
                _cmd.data(), elapsed_s * 1000, _size / (1024. * 1024 * 1024.) / elapsed_s);
    ;
  };

private:
  time_point  _start;
  size_t      _size;
  std::string _cmd;
};

__attribute__((noinline)) char* benchcalloc(size_t s) {
  auto  t    = Timer{s, "calloc"};
  char* buf1 = (char*)calloc(s, sizeof(char));
  // we need to touch the memory because calloc will cheat
  for (size_t i = 0; i < s; i += page_size) buf1[i] = 0;
  buf1[s - 1] = 0;
  escape(&buf1);
  return buf1;
}

__attribute__((noinline)) char* bench(size_t s) {
  auto  t    = Timer{s, "new char[s]()"};
  char* buf1 = new char[s]();
  escape(&buf1);
  return buf1;
}

__attribute__((noinline)) char* benchnothrow(size_t s) {
  auto  t    = Timer{s, "new(std::nothrow) char[s]()"};
  char* buf1 = new (std::nothrow) char[s]();
  escape(&buf1);
  return buf1;
}

__attribute__((noinline)) char* benchallocandtouch(size_t s) {
  auto  t    = Timer{s, "new char[s] + touch"};
  char* buf1 = new char[s];
  for (size_t i = 0; i < s; i += page_size) buf1[i] = 0;
  buf1[s - 1] = 0;
  escape(&buf1);
  return buf1;
}

__attribute__((noinline)) void benchmemset(size_t s, char* buf) {
  auto t = Timer{s, "memset"};
  memset(buf, 0, s);
  escape(&buf);
}

__attribute__((noinline)) char* benchmemcpy(size_t s, char* buf) {
  char* newbuf = new char[s]();
  escape(&newbuf);
  auto t = Timer{s, "memcpy"};
  memcpy(newbuf, buf, s);
  escape(&newbuf);
  return newbuf;
}

int main() {
  for (size_t i = 256 * MB; i <= 1024 * MB; i *= 2) {
    for (int z = 0; z < 1; z++) {
      char* bc = benchcalloc(i);
      free(bc);
      char* b0 = benchallocandtouch(i);
      char* bz = benchnothrow(i);
      char* b  = bench(i);
      benchmemset(i, b);
      char* b2 = benchmemcpy(i, b);
      delete[] b;
      delete[] b2;
      delete[] b0;
      delete[] bz;
      std::cout << '\n';
    }
  }
  char* buftmp = new (std::nothrow) char[10'000'000'000'000L]; // just to show that it can be done
  if (buftmp != nullptr) {
    std::cout << "I just allocated a gigantic array." << '\n';
  }
  delete[] buftmp;
}
