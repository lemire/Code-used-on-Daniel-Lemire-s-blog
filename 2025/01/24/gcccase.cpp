#include <iostream>
#include <regex>
#include <chrono>

int main() {
    std::string text = "Everyone loves Lucy.";
    std::regex pattern(R"(.*+s}}@w)"); // Raw string literal for regex

    // Start timing
    std::cout << "starting the regex operation" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    // Perform regex search
    std::smatch match;
    bool found = std::regex_search(text, match, pattern);

    // End timing
    auto end = std::chrono::high_resolution_clock::now();

    // Compute duration in seconds
    std::chrono::duration<double, std::ratio<1>> duration = end - start;

    // Output results
    std::cout << "Regex search result: " << (found ? "Match found" : "No match") << std::endl;
    std::cout << "Duration: " << duration.count() << " seconds" << std::endl;

    return 0;
}
