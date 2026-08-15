// stress_gc.go
//
// Instrumented version of the GC stress test from
// https://lemire.me/blog/2026/02/15/how-bad-can-python-stop-the-world-pauses-get/
//
// The algorithm is unchanged: build one large linked list that stays live for
// the whole run, then allocate and immediately discard many small linked lists.
// We record the maximum delay between two consecutive iterations of the loop.
//
// In addition, we report the GC cost as seen by the runtime itself:
//   - the stop-the-world pause distribution (runtime/metrics /gc/pauses:seconds)
//   - the fraction of total CPU time spent in the garbage collector
//     (runtime/metrics /cpu/classes/gc/total:cpu-seconds)
//
// Output is a single JSON object so results can be aggregated across Go
// versions.
package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"os"
	"runtime"
	"runtime/metrics"
	"sort"
	"time"
)

type Node struct {
	value int
	next  *Node
}

func createLinkedList(num int) *Node {
	head := &Node{value: 0}
	current := head
	for i := 1; i < num; i++ {
		current.next = &Node{value: i}
		current = current.next
	}
	return head
}

// histogram summary helpers ---------------------------------------------------

// quantile returns the value at the given quantile of a float64 histogram.
// Buckets are open on the left, closed on the right: bucket i covers
// (Buckets[i], Buckets[i+1]].
func quantile(h *metrics.Float64Histogram, q float64) float64 {
	var total uint64
	for _, c := range h.Counts {
		total += c
	}
	if total == 0 {
		return 0
	}
	target := uint64(q * float64(total))
	var seen uint64
	for i, c := range h.Counts {
		seen += c
		if seen >= target {
			// Report the upper edge of the bucket; +Inf can only be the last
			// bucket, in which case fall back to the lower edge.
			hi := h.Buckets[i+1]
			if hi > 1e18 {
				return h.Buckets[i]
			}
			return hi
		}
	}
	return h.Buckets[len(h.Buckets)-1]
}

func histCount(h *metrics.Float64Histogram) uint64 {
	var total uint64
	for _, c := range h.Counts {
		total += c
	}
	return total
}

// histMean approximates the mean using bucket midpoints.
func histMean(h *metrics.Float64Histogram) float64 {
	var total uint64
	var sum float64
	for i, c := range h.Counts {
		if c == 0 {
			continue
		}
		lo, hi := h.Buckets[i], h.Buckets[i+1]
		mid := lo
		if hi <= 1e18 {
			mid = (lo + hi) / 2
		}
		sum += mid * float64(c)
		total += c
	}
	if total == 0 {
		return 0
	}
	return sum / float64(total)
}

type Result struct {
	GoVersion    string  `json:"go_version"`
	GOEXPERIMENT string  `json:"goexperiment"`
	GOMAXPROCS   int     `json:"gomaxprocs"`
	BigList      int     `json:"big_list"`
	Batches      int     `json:"batches"`
	SmallList    int     `json:"small_list"`
	BuildSecs    float64 `json:"build_secs"`
	LoopSecs     float64 `json:"loop_secs"`
	MaxDelayMS   float64 `json:"max_delay_ms"`
	P99DelayMS   float64 `json:"p99_delay_ms"`
	// Stop-the-world pauses, as reported by the runtime.
	NumPauses  uint64  `json:"num_pauses"`
	PauseMeanU float64 `json:"pause_mean_us"`
	PauseP50U  float64 `json:"pause_p50_us"`
	PauseP99U  float64 `json:"pause_p99_us"`
	PauseMaxU  float64 `json:"pause_max_us"`
	PauseSumMS float64 `json:"pause_total_ms"`
	// CPU accounting over the measured loop.
	GCCPUSecs    float64 `json:"gc_cpu_secs"`
	TotalCPUSecs float64 `json:"total_cpu_secs"`
	GCCPUPercent float64 `json:"gc_cpu_percent"`
	NumGC        uint64  `json:"num_gc"`
	HeapLiveMB   float64 `json:"heap_live_mb"`
	HeapGoalMB   float64 `json:"heap_goal_mb"`
}

