#include <stdio.h>
#include <stdlib.h>

int main() {
  size_t large = 1099511627776;
  printf("allocating %zu bytes \n", large);
  printf("allocating %zu GB \n", large / (1024 * 1024));
  char *buffer = (char *)malloc(large);
  if (buffer == NULL) {
    printf("error!\n");
    return EXIT_FAILURE;
  }
  printf("Memory allocated\n");
  for (size_t i = 0; i < large; i += 4096) {
    buffer[i] = 0;
  }
  free(buffer);
  return EXIT_SUCCESS;
}
