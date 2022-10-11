from math import log


def log2(x):
  return log(x) / log(2)


def f(n):
  return n * log2(n) - 2**(log2(n)) + 1 - 0.2645 * n


for k in range(5, 13):
  print(k, f(2**k) / 2**k)

