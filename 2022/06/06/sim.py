

def count_cross(size, times):
    cross = 0
    for i in range(times):
        location = i * size
        endlocation = i * size + size - 1
        cross += (endlocation // 64) - (location // 64)
    return cross

from math import gcd

volume = 10000
for z in range(1,128 + 1):
    g = gcd(z,64)
    f = count_cross(z, volume) / volume * (64//g)
    print(z, count_cross(z, volume) / volume, (z//g - 1) / (64//g), (z-g)/64)


for z in range(1,64+1):
    g = gcd(z,64)
    print("<tr><td>"+str(z)+"</td><td>"+str(1+(z-g)/64)+"</td></tr>")