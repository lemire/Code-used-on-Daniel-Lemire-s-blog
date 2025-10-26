package main

import (
	"math"
	"strconv"
	"testing"
)

// floatStrings contains 10,000 float strings for benchmarking
var floatStrings []string

func init() {
	floatStrings = make([]string, 10000)
	for i := 0; i < 10000; i++ {
		// Generate various float representations
		if i%4 == 0 {
			floatStrings[i] = strconv.FormatFloat(float64(i)*pi*1e-20, 'f', -1, 64)
		} else if i%4 == 1 {
			floatStrings[i] = strconv.FormatFloat(float64(i)*eConst*1e100, 'f', 6, 64)
		} else if i%4 == 2 {
			floatStrings[i] = strconv.FormatFloat(float64(i)/1000000.0, 'e', -1, 64)
		} else {
			floatStrings[i] = strconv.FormatFloat(float64(i)*sqrt2*1e100, 'g', -1, 64)
		}
	}
}

// Use math constants for readability and correctness
const (
	pi     = math.Pi
	eConst = math.E
	sqrt2  = math.Sqrt2
	eulerG = 0.5772156649015328606 // Euler-Mascheroni constant (approx)
)

// BenchmarkParseFloat benchmarks the strconv.ParseFloat function
// which converts string representations to floating-point numbers.
// This benchmark runs for a longer duration to generate better flame charts.
func BenchmarkParseFloat(b *testing.B) {
	for i := 0; i < b.N; i++ {
		idx := i % len(floatStrings)
		_, _ = strconv.ParseFloat(floatStrings[idx], 64)
	}
}

// To run the benchmarks:
// go test -bench=. -benchmem
//
// To generate CPU profile for flamegraph:
// go test -bench=. -cpuprofile=cpu.prof
//
// To view the profile:
// go tool pprof -http=:8080 cpu.prof
