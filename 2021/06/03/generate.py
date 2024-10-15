from functools import partial
import math

def log2(n: int) -> int:
  return int(math.log2(n | 1))

def log10(n: int) -> int:
  return int(math.log10(n | 1))

def row(i: int, for_digit_count: bool = False) -> int:
  d = log10(2**i)
  # max(0, _) to avoid overflow
  return ((d + for_digit_count) << 32) + max(0, ((1 << 32) - 10**(d + 1)))

log10_table = list(map(row, range(32)))

def log10_fast(n: int) -> int:
  assert 0 <= n < 2**32, n
  return (n + log10_table[log2(n)]) >> 32

digit_count_table = list(map(partial(row, for_digit_count=True), range(32)))

def digit_count(n: int) -> int:
  assert 0 <= n < 2**32, n
  return (n + digit_count_table[log2(n)]) >> 32

tests = [
  0, 1, 2, 3, 9, 10, 99, 100, 999, 1000, 9999, 10000, 99999, 100000, 999999, 1000000, 9999999,
  10000000, 99999999, 100000000, 999999999, 1000000000, (1 << 32) - 1
]

for i in tests:
  log10_ref, log10_res = log10(i), log10_fast(i)
  dc_ref, dc_res = digit_count(i), digit_count(i)
  assert log10_ref == log10_res == dc_ref - 1 == dc_res - 1
