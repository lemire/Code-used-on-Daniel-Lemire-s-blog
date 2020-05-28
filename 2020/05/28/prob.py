import random


def high(x,y):
    return (x*y >> 64) % 2**64



def runtests(N):
  o = ((2**64) % N)*2**64 // N
  m = 2**64 // N
  def inv(x):
    return m*(x+1) + high(x,o)
  worse = 0 
  for i in range(20000):
    z = random.randint(0,1<<64)
    H = high(z,N)
    I = inv(H)
    gap = abs(z-I)
    if(gap > worse): worse = gap
  return gap
from math import ceil
reallybad =  0
for t in range(200):
    N = random.randint(0,2**64)
    b = runtests(N)
    relerror = b / (ceil(2**64/N))
    if(relerror < 1.0):
        print("good")
    else: 
        print("bad")
    if(b > reallybad): 
        print("N=",N, " gap=",b)
        reallybad = b

print(reallybad)
