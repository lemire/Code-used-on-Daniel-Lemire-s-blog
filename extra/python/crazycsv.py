#! /usr/bin/env python

import csv
import sys

if(len(sys.argv) <2):
    print("specify input file name")
    sys.exit(-1)
name=sys.argv[1]
outname=name+".cvs.txt"
counter = 0
linecounter = 0
print("reading "+name)
with open(outname, 'w', newline='') as csvfile:
    csvwriter = csv.writer(csvfile, delimiter='\t',
                            quotechar='"', quoting=csv.QUOTE_MINIMAL)
    with open(name, "r") as inputfile :
        for line in inputfile :
            linecounter +=1
            line = line.strip()
            if(line.startswith("<SourceLanguage>")):
              source = line[line.index(">")+1:]
            elif(line.startswith("<TargetLanguage>")):
              target = line[line.index(">")+1:]
            elif(line.startswith("<Justification>")):
              justification = line[line.index(">")+1:]
              csvwriter.writerow([source,target,justification])
              sys.stdout.write(".")
              sys.stdout.flush()
              counter += 1
print()
print("produced "+str(counter)+" entries")
print("read "+str(linecounter)+" lines")

print("see "+outname)
