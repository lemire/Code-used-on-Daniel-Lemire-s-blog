#Idea is to generate a fair "shuffle" of the numbers between
#1 and x where x <= 12



precomp_factorial=[1,2,6,24,120,720,5040,40320,362880,3628800,39916800,479001600]


#[0,x) / (x-1)!

#m*d > 2^p




#n=3

#general numbers in [0,6]

#divide by 2, get number in [0,3), reminder in [0,2)
from math import *
def magicgu(nmax,d):
    nc = nmax - (nmax%d) - 1
    nbits = int(log(nmax,2))+1
    for p in range(2*nbits):
        if 2**p > nc* (d-1-(2**p-1)%d):
            m = (2**p +d - 1 - (2**p-1)%d)//d
            return m,p
    #return 1,1
    print("can't find p")


for i in range(1,12):
    f = precomp_factorial[i-1]
    m,p = magicgu((1<<32),f)
    print (f,hex(m),p,log(m,2))
    m,p = magicgu((1<<31),f)
    print (f,hex(m),p,log(m,2))
    print()
