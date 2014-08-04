def count(x):
  t = 0
  for i in range(8):
     if((x & (1<<i)) <> 0) : t = t +1
  return t


def extract(x):
  ans = []
  while(x <> 0): 
    t = x & -x
    ans.append(count(t-1))
    x = x ^ t
  return ans

for i in range(256):
  a = extract(i)
  s = str(len(a))+","
  t = -1
  for z in a:
    t = z 
    s = s + str(z)+","
  s = s + str(7-t)
  print("{"+s+"},")
  
print()
print()

for i in range(256):
  a = extract(i)
  t = -1
  print ("case "+str(i)+":")
  for i in range(len(a)): 
    z= a[i]
    t = z 
    print ("out["+str(i)+"]=off+"+str(z)+";")
  print("out+="+str(len(a))+";")
  print("break;")
