## interesting puzzle: enumerate all values having a given weight
def nextperm(x) :
    t = (v | (v - 1)) + 1;
    return t | ((((t & -t) // (v & -v)) >> 1) - 1);

def CountBits(n):
  n = (n & 0x5555555555555555) + ((n & 0xAAAAAAAAAAAAAAAA) >> 1)
  n = (n & 0x3333333333333333) + ((n & 0xCCCCCCCCCCCCCCCC) >> 2)
  n = (n & 0x0F0F0F0F0F0F0F0F) + ((n & 0xF0F0F0F0F0F0F0F0) >> 4)
  n = (n & 0x00FF00FF00FF00FF) + ((n & 0xFF00FF00FF00FF00) >> 8)
  n = (n & 0x0000FFFF0000FFFF) + ((n & 0xFFFF0000FFFF0000) >> 16)
  n = (n & 0x00000000FFFFFFFF) + ((n & 0xFFFFFFFF00000000) >> 32) # This last & isn't strictly necessary.
  return n

print("gf_set_data(2);")
for major in [127]: # we reduce to 128 bits
  offset = 128 - major
  bitwidth = (128 // 8) - 1
  for bi in range(1<<bitwidth):
    if(CountBits(bi) > 3): continue # arbitrary
    i = (bi << 1)
    polynomial = "1 + "
    polynomial += "x^"+str(major)
    for bit in range(bitwidth + 1):
        if (i &(1<<bit)) != 0:
            if(len(polynomial)!=0): polynomial += " + "
            polynomial += "x^"+str(8*bit - offset)
    print("gf_factor("+polynomial+");")