func main() {
	big := flag.Int("big", 50_000_000, "size of the long-lived linked list")
	batches := flag.Int("batches", 1_000_000, "number of small linked lists")
	small := flag.Int("small", 1000, "size of each small linked list")
	flag.Parse()

	samples := []metrics.Sample{
		{Name: "/gc/pauses:seconds"},
		{Name: "/cpu/classes/gc/total:cpu-seconds"},
		{Name: "/cpu/classes/total:cpu-seconds"},
		{Name: "/gc/cycles/total:gc-cycles"},
		{Name: "/memory/classes/heap/objects:bytes"},
		{Name: "/gc/heap/goal:bytes"},
	}
	read := func() {
		metrics.Read(samples)
	}

	buildStart := time.Now()
	objects1 := createLinkedList(*big)
	buildSecs := time.Since(buildStart).Seconds()

	// Baseline for the CPU counters: we only want to attribute the cost of the
	// measured loop, not the cost of building the long-lived list.
	read()
	gcCPU0 := samples[1].Value.Float64()
	totCPU0 := samples[2].Value.Float64()
	numGC0 := samples[3].Value.Uint64()

	// Delay histogram: the original benchmark only kept the maximum, we also
	// keep every sample so we can report a high quantile.
	delays := make([]float64, 0, *batches)

	// Nothing is read from the runtime inside the loop: metrics.Read stops the
	// world, which would show up in the very delays we are trying to measure.
	maxdiff := time.Duration(0)
	loopStart := time.Now()
	start := loopStart

	for batch := 0; batch < *batches; batch++ {
		batchStart := time.Now()
		diff := batchStart.Sub(start)
		delays = append(delays, diff.Seconds()*1000)
		if diff > maxdiff {
			maxdiff = diff
		}
		start = batchStart
		objects := createLinkedList(*small)
		_ = objects
	}
	loopSecs := time.Since(loopStart).Seconds()

	read()
	pauses := samples[0].Value.Float64Histogram()
	gcCPU := samples[1].Value.Float64() - gcCPU0
	totCPU := samples[2].Value.Float64() - totCPU0
	numGC := samples[3].Value.Uint64() - numGC0
	heapLive := samples[4].Value.Uint64()
	heapGoal := samples[5].Value.Uint64()

	runtime.KeepAlive(objects1)

	// The pause histogram is cumulative from process start; the build phase
	// contributes a handful of cycles, which is negligible next to the loop.
	var pauseSum float64
	for i, c := range pauses.Counts {
		if c == 0 {
			continue
		}
		lo, hi := pauses.Buckets[i], pauses.Buckets[i+1]
		mid := lo
		if hi <= 1e18 {
			mid = (lo + hi) / 2
		}
		pauseSum += mid * float64(c)
	}

	// p99 of the inter-iteration delays.
	p99 := 0.0
	if n := len(delays); n > 0 {
		// partial selection is enough; a full sort of 1e6 floats is cheap
		sorted := make([]float64, n)
		copy(sorted, delays)
		sort.Float64s(sorted)
		p99 = sorted[int(0.99*float64(n))]
	}

	res := Result{
		GoVersion:    runtime.Version(),
		GOEXPERIMENT: os.Getenv("GOEXPERIMENT"),
		GOMAXPROCS:   runtime.GOMAXPROCS(0),
		BigList:      *big,
		Batches:      *batches,
		SmallList:    *small,
		BuildSecs:    buildSecs,
		LoopSecs:     loopSecs,
		MaxDelayMS:   maxdiff.Seconds() * 1000,
		P99DelayMS:   p99,
		NumPauses:    histCount(pauses),
		PauseMeanU:   histMean(pauses) * 1e6,
		PauseP50U:    quantile(pauses, 0.50) * 1e6,
		PauseP99U:    quantile(pauses, 0.99) * 1e6,
		PauseMaxU:    quantile(pauses, 1.0) * 1e6,
		PauseSumMS:   pauseSum * 1e3,
		GCCPUSecs:    gcCPU,
		TotalCPUSecs: totCPU,
		GCCPUPercent: 100 * gcCPU / totCPU,
		NumGC:        numGC,
		HeapLiveMB:   float64(heapLive) / (1 << 20),
		HeapGoalMB:   float64(heapGoal) / (1 << 20),
	}

	enc := json.NewEncoder(os.Stdout)
	enc.SetIndent("", "  ")
	if err := enc.Encode(res); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}
