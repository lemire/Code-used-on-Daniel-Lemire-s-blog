#include <bit>
#include <utility>

// computes the greatest common divisor between u and v
template <std::unsigned_integral int_type>
int_type naive_gcd(int_type u, int_type v) {
  return (u % v) == 0 ? v : naive_gcd(v, u % v);
}

template <std::unsigned_integral int_type>
int_type gcd_noswap(int_type u, int_type v) {
  if (v == 0) {
    return u;
  }
  do {
    int_type r = u % v;
    if (r == 0)
      return v;
    u = v;
    v = r;
  } while (true);
}

// computes the greatest common divisor between u and v
template <std::unsigned_integral int_type>
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

// credit: Jonathan Webster
template <std::unsigned_integral int_type>
int_type gcd_webster1(int_type u, int_type v) {
  int shift;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);
  do {
    v >>= __builtin_ctz(v);
    if (u > v)
      std::swap(u, v);
    v = v - u;
  } while (v != 0);
  return u << shift;
}

// credit: Jonathan Webster
// reference: https://maths-people.anu.edu.au/~brent/pub/pub077.html
template <std::unsigned_integral int_type>
int_type gcd_webster2(int_type u, int_type v) {
  int shift;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);
  do {
    v >>= __builtin_ctz(v);
    if (u > v)
      std::swap(u, v);
    (2 & (u^v)) ? v = v + u : v =  v - u; // warning: might overflow!!!
  } while (v != 0);
  return u << shift;
}


// inspired by: Jonathan Webster
// reference: https://maths-people.anu.edu.au/~brent/pub/pub077.html
template <class T> T hybrid2_binary_gcd(T u, T v) {
  if (u < v) {
    std::swap(u, v);
  }
  if (v == 0) {
    return u;
  }
  u %= v;
  if (u == 0) {
    return v;
  }
  auto zu = std::countr_zero(u);
  auto zv = std::countr_zero(v);
  auto shift = std::min(zu, zv);
  u >>= zu;
  v >>= zv;
  do {
    bool is_set = (2 & (u^v));
    T u_plus_v = u + v;
    T u_minus_v = u - v;
    int shift = is_set ? std::countr_zero(u_plus_v) : std::countr_zero(u_minus_v);
    if (u > v) {
      u = v, v = is_set ? u_plus_v : u_minus_v;
    } else {
      v = is_set ? u_plus_v : - u_minus_v;
    }
    v >>= shift;
  } while (v != 0);
  return u << shift;
}

