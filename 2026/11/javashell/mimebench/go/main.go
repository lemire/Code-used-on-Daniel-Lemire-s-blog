// MIME base64 decoding (76-character lines, CRLF) with encoding/base64,
// which ignores '\r' and '\n'.
package main

import (
	"encoding/base64"
	"fmt"
	"os"
	"runtime"
	"time"
)

var sink int

func bench(label string, inLen, expLen int, expSum uint64, f func() []byte) {
	out := f()
	var s uint64
	for _, b := range out {
		s += uint64(b)
	}
	if len(out) != expLen || s != expSum {
		panic("bad decode " + label)
	}
	iters := 200
	for i := 0; i < 3*iters; i++ {
		sink += len(f())
	}
	best := 1e300
	for t := 0; t < 10; t++ {
		st := time.Now()
		for i := 0; i < iters; i++ {
			sink += len(f())
		}
		best = min(best, float64(time.Since(st).Nanoseconds())/float64(iters))
	}
	fmt.Printf("go %s mime %s: %.2f GB/s\n", runtime.Version(), label, float64(inLen)/best)
}

func main() {
	var expLen int
	var expSum uint64
	ef, _ := os.Open("expected.txt")
	fmt.Fscan(ef, &expLen, &expSum)
	in, err := os.ReadFile("mime.txt")
	if err != nil {
		panic(err)
	}
	str := string(in)
	bench("base64.StdEncoding.DecodeString", len(in), expLen, expSum, func() []byte {
		out, err := base64.StdEncoding.DecodeString(str)
		if err != nil {
			panic(err)
		}
		return out
	})
	dst := make([]byte, base64.StdEncoding.DecodedLen(len(in)))
	bench("base64.StdEncoding.Decode (preallocated)", len(in), expLen, expSum, func() []byte {
		n, err := base64.StdEncoding.Decode(dst, in)
		if err != nil {
			panic(err)
		}
		return dst[:n]
	})
}
