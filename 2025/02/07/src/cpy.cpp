#include <cstring>
#include <iostream>
#include <thread>

// Size of the arrays
const int array_size = 4096;
const int copy_count = 1000000;

// Arrays to be copied between
char A[array_size] = {0};
char B[array_size] = {0};

// Function for thread copying from A to B
void copy_A_to_B() {
  for (int i = 0; i < copy_count; ++i) {
    std::memcpy(B, A, array_size);
  }
}

// Function for thread copying from B to A
void copy_B_to_A() {
  for (int i = 0; i < copy_count; ++i) {
    std::memcpy(A, B, array_size);
  }
}

int main() {
  // Initialize array A with some values for demonstration
  for (int i = 0; i < array_size; ++i) {
    A[i] = 'A';
    B[i] = 'B';
  }

  // Create threads
  std::thread t1(copy_A_to_B);
  std::thread t2(copy_B_to_A);

  // Wait for threads to finish
  t1.join();
  t2.join();

  // Check the results (just the first few bytes for brevity)
  std::cout << "First few bytes of A: ";
  for (int i = 0; i < 10; ++i) {
    std::cout << A[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "First few bytes of B: ";
  for (int i = 0; i < 10; ++i) {
    std::cout << B[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}