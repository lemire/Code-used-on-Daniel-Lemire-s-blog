def printcol(array, index):
  msg = ""
  for i in range(len(array)):
    if(i == index): 
      msg += "▓"
    elif(array[i] == None): 
      msg += " "
    else:
      msg += "X"
  return msg
     


def randominsert(data, array, load) :
  collisions = 0
  itemsinarray = 0
  for val in data:
      insertpoint = val % len(array)
      while (True):
        if(array[insertpoint] == None):
          array[insertpoint] = val
          itemsinarray = itemsinarray + 1 
          #print(printcol(array,None))
          break
        collisions += 1
        print(printcol(array,insertpoint))
        insertpoint = insertpoint + 1
        insertpoint = insertpoint % len(array)
      if (itemsinarray >  load * len (array)):
        # we resize
        tmpdata = list(filter(lambda x : x != None, array))
        array = [None for i in range(2*len(array))]
        itemsinarray = 0
        for oldval in tmpdata:
          insertpoint = oldval % len(array)
          while (True):
            if(array[insertpoint] == None):
              array[insertpoint] = oldval
              #print(printcol(array,None)+" (resizing)")
              itemsinarray = itemsinarray + 1 
              break
            collisions += 1
            print(printcol(array,insertpoint)+" (collision while resizing)")
            insertpoint = insertpoint + 1
            insertpoint = insertpoint % len(array)
  return array,collisions

import random
import math

def demo(N, load = 0.7) :
  data = [random.randint(0,1<<32) for i in range(N)]
  array = [None for i in range(4)]
  array, col = randominsert(data, array, load)
  #print("collisions = ", col)
  data = list(filter(lambda x : x != None, array))
  if(len(data) != N) : print("bug")
  array = [None for i in range(4)]
  array, ocol = randominsert(data, array, load)
  #print("ordered collisions = ", col)
  return col, ocol
  
if __name__=="__main__":
  times = 20
  for i in [64] : #, 128, 256, 512, 1024, 2048, 4096]:
    col = 0
    ocol = 0
    for t in range(times):
      tcol, tocol = demo(i)
      col += tcol
      ocol += tocol
    print(i,col,ocol)
