#include <iostream>
#include <string>

int main() {
  std::string my_string;
  size_t capacity = size_t(-1);
  while(my_string.size() <= 10'000'000) {
    my_string += "a";
    if(my_string.capacity() != capacity) {
        std::cout << "Length: " << my_string.length() << ", Capacity: " << my_string.capacity() << std::endl;
        capacity = my_string.capacity();
    }
  }
  return EXIT_SUCCESS;
}