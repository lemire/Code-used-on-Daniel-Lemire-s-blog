int fast_decode(unsigned int x) {
    return (x >> 1) ^ (-(x&1));
}

unsigned int fast_encode(int x) {
    return (2*x) ^ (x >>(sizeof(int) * 8 - 1));
}

#include <iostream>

int main() {
    for(int x = -20; x <= 20; x++) {
        std::cout << "<tr><td>"<< x << "</td><td>" << fast_encode(x) << "</td></tr>" << std::endl;
        assert(x == fast_decode(fast_encode(x)));
    }
}