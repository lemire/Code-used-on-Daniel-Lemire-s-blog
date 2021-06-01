
def rotate(x,i):
    return ((x>>i)|(x<<(64-i))) & 0xFFFFFFFFFFFFFFFF

def selection(x, wordcount):
    return x % wordcount

# number of elements
N = 100000
# build set
import random
myset = [random.randint(0,2**64-1) for i in range(N)]

def fpp(myset, fingerprint):
  capacity_in_words = len(myset) * 2 // 8
  array = [0 for i in range(capacity_in_words)]
  for x in myset:
    array[selection(x,len(array))] |= fingerprint(x)
  test = 10000
  bad = 0
  def contain(x, array):
    h = fingerprint(x)
    c = array[selection(x,len(array))]
    return (c & h) == h
  for i in range(test):
    v = random.randint(0,2**64-1)
    if(contain(v, array)): 
        h = fingerprint(v)
        c = array[selection(v,len(array))]
        bad += 1
  return(bad*1.0/test)

best = [1,0,0,0,0]
for i in range(1000):
    a = random.randint(0,2**64-1)|1
    b = random.randint(0,2**64-1)|1
    c1 = random.randint(0,64)
    c2 = random.randint(0,64)
    score = fpp(myset,lambda x: (rotate(x,c1) & (a*rotate(x,c2)) & (b*x) &0xFFFFFFFFFFFFFFFF ))
    #print(a,b,c1, score)
    if(score < best[0]):
        best=[score,a,b,c1,c2]
        print("current best = ", best)
print(best)