#ifndef TRIM_H
#define TRIM_H
#include <string_view>
#include <ranges>


constexpr bool is_control_or_space(const char c) noexcept {
  return (unsigned char)c <= ' ';
}

std::string_view trim_classic(std::string_view input) noexcept {
  while (!input.empty() &&
         is_control_or_space(input.front())) {
    input.remove_prefix(1);
  }
  while (!input.empty() && is_control_or_space(input.back())) {
    input.remove_suffix(1);
  }
  return input;
}


std::string_view trim_ranges(std::string_view s) {
    auto view = s | std::views::drop_while(is_control_or_space) 
                    | std::views::reverse 
                    | std::views::drop_while(is_control_or_space) 
                    | std::views::reverse;
    if(std::ranges::empty(view)) {
        return std::string_view{};
    }

   return std::string_view{&*std::ranges::begin(view), 
    size_t(std::ranges::size(view))};
}


std::string_view trim_simple_ranges(std::string_view s) {
    auto first = std::ranges::find_if_not(s, is_control_or_space);
    if (first == s.end()) {
        return {};
    }
    auto rfirst = std::ranges::find_if_not(s | std::views::reverse, is_control_or_space);
    auto last = rfirst.base();
    return {first, last};
}
#endif  // TRIM_H