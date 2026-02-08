#include "mylib.h"

int add(int a, int b) {
    return a + b;
}

size_t count_spaces(std::string_view sv) {
    size_t count = 0;
    for (char c : sv) {
        if (c == ' ') ++count;
    }
    return count;
}