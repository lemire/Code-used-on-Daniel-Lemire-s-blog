import random, time

def withset(N,NMax):
  L = set([])
  while(len(L)<N):
    L.add(random.randint(0,NMax-1))
  L = list(L)
  L.sort()
  return L


def withreservoir(N,NMax):
  L = [i for i in range(N)]
  for k in range(N,NMax):
    v = random.randint(0,k)
    if(v < N): L[v] = k
  L.sort()
  return L

def withshuf(N,NMax):
  L = [i for i in range(NMax)]
  random.shuffle(L)
  ans = L[:N]
  ans.sort()
  return ans

def withfloat(N,NMax):
  L=[random.random() for x in range(NMax)]
  return sorted(range(len(L)), key=L.__getitem__)[:N]
  


if __name__ == "__main__":
  NMax = 1024*16
  N = 16
  print("reporting speed")
  while(N<NMax):
    s = time.time()
    test1 = withset(N,NMax) 
    e = time.time()
    t1 = e-s
    s = time.time()
    test2 = withreservoir(N,NMax)
    e = time.time()
    t2 = e-s
    s = time.time()
    test3 = withshuf(N,NMax)
    e = time.time()
    t3 = e-s
    s = time.time()
    test4 = withfloat(N,NMax)
    e = time.time()
    t4 = e-s
    print(str(int(NMax/N))+"\t"+str(N/t1)+"\t"+str(N/t2)+"\t"+str(N/t3)+"\t"+str(N/t4))
    N = N * 2