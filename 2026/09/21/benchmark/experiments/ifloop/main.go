// A Go loop with an if in it, laid out so a miss is a taken skip and
// the loop back-edge is also taken.
//
// Indexed three-clause for and range do not do this: range becomes CSEL,
// and a three-clause for rotates so the back-edge is not taken. An
// infinite for that breaks when the index hits len(p) keeps both
// branches. (gc still emits a not-taken bounds check on p[i].)
//
// Always-miss: two taken branches per iteration. Always-hit: only the
// back-edge. A core that can retire two taken branches per cycle should
// run the miss path at about one cycle per iteration.
//
//   go run .
package main

import (
	"fmt"
	"os"
	"time"
)

//go:noinline
func lastHit(p []byte, thresh byte, last *byte) {
	n := len(p)
	if n == 0 {
		return
	}
	i := 0
	for {
		v := p[i]
		if v > thresh {
			*last = v
		}
		i++
		if i == n {
			break
		}
	}
}

func bench(name string, n int, fn func()) {
	for i := 0; i < 50; i++ {
		fn()
	}
	const want = 200 * time.Millisecond
	start := time.Now()
	iters := 0
	for time.Since(start) < want {
		fn()
		iters++
	}
	elapsed := time.Since(start)
	ns := float64(elapsed.Nanoseconds()) / float64(iters) / float64(n)
	fmt.Printf("%-12s ns/iter %8.4f  inner_iters %d\n", name, ns, iters*n)
}

func main() {
	const n = 4096
	low := make([]byte, n)
	high := make([]byte, n)
	for i := 0; i < n; i++ {
		low[i] = 1    // never > 128: skip taken, back-edge taken
		high[i] = 255 // always > 128: skip not-taken, only the back-edge
	}
	var last byte
	run := "both"
	if len(os.Args) > 1 {
		run = os.Args[1]
	}
	if run == "both" || run == "miss" {
		bench("always miss", n, func() { lastHit(low, 128, &last) })
	}
	if run == "both" || run == "hit" {
		bench("always hit", n, func() { lastHit(high, 128, &last) })
	}
}
