

#include <algorithm>
#include <iostream>
#include <list>
#include <chrono> 
#include <vector>
int main() {
  for(size_t size = 1024; size <  1024 * 1024; size*=2) {
    std::list<int> l;
    std::vector<int> v;
    for(int z = 0; z < (int) size; z++) l.push_back(z);
    for(int z = 0; z < (int) size; z++) v.push_back(z);
     std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    int sum = 0;
    for (int n : l) { sum += n; }
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    for (int n : v) { sum += n; }
    if(sum  == 0) printf("bug\n");
    std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
    double dif = std::chrono::duration_cast<std::chrono::nanoseconds>( t2 - t1 ).count();
    double div = std::chrono::duration_cast<std::chrono::nanoseconds>( t3 - t2 ).count();
    std::cout << size << " " << dif / size << " " << div /size << " " << dif / div<< std::endl; 
  }
}
