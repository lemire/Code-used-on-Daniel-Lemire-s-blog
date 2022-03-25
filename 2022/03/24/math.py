m=2**45//10000  + 1
print(hex(m))
for i in range(100000000):
    expected = i //10000
    act =(m * i)//2**45
    if(act != expected):
        print("fk",i, act, expected)
        break
