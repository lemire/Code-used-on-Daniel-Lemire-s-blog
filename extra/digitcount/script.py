import math, sys
def log2(n: int) -> int:
  return int(math.log2(n | 1))

def log10(n: int) -> int:
  return int(math.log10(n | 1))

def f32(i):
    d = log10(2**i)
    # max(0, _) to avoid overflow
    return ((d + 1) << 32) + max(0, ((1 << 32) - 10**(d + 1)))

print([hex(f32(j)) for j in range(0, 32)])


def digit_count32(n: int) -> int:
  assert 0 <= n < 2**32, n
  return (n + f32(log2(n))) >> 32
number = 1
while (number < 2**32):
    assert(digit_count32(number) == len(str(number)))
    number *= 10

def f64(i):
    d = log10(2**i)
    # max(0, _) to avoid overflow
    return ((d + 1) << 64) + max(0, ((1 << 64) - 10**(d + 1)))


def count_digit64(x):
    return (x + f64(log2(x))) // 2**64
number = 1


while (number < 2**64):
    print("===")
    print(count_digit64(number), len(str(number)), number)
    assert(len(str(number)) == count_digit64(number))
    print(count_digit64(number-1), len(str(number-1)), number-1)
    assert(len(str(number-1)) == count_digit64(number-1))
    print(count_digit64(number+1), len(str(number+1)), number+1)
    assert(len(str(number+1)) == count_digit64(number+1))
    number *= 10


print([[hex(f64(j)//2**64), hex(f64(j)%2**64)] for j in range(0, 64)])

table = [[f64(j) // 2**64, f64(j) % 2**64] for j in range(0, 64)]
print("uint64_t table[64][2] = {")
for pair in table:
    print(f"    {{ 0x{hex(pair[0])[2:]:0>2}, 0x{hex(pair[1])[2:]:0>16}ULL }},")
print("};")

print("uint64_t tablelow[64] = {")
for pair in table:
    print(f"    0x{hex(pair[0])[2:]:0>2},")
print("};")

print("uint64_t tablehigh[64] = {")
for pair in table:
    print(f"     0x{hex(pair[1])[2:]:0>16}ULL,")
print("};")
