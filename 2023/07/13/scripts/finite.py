t=["A", "A6" ,"AAAA" ,"AFSDB", "APL", "CAA", "CDS", "CDNSKEY", "CERT", "CH", "CNAME", "CS", "CSYNC", "DHCID", "DLV", "DNAME", "DNSKEY", "DS", "EUI48", "EUI64", "GPOS", "HINFO", "HIP", "HS", "HTTPS", "IN", "IPSECKEY", "ISDN", "KEY" ,"KX", "L32", "L64", "LOC", "LP", "MB", "MD", "MF", "MG", "MINFO", "MR", "MX", "NAPTR", "NID", "NS", "NSAP", "NSAP-PTR", "NSEC", "NSEC3", "NSEC3PARAM", "NULL", "NXT", "OPENPGPKEY", "PTR", "PX", "RP", "RRSIG", "RT", "SIG", "SMIMEA", "SOA", "SPF", "SRV", "SSHFP", "SVCB", "TLSA", "TXT", "URI", "WKS", "X25", "ZONEMD"]
t.sort()

#states = len(t) + 2 # 0 is initial state, 1...71 are valid states and 72 is the error state
#int tok = char2token(*str);
#s = statetable[s][tok];
#states = len(t) + 1
#matrix =
allvalid = set() 
for s in t:
    for c in s:
        allvalid.add(c)
    #for c in s.lower():
    #    allvalid.add(c)
allvalidlist=list(allvalid)
allvalidlist.sort()
chars = len(allvalidlist) + 1
#print(allvalidlist)

def char_index(c):
    return allvalidlist.index(c) + 1

char2token = [0 for i in range(256)]

for s in t:
    sl = s.lower()
    for i in range(len(s)):
        char2token[ord(s[i])] = char_index(s[i])
        char2token[ord(sl[i])] = char_index(s[i])



states = [0]
for s in t:
    states.append(len(states))
matrix = {}
success =[]
init_state = len(states)
states.append(init_state)
counter = 1
for s in t:
    mystate = init_state
    for c in s:
        t = (mystate,char_index(c))
        if(not t in matrix):
            mystate = len(states)
            states.append(mystate)
            matrix[t] = mystate
        else:
            mystate = matrix[t]
    t = (mystate,0)
    matrix[t] = -counter
    success.append((counter,s))
    counter +=1
print(matrix)
print("states = ", len(states))
mat = [0 for i in range(len(states)*chars)]
print("---------")
for (st,c) in matrix.keys():
    mat[chars*st + c] = matrix[(st,c)]
print("---")
print(mat)
print("---")
print(char2token)
print(len(states))
print(success)
print(len(mat))
print("init_state", init_state)
print("chars", chars)

#const int num_states = ...;
#int char2token[256] = { ... };
#int statetable[num_states][num_tokens] = {...};

#bool match(const char *str, uint)
#{
#int s = 1;
#while (*str && s >= 0) {
#int tok = char2token(*str);
#s = statetable[s][tok];
#}
#return s != 0;
#}