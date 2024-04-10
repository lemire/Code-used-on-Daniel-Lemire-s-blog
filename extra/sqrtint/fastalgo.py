# We compute sqrt(2) byte by byte using the fast algorithm
# https://arxiv.org/pdf/2312.15338.pdf
# We then correct to get the value of 1-sqrt(0.5) to the desired precision

B = 2**8

def biterate(t):
    if t[0] >= 2 * t[1] + 1:
        return (t[0] - 2 * t[1] - 1, t[1] + 1)
    else:
        return (B**2 * t[0], B * t[1])


def broot(x, words):
    start = (x, 0)
    while start[1] < B**words:
        start = biterate(start)
    return start


def compute(bytes):
    _, t1 = broot(2, bytes + 1)
    t1 //= B
    return B ** (bytes) - t1 // 2 - 1


for bytes in [2, 4, 8, 16, 32]:
    print(bytes, compute(bytes), bin(compute(bytes)))

# Expected output:
# 2 19195 0b100101011111011
# 4 1257966796 0b1001010111110110000110011001100
# 8 5402926248376769403 0b100101011111011000011001100110000000110001000011001101101111011
# ...
