def rotate(x,i):
    return ((x>>i)|(x<<(64-i))) & 0xFFFFFFFFFFFFFFFF

def selection(x, wordcount):
    return x % wordcount

def fingerprint(x):
    return (1 << (x%64)) | (1 << ((x//64)%64)) | (1 << ((x//(64**2))%64)) | (1 << ((x//(64**3))%64))| (1 << ((x//(64**4))%64))| (1 << ((x//(64**5))%64))| (1 << ((x//(64**6))%64))#return x & rotate(x*12312341135,1) & rotate(x*12312341,5)

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
