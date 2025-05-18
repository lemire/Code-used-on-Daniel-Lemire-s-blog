#include <cstdlib>
#include <print>
#include <expected>
#include <string>
#include <concepts>

// Function using std::expected with concepts
template <std::integral int_type>
std::expected<int_type, std::string> divide(int_type a, int_type b) {
    if (b == 0) {
        return std::unexpected("Error: Division by zero");
    }
    return a / b;
}

int main() {
    if (auto res1 = divide(10, 2)) {
        std::println("Result: {:>4}, Message: Success", res1.value());
    } else {
        std::println("Error: {}", res1.error());
    }

    // Method 2: Ignore error, access value directly (with default if error)
    int quotient = divide(15, 3).value_or(0);
    std::println("Quotient only: {:>4}", quotient);

    return EXIT_SUCCESS;
}