def rotate(x,i):
    return ((x>>i)|(x<<(64-i))) & 0xFFFFFFFFFFFFFFFF




# number of elements
N = 100000
import random
myset = [random.randint(0,2**64-1) for i in range(N)]

def count_ones(x):
    s = 0
    for i in range(64):
        s += (x >> i)&1
    return s

def compute_histo(fn):
    histo = [0 for i in range(65)]
    n = len(myset)
    for x in myset:
       histo[count_ones(fingerprint(x))] += 1.0 / n
    return histo
def fingerprint(x):
    return x & rotate(x*12312341135,1) & rotate(x*12312341,5)

print([int(x*100) for x in compute_histo(fingerprint)])

def fingerprint(x):
    return x & rotate(x,1) & rotate(x,5)

print([int(x*100) for x in compute_histo(fingerprint)])

def fingerprint(x):
    (1 << (x%64)) | (1 << ((x//64)%64)) | (1 << ((x//(64**2))%64)) | (1 << ((x//(64**3))%64))| (1 << ((x//(64**4))%64))| (1 << ((x//(64**5))%64))| (1 << ((x//(64**6))%64)) 

print([int(x*100) for x in compute_histo(fingerprint)])

