#include <cstdio>
#include <cstdlib>
#include <cstring>

int main() {
  char * array = new char[20];
  memset(array,12,20);
  delete[] array;
  array = new char[20];
  for(size_t i = 0; i < 21; i++) {
    printf("%d ", (int)array[i]);
  }
  printf("\n");
}
