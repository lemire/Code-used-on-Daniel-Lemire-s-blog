from math import log
def magicgu32(d):
    nmax = 1<<32
    nc = nmax - (nmax%d) - 1
    nbits = int(log(nmax,2))+1
    for p in [64] : 
        if 2**p > nc* (d-1-(2**p-1)%d):
            m = (2**p +d - 1 - (2**p-1)%d)//d
            return m,p
    #return 1,1
    throw ("can't find p")

for i in range(1,24):
  print (i,magicgu32(i))
