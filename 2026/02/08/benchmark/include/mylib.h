#ifndef MYLIB_H
#define MYLIB_H

#include <string_view>

inline int inline_add(int a, int b) {
    return a + b;
}
int add(int a, int b);

inline size_t count_spaces_inline(std::string_view sv) {
    size_t count = 0;
    for (char c : sv) {
        if (c == ' ') ++count;
    }
    return count;
}

size_t count_spaces(std::string_view sv);

#endif // MYLIB_H