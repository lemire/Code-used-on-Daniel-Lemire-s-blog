void fibo(int * x, int * y) {
  int c = * y;
  *y = *x + *y;
  *x = c;
}
