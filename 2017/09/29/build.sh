echo "get swiftdisas.py from https://github.com/lemire/SwiftDisas"
wget https://raw.githubusercontent.com/lemire/SwiftDisas/master/swiftdisas.py
swiftc -O prot.swift 
python swiftdisas.py prot sum17
