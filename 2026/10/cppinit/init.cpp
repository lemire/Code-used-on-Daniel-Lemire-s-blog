struct span {
  const char *p;
  int len;
  const char *end;
#ifdef BRACES
  span() {}
#else
  span() = default;
#endif
  span(const char *s, int n) : p(s), len(n), end(s + n) {}
};

struct box {
  span s;
  box() : s() {}
  box(span in) : s(in) {}
};

void consume(box &);

void walk(const char *s, int n) {
  for (int i = 0;; i++) {
    box b;
    b = i == n ? box() : box(span(s, i + 1));
    if (i == n) { return; }
    consume(b);
  }
}
