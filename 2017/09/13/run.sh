swiftc bare.swift -O -Xcc -march=native
lldb --batch  bare -o "disas -n multiplyHighPointer"
lldb --batch  bare -o "disas -n popcntArray"
lldb --batch  bare -o "disas -n multiplyHigh"
lldb --batch  bare -o "disas -n popCnt"


