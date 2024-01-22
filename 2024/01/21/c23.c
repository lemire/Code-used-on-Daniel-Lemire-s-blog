// Only include stdio.h if it exists
#if __has_include (<stdio.h>)
#include <stdio.h>
#endif

#include <stdlib.h>

[[deprecated]]
void f() {

}

[[nodiscard]]
int g(int x) {
    return x + 1;
}

int main() {
    f();   // compile-time warning: 'f' is deprecated
    g(1);  // compile-time warning: ignoring return value of function declared with 'nodiscard' attribute
    auto x = 0b1111;
    typeof(x) y = 1'000'000; // type of y is the same as x
    printf("%d\n", x); // prints 15
    printf("%d\n", y); // prints 1000000
    constexpr int N = 10;
    // compile-time asserts using static_assert
    static_assert (N == 10, "N must be 10");
    bool a[N]; // array of N booleans
    for (int i = 0; i < N; ++i) {
        a[i] = true;
    }
    printf("%d\n", a[0]); // prints 1

}