# no 4->4 bit mixing function has perfect avalanche and invertibility
# the functions go from {0,1,2,3} to {0,1,2,3}

w=3

basearr = [i for i in range(1<<w)]

import itertools

def isinvertible(arr):
    return len(set(arr))== len(arr)

def deltareg(arr):
    dreg = True
    minsize = (1<<w)
    bady = 0
    maps = []
    for y in range((1<<w)):
        if(y == 0) : continue
        d = set()
        for x in range((1<<w)) :
            diff = (arr[x]-arr[(x-y)%(1<<w)]) % (1<<w)
            d.add(diff)
        maps.append([y,d])
        if(minsize > len(set(d))):
            minsize = len(set(d))
            bady = y
        if (len(set(d))<> len(arr)) :
            dreg = False
    return minsize

bestdeltareg = 0
for perm in itertools.permutations(basearr):
    tdr = deltareg(perm)
    if(bestdeltareg < tdr):
        bestdeltareg = tdr
        print("best deltareg = ", tdr, " out of ", 1<<w)
        if(tdr == 1 << w):  print "success"
