dix = [10**i-1 for i in range(19)]

def countl(x):
    if(x == 0):
        return 64
    return 64 - (len(bin(x))-2)

maxv = 0xFFFFFFFFFFFFFFFF
for i in range(64+1):
    found = 0
    for x in dix:
        if(countl(x) == i):
            print(str(x) + "ULL,", end=" ")
            found = 1
            maxv  = x
            break
    if(found == 0):
        print(str(maxv) + "ULL,", end=" ")
print()

array = [-1 for i in range(64+1)]
for i in range(64):
    #print(i,countl(2**i), len(str(2**i)))
    array[countl(2**i)] = len(str(2**i))
array[countl(0)] = 1
print(array)