

def zigzag_encode(val) :
    if val < 0:
        return - 2 * val  + 1
    return 2 * val

def zigzag_decode(val) :
    if val & 1 == 1 :
        return - (val // 2)
    return val // 2

for i in range(-10,11):
    print(i, zigzag_encode(i) )
    assert(zigzag_decode(zigzag_encode(i)) == i)