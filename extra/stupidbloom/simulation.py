def rotate(x,i):
    return ((x>>i)|(x<<(64-i))) & 0xFFFFFFFFFFFFFFFF

def selection(x, wordcount):
    return x % wordcount

def fingerprint(x):
    #0.00559
    return (1 << (x%64)) | (1 << ((x//64)%64)) | (1 << ((x//(64**2))%64)) | (1 << ((x//(64**3))%64))| (1 << ((x//(64**4))%64))| (1 << ((x//(64**5))%64))| (1 << ((x//(64**6))%64))#| (1 << ((x//(64**7))%64))
    #0.00879
    #return (1 << (x%8)) | (1 << ((x>>8)%8 + 8)) | (1 << ((x>>16)%8 + 16))  | (1 << ((x>>24)%8 + 24))| (1 << ((x>>32)%8 + 32))| (1 << ((x>>40)%8 + 40)) | (1 << ((x>>48)%8 + 48))| (1 << ((x>>56)%8 + 56))
    # 0.01751
    #a = 4007277594418911151
    #b = 5509871213009170175
    #c1 = 43
    #c2 = 28
    #return (rotate(x,c1) & (a*rotate(x,c2)) & (b*x) &0xFFFFFFFFFFFFFFFF )
    # 0.01848
    #
    #return x & rotate(x*12312341135,1) & rotate(x*12312341,5)
    # 0.01848
    #return x & rotate(x*12312341135,1) & rotate(x*12312341,5)
    # 0.02799
    # return ((x<<1)|(x>>1)) & x & (7 * x)& (7311111 * x)
    # 0.00469
    #return (1 << (x%64)) | (1 << ((x//64)%64)) | (1 << ((x//(64**2))%64)) | (1 << ((x//(64**3))%64))| (1 << ((x//(64**4))%64))| (1 << ((x//(64**5))%64))| (1 << ((x//(64**6))%64))

# number of elements
N = 1000000

# let us allocate 16 bits to each element
capacity_in_bytes = N * 2

# we use 64 bit words
capacity_in_words = capacity_in_bytes // 8
array = [0 for i in range(capacity_in_words)]

# build set
import random
myset = [random.randint(0,2**64-1) for i in range(N)]

# let us create 
zero_count = 0
for x in myset:
    if(fingerprint(x) == 0): zero_count += 1
    array[selection(x,len(array))] |= fingerprint(x)

print(zero_count * 1.0 / N)

def contain(x, array):
    h = fingerprint(x)
    c = array[selection(x,len(array))]
    return (c & h) == h
#check
for x in myset:
    if(not contain(x, array)):
        raise "error"

print("ok")
test = 100000
bad = 0
for i in range(test):
    v = random.randint(0,2**64-1)
    if(contain(v, array)): 
        h = fingerprint(v)
        c = array[selection(v,len(array))]
        bad += 1
print(bad*1.0/test)
