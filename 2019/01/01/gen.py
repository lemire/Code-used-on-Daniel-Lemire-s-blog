#!/usr/bin/env python3

print("#include \"common.hpp\"\n#include \"math.h\"\n")

for i in range(1,100):
    print("uint64_t naked_access_%d(uint64_t *bigarray, size_t howmanyhits) {"%(i))
    for j in range(1,i+1):
        print("  uint64_t val%d = %d;" %(j,j))
    print ("  size_t howmanyhits_perlane = howmanyhits / %d;"%(i))
    print("  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {")
    for j in range(1,i+1):
        print("    val%d = bigarray[val%d];" %(j,j))
    print("  }")
    ret = "  val1 "
    for  j in range(2,i+1):
      ret += "+ val%d " %(j)
    print("  return ", ret,";")
    print("}")


print("void naked_measure_body(float (&time_measure)[NAKED_MAX], uint64_t *bigarray, size_t howmanyhits, size_t repeat) {")
for i in range(1, 100):
    first = "0.0" if i == 1 else "time_measure[1]"
    last  = "0.0" if i == 1 else "time_measure[%d]"%(i-1)
    print("  time_measure[%d] = time_one(bigarray, howmanyhits, repeat, naked_access_%d, %d, %s, %s);"%(i,i,i,first,last))
print("}")
