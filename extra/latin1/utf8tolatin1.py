latin1 = 'iso-8859-1'
utf8 = 'utf-8'

def bytetostring(b):
  s = ""
  for i in b:
    s += bin(i) + ","
  return s

for i in range(1,256):
    b = bytearray(1)
    b[0] = i
    r = b.decode(latin1).encode("utf-8")
    print(bytetostring(b), bytetostring(r))

