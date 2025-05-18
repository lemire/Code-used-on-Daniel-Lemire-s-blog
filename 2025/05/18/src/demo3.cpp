#include <optional>
#include <iostream>

std::optional<int> divide(int a, int b) {
    if (b == 0) {
        return std::nullopt;
    }
    return a / b;
}

int main() {
    // Example 1: Valid division
    if (auto result = divide(10, 2)) {
        std::cout << "Result: " << *result << "\n";
    } else {
        std::cout << "Error: Division by zero\n";
    }

    // Example 2: Division by zero
    if (auto result = divide(8, 0)) {
        std::cout << "Result: " << *result << "\n";
    } else {
        std::cout << "Error: Division by zero\n";
    }

    // Example 3: Using value_or for a default
    int quotient = divide(15, 3).value_or(0);
    std::cout << "Quotient with default: " << quotient << "\n";

    return 0;
}