#include <iostream>
#include <functional>

auto return1 = [](int n) -> int { return 1; };
/*
//  error: variable 'fact' declared with deduced type 'auto' cannot appear in its own initializer
auto fact1 = [](int n) -> int {
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


int (*factorial)(int) = [](int n) -> int {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
};
void print(int(*f)(int)) {
    for(int k = 1; k < 10; ++k) {
        std::cout << "Factorial of " << k << " is " << f(k) << std::endl;
    }
}


int (*lfactorial)(int) =  [](int n)  -> int {
    if (n == 0) {
        return 1;
    } else {
        return n * lfactorial(n - 1);
    }

};

int factorialc(int n) {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int functionc() {
    return factorialc(10);
}


int functionl() {
    return lfactorial(10);
}

int main() {
    int n = 5;
    std::cout << "Factorial of " << n << " is " << fact(n) << std::endl;
    //print(fact); // NOT OK

    print(factorial); // OK
    print([](int n) -> int { return fact(n); }); // OK 
    return EXIT_SUCCESS;
}