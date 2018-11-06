for i in range(1,30):
    print("uint64_t naked_access_%d(uint64_t *bigarray, size_t howmanyhits) {"%(i))
    for j in range(1,i+1):
        print("uint64_t val%d = %d;" %(j,j))
    print ("size_t howmanyhits_perlane = howmanyhits / %d;"%(i))
    print("for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {")
    for j in range(1,i+1):
        print("val%d = bigarray[val%d];" %(j,j))
    print("}")
    ret = "val1 "
    for  j in range(2,i+1):
      ret += "+ val%d " %(j)
    print("return ", ret,";")
    print("}")
    print("""

float time_me%d(uint64_t *bigarray, size_t howmanyhits,
              size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_%d(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << %d << " " << mintime << std::endl;
  return mintime;
}
    """%(i,i,i))



for i in range(1, 30):  print("time_measure[%d] = time_me%d(bigarray, howmanyhits, repeat);"%(i,i))