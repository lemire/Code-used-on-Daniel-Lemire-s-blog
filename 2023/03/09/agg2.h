#pragma once
#include "agg1.h"
#include <string>


template<typename T, typename... Args>
inline void inner_concat_copy(char * out, T t) {
    t.copy(out, t.size());
}
template<typename T, typename... Args>
inline void inner_concat_copy(char * out, T t, Args... args) {
   t.copy(out, t.size());
   return inner_concat_copy(out + t.size(), args...);
}

template<typename... Args>
std::string concat2(Args... args) {
    if(count_all(args...) <= 2) {
        return concat0(args...);
    }
    std::string answer;
    answer.resize(size_all(args...));
    inner_concat_copy(answer.data(), args...);
    return answer;
}
