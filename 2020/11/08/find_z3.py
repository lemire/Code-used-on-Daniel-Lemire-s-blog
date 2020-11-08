from z3 import *
# pip3 install z3
import random
import sys
if sys.version_info < (3, 0):
    sys.stdout.write("Sorry, requires Python 3.x or better\n")
    sys.exit(1)
counter = 0

total = 1000
for i in range(total):
  s = Solver()
  a = BitVec('a', 64)
  s.add(a*i  == 1)
  if((i % 100)==0) :
      print(".",end="",flush=True)
  if(s.check() == z3.sat):
      counter = counter + 1

print(counter*100./total)