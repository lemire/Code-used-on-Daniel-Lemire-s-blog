#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>

static void escape(void* p) { asm volatile("" : : "g"(p) : "memory"); }

static void clobber() { asm volatile("" : : : "memory"); }

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

void bench_calloc(size_t s) {
  char* buf;
  {
    auto  t   = Timer{s, std::string(__FUNCTION__).substr(6)};
    char* buf = (char*)calloc(s, sizeof(char));
    // we need to touch the memory because calloc will cheat
    for (size_t i = 0; i < s; i += page_size) buf[i] = 0;
    buf[s - 1] = 0;
    escape(&buf);
  }
  free(buf);
}

void bench_new_and_touch(size_t s) {
  char* buf;
  {
    auto  t   = Timer{s, std::string(__FUNCTION__).substr(6)};
    char* buf = new char[s];
    for (size_t i = 0; i < s; i += page_size) buf[i] = 0;
    buf[s - 1] = 0;
    escape(&buf);
  }
  delete[] buf;
}

void bench_new_and_value_init(size_t s) {
  char* buf;
  {
    auto  t   = Timer{s, std::string(__FUNCTION__).substr(6)};
    char* buf = new char[s]();
    escape(&buf);
  }
  delete[] buf;
}

void bench_new_and_value_init_nothrow(size_t s) {
  char* buf;
  {
    auto  t   = Timer{s, std::string(__FUNCTION__).substr(6)};
    char* buf = new (std::nothrow) char[s]();
    escape(&buf);
  }
  delete[] buf;
}

void bench_memset_existing_allocation(size_t s) {
  char* buf = new char[s]();
  escape(&buf);
  {
    auto t = Timer{s, std::string(__FUNCTION__).substr(6)};
    memset(buf, 1, s);
    escape(&buf);
  }
  delete[] buf;
}

void bench_mempy_into_existing_allocation(size_t s) {
  char* buf = new char[s]();
  escape(&buf);
  char* newbuf = new char[s]();
  escape(&newbuf);
  {
    auto t = Timer{s, std::string(__FUNCTION__).substr(6)};
    memcpy(newbuf, buf, s);
    escape(&newbuf);
  }
  delete[] buf;
  delete[] newbuf;
}

int main() {
  for (size_t i = 256 * MB; i <= 1024 * MB; i *= 2) {
    bench_calloc(i);
    bench_new_and_touch(i);
    bench_new_and_value_init_nothrow(i);
    bench_new_and_value_init(i);
    bench_memset_existing_allocation(i);
    bench_mempy_into_existing_allocation(i);
    std::cout << '\n';
  }
}
