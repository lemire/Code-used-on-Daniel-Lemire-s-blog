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

class FlatHash:
    def __init__(self):
      self.__array__=[None for i in xrange(4)]
      self.max = 4
    def add(self,x):
      pos = hash(x) % len(self.__array__)
      for i in range(self.max):
          p = (pos + i)%len(self.__array__)
          if(self.__array__[p] == None):
              self.__array__[p] = x
              return
      self.grow()
      self.add(x)
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
    def count(self):
        c = 0
        for x in self.__array__:
          if(x <> None):
              c+=1
        return c
    def redundancy(self):
        return len(self.__array__)*1.0/self.count()

print("max probing distance vs. storage redundancy")
z = 1024 * 32
while ( z <= 1024 * 1024):
  print("experimenting with hash tables of size ", z)
  for m in [4,8,16,32]:
    base = 1000000
    h = FlatHash()
    hsmart = FlatHash()
    h.max = m
    hsmart.max = m
    for x in xrange(base,base+z):
      h.add(str(x))
      hsmart.smartadd(str(x))
    print(" dump max probing/redundancy:",h.max,h.redundancy(),"  smart max probing/redundancy:",hsmart.max,hsmart.redundancy(),)

  z *= 2
  print()
