
import uuid
import os
import time

from pyxorfilter import Xor8, Xor16, Fuse8, Fuse16

print("generating data")
data = [uuid.uuid4() for i in range(1000000)]

N = 1000000
testdata = [uuid.uuid4() for i in range(N)]

print("Number of keys", len(data))

for filtertype in [Xor8, Xor16, Fuse8, Fuse16]:
    print("using filter type", filtertype.__name__)
    filter = filtertype(len(data))
    filter.populate(data)
    for d in data:
        assert filter.contains(d)
    filename = 'myfilter.bin'
    f = open(filename, 'wb')
    f.write(filter.serialize())
    f.close()
    memper = os.path.getsize(filename)*8.0/len(data)
    # reload the filter *from* disk
    filter = filtertype.deserialize(open(filename, 'rb').read())

    for d in data:
        assert filter.contains(d)

    # estimate false positive rate
    
    count = 0
    start = time.time()
    for x in testdata:
        count += filter.contains(x)
    end = time.time()
    querytime = end - start
    fpp = count/N*100.0
    print("false positive rate", '{:.5f}'.format(fpp), "%", ", memory per key", '{:.1f}'.format(memper), "bits", ", millions of queries per second: ", '{:.2f}'.format(N/querytime/1000000) , ", total memory", '{:.2f}'.format(os.path.getsize(filename)/1024.0/1024.0), "MiB" )