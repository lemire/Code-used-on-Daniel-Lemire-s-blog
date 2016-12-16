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

M = 1<<8



# for modp in [18]:#range(2,M):
#     print("modp = ", modp)
#     binwidth = (modp-1).bit_length()
#     print("binwidth = ",binwidth)
#     def part1(z,p):
#         return (2**binwidth - modp) * (i // (2**binwidth))
#     def part2(z,p):
#         return  (z % (2**binwidth))
#     def fastmod(z,p):
#         return ((part1(z,p)+part2(z,p)) % (2**binwidth) )% modp
#     is_ok = True
#     for i in range(M):
#         #print(i,fastmod(i,modp),i%modp,part1(i,modp),part2(i,modp),part1(i,modp)+part2(i,modp))
#         if fastmod(i,modp) != i % modp :
#             is_ok = False
#             print("fails with ", i)
#             break
#         else:
#             print("works with ",i)


#for modp in range(1,M):
#    Mbinwidth = (M-1).bit_length()
#    binwidth = (modp-1).bit_length()
#    # we are going to use brute force
#    is_found = False
#    for multiplier in  range(1<<(4*Mbinwidth)):
#        if is_found : break
#        for shift in range(binwidth,4*Mbinwidth):
#          if is_found : break
#          is_mod = True
#          #print(modp,multiplier,shift)
#          for z in range(M):
#            if z % modp != ((z * multiplier)>>(shift-binwidth))%(1<<binwidth) :
#              is_mod = False
#              break
#          if is_mod:
#            print("found it",modp,multiplier,shift)
#            is_found = True

#import sys
#sys.exit(0)

for w in range(1,(M-1).bit_length()):
    modp = 2**w - 1
    binwidth = (modp-1).bit_length()
    print(modp,binwidth)
    #print(modp,binwidth)
    m,s=magicgu(M,modp)
    print(modp,m,s)
    for z in range(M):
        assert z // modp == (z * m)>>s
    is_mod = True
    for z in range(M):
        if z % modp != ((z * m)>>(s-binwidth))%(1<<binwidth) :
            is_mod = False
            break
    print(modp,is_mod,[((z * m)>>(s-binwidth))%(1<<binwidth) for z in range(M)])
    print(modp,is_mod,[z %modp for z in range(M)])
