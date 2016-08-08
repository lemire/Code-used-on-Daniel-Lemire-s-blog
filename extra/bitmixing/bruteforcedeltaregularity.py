# no 4->4 bit mixing function has perfect avalanche and invertibility
# the functions go from {0,1,2,3} to {0,1,2,3}

def isinvertible(arr):
    return len(set(arr))== len(arr)

def deltareg(arr):
    count = [[0 for i in range(2)] for j in range(2)]
    dreg = True
    minsize = 4
    bady = 0
    for y in range(4):
        if(y == 0) : continue
        d = set()
        for x in range(4) :
            diff = (arr[x]+arr[(x-y)%4]) % 4#(arr[x] - arr[(x+y)%4]) % 4
            d.add(diff)
        if(minsize > len(set(d))):
            minsize = len(set(d))
            bady = y
        if (len(set(d))<> len(arr)) :
            dreg = False
    print(arr,minsize,bady)
    return dreg

for i1 in range(4):
    for i2 in range(4):
        for i3 in range(4):
            for i4 in range(4):
                arr = [i1,i2,i3,i4]
                if(isinvertible(arr)):
                  if(deltareg(arr)):
                    for i in range(100): print "success"
