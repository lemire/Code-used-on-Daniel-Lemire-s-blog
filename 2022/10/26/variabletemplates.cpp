#include <iostream>

template <typename T>
constexpr T PI = T(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679);

int main() {
    std::cout << std::hexfloat;
    std::cout << PI<double> << std::endl;
    std::cout << PI<float> << std::endl;
    std::cout << PI<int> << std::endl;

}