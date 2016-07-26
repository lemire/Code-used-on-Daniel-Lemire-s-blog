w=8

def clmul(a,x):
    i = 0
    answer = 0
    while ( ( a>>i ) > 0 ) :
        bitset = (((a >> i) & 1) == 1)
        if(bitset):
            answer = answer ^ (x<<i)
        i = i + 1
    return answer

def ms(a,x):
    product = a*x#clmul(a,x)
    folded = ( ( product >> (w-1)) ) %(1<<w)
    return folded

def msRegularity(a):
    counter = [0 for i in range(1<<w)]
    for x in range(1 << w):
        counter[ms(a,x)] += 1
    #print(counter)
    return max(counter)

# regular, cannot be XOR universal.
def mstabs(x,y):
  counter = [0 for i in range(1<<w)]
  for a in range(1<<(2*w-1)):
      index = ms(a,x) ^ ms(a,y)
      counter[index] +=1
  return counter

counter = 0
collection =[]
for a in range(1<<(2*w-1)):
    if(msRegularity(a) == 1):
        counter += 1
        collection.append(bin(a))#a>>(w-1))
    #print(a,bin(a),msRegularity(a))
print(collection)
print(counter,(1<<(2*w-1)),w,counter * 1.0 / (1<<(2*w-1)))

#for x in range(1<<w):
#    for y in range(1<<w):
#      if(x <> y):
#        print (x,y,mstabs(x,y))
