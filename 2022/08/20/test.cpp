#include <iostream>
#include <string>
#include <stdlib.h>

// no thread-safe, for multithreaded programs, it gets more challenging.
std::string message;

extern "C" { 
void __asan_on_error() {
  std::cout << "You caused an error: " << message << std::endl;
}
}


int main() {
  int array[8];
  for(int k = 0;; k++) {
    message = std::string("access at ") + std::to_string(k); 
    array[k] = 0;
  }
  return EXIT_SUCCESS;
}
