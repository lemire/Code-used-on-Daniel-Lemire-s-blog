# if corresponding bit is 1, then value must be skipped
mystr=""
for j in range(16):
    vals=[]
    for i in range(4):
        if((j&(1<<i))==0):
            vals.append(i)
    pos = 0

    for i in range(len(vals)):
        for byt in range(4):
            mystr+=hex(vals[i] * 4 + byt)+","
            pos += 1
    while(pos < 16):
        mystr+="0xFF,"
        pos += 1
    mystr+="\n"
print(mystr)


mystr=""
for j in range(256):
    vals=[]
    for i in range(8):
        if((j&(1<<i))==0):
            vals.append(i)
    pos = 0

    for i in range(len(vals)):
        for byt in range(2):
            mystr+=hex(vals[i] * 2 + byt)+","
            pos += 1
    while(pos < 16):
        mystr+="0xFF,"
        pos += 1
    mystr+="\n"
print(mystr)
