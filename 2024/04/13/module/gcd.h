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

// From section 14.61 in https://cacr.uwaterloo.ca/hac/
// signed integer overflow may occur if
// std::max(a, b) >= std::numeric_limits<int_type>::max() / 8
template <std::unsigned_integral int_type>
bezout<int_type> binary_extended_gcd(int_type a, int_type b) {
  auto r = std::countr_zero(a | b);
  a >>= r;
  b >>= r;

  using sint_type = typename std::make_signed<int_type>::type;
  sint_type x = (sint_type)a;
  sint_type y = (sint_type)b;
  sint_type s = 1;
  sint_type t = 0;
  sint_type u = 0;
  sint_type v = 1;
  while (x) {
    while ((x & 1) == 0) { // a is even
      x /= 2;
      if (((s | t) & 1) == 0) {
        s /= 2;
        t /= 2;
      } else {
        s = (s + (sint_type)b) / 2;
        t = (t - (sint_type)a) / 2;
      }
    }
    while ((y & 1) == 0) { // b is even
      y /= 2;
      if (!((u | v) & 1)) {
        u /= 2;
        v /= 2;
      } else {
        u = (u + (sint_type)b) / 2;
        v = (v - (sint_type)a) / 2;
      }
    }
    if (x >= y) {
      x -= y;
      s -= u;
      t -= v;
    } else {
      y -= x;
      u -= s;
      v -= t;
    }
  }

  // Enable below if you want to make sure that
  // |x| + |y| is the minimal (primary)
  // and x <= y (secondarily)

  // if (y > 1) {
  //   a /= (int_type)y;
  //   b /= (int_type)y;
  // }
  // if (a && (int_type)std::abs(v) >= a) {
  //   sint_type _ = v / (sint_type)a;
  //   v -= _ * (sint_type)a;
  //   u += _ * (sint_type)b;
  // }
  // if (b && (int_type)std::abs(u) >= b) {
  //   sint_type _ = u / (sint_type)b;
  //   u -= _ * (sint_type)b;
  //   v += _ * (sint_type)a;
  // }
  // {
  //   sint_type u_ = u + (sint_type)b;
  //   sint_type v_ = v - (sint_type)a;
  //   if (std::abs(u_) + std::abs(v_) <= std::abs(u) + std::abs(v)) {
  //     u = u_;
  //     v = v_;
  //   }
  // }
  // {
  //   sint_type u_ = u - (sint_type)b;
  //   sint_type v_ = v + (sint_type)a;
  //   if (std::abs(u_) + std::abs(v_) <= std::abs(u) + std::abs(v)) {
  //     u = u_;
  //     v = v_;
  //   }
  // }

  return {(int_type)y << r, (int_type)u, (int_type)v};
}
