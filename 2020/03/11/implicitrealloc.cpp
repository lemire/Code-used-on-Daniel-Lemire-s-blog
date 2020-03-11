#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
void demo(int p) {
  char * array = new char[p]; 
  delete[] array; 
  char * newarray = new char[p+1];
  if(newarray != array) {
    std::cout << p << std::endl;
  }
}

int main() {
  for(int p = 1; p < 2000; p++)
    demo(p);
}
