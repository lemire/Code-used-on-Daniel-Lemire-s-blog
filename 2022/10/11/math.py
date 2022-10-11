def f(n,i):
  sum = 0.0
  for k in range(1,n-i+1):
    sum += 1.0/(k+1)
  return sum


def g(n):
  sum = 0.0
  for i in range(1,n):
    sum += f(n,i)
  return 2*sum

for N in range(2, 19):
  print(N,g(2**N)/2**N)
