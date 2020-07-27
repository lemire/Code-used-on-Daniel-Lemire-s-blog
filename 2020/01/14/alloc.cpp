#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>

#ifdef __linux__
#include <sys/mman.h>
#endif

static void escape(void* p) { asm volatile("" : : "g"(p) : "memory"); }

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
    auto duration   = clk::now() - _start;
    auto elapsed_s  = duration_cast<dur_double>(duration).count();
    auto elapsed_ms = elapsed_s * 1000;
    auto pages      = _size / page_size;
    auto mbs        = _size / MB;
    auto gb_per_sec = _size / (1024. * MB) / elapsed_s;
    std::printf("%7lu pages %5lu MB   %-30s %9.3f ms  %7.2f GB/s \n", pages, mbs, _cmd.data(),
                elapsed_ms, gb_per_sec);
    ;
  };

private:
  size_t      _size;
  std::string _cmd;
  time_point  _start;
};

void calloc(size_t size) {
  char* buf;
  {
    auto t = Timer{size, __FUNCTION__};
    buf    = (char*)calloc(size, sizeof(char));
    for (size_t i = 0; i < size; i += page_size) buf[i] = 0;
    buf[size - 1] = 0;
    escape(&buf);
  }
  free(buf);
}

#ifdef __linux__
void mmap_populate(size_t size) {
  char* buf;
  {
    auto t = Timer{size, __FUNCTION__};
    buf    = (char *) mmap (NULL, size, PROT_READ|PROT_WRITE,
	      MAP_PRIVATE|MAP_POPULATE|MAP_ANONYMOUS, -1, 0);
    escape(&buf);
  }
  munmap(buf, size);
}
#endif

void just_new(size_t size) {
  char* buf;
  {
    auto t = Timer{size, __FUNCTION__};
    buf    = new(std::nothrow) char[size];
    escape(&buf);
  }
  delete[] buf;
}

void just_memalign(size_t size) {
  char* buf;
  {
    auto t = Timer{size, __FUNCTION__};
    posix_memalign((void**)&buf,64,size);
    escape(&buf);
  }
  free(buf);
}

void new_and_touch(size_t size) {
  char* buf;
  {
    auto t = Timer{size, __FUNCTION__};
    buf    = new char[size];
    for (size_t i = 0; i < size; i += page_size) buf[i] = 0;
    buf[size - 1] = 0;
    escape(&buf);
  }
  delete[] buf;
}

void new_and_memset(size_t size) {
  char* buf;
  {
    auto t = Timer{size, __FUNCTION__};
    buf    = new char[size];
    memset(buf,0,size);
    escape(&buf);
  }
  delete[] buf;
}
void new_and_value_init(size_t size) {
  char* buf;
  {
    auto t = Timer{size, __FUNCTION__};
    buf    = new char[size]();
    escape(&buf);
  }
  delete[] buf;
}

void new_and_value_init_nothrow(size_t size) {
  char* buf;
  {
    auto t = Timer{size, __FUNCTION__};
    buf    = new (std::nothrow) char[size]();
    escape(&buf);
  }
  delete[] buf;
}

void memset_existing_allocation(size_t size) {
  // note that we are NOT timing the value initialization of buf
  char* buf = new char[size]();
  escape(&buf);
  {
    auto t = Timer{size, __FUNCTION__};
    memset(buf, 1, size); // overwriting existing initialized allocation with `1`s
    escape(&buf);
  }
  delete[] buf;
}

void mempy_into_existing_allocation(size_t size) {
  char* buf = new char[size]();
  escape(&buf);
  // note that we are NOT timing the value initialization of buf or newbuf
  char* newbuf = new char[size]();
  escape(&newbuf);
  {
    auto t = Timer{size, __FUNCTION__};
    // copying from existing initialized allocation into another existing initialized allocation
    memcpy(newbuf, buf, size);
    escape(&newbuf);
  }
  delete[] buf;
  delete[] newbuf;
}

int main() {
  // for (size_t i = 256 * MB; i <= 1024 * MB; i *= 2) {
  for (size_t i = 34588; i <= 34588; i *= 2) {
    for(size_t j = 0; j < 10; j++) {
      calloc(i);
      just_new(i);
      just_memalign(i);
      new_and_touch(i);
      new_and_memset(i);
      new_and_value_init(i);
      new_and_value_init_nothrow(i);
      memset_existing_allocation(i);
      mempy_into_existing_allocation(i);
#ifdef __linux__
      mmap_populate(i);
#endif
      printf("=======\n");
    }
    std::cout << '\n';
  }
}
