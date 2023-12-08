file = open("file.txt", "r")

t=[]
# loop through each line in the file
for line in file:
    # strip the newline character from the line
    line = line.strip().lower()
    t.append(line)
    
# close the file
file.close()

print("function toLowerCase(s) {")
print("  var len = s.length;")
print("switch(len) {")
for i in range(40):
    filter = [x for x in t if len(x) == i]
    if(len(filter) == 0):
        continue
    print("case "+str(i)+": ")
    if(len(filter) == 1):
        j = filter[0]
        condition = ""
        for i in range(0, i):
            if(len(condition) > 0):
                condition += " && "
            if(j[i] >= 'a') and (j[i] <= 'z'):
                condition += "(s["+str(i)+"] == "+str(ord(j[i]))+" || s["+str(i)+"] == "+str(ord(chr(ord(j[i])^0x20)))+")"
            else:
                condition += "(s["+str(i)+"] == "+str(ord(j[i]))+")"
        print("// check that the buffer is equal to "+j+" and return it if so")
        print("if("+condition+") { return \""+j+"\"; }")
        print("break;")
        continue
    x = [z[0] for z in filter]
    if(len(x) == len(set(x))):
        print("switch(s[0]) {")
        for j in filter:
            print("case "+str(ord(j[0]))+":", end="")
            print("case "+str(ord(chr(ord(j[0])^0x20)))+":")
            condition = ""
            for i in range(1, len(j)):
                if(len(condition) > 0):
                    condition += " && "
                condition += "(s["+str(i)+"] == "+str(ord(j[i]))+" || s["+str(i)+"] == "+str(ord(chr(ord(j[i])^0x20)))+")"
            print("// check that the buffer is equal to "+j+" and return it if so")
            print("if("+condition+") { return \""+j+"\"; }")
            print("break;")
        print(" default: break;")
        print("}")
        print("break;")
        continue
    done = False
    for L in range(1, i):
      x = [z[L] for z in filter]
      if(len(x) == len(set(x))):
        done = True
        print("switch(s["+str(L)+"]) {")
        for j in filter:
            print("case "+str(ord(j[L]))+":", end="")
            print("case "+str(ord(chr(ord(j[L])^0x20)))+":")
            condition = ""
            for i in range(0, len(j)):
                if(i == L):
                    continue
                if(len(condition) > 0):
                    condition += " && "
                if(j[i] >= 'a') and (j[i] <= 'z'):
                    condition += "(s["+str(i)+"] == "+str(ord(j[i]))+" || s["+str(i)+"] == "+str(ord(chr(ord(j[i])^0x20)))+")"
                else:
                    condition += "(s["+str(i)+"] == "+str(ord(j[i]))+")"
            print("// check that the buffer is equal to "+j+" and return it if so")
            print("if("+condition+") { return \""+j+"\"; }")
            print("break;")
        print(" default: break;")
        print("}")
        print("break;")
        break
    if(not done):
        x = [z[0]+z[-1] for z in filter]
        print(filter)
        if(len(x) == len(set(x))):
            print("switch(s[0]+s["+str(i-1)+"]) {")
            for j in filter:
                print("case "+str(ord(j[0])+ord(j[-1]))+":", end="")
                print("case "+str(ord(j[0])+ord(j[-1])^0x20)+":", end="")
                print("case "+str(ord(j[0])^0x20+ord(j[-1]))+":", end="")
                print("case "+str(ord(j[0])^0x20+ord(j[-1])^0x20)+":")
                condition = ""
                for i in range(0, len(j)):
                    if(len(condition) > 0):
                        condition += " && "
                    if(j[i] >= 'a') and (j[i] <= 'z'):
                        condition += "(s["+str(i)+"] == "+str(ord(j[i]))+" || s["+str(i)+"] == "+str(ord(chr(ord(j[i])^0x20)))+")"
                    else:
                        condition += "(s["+str(i)+"] == "+str(ord(j[i]))+")"
                print("// check that the buffer is equal to "+j+" and return it if so")
                print("if("+condition+") { return \""+j+"\"; }")
            print(" default: break;")
            print("}")
            print("break;")
print("default: break;")
print("}")
print("return s.toString().toLowerCase();")

print("}")
print("console.log(toLowerCase(Buffer.from('Content-Encoding')));") 
