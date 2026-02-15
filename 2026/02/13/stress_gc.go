package main

import (
	"fmt"
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
	_ = objects1 // prevent optimization
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
	fmt.Printf("Total time: %v\n", end.Sub(start))
	fmt.Printf("Max delay between batch start and overall start: %v\n", maxdiff)
	fmt.Println("GC stress test completed.")
}
