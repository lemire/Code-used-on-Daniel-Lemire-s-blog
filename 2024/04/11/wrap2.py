M = 2
B = 2**8

x = 0
power = 0
limit = M
for i in range(10):
    print("i", i, "x", x/B**i, "power", x, "/", B**i)
    limit -= power
    limit *= B**2
    power = 0
    x*=B
    while power + 2*x + 1 <= limit:
        power += 2*x + 1
        x += 1

print(x/B**(10))