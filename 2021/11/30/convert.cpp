#include <iostream>
#include <stdlib.h>
#include <string>

float parse1(const char *c) {
  char *end;
  return strtod(c, &end);
}

float parse2(const char *c) {
  char *end;
  return strtof(c, &end);
}

double parsed1(const char *c) {
  char *end;
  return strtod(c, &end);
}

double parsed2(const char *c) {
  char *end;
  return strtof(c, &end);
}

void demo() {
  const char *test = "3.14159265358979323846264338327950";
  if (parsed1(test) != parsed2(test)) {
    std::cout << std::hexfloat;
    std::cout << parsed1(test) << std::endl;
    std::cout << parsed2(test) << std::endl;
    std::cout.precision(200);
    std::cout << std::fixed;
    std::cout << parsed1(test) << std::endl;
    std::cout << parsed2(test) << std::endl;
    double x1 = parsed1(test);

    double x2 = parsed2(test);
    double x3 = 3.14159265358979323846264338327950;
    std::cout << x3 - x1 << std::endl;
    std::cout << x2 - x3 << std::endl;
    std::cout << test << std::endl;
  }
}

void demo2() {
  const char *test = "0.004221370676532388";
  if (parse1(test) != parse2(test)) {
    std::cout << std::hexfloat;
    std::cout << parse1(test) << std::endl;
    std::cout << parse2(test) << std::endl;
    std::cout.precision(200);
    std::cout << std::fixed;
    std::cout << parse1(test) << std::endl;
    std::cout << parse2(test) << std::endl;
    double x1 = parse1(test);

    double x2 = parse2(test);
    double x3 = 0.004221370676532388;
    std::cout << x3 - x1 << std::endl;
    std::cout << x2 - x3 << std::endl;
    std::cout << test << std::endl;
  }
}

int main() {
  demo();
  demo2();
}
