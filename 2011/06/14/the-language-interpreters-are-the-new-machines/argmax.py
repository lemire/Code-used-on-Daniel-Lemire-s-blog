# The language interpreters are the new machines
# http://lemire.me/blog/archives/2011/06/14/the-language-interpreters-are-the-new-machines/
#

import time
import random

def maxarg(arr):
    counter = 0
    arg = 0
    m = arr[0]
    for x in arr:
        if x > m:
            m = x
            arg = counter
        counter += 1
    return arg

if __name__=="__main__":
  arrays=[[i for i in range(1000000)] for j in range(10)]
  for x in arrays: random.shuffle(x)
  before = time.time()
  for array in arrays: print ("    ignore this :" , array.index(max(array)))
  after = time.time()
  print( after-before)
  before = time.time()
  for array in arrays: print ("    ignore this :" , maxarg(array))
  after = time.time()
  print( after-before)
  before = time.time()
  for array in arrays: print ("    ignore this :" , max(zip(array, range(len(array))))[1])
  after = time.time()
  print( after-before)
  before = time.time()
  for array in arrays: print("    ignore this :" ,  max([array[i],i] for i in range(len(array)))[1])
  after = time.time()
  print (after-before)
  before = time.time()
  for array in arrays: print("    ignore this :" ,  max((array[i],i) for i in range(len(array)))[1])
  after = time.time()
  print (after-before)
  before = time.time()
  for array in arrays:  print("    ignore this :" , max(range(len(array)), key=array.__getitem__))
  after = time.time()
  print (after-before)