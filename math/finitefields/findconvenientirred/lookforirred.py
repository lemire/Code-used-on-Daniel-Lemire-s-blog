import re
oldline =""
for line in open("allfactors.txt"):
    if re.fullmatch("\(%o\d+\)[^\(]*",line) :
        print("FOUND:")
        print(oldline,end="")
        print(line)
    oldline = line
