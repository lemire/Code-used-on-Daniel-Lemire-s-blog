# no 4->4 bit mixing function has perfect avalanche and invertibility

def isinvertible(arr):
    return len(set(arr))== len(arr)

def avalanche(arr):
    count = [[0 for i in range(2)] for j in range(2)]
    for i in range(4):
        base = arr[i]
        for j in range(2):
            off = arr[i ^ (1<<j)]
            #print(base ^off)
            for k in range(2):
                if(((base ^ off)>>k)&1 == 1):
                    count[k][j]+=1
    target = 4/2
    for i in range(2):
        for j in range(2):
            if(count[i][j] <> target): return False
    return True

for i1 in range(4):
    for i2 in range(4):
        for i3 in range(4):
            for i4 in range(4):
                if(avalanche([i1,i2,i3,i4])):
                    print("success", isinvertible([i1,i2,i3,i4]))
