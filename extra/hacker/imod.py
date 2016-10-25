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


def p2(x):
  return (2**(x-1).bit_length())

M = 1<<4
for modp in range(1,M):
    binwidth = (modp-1).bit_length()
    #print(modp,binwidth)
    m,s=magicgu(M,modp)
    for z in range(M):
        assert z // modp == (z * m)>>s
    is_mod = True
    for z in range(M):
        if z % modp != ((z * m)>>(s-binwidth))%(1<<binwidth) :
            is_mod = False
            break
    print(modp,is_mod,[((z * m)>>(s-binwidth))%(1<<binwidth) for z in range(M)])
