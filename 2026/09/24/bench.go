package main

import (
	"fmt"
	"runtime"
	"strconv"
	"time"
)

var buf = make([]string, 1024)

//go:noinline
func fromInt(n int) {
	for i := 0; i < n; i++ {
		buf[i&1023] = strconv.Itoa(i)
	}
}

func main() {
	const N = 100_000_000
	fromInt(1_000_000) // warm up
	best := 1e300
	for r := 0; r < 5; r++ {
		t0 := time.Now()
		fromInt(N)
		dt := float64(time.Since(t0).Nanoseconds())
		if dt < best {
			best = dt
		}
	}
	total := 0
	for _, s := range buf {
		total += len(s)
	}
	ns := best / N
	fmt.Printf("Go %s\n", runtime.Version())
	fmt.Printf("%-24s %7.2f ns/string  %8.1f M/s   (check %d)\n", "strconv.Itoa(i)", ns, 1e3/ns, total)
}
