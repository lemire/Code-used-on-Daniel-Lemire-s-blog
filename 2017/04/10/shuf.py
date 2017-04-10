for i in range(256):
    l = []
    for j in range(8):
        if(i & ( 1<< j) == 0):
            l.append(j)
    while(len(l) < 8): l.append(0)
    for z in l:
        print(str(z)+",",end="")
    print()
