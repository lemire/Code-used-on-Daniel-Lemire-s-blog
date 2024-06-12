#include <malloc.h>
#include <stdio.h>
int main() {
  for (size_t i = 1; i < 1000; i++) {
    char *c = (char *)malloc(i);
    printf("%d %d %d\n", i, malloc_usable_size(c), malloc_usable_size(c) - i);
    free(c);
  }
}