template <class T> T hybrid_binary_gcd(T u, T v) {
  if (u < v) {
    std::swap(u, v);
  }
  if (v == 0) {
    return u;
  }
  u %= v;
  if (u == 0) {
    return v;
  }
  auto zu = std::countr_zero(u);
  auto zv = std::countr_zero(v);
  auto shift = std::min(zu, zv);
  u >>= zu;
  v >>= zv;
  do {
    T u_minus_v = u - v;
    if (u > v)
      u = v, v = u_minus_v;
    else
      v = v - u;
    v >>= std::countr_zero(u_minus_v);
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

// This computes just one of the Bézout coefficients
template <std::unsigned_integral int_type>
bezout<int_type> extended_one_gcd(int_type u, int_type v) {
  pair<int_type> r = {u, v};
  pair<int_type> s = {1, 0};
  while (r.new_value != 0) {
    auto quotient = r.old_value / r.new_value;
    r = {r.new_value, r.old_value - quotient * r.new_value};
    s = {s.new_value, s.old_value - quotient * s.new_value};
  }
  return {r.old_value, s.old_value, 0};
}

// From section 14.61 in https://cacr.uwaterloo.ca/hac/
// Warning: signed integer overflow may occur if
// std::max(a, b) >= std::numeric_limits<int_type>::max() / 16
template <std::unsigned_integral int_type>
bezout<int_type> binary_extended_gcd(int_type a, int_type b) {
  using sint_type = typename std::make_signed<int_type>::type;

  if (a == 0)
    return {b, 0, !!b}; // {0, 0, 0} if b == 0 else {b, 0, 1}
  if (b == 0)
    return {a, 1, 0};

  bool swapped = false;
  if (a > b) {
    swapped = true;
    std::swap(a, b);
  }

  auto r = std::countr_zero(a | b);
  a >>= r;
  b >>= r;

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
        s = std::midpoint(s, (sint_type)b);
        t = std::midpoint(t, -(sint_type)a);
      }
    }
    while ((y & 1) == 0) { // b is even
      y /= 2;
      if (((u | v) & 1) == 0) {
        u /= 2;
        v /= 2;
      } else {
        u = std::midpoint(u, (sint_type)b);
        v = std::midpoint(v, -(sint_type)a);
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

  if (swapped) {
    std::swap(a, b);
    std::swap(u, v);
    std::swap(s, t);
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



template <class int_type> int_type hybrid_pm_binary_gcd(int_type u, int_type v) {
  if (u < v) {
    std::swap(u, v);
  }
  if (v == 0) {
    return u;
  }
  u %= v;
  if (u == 0) {
    return v;
  }
  auto zu = std::countr_zero(u);
  auto zv = std::countr_zero(v);
  auto shift = std::min(zu, zv);
  u >>= zu;
  v >>= zv;
  do {
      v >>= std::countr_zero(v);
      if (u > v) {
        std::swap(u,v);
      }
      (2 & (u^v)) ? v = v + u : v = v - u;
    } while (v != 0);

  return u << shift;
}

template <std::unsigned_integral int_type>
unsigned int ui_hybrid_pm_binary_gcd(int_type u, int_type v) {
  if (u < v) {
    std::swap(u, v);
  }
  if (v == 0) {
    return u;
  }
  u %= v;
  if (u == 0) {
    return v;
  }
  unsigned int zu = std::countr_zero(u);
  unsigned int zv = std::countr_zero(v);
  int shift = std::min(zu, zv);
  u >>= zu;
  v >>= zv;
  do {
      v >>= std::countr_zero(v);
      if (u > v) {
        std::swap(u,v);
      }
      (2 & (u^v)) ? v = v + u : v = v - u;
    } while (v != 0);

  return u << shift;
}


template <std::unsigned_integral int_type>
int_type hybrid_binary_gcd_noswap(int_type u, int_type v) {
  if (u < v) {
    std::swap(u, v);
  }
  if (v == 0) {
    return u;
  }
  u %= v;
  if (u == 0) {
    return v;
  }
  auto zu = std::countr_zero(u);
  auto zv = std::countr_zero(v);
  auto shift = std::min(zu, zv);
  u >>= zu;
  v >>= zv;
  do {
    int_type u_minus_v = u - v;
    if (u > v)
      u = v, v = u_minus_v;
    else
      v = v - u;
    v >>= std::countr_zero(u_minus_v);
  } while (v != 0);
  return u << shift;
}

template <std::unsigned_integral int_type>
int_type ui_hybrid_binary_gcd_noswap(int_type u, int_type v) {
  if (u < v) {
    std::swap(u, v);
  }
  if (v == 0) {
    return u;
  }
  u %= v;
  if (u == 0) {
    return v;
  }
  int_type zu = std::countr_zero(u);
  int_type zv = std::countr_zero(v);
  int shift = std::min(zu, zv);
  u >>= zu;
  v >>= zv;
  do {
    int_type u_minus_v = u - v;
    if (u > v)
      u = v, v = u_minus_v;
    else
      v = v - u;
    v >>= std::countr_zero(u_minus_v);
  } while (v != 0);
  return u << shift;
}



template <std::unsigned_integral int_type>
int_type ui_pm_binary_gcd(int_type u, int_type v) {
  int shift;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  shift = std::countr_zero(u | v);
  u >>= std::countr_zero(u);
  do {
    v >>= std::countr_zero(v);
    if (u > v)
      std::swap(u, v);
    (2 & (u^v)) ? v = v + u : v =  v - u;
  } while (v != 0);
  return u << shift;
}

template <std::unsigned_integral int_type>
int_type ui_binary_gcd_noswap(int_type u, int_type v) {
  if (u == 0) {
    return v;
  }
  if (v == 0) {
    return u;
  }
  int shift = std::countr_zero(u | v);
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