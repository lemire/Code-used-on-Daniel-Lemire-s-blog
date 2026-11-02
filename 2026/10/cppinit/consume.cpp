struct span {
  const char *p;
  int len;
  const char *end;
};

struct box {
  span s;
};

long counter = 0;

void consume(box &b) { counter += b.s.len; }
