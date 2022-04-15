#include <iostream>

int main() {
    float x1 = 10000;
    float x2 = 10002;
    float avg = (x1+x2)/2;
    std::cout << ((x1-avg)*(x1-avg) + (x2-avg)*(x2-avg))/2 << std::endl;
    double dx1 = 10000;
    double dx2 = 10002;
    double davg = (dx1+dx2)/2;
    std::cout << ((dx1-davg)*(dx1-davg) + (dx2-davg)*(dx2-davg))/2 << std::endl;
}