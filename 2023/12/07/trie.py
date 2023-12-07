t=["accept","accept-encoding","accept-language","accept-ranges","access-control-allow-credentials","access-control-allow-headers","access-control-allow-methods","access-control-allow-origin","access-control-expose-headers","access-control-max-age","access-control-request-headers","access-control-request-method","age","allow","alt-svc","alt-used","authorization","cache-control","clear-site-data","connection","content-disposition","content-encoding","content-language","content-length","content-location","content-range","content-security-policy","content-security-policy-report-only","content-type","cookie","cross-origin-embedder-policy","cross-origin-opener-policy","cross-origin-resource-policy","date","device-memory","downlink","ect","etag","expect","expect-ct","expires","forwarded","from","host","if-match","if-modified-since","if-none-match","if-range","if-unmodified-since","keep-alive","last-modified","link","location","max-forwards","origin","permissions-policy","pragma","proxy-authenticate","proxy-authorization","rtt","range","referer","referrer-policy","refresh","retry-after","sec-websocket-accept","sec-websocket-extensions","sec-websocket-key","sec-websocket-protocol","sec-websocket-version","server","server-timing","service-worker-allowed","service-worker-navigation-preload","set-cookie","sourcemap","strict-transport-security","supports-loading-mode","te","timing-allow-origin","trailer","transfer-encoding","upgrade","upgrade-insecure-requests","user-agent","vary","via","www-authenticate","x-content-type-options","x-dns-prefetch-control","x-frame-options","x-permitted-cross-domain-policies","x-powered-by","x-requested-with","x-xss-protection"]


code = {}

for x in t:
    current = code
    for i in range(0, len(x)):
        if(x[i] not in current):
            current[x[i]] = {}
        current = current[x[i]]


def switch(mylist, level=0):
    print(" "*level,"switch(s["+ str(level)+ "]) {")
    currentchar = None
    defaultval = -1
    for x in mylist:
        if level == len(x):
            defaultval = t.index(x)
            continue
        if(currentchar != x[level]):
            currentchar = x[level]
            if(ord(currentchar) >= ord('a')) and (ord(currentchar) <= ord('z')):
                print(" "*level,"", "case "+ str(ord(currentchar))+ ": case "+ str(ord(chr(ord(currentchar)^0x20)))+ ":")
            else:
                print(" "*level,"", "case "+ str(ord(currentchar))+ ":")
            mysublist = [y for y in mylist if level  < len(y) and y[level] == currentchar]
            if(len(mysublist) == 1) and (len(mysublist[0]) == level+1):  
              print(" "*level," return len == "+str(level+1)+"?"+str(t.index(mysublist[0]))+":-1;")
            else:
              print(" "*level,"if(len == "+str(level)+") { return -1; } ")
              switch(mysublist, level+1)
              print(" "*level," ", "break;")        
    print(" "*level, " ","default:")
    if(defaultval == -1):
        print(" "*level, " ", "return -1;")
    else:
        print(" "*level, " ", "return len == "+str(level)+"?"+str(defaultval)+": -1;")
    print(" "*level,"};")
print("function classify(s) {")
print("  var len = s.length;")
print("if(len == 0) { return -1; } ")

switch(t)
print("} ")
print("console.log(classify(Buffer.from('Content-Type')));")
