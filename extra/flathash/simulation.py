def mix32(h) :
    """ murmur """
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;

def hash(x):
    return mix32(x.__hash__())

class BaseHash:
    def __init__(self):
      self.__array__=[None for i in xrange(1)]
      self.__count__ = 0
      self.__load__ = 0.5
    def addRobinHood(self,x):
      #print("adding ",hex(hash(x)&0xFFFF))
      pos = hash(x) % len(self.__array__)
      for i in range(len(self.__array__)):
          p = (pos + i)%len(self.__array__)
          myage = ((p - (hash(x) % len(self.__array__))) %len(self.__array__))
          if(self.__array__[p] == None):
              self.__array__[p] = x
              self.__count__ += 1
              assert self.count() == self.__count__
              if(self.__load__ * len(self.__array__) < self.__count__): self.growrh()
              return
          tp = ((p - (hash(self.__array__[p]) % len(self.__array__))) %len(self.__array__))
          if(tp < myage):
              newx = self.__array__[p]
              self.__array__[p] = x
              x = newx
          #tp = self.probedist(self.__array__[p])
          #if(tp < i):
          #  newx = self.__array__[p]
          #  #print("conflict won with ",hex(hash(newx)&0xFFFF))
          # self.__array__[p] = x
          #self.addRobinHood(newx)
          #return
      self.growrh()
      self.add(x)
    def add(self,x):
      pos = hash(x) % len(self.__array__)
      for i in range(len(self.__array__)):
          p = (pos + i)%len(self.__array__)
          if(self.__array__[p] == None):
              self.__array__[p] = x
              self.__count__ += 1
              assert self.count() == self.__count__
              if(self.__load__ * len(self.__array__) < self.__count__): self.grow()
              return
      self.grow()
      self.add(x)
    def probedist(self,x):
      pos = hash(x) % len(self.__array__)
      for i in range(len(self.__array__)):
          p = (pos + i)%len(self.__array__)
          assert(self.__array__[p] <> None)
          if(self.__array__[p] == x): return i
      assert False
    def grow(self):
      old = self.__array__
      self.__count__ = 0
      self.__array__ = [None for i in xrange(2 * len(old))]
      for x in old:
          if(x <> None):
              self.add(x)
    def growrh(self):
      old = self.__array__
      self.__count__ = 0
      self.__array__ = [None for i in xrange(2 * len(old))]
      #print("GROWING")
      for x in old:
          if(x <> None):
              self.addRobinHood(x)
    def capacity(self):
        return len(self.__array__)

    def count(self):
        c = 0
        for x in self.__array__:
          if(x <> None):
              c+=1
        return c
    def maxprobe(self):
        mp = 0
        for x in self.__array__:
          if(x <> None):
              tp = self.probedist(x)
              if(tp > mp): mp = tp
        return mp
    def describe(self):
        s = ""
        for x in self.__array__:
          if ( x == None):
              s+="[None]"
          else:
              s+="["+hex(hash(x)&0xFFFF)+";"+str(hash(x)%self.capacity())+":"+str(self.probedist(x))+"]"
        return s
    def keys(self):
        ans = []
        for x in self.__array__:
          if(x <> None):
              ans.append(x)
        return ans
    def redundancy(self):
        assert self.count() == self.__count__
        return len(self.__array__)*1.0/self.count()

