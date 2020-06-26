from mpmath import mp
mp.dps = 1000
x=mp.mpf(50178230318)/mp.mpf(100000000000)
x1=mp.mpf(4519653187245114)*(mp.mpf(2)**mp.mpf(-53))
x2=mp.mpf(4519653187245115)*(mp.mpf(2)**mp.mpf(-53))

dist1 = x - x1
dist2 = x - x2
if(dist1 > dist2):
    print("x2 is closer")
elif(dist2 < dist1):
    print("x1 is closer")
else:
    print("should round to even")
print("x1:",x1)
print("x2:",x2)
print(x)
print("%18.18f\n" %float(x))

print(x * (mp.mpf(2)**mp.mpf(53)))
