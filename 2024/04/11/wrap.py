M = 2
B = 2**8

x = 0
power = 0
limit = M
for i in range(10):
    print("i", i, "x", x/B**i, "power", x, "/", B**i)
    limit *= B**2
    power *= B**2
    x*=B
    while power + 2*x + 1 <= limit:
        power += 2*x + 1
        x += 1
        assert(x**2 == power)
        assert(x**2 <= limit)

print(x/B**(10))