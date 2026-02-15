package main

import (
	"fmt"
	"time"
)

type Node struct {
	value int
	next  *Node
}

func createCyclicObjects(num int) []*Node {
	objects := make([]*Node, 0, num)
	for i := 0; i < num; i++ {
		a := &Node{value: i}
		b := &Node{value: i + 1}
		a.next = b
		b.next = a
		objects = append(objects, a)
	}
	return objects
}

func main() {
	fmt.Println("Starting GC stress test...")
	maxdiff := time.Duration(0)

	objects1 := createCyclicObjects(50_000_000)
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
		objects := createCyclicObjects(100)
		_ = objects // use the variable to avoid unused error
	}

	end := time.Now()
	fmt.Printf("Total time: %v\n", end.Sub(start))
	fmt.Printf("Max delay between batch start and overall start: %v\n", maxdiff)
	fmt.Println("GC stress test completed.")
}
