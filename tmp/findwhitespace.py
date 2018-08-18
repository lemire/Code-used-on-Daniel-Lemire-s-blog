whitespace = [0x9, 0xA, 0xB, 0xC, 0xD, 0x20, 0x85, 0xA0, 0x1680,  0x2000, 0x2001, 0x2002, 0x2003, 0x2004, 0x2005, 0x2006, 0x2007, 0x2008,0x2009, 0x200A, 0x2028, 0x2029, 0x202F, 0x205F, 0x3000]


nibble1 = [0 for i in range(16)]

nibble2 = [0 for i in range(16)]

nibble3 = [0 for i in range(16)]

nibble4 = [0 for i in range(16)]

# 0
for x in [0x9, 0xA, 0xB, 0xC, 0xD]:
    nibble1[x] |= 1
nibble2[0]|=1
nibble3[0]|=1
nibble4[0]|=1


# 1 0x85
nibble1[0x5]|=2
nibble2[0x8]|=2
nibble3[0]|=2
nibble4[0]|=2

# 2
nibble1[0]|=4
for x in [0x20, 0xA0]:
    nibble2[x>>4] |= 4
nibble3[0]|=4
nibble4[0]|=4

# 3 0x1680
nibble1[0]|=8
nibble2[0x8]|=8
nibble3[0x6]|=8
nibble4[0x1]|=8


# 4
for x in [0x2000,0x2001, 0x2002, 0x2003, 0x2004, 0x2005, 0x2006, 0x2007, 0x2008,0x2009, 0x200A]:
    nibble1[x & 0xF] |= 16
nibble2[0]|=16
nibble3[0]|=16
nibble4[0x2]|=16

# 5
for x in [0x2028, 0x2029, 0x202F]:
    nibble1[x & 0xF] |= 32
nibble2[0x2]|=32
nibble3[0]|=32
nibble4[0x2]|=32



# 6 0x205F,
nibble1[0xF]|=64
nibble2[0x5]|=64
nibble3[0x0]|=64
nibble4[0x2]|=64


# 7 0x3000
nibble1[0x0]|=128
nibble2[0x0]|=128
nibble3[0x0]|=128
nibble4[0x3]|=128

print(nibble1)
print(nibble2)
print(nibble3)
print(nibble4)
print()
nibble1.reverse()
nibble2.reverse()
nibble3.reverse()
nibble4.reverse()
print(nibble1)
print(nibble2)
print(nibble3)
print(nibble4)
print()
nibble1.reverse()
nibble2.reverse()
nibble3.reverse()
nibble4.reverse()
print(nibble1)
print(nibble2)
print(nibble3)
print(nibble4)
for i in whitespace:
    #print("i = ",i)
    n1 = i & 0xF
    n2 = (i >> 4) & 0xF
    n3 = (i >> 8) & 0xF
    n4 = (i >> 12)
    bigand = (nibble1[n1] & nibble2[n2] & nibble3[n3] & nibble4[n4]) != 0
    isw = i in whitespace
    #print(isw," ",bigand)
    assert(bigand == isw)
for i in range(1<<16):
    n1 = i & 0xF
    n2 = (i >> 4) & 0xF
    n3 = (i >> 8) & 0xF
    n4 = (i >> 12)
    bigand = (nibble1[n1] & nibble2[n2] & nibble3[n3] & nibble4[n4]) != 0
    isw = i in whitespace
    assert(bigand == isw)
