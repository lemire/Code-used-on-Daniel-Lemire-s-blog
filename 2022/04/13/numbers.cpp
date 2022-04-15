#include <iostream>

int main() {
    float x1 = 100000000;
    float x2 = 100000002;
    float avg = (x1+x2)/2;
    std::cout << ((x1-avg)*(x1-avg) + (x2-avg)*(x2-avg))/2 << std::endl;
    double dx1 = 100000000;
    double dx2 = 100000002;
    double davg = (dx1+dx2)/2;
    std::cout << ((dx1-davg)*(dx1-davg) + (dx2-davg)*(dx2-davg))/2 << std::endl;
}