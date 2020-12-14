
#include <random>
#include <cstdlib>
#include <cstring>
#include <iostream>


struct uniform_generator {
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<double> dis;
  explicit uniform_generator(double a = 0, double b = 1): rd(), gen(rd()),  dis(a, b) {}

  double new_double() {
    return dis(gen);
  }
};


uint64_t to_uint64(double x) {
    uint64_t a;
    ::memcpy(&a,&x,sizeof(x));
    return a;
}

// works if x1, x2 >= 0
bool always_positive_comparator(double x1, double x2) {
    uint64_t i1 = to_uint64(x1);
    uint64_t i2 = to_uint64(x2);
    return i1 < i2;
}


void positive_comparison_test(uniform_generator & g) {
    double x1 = g.new_double();
    double x2 = g.new_double();
    if((x1 < x2) != always_positive_comparator(x1,x2)) {
        std::cout << to_uint64(x1) << " vs " << to_uint64(x2) << std::endl;
    }
}

uint64_t sign_flip(uint64_t x) {
   // credit http://stereopsis.com/radix.html
   // when the most significant bit is set, we need to
   // flip all bits
   uint64_t mask = uint64_t(int64_t(x) >> 63);
   // in all case, we need to flip the most significant bit
   mask |= 0x8000000000000000;
   return x ^ mask;
}    

bool generic_comparator(double x1, double x2) {
    uint64_t i1 = sign_flip(to_uint64(x1));
    uint64_t i2 = sign_flip(to_uint64(x2));
    return i1 < i2;
}


void generic_comparison_test(uniform_generator & g) {
    double x1 = g.new_double();
    double x2 = g.new_double();
    if((x1 < x2) != generic_comparator(x1,x2)) {
        std::cout << to_uint64(x1) << " vs " << to_uint64(x2) << std::endl;
    }
}

int main() {
    uniform_generator g(0,1);
    for(size_t i = 0; i < 100000; i ++) {
        positive_comparison_test(g);
    }
    uniform_generator gs(-100,100);
    for(size_t i = 0; i < 100000; i ++) {
        generic_comparison_test(gs);
    }
    return EXIT_SUCCESS;
}