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
        for z in range(0, i):
            if(len(condition) > 0):
                condition += " && "
            if(j[z] >= 'a') and (j[z] <= 'z'):
                condition += "((s["+str(z)+"] & 0xDF) == "+str(ord(j[z].swapcase()))+")"
            else:
                condition += "(s["+str(z)+"] == "+str(ord(j[z]))+")"
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
            for z in range(1, len(j)):
                if(len(condition) > 0):
                    condition += " && "
                if(j[z] >= 'a') and (j[z] <= 'z'):
                    condition += "((s["+str(z)+"] & 0xDF) == "+str(ord(j[z].swapcase()))+")"
                else:
                    condition += "(s["+str(z)+"] == "+str(ord(j[z]))+")"
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
            if(j[L] >= 'a') and (j[L] <= 'z'):
                print("case "+str(ord(j[L]))+":", end="")
                print("case "+str(ord(chr(ord(j[L])^0x20)))+":")
            else:
                print("case "+str(ord(j[L]))+":")
            condition = ""
            for z in range(0, len(j)):
                if(z == L):
                    continue
                if(len(condition) > 0):
                    condition += " && "
                if(j[z] >= 'a') and (j[z] <= 'z'):
                    condition += "((s["+str(z)+"]  & 0xDF) == "+str(ord(j[z].swapcase()))+")"
                else:
                    condition += "(s["+str(z)+"] == "+str(ord(j[z]))+")"
            print("// check that the buffer is equal to "+j+" and return it if so")
            print("if("+condition+") { return \""+j+"\"; }")
            print("break;")
        print(" default: break;")
        print("}")
        print("break;")
        break
    if(not done):
        for S in range(0, i):
          for L in range(i-1, S, -1):
            x = [ord(z[S])^ord(z[L]) for z in filter]
            if(not done and len(x) == len(set(x))):
                print("switch(s["+str(S)+"]^s["+str(L)+"]) {")
                for j in filter:
                    if(j[S] >= 'a') and (j[S] <= 'z') or ( j[L] >= 'a') and (j[L] <= 'z'):
                      print("case "+str(ord(j[S])^ord(j[L]))+":", end="")
                      print("case "+str(ord(j[S])^ord(j[L])^0x20)+":")
                    else:
                      print("case "+str(ord(j[S])^ord(j[L]))+":")
                    condition = ""
                    for z in range(0, len(j)):
                        if(len(condition) > 0):
                            condition += " && "
                        if(j[z] >= 'a') and (j[z] <= 'z'):
                            condition += "((s["+str(z)+"] & 0xDF) == "+str(ord(j[z].swapcase()))+")"
                        else:
                            condition += "(s["+str(z)+"] == "+str(ord(j[z]))+")"
                    print("// check that the buffer is equal to "+j+" and return it if so")
                    print("if("+condition+") { return \""+j+"\"; }")
                print(" default: break;")
                print("}")
                print("break;")
                done = True
        if(not done):
            print("ERROR:"+str(filter))
            exit(-1)
print("default: break;")
print("}")
print("return s.toString().toLowerCase();")

print("}")
print("console.log(toLowerCase(Buffer.from('Content-Encoding')));") 
