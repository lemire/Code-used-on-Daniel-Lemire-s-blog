import random

stringarray=["AT", "AA","GT", "GG", "AG","CT"]

counter = 0
for j in range(1,23):
  for i in range(50*1000):
    counter = counter + 1
    print("rs"+str(counter)+"\t"+str(j)+"\t"+str(random.randint(10,1000000))+"\t"+random.choice(stringarray))