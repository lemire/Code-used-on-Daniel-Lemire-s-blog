#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
void demo() {
  char * array = new char[20]; 
  delete[] array; 
  char * newarray = new char[21];
  if(newarray == array) {
    std::cout << "they are the same!" << std::endl;
  }
}

int main() {
  demo();
}
