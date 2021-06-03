
from math import floor, log, ceil
def log2(x):
    return len(bin(x))-2
table = []
def mylog10(x):
    return int(ceil(log(x)/log(10)))
table.append(0)
for j in range(1,33):
    smallest = 2**(j-1)
    largest = 2**(j) - 1
    if(j < 31):
      print("(1 << 32) -  "+str(10**mylog10(smallest)) + " + ("+str(mylog10(smallest))+ " << 32), // ceil(log10("+str(smallest)+")) = "+str(mylog10(smallest)))
      table.append((2**32 - 10**mylog10(smallest)) + (mylog10(smallest) <<32))
    else:
      print(" ("+str(mylog10(smallest))+ " << 32), // ceil(log10("+str(smallest)+")) = "+str(mylog10(smallest)))
      table.append((mylog10(smallest) <<32))



print(table)
print(len(table))
def fast_digit_count(x):
    return (x+table[log2(x)])//(2**32)#return (x+table[log2(x)]) >> 32
#for x in range(1100):
#    print(x, fast_digit_count(x))

#print (mylog10(100))
print(fast_digit_count(1073741824))
print(mylog10(1073741824))
print(log2(1073741824))