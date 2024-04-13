#include <bit>

export module gcd;

// computes the greatest common divisor between u and v
export template <typename int_type> int_type naive_gcd(int_type u, int_type v) {
  return (u % v) == 0 ? v : naive_gcd(v, u % v);
}

// computes the greatest common divisor between u and v
export template <typename int_type>
int_type binary_gcd(int_type u, int_type v) {
  if (u == 0) {
    return v;
  }
  if (v == 0) {
    return u;
  }
  auto shift = std::countr_zero(u | v);
  u >>= std::countr_zero(u);
  do {
    v >>= std::countr_zero(v);
    if (u > v) {
      std::swap(u, v);
    }
    v = v - u;
  } while (v != 0);
  return u << shift;
}

template <typename int_type> struct bezout {
  int_type gcd;
  int_type x;
  int_type y;
};

// computes the greatest common divisor between a and b,
// as well as the Bézout coefficients x and y such as
// a*x + b*y = gcd(a,b)
export template <typename int_type>
bezout<int_type> extended_gcd(int_type u, int_type v) {
  std::pair<int_type, int_type> r = {u, v};
  std::pair<int_type, int_type> s = {1, 0};
  std::pair<int_type, int_type> t = {0, 1};
  while (r.second != 0) {
    auto quotient = r.first / r.second;
    r = {r.second, r.first - quotient * r.second};
    s = {s.second, s.first - quotient * s.second};
    t = {t.second, t.first - quotient * t.second};
  }
  return {r.first, s.first, t.first};
}