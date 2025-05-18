#include <print>
#include <string>
#include <concepts>


// Function using auto with trailing return type
template <std::integral int_type>
std::tuple<int_type, std::string> divide(int_type a, int_type b) {
    if (b == 0) {
        return {0, "Error: Division by zero"};
    }
    return {a / b, "Success"};
}


int main() {
    // Method 1: Recover both values using structured binding
    auto [result, message] = divide(10, 2);
    std::println("Result: {}, Message: {}", result, message);

    // Method 2: Ignore message using C++23 _ placeholder
    auto [quotient, _] = divide(15, 3);
    std::println("Quotient only: {}", quotient);

    std::string msg = "Hello";
    int i = 42;
    std::tie(i,msg) = divide(3, 0);

    // Method 3: Handle error case
    auto [res, err] = divide(8, 0);
    if (err != "Success") {
        std::println("Error occurred: {}", err);
    } else {
        std::println("Result: {}", res);
    }


    // Method 3: Handle error case
    if (auto [res, err] = divide(8, 0); err != "Success") {
        std::println("Error occurred: {}", err);
    } else {
        std::println("Result: {}", res);
    }

    return EXIT_SUCCESS;
}