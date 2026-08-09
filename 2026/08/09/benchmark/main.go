// A small JSON parsing benchmark used to measure Go's profile-guided
// optimization. It parses documents into interface{}, which is the general
// path through encoding/json (no struct-specific fast paths).
//
//   go run . -bench                 measure every document
//   go run . -profile cpu.pprof -train twitter.json
package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"os"
	"path/filepath"
	"runtime/pprof"
	"sort"
	"time"
)

var docs = []string{"twitter.json", "canada.json", "citm_catalog.json"}

func load(dir, name string) []byte {
	b, err := os.ReadFile(filepath.Join(dir, name))
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
	return b
}

// parse is the measured work: decode into a generic value.
func parse(data []byte) int {
	var v interface{}
	if err := json.Unmarshal(data, &v); err != nil {
		fmt.Fprintln(os.Stderr, "parse error:", err)
		os.Exit(1)
	}
	switch t := v.(type) {
	case map[string]interface{}:
		return len(t)
	case []interface{}:
		return len(t)
	}
	return 1
}

// best runs the parse repeatedly and returns the fastest observed throughput.
func best(data []byte, rounds, iters int) float64 {
	sink := 0
	for i := 0; i < iters; i++ {
		sink += parse(data)
	}
	fastest := time.Duration(1<<62 - 1)
	for r := 0; r < rounds; r++ {
		start := time.Now()
		for i := 0; i < iters; i++ {
			sink += parse(data)
		}
		if d := time.Since(start); d < fastest {
			fastest = d
		}
	}
	if sink < 0 {
		fmt.Println("impossible")
	}
	mb := float64(len(data)) * float64(iters) / 1e6
	return mb / fastest.Seconds()
}

func main() {
	dir := flag.String("dir", ".", "directory holding the JSON documents")
	bench := flag.Bool("bench", false, "benchmark every document")
	profile := flag.String("profile", "", "write a CPU profile here")
	train := flag.String("train", "", "document to parse while profiling")
	rounds := flag.Int("rounds", 9, "timed rounds")
	seconds := flag.Int("seconds", 8, "profiling duration, seconds")
	flag.Parse()

	if *profile != "" {
		data := load(*dir, *train)
		f, err := os.Create(*profile)
		if err != nil {
			fmt.Fprintln(os.Stderr, err)
			os.Exit(1)
		}
		if err := pprof.StartCPUProfile(f); err != nil {
			fmt.Fprintln(os.Stderr, err)
			os.Exit(1)
		}
		deadline := time.Now().Add(time.Duration(*seconds) * time.Second)
		n := 0
		for time.Now().Before(deadline) {
			parse(data)
			n++
		}
		pprof.StopCPUProfile()
		f.Close()
		fmt.Fprintf(os.Stderr, "profiled %d parses of %s\n", n, *train)
		return
	}

	if *bench {
		for _, d := range docs {
			data := load(*dir, d)
			iters := 1
			if n := 30 * 1000000 / (len(data) + 1); n > 1 {
				iters = n // roughly 30 MB of work per timed round
			}
			vals := make([]float64, 0, 5)
			for k := 0; k < 5; k++ {
				vals = append(vals, best(data, *rounds, iters))
			}
			sort.Float64s(vals)
			fmt.Printf("%-20s %8.1f MB/s   (spread %.1f%%)\n",
				d, vals[len(vals)/2], (vals[len(vals)-1]/vals[0]-1)*100)
		}
	}
}
