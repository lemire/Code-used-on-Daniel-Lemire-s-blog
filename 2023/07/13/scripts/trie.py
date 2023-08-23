t=["A", "A6" ,"AAAA" ,"AFSDB", "APL", "CAA", "CDS", "CDNSKEY", "CERT", "CH", "CNAME", "CS", "CSYNC", "DHCID", "DLV", "DNAME", "DNSKEY", "DS", "EUI48", "EUI64", "GPOS", "HINFO", "HIP", "HS", "HTTPS", "IN", "IPSECKEY", "ISDN", "KEY" ,"KX", "L32", "L64", "LOC", "LP", "MB", "MD", "MF", "MG", "MINFO", "MR", "MX", "NAPTR", "NID", "NS", "NSAP", "NSAP-PTR", "NSEC", "NSEC3", "NSEC3PARAM", "NULL", "NXT", "OPENPGPKEY", "PTR", "PX", "RP", "RRSIG", "RT", "SIG", "SMIMEA", "SOA", "SPF", "SRV", "SSHFP", "SVCB", "TLSA", "TXT", "URI", "WKS", "X25", "ZONEMD"]


code = {}

for x in t:
    current = code
    for i in range(0, len(x)):
        if(x[i] not in current):
            current[x[i]] = {}
        current = current[x[i]]

#print(code)

def switch(mylist, level=0):
    print(" "*level,"switch(s["+ str(level)+ "]) {")
    currentchar = None
    defaultval = -1
    for x in mylist:
        #if level == len(x):
        #    print(" "*level," ", "case '"+ str(currentchar)+ "' :")
        #    print(" "*level," ", "return "+str(t.index(x))+";")
        #    continue
        if level == len(x):
            defaultval = t.index(x)
            continue
        if(currentchar != x[level]):
            currentchar = x[level]
            if(ord(currentchar) >= ord('A')) and (ord(currentchar) <= ord('Z')):
                print(" "*level,"", "case '"+ str(currentchar)+ "': case '"+ str(chr(ord(currentchar)|0x20))+ "':")
            else:
                print(" "*level,"", "case '"+ str(currentchar)+ "' :")
            mysublist = [y for y in mylist if level  < len(y) and y[level] == currentchar]
            if(len(mysublist) == 1) and (len(mysublist[0]) == level+1):
              print(" "*level," return is_separator(s["+str(level+1)+"])? "+str(t.index(mysublist[0]))+": -1;")
            else:
              switch(mysublist, level+1)
              print(" "*level," ", "break;")        
    print(" "*level, " ","default:")
    if(defaultval == -1):
        print(" "*level, " ", "return -1;")
    else:
        print(" "*level, " ", "return is_separator(s["+str(level+1)+"])? "+str(defaultval)+": -1;")
    print(" "*level,"};")
switch(t)