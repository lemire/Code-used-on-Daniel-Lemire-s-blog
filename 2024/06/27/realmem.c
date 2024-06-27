#include <stdio.h>
#include <stdlib.h>
#if __linux__
#include <malloc.h>
size_t available(const void *ptr) {
  return malloc_usable_size(ptr);
}
#elif defined(__APPLE__) || defined(__FreeBSD__)
#include <malloc/malloc.h>
size_t available(const void *ptr) {
  return malloc_size(ptr);
}
#else
#error "Unsupported platform"
#endif
int main(void) {
  for (size_t i = 1; i < 100; i++) {
    char *c = (char *)malloc(i);
    printf("%zu %zu %zu\n", i, available(c), available(c) - i);
    free(c);
  }
}
