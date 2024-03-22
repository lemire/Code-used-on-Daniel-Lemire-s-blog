#include <iostream>

/*
//  error: variable 'fact' declared with deduced type 'auto' cannot appear in its own initializer
auto fact = [](int n) -> int {
    if (n == 0) {
        return 1;
    } else {
        return n * fact(n - 1);
    }
};*/

std::function<int(int)> fact = [](int n) -> int {
    if (n == 0) {
        return 1;
    } else {
        return n * fact(n - 1);
    }
};

void print(int(*f)(int)) {
    for(int k = 1; k < 10; ++k) {
        std::cout << "Factorial of " << k << " is " << f(k) << std::endl;
    }
}


int main() {
    int n = 5;
    std::cout << "Factorial of " << n << " is " << fact(n) << std::endl;
    print([](int n) -> int { return fact(n); }); // OK 
    return EXIT_SUCCESS;
}