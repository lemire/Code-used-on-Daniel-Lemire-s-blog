## Overhead of calling assembly from Go

Assumption: recent Intel CPU.


Usage:

```
$ go get github.com/klauspost/cpuid

$ go test -bench=.
``

Result on a Skylake-based server configured for testing:
```
$ go test -bench=.
testing: warning: no tests to run
Benchmark100CountTrailingZerosDeBruijn-2        1000000          1045 ns/op
Benchmark100CountTrailingZerosAsm-2             1000000          2379 ns/op
PASS
ok      
```
