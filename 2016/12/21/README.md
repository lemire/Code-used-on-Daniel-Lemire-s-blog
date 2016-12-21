## Overhead of calling assembly from Go

Assumption: recent Intel CPU.

Credit: Most of the code was written by the Go team at Google,
        and cryptographer Adam Langely (https://www.imperialviolet.org/)
        in particular; it comes from the Go standard library go/src/math/big
        package. 

Usage:

```
$ go get github.com/klauspost/cpuid

$ go test -bench=.
```

Result on a Skylake-based server configured for testing:

```
$ go version
go version go1.7.1 linux/amd64
$ go test -bench=.
testing: warning: no tests to run
Benchmark100CountTrailingZerosDeBruijn-2        1000000          1045 ns/op
Benchmark100CountTrailingZerosAsm-2             1000000          2379 ns/op
PASS
ok      
```
