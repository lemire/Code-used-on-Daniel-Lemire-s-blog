// assume clang++ 18 or g++ 13
// compile with: c++ -std=c++23 -o csv csv.cpp
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_set>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::println("Usage: {} <csv_file>", argv[0]);
    return EXIT_FAILURE;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::println("Error: Cannot open file {}\n", argv[1]);
    return EXIT_FAILURE;
  }
  // Read first line for column labels
  std::string line;
  if (!std::getline(file, line)) {
    std::println("Error: Empty file {}", argv[1]);
    return EXIT_FAILURE;
  }

  // Parse first line into labels
  std::map<size_t, std::string> labels;
  auto cells = line | std::ranges::views::split(',');
  for (auto cell : cells) {
    std::string label(cell.begin(), cell.end());
    labels[labels.size()] = label;
  }
  std::map<std::string, std::unordered_set<std::string>> columns;
  while (std::getline(file, line)) {
    auto cells = line | std::ranges::views::split(',');
    for (auto [idx, cell] : std::ranges::views::enumerate(cells)) {
      columns[labels[idx]].insert(std::string(cell.begin(), cell.end()));
    }
  }
  // Print results using labels
  for (const auto &[label, values] : columns) {
    std::println("Column {}: {} distinct values", label, values.size());
  }
  return EXIT_SUCCESS;
}