#include <utility>
#include <bit>

// computes the greatest common divisor between u and v
template <std::unsigned_integral int_type> int_type naive_gcd(int_type u, int_type v) {
  return (u % v) == 0 ? v : naive_gcd(v, u % v);
}

// computes the greatest common divisor between u and v
template <std::unsigned_integral int_type> int_type binary_gcd(int_type u, int_type v) {
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

// credit: Paolo Bonzini
template <std::unsigned_integral int_type>
int_type binary_gcd_noswap(int_type u, int_type v) {
  if (u == 0) {
    return v;
  }
  if (v == 0) {
    return u;
  }
  auto shift = std::countr_zero(u | v);
  u >>= std::countr_zero(u);
  do {
    int_type t = v >> std::countr_zero(v);
    if (u > t)
      v = u - t, u = t;
    else
      v = t - u;
  } while (v != 0);
  return u << shift;
}

template <std::unsigned_integral int_type> struct bezout {
  int_type gcd;
  int_type x;
  int_type y;
};

template <std::unsigned_integral int_type> struct pair {
  int_type old_value;
  int_type new_value;
};

// computes the greatest common divisor between a and b,
// as well as the Bézout coefficients x and y such as
// a*x + b*y = gcd(a,b)
template <std::unsigned_integral int_type>
bezout<int_type> extended_gcd(int_type u, int_type v) {
  pair<int_type> r = {u, v};
  pair<int_type> s = {1, 0};
  pair<int_type> t = {0, 1};
  while (r.new_value != 0) {
    auto quotient = r.old_value / r.new_value;
    r = {r.new_value, r.old_value - quotient * r.new_value};
    s = {s.new_value, s.old_value - quotient * s.new_value};
    t = {t.new_value, t.old_value - quotient * t.new_value};
  }
  return {r.old_value, s.old_value, t.old_value};
}


template <std::unsigned_integral int_type>
bezout<int_type> binary_extended_gcd(int_type a, int_type b) {
  int_type u = 1;
  int_type v = 0;
  int_type s = 0;
  int_type t = 1;
  auto r = std::countr_zero(a | b);
  a >>= r;
  b >>= r;

  auto alpha = a;
  auto beta = b;
  while ((a & 1) == 0) { // a is even
    a = a >> 1;
    if (((u | v) & 1) == 0) {
      u = u >> 1;
      v = v >> 1;
    } else {
      u = u + beta;
      u = u >> 1;
      v = v - alpha;
      v = v >> 1;
    }
  }
  while (a != b) {
    if ((b & 1) == 0) {
      b = b >> 1;
      if (((s | t) & 1) == 0) {
        s = s >> 1;
        t = t >> 1;
      } else {
        s = s + beta;
        s = s >> 1;
        t = t - alpha;
        t = t >> 1;
      }
    } else if (b < a) {
      std::swap(a, b);
      std::swap(u, v);
      std::swap(s, t);
    } else {
      b = b - a;
      s = s - u;
      t = t - v;
    }
  }
  return {a << r, s, t};
}