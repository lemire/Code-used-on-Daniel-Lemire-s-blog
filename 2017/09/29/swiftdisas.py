#!/usr/bin/env python

import sys
import re
import subprocess
import io
import os

try: print(subprocess.check_output('which lldb',shell=True).decode())
except: sys.exit('Make sure that lldb is installed.')

try: print(subprocess.check_output('which swift',shell=True).decode())
except: sys.exit('Make sure that swift is installed.')

if(len(sys.argv)<3):
    sys.exit("usage: swiftdisas.py binary function")

def demangle(name) :
   return subprocess.check_output(["swift", "demangle", "-compact", name]).strip()


mybinary = sys.argv[1]
myfunction = sys.argv[2]

if(not(os.path.isfile(mybinary))):
    sys.exit("usage: swiftdisas.py binary function, I can't find "+mybinary)

print("processing: "+ mybinary)

pipe = subprocess.Popen(["lldb", "--batch", mybinary, "-o" , "image lookup -r -s "+myfunction+" "], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
res = pipe.communicate()[0].decode().split("\n")
x = 0
answer = {}
while (x < len(res)):
    l = res[x]
    address = ""
    if(re.match(".*symbols.*"+mybinary+".*",l)):
        count = int(re.search("(\d+)\ssymbols",l).group(1))
        while((x + 1 < len(res))):
            if(re.match("\s*Summary:.*Address:.*",res[x+1])):
                l = res[x + 1]
                x = x + 1
                m = re.search("\s*Summary:.*`(.*)Address:.*\[(.*)\]",l)
                fncname = m.group(1).strip()
                address = m.group(2).strip()
                answer[fncname] = address
                address = ""
            elif(re.match("\s*Summary:.*",res[x+1])):
                l = res[x + 1]
                x = x + 1
                m = re.search("\s*Summary:.*`(.*)",l)
                fncname = m.group(1).strip()
                if(len(address)>0):
                   answer[fncname] = address
                   address = ""
            elif (re.match("\s*Address:.*",res[x+1])):
                l = res[x + 1]
                m = re.search("Address:.*\[(.*)\]",l)
                address = m.group(1).strip()
                x = x + 1
            else:
                break
    x = x + 1
catalog = set()
if(len(answer) == 0): print("cannot find anything matching the requested function.")
for name in answer:
    print("function: "+demangle(name))
    if(answer[name] in catalog): 
       print("duplicate address")
    catalog.add(answer[name])
    pipe = subprocess.Popen(["lldb", "--batch", mybinary, "-o" , "disas -m -a "+answer[name]+" "], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    res = pipe.communicate()[0].decode().split("\n")
    p = False
    for line in res:
        if(p):
            print(line)
        if(re.search("disas -m -a",line)):
            print("======")
            p = True
    print("======")
    print("")
