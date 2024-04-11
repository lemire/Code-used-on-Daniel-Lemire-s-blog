

M = 7
B = 2**8


x1 = 0
while (x1+1)**2 <= M:
    x1 += 1
x2 = 0
while (x1*B + x2 + 1)**2 <= M*B**2:
    x2 += 1
x3 = 0
while (x1*B**2 + x2*B + x3)**2 <= M*B**3:
    x3 += 1

print((x1*B**2+ x2*B + x3)/(B**2))


