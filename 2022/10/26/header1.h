

template <class T>
T f(T x, T y) {
    return x + y;
}

extern template int f(int,int);