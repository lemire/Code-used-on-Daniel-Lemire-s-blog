#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <ranges>
#include <string_view>

#if __cpp_lib_ranges_to_container >= 202202L
auto get_column_cxx23(std::string_view data, 
    size_t row_number, char delimiter = ',') {
  auto rows = data | std::views::split('\n');
  auto column =
      rows |
      std::views::transform(
          [delimiter, row_number](auto &&row) {
            auto fields = row | std::views::split(delimiter);
            auto it = std::ranges::begin(fields);
            std::advance(it, row_number);
            return *it;
          }) | std::ranges::to<std::string_view>();

  return column;
}
#endif
auto get_column_cxx20(std::string_view data, 
    size_t row_number, char delimiter = ',') {
  auto rows = data | std::views::split('\n');
  auto column =
      rows |
      std::views::transform(
          [delimiter, row_number](auto &&row) {
            auto fields = row | std::views::split(delimiter);
            auto it = std::ranges::begin(fields);
            std::advance(it, row_number);
            return *it;
          }) |
      std::views::transform([](auto &&rng) -> std::string_view {
        return std::string_view(&*rng.begin(), std::ranges::distance(rng));
      });
  return column;
}

std::vector<std::string> get_column_old(const std::string& input, size_t row_number, char delimiter = ',') {
    std::vector<std::string> rows;
    std::istringstream iss(input);
    std::string row;

    // Split the string into rows
    while (std::getline(iss, row)) {
        rows.push_back(row);
    }

    std::vector<std::string> result;
    for (const auto& r : rows) {
        std::istringstream row_stream(r);
        std::string field;
        size_t index = 0;

        while (std::getline(row_stream, field, delimiter)) {
            if (index == row_number) {
                result.push_back(field);
                break;
            }
            ++index;
        }
    }

    return result;
}


#endif