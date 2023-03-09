#pragma once
template<typename T>
inline size_t size_all(T t) {
   return t.size();
}
template<typename T, typename... Args>
inline size_t size_all(T t, Args... args) {
   return t.size() + size_all(args...) ;
}

template<typename T>
inline size_t count_all(T ) {
   return 1;
}
template<typename T, typename... Args>
inline size_t count_all(T , Args... args) {
   return 1 + count_all(args...) ;
}