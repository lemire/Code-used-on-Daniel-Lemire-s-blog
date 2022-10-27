def parity(x):
    p = 0
    while(x>0):
        p ^= x & 1
        x >>= 1
    return p


def add_gray(lhs, rhs):
    # e and f, initialized with the parity of lhs and rhs
    # (0 means even, 1 means odd)
    e = parity(lhs)
    f = parity(rhs)
    res = 0
    pos = 1
    overflow_bit = 0
    while ((rhs>0) or (lhs>0) or ((e & f) > 0)):
        lhs_i = lhs & 1
        lhs >>=1
        rhs_i = rhs & 1
        rhs >>=1
        if((rhs == 0) and (lhs == 0)):
            overflow_bit = 1

        e_cpy = e
        f_cpy = f

        res_i = (e_cpy & f_cpy) ^ lhs_i ^ rhs_i
        res |= res_i * pos
        pos <<= 1

        e = (e_cpy & (~f_cpy)) ^ lhs_i
        f = ((~e_cpy) & f_cpy) ^ rhs_i
    
    return res

x = 0
inc = 1
for i in range(1023):
    x = add_gray(x,inc)
    print(bin(x))
b255 = 0b10000000
b1023 = 0b1000000000

print("---")
print(bin(b255))
print(bin(b1023))

print(bin(add_gray(b255,b1023)))

