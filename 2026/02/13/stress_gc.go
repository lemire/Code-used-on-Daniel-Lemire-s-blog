package main

import (
	"fmt"
	"runtime"
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

func main() {
	fmt.Println("Starting GC stress test...")
	maxdiff := time.Duration(0)

	objects1 := createLinkedList(50_000_000)
	start := time.Now()

	// Create a lot of cyclic garbage
	for batch := 0; batch < 1000000; batch++ {
		batchStart := time.Now()
		// compute the delay between batchStart and start
		diff := batchStart.Sub(start)
		if diff > maxdiff {
			fmt.Printf("Batch %d: delay between batch start and overall start: %v\n", batch, diff)
			maxdiff = diff
		}
		start = batchStart
		objects := createLinkedList(1000)
		_ = objects // use the variable to avoid unused error
	}

	end := time.Now()
	// Keep the big list reachable for the whole loop, so that the garbage
	// collector has to scan it on every cycle. Assigning to the blank
	// identifier (_ = objects1) does NOT do this: Go's liveness analysis
	// treats objects1 as dead right after the assignment, and the whole list
	// is collected before the loop even starts. This matches the Python
	// version, where `x` stays alive until the program ends.
	runtime.KeepAlive(objects1)
	fmt.Printf("Total time: %v\n", end.Sub(start))
	fmt.Printf("Max delay between batch start and overall start: %v\n", maxdiff)
	fmt.Println("GC stress test completed.")
}
