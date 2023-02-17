
for l in range(1,12):
  format = 'iso-8859-'+str(l)
  print(format)
  try:
    for i in range(1,256):
      b = bytearray(1)
      b[0] = i
      predicted = 1
      if(i>=128): predicted = 2
      actual = len(b.decode(format).encode("utf-8"))
      if(predicted != actual): 
        print("BUG "+ format + " "+ str(i) + " " +str(b.decode(format).encode("utf-8")))
        break
  except  Exception as e:
    print("problem", e, format)

format = 'windows-1252'
print(format)
try:
    for i in range(1,256):
        b = bytearray(1)
        b[0] = i
        predicted = 1
        if(i>=128): predicted = 2
        actual = len(b.decode(format).encode("utf-8"))
        if(predicted != actual): 
          print("BUG "+ format + " "+ str(i) + " " +str(b.decode(format).encode("utf-8")))
        break
except  Exception as e:
    print("problem", e, format)