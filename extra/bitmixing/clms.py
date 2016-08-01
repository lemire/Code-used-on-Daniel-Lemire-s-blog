w=4

def clmul(a,x):
    i = 0
    answer = 0
    while ( ( a>>i ) > 0 ) :
        bitset = (((a >> i) & 1) == 1)
        if(bitset):
            answer = answer ^ (x<<i)
        i = i + 1
    return answer

#def ms(a,x):
#    product = a * x # clmul(a,x)
#    #folded = ((product >> (w)) ^ product )%(1<<w)
#    folded = ( ( product >> (w)) ) %(1<<w)
#    return folded

def twomult(a,b,x):
    x = (x ^ ( x<< (w/2)))%(1<<w)
    x = (a * x)#%(1<<w)
    x = (x ^ ( x<< (w/2)))%(1<<w)
    x = (b * x)#%(1<<w)
    x = (x ^ ( x<< (w/2)))%(1<<w)
    return x%(1<<w)


def ms(a,x):
    product =a*x #clmul(a,x)
    return ((product >> (w-1))  )%(1<<w)

def msRegularity(a):
    counter = [0 for i in range(1<<w)]
    for x in range(1 << w):
        counter[ms(a,x)] += 1
    #print(counter)
    return max(counter)

def fncRegularity(fnc):
    counter = [0 for i in range(1<<w)]
    for x in range(1 << w):
        counter[fnc(x)] += 1
    #print(counter)
    return max(counter)


# regular, cannot be XOR universal.
def mstabs(x,y):
  counter = [0 for i in range(1<<w)]
  for a in range(1<<(2*w-1)):
      index = ms(a,x) ^ ms(a,y)
      counter[index] +=1
  return counter


def popcount(x) :
    i = 0
    answer = 0
    while ( ( x>>i ) > 0 ) :
        bitset = (((x >> i) & 1) == 1)
        if(bitset):
            answer = answer + 1
        i = i + 1
    return answer

def popcountcnt(x,counter) :
    for i in range(w) :
        bitset = (((x >> i) & 1) == 1)
        if(bitset):
            counter[i] += 1
        i = i + 1

def avalanche(a):
    counter = [0.0 for i in range(w)]
    count = 0.0
    for x in range(1<<w):
        basichash = ms(a,x)
        for i in range(w):
            flippedhash = ms(a,x^(1<<i))
            count += 1
            #popcountcnt(basichash ^ flippedhash,counter)
            counter[i] += popcount(basichash ^ flippedhash)
    return [x/count for x in counter]

def is_set(x,i):
    return ((x>>i) & 1) == 1

def strict_avalanche(fnc):
    counter = [[0.0 for i in range(w)] for i in range(w)]
    count = 0.0
    for x in range(1<<w):
        basichash = fnc(x)
        count += 1
        for i in range(w):
            flippedhash = fnc(x^(1<<i))
            for j in range(w):
                if(is_set(basichash ^ flippedhash,j)):
                    counter[i][j] += 1
    return [[x/count for x in c] for c in counter]


counter = 0
collection =[]

def maxa(arr):
    answer = 0
    for c in arr:
        for z in c:
            if(z > answer): answer = z
    return answer

def maxdeva(x,arr):
    answer = 0
    for c in arr:
        for z in c:
            dev = abs(z - x)
            if(dev > answer): answer = dev
    return answer

def mina(arr):
    answer = 100000
    for c in arr:
        for z in c:
            if(z < answer): answer = z
    return answer
import random


for a in range(1,1<<(w-1)):
    for b in range(a,1<<(w-1)):
        fnc = lambda x: twomult(2*a+1,2*b+1,x)
        assert fncRegularity(fnc )==1

bestdev = 1
bestfnc = lambda x:x
assert fncRegularity(bestfnc) == 1

def createms(a,b):
    return lambda x:ms(b,ms(a,x))
def createms3(a,b,c):
    return lambda x:ms(c,ms(b,ms(a,x)))

def createtwomult(a,b):
    return lambda x: twomult(a,b,x)


for a in range(1,1<<(2*w)):
    for b in range(1,1<<(2*w)):

        fnc =  createms(a,b) #lambda x: twomult(a,b,x)
        #print("considering ",a,b)
        #if(fncRegularity(fnc )<> 1): continue
        sv = strict_avalanche(fnc)
        #print(sv)
        md = maxdeva(0.5,sv)
        if(md < bestdev):
            bestdev = md
            bestfnc = fnc
            #assert fncRegularity(bestfnc) == 1
#assert fncRegularity(bestfnc) == 1

print("bestdev = ",bestdev,fncRegularity(bestfnc ))
for x in range(1<<w):
    print(bestfnc(x))


for a in range(1<<(2*w-1)):
    if(msRegularity(a) == 1):
        counter += 1
        core = a%(1<<(w-1))
        top = a>>(w-1)
        avl = avalanche(a)
        #print(popcount(top)*1.0/w,top,avl)
        if((max(avl) == min(avl)) and (max(avl) == 0.5)):
            print(popcount(top)*1.0/w,top,avl)
            print(strict_avalanche(lambda x: ms(a,x)))
            raw_input("all equal")
        collection.append((top,core));#a>>(w-1))
    #print(a,bin(a),msRegularity(a))
print(collection)
print(counter,(1<<(2*w-1)),w,counter * 1.0 / (1<<(2*w-1)))

#for x in range(1<<w):
#    for y in range(1<<w):
#      if(x <> y):
#        print (x,y,mstabs(x,y))