class FlatHash:
    def __init__(self):
      self.__array__=[None for i in xrange(4)]
      self.max = 4
    def addRobinHood(self,x):
      pos = hash(x) % len(self.__array__)
      p = pos
      while True:
          #for i in range(self.max):
          myage = ((p - (hash(x) % len(self.__array__))) %len(self.__array__))
          if(myage>self.max):
              break
          if(self.__array__[p] == None):
              self.__array__[p] = x
              return
          #tp = self.probedist(self.__array__[p])
          tp = ((p - (hash(self.__array__[p]) % len(self.__array__))) %len(self.__array__))
          if(tp < myage):
              newx = self.__array__[p]
              self.__array__[p] = x
              x = newx
          p += 1
          if(p == len(self.__array__)): p = 0
          if(p == pos): break
      self.growrh()
      self.add(x)
    def add(self,x):
      pos = hash(x) % len(self.__array__)
      for i in range(self.max):
          p = (pos + i)%len(self.__array__)
          if(self.__array__[p] == None):
              self.__array__[p] = x
              return
      self.grow()
      self.add(x)
    def probedist(self,x):
      pos = hash(x) % len(self.__array__)
      for i in range(len(self.__array__)):
          p = (pos + i)%len(self.__array__)
          assert(self.__array__[p] <> None)
          if(self.__array__[p] == x): return i
      assert False
    def __trymoving(self,x):
        pos = hash(x) % len(self.__array__)
        for i in range(self.max):
            p = (pos + i)%len(self.__array__)
            if(self.__array__[p] == None):
                self.__array__[p] = x
                return True
        return False
    def smartadd(self,x):
      pos = hash(x) % len(self.__array__)
      for i in range(self.max):
          p = (pos + i)%len(self.__array__)
          if(self.__array__[p] == None):
              self.__array__[p] = x
              return
      for i in range(self.max):
          p = (pos + i)%len(self.__array__)
          if(self.__trymoving(self.__array__[p])):
              self.__array__[p] = x
              return
      self.grow()
      self.add(x)
    def grow(self):
      old = self.__array__
      self.__array__ = [None for i in xrange(2 * len(old))]
      for x in old:
          if(x <> None):
              self.add(x)
    def growrh(self):
      old = self.__array__
      self.__array__ = [None for i in xrange(2 * len(old))]
      for x in old:
          if(x <> None):
              self.addRobinHood(x)
    def count(self):
        c = 0
        for x in self.__array__:
          if(x <> None):
              c+=1
        return c
    def keys(self):
        ans = []
        for x in self.__array__:
          if(x <> None):
              ans.append(x)
        return ans

    def redundancy(self):
        return len(self.__array__)*1.0/self.count()

import random
def check():
  z = 4
  base = 1000000
  while ( z <= 1024 * 1024):
    #data = [str(random.randint(1+base,256+base)) for x in range(z)]
    base = int(random.random()*10000000)
    data = xrange(base,base+z)
    h = BaseHash ()
    hrh = BaseHash ()
    for d in xrange(base,base+z):
        h.add(d)
        hrh.addRobinHood(d)
    assert h.count() == len(data)
    assert hrh.count() == len(data)
    mp, mprh = h.maxprobe(), hrh.maxprobe()
    #print(z,mp, mprh,h.redundancy(), hrh.redundancy())
    if(mprh> mp):
        print("RH has worse max probe!")
        print("rh : "+hrh.describe())
        print("h : "+h.describe())
        hrh = BaseHash ()
        for d in xrange(base,base+z):
          hrh.addRobinHood(d)
          print("rh : "+hrh.describe())
        break
    K = h.keys()
    m = mp
    if(mprh < m):
        m = mprh
    le = h.capacity()
    le = le / 2
    t=[(hash(x) % le, x) for x in K]
    t.sort()
    d = [None for x in xrange(le)]
    compacted = True
    for i,j in t:
        p = False
        for k in range(m):
            if(d[(i+k)%le] == None):
                d[(i+k)%le] = j
                p = True
                break
        if (not(p)):
            compacted = False
            break
    if(compacted):
        print("could compact further!")
        print("rh : "+hrh.describe())
        print("h : "+h.describe())
        break
    #z *= 2

#check()
def demo():
    print("max probing distance vs. storage redundancy")
    z = 1024 * 32
    while ( z <= 1024 * 1024):
      print("experimenting with hash tables of size ", z)
      for m in [4,8,16,32,64]:
        base = 1000000
        h = FlatHash()
        h.max = m
        hsmart = FlatHash()
        hsmart.max = m
        hrh = FlatHash()
        hrh.max = m

        for x in xrange(base,base+z):
          h.add(str(x))
          hsmart.smartadd(str(x))
          hrh.addRobinHood(str(x))
        assert h.count() == z
        assert hrh.count() == z
        assert hsmart.count() == z

        ## follows strange stuff
        K = hsmart.keys()
        le = len(hsmart.__array__)
        le = le / 2
        t=[(hash(x) % le, x) for x in K]
        t.sort()
        d = [None for x in xrange(le)]
        compacted = True
        for i,j in t:
            p = False
            for k in range(m):
                if(d[(i+k)%le] == None):
                    d[(i+k)%le] = j
                    p = True
                    break
            if (not(p)):
                compacted = False
                break
        if(compacted): print("could compact further!")
        print(" dump max probing/redundancy:",h.max,h.redundancy(),"  smart max probing/redundancy:",hsmart.max,hsmart.redundancy(),"  RH max probing/redundancy:",hrh.max,hrh.redundancy())

      z *= 2
      print()

demo()

#######
## Opti would not be hard. sort by hash value and insert. Except for wrap-around, it is going to be
## best.  Problem is that to sort on the hash value, you need to know the capacity.
######
