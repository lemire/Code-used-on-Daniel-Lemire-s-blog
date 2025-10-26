package main

import (
	"bytes"
	"fmt"
	"log"
	"net/http"
	_ "net/http/pprof" // register pprof handlers on the default mux
	"os"
	"runtime"
	"runtime/pprof"
	"runtime/trace"
	"strconv"
	"sync"
	"time"
)

var profilingMu sync.Mutex
var profiling bool

func main() {
	http.HandleFunc("/cpu", handleCPUProfile)
	http.HandleFunc("/mem", handleMemProfile)
	http.HandleFunc("/trace", handleTrace)

	addr := ":6060"
	if a := os.Getenv("PPROF_ADDR"); a != "" {
		addr = a
	}
	log.Printf("starting profiler server on %s (pprof endpoints also available)", addr)
	log.Printf("CPU profile: /cpu?seconds=30")
	log.Printf("Heap profile: /mem")
	log.Printf("Trace: /trace?seconds=5")
	log.Fatal(http.ListenAndServe(addr, nil))
}

// handleCPUProfile captures a CPU profile for the requested duration (seconds)
// and returns the pprof data as the response body.
func handleCPUProfile(w http.ResponseWriter, r *http.Request) {
	secs := 30
	if s := r.URL.Query().Get("seconds"); s != "" {
		if v, err := strconv.Atoi(s); err == nil && v > 0 {
			secs = v
		}
	}

	profilingMu.Lock()
	if profiling {
		profilingMu.Unlock()
		http.Error(w, "another profile is already running", http.StatusConflict)
		return
	}
	profiling = true
	profilingMu.Unlock()

	defer func() {
		profilingMu.Lock()
		profiling = false
		profilingMu.Unlock()
	}()

	var buf bytes.Buffer
	if err := pprof.StartCPUProfile(&buf); err != nil {
		http.Error(w, fmt.Sprintf("failed to start CPU profile: %v", err), http.StatusInternalServerError)
		return
	}

	// Profile for the requested duration
	time.Sleep(time.Duration(secs) * time.Second)
	pprof.StopCPUProfile()

	w.Header().Set("Content-Type", "application/octet-stream")
	w.Header().Set("Content-Disposition", fmt.Sprintf("attachment; filename=cpu-%ds.prof", secs))
	if _, err := w.Write(buf.Bytes()); err != nil {
		log.Printf("error writing profile response: %v", err)
	}
}

// handleMemProfile forces a GC and returns a heap profile
func handleMemProfile(w http.ResponseWriter, r *http.Request) {
	profilingMu.Lock()
	if profiling {
		profilingMu.Unlock()
		http.Error(w, "a CPU profile is running, try later", http.StatusConflict)
		return
	}
	profilingMu.Unlock()

	// force GC so the heap profile is up to date
	runtime.GC()
	w.Header().Set("Content-Type", "application/octet-stream")
	w.Header().Set("Content-Disposition", "attachment; filename=mem.prof")
	if err := pprof.WriteHeapProfile(w); err != nil {
		http.Error(w, fmt.Sprintf("failed to write heap profile: %v", err), http.StatusInternalServerError)
		return
	}
}

// handleTrace captures a runtime/trace trace for the given seconds and returns it
func handleTrace(w http.ResponseWriter, r *http.Request) {
	secs := 5
	if s := r.URL.Query().Get("seconds"); s != "" {
		if v, err := strconv.Atoi(s); err == nil && v > 0 {
			secs = v
		}
	}

	profilingMu.Lock()
	if profiling {
		profilingMu.Unlock()
		http.Error(w, "another profile is already running", http.StatusConflict)
		return
	}
	profiling = true
	profilingMu.Unlock()

	defer func() {
		profilingMu.Lock()
		profiling = false
		profilingMu.Unlock()
	}()

	var buf bytes.Buffer
	if err := trace.Start(&buf); err != nil {
		http.Error(w, fmt.Sprintf("failed to start trace: %v", err), http.StatusInternalServerError)
		return
	}
	time.Sleep(time.Duration(secs) * time.Second)
	trace.Stop()

	w.Header().Set("Content-Type", "application/octet-stream")
	w.Header().Set("Content-Disposition", fmt.Sprintf("attachment; filename=trace-%ds.out", secs))
	if _, err := w.Write(buf.Bytes()); err != nil {
		log.Printf("error writing trace response: %v", err)
	}
}
