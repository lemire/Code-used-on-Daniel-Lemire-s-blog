
int gold(int x, int y) {
    auto lambda = [](int x, int y) -> int { return x + y; };
    return lambda(x, y);
}
int g(int x, int y) {
    auto lambda = []<class T>(T x, T y) -> T { return x + y; };
    return lambda(x, y);
}

int main() {
    return g(1,0);
}