for i in range(10000):
    s=""
    s+=hex((i//1000)+0x30)+","
    i=i%1000
    s+=hex((i//100)+0x30)+","
    i=i%100
    s+=hex((i//10)+0x30)+","
    s+=hex((i%10)+0x30)+","
    print(s)