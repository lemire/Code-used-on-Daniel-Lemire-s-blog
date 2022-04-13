def difloc(s1,s2):
    for i in range(len(s1)):
        if s1[i] != s2[i]:
            return i

x = 94065645841247
difflocation = 1000
while (x > 0):
    s = "4."+str(x)+"e-324"
    y = float(s)
    ns = "%.14e"%y
    x += 1
    if s != ns :
        tl = difloc(s,ns)
        if tl < difflocation:
            difflocation = tl
            print(difflocation)
            print(s,ns)
        #break