```
always [madvise] never
mode:  always
[always] madvise never
page count: 65536, volume:256 MB
calloc             : 19.1396 GB/s
new char[s] + touch: 19.3163 GB/s
new(std::nothrow) char[s](): 2.41067 GB/s
new char[s]()      : 2.41199 GB/s
memset: 30.5603 GB/s
memcpy: 12.5042 GB/s

calloc             : 19.3316 GB/s
new char[s] + touch: 19.1213 GB/s
new(std::nothrow) char[s](): 2.41152 GB/s
new char[s]()      : 2.41095 GB/s
memset: 30.6245 GB/s
memcpy: 12.4952 GB/s

calloc             : 19.2318 GB/s
new char[s] + touch: 19.4171 GB/s
new(std::nothrow) char[s](): 2.41123 GB/s
new char[s]()      : 2.41167 GB/s
memset: 30.4976 GB/s
memcpy: 12.4728 GB/s




page count: 131072, volume:512 MB
calloc             : 19.5595 GB/s
new char[s] + touch: 19.5814 GB/s
new(std::nothrow) char[s](): 2.4164 GB/s
new char[s]()      : 2.41635 GB/s
memset: 30.7489 GB/s
memcpy: 12.5506 GB/s

calloc             : 19.5392 GB/s
new char[s] + touch: 19.6057 GB/s
new(std::nothrow) char[s](): 2.41638 GB/s
new char[s]()      : 2.41632 GB/s
memset: 30.761 GB/s
memcpy: 12.5452 GB/s

calloc             : 19.5865 GB/s
new char[s] + touch: 19.6504 GB/s
new(std::nothrow) char[s](): 2.41628 GB/s
new char[s]()      : 2.41669 GB/s
memset: 30.7553 GB/s
memcpy: 12.4829 GB/s




mode:  never
always madvise [never]
page count: 65536, volume:256 MB
calloc             : 3.68711 GB/s
new char[s] + touch: 3.7149 GB/s
new(std::nothrow) char[s](): 1.60164 GB/s
new char[s]()      : 1.59693 GB/s
memset: 30.1166 GB/s
memcpy: 12.3684 GB/s

calloc             : 3.69288 GB/s
new char[s] + touch: 3.71848 GB/s
new(std::nothrow) char[s](): 1.60126 GB/s
new char[s]()      : 1.59673 GB/s
memset: 30.1816 GB/s
memcpy: 12.3666 GB/s

calloc             : 3.68796 GB/s
new char[s] + touch: 3.71717 GB/s
new(std::nothrow) char[s](): 1.60232 GB/s
new char[s]()      : 1.59875 GB/s
memset: 30.1342 GB/s
memcpy: 12.3544 GB/s




page count: 131072, volume:512 MB
calloc             : 3.76729 GB/s
new char[s] + touch: 3.78696 GB/s
new(std::nothrow) char[s](): 1.60371 GB/s
new char[s]()      : 1.59893 GB/s
memset: 30.128 GB/s
memcpy: 12.4643 GB/s

calloc             : 3.76341 GB/s
new char[s] + touch: 3.78647 GB/s
new(std::nothrow) char[s](): 1.6028 GB/s
new char[s]()      : 1.599 GB/s
memset: 30.2341 GB/s
memcpy: 12.4607 GB/s

calloc             : 3.7655 GB/s
new char[s] + touch: 3.78688 GB/s
new(std::nothrow) char[s](): 1.60277 GB/s
new char[s]()      : 1.59824 GB/s
memset: 30.2677 GB/s
memcpy: 12.4747 GB/s




Done.
Restauring hugepages to madvise
```
