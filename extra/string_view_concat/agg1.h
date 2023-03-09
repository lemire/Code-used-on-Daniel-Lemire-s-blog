#pragma once
#include "sum.h"
#include <string>

template<typename T, typename... Args>
inline void inner_concat(std::string& buffer, T t) {
   buffer.append(t);
}

template<typename T, typename... Args>
inline void inner_concat(std::string& buffer, T t, Args... args) {
   buffer.append(t);
   return inner_concat(buffer, args...);
}

template<typename... Args>
std::string concat1(Args... args) {
    std::string answer;
    answer.reserve(size_all(args...));
    inner_concat(answer, args...);
    return answer;
}




template<typename... Args>
std::string concat0(Args... args) {
    std::string answer;
    inner_concat(answer, args...);
    return answer;
}
