package main

import (
	"fmt"
	"math/rand"
	"time"
)

const (
	arraySize = 1 << 26
	runs      = 20
)

func access(arr []int32, indices []int) int64 {
	var sum int64
	for _, i := range indices {
		sum += int64(arr[indices[i]])
	}
	return sum
}

func main() {
	arr := make([]int32, arraySize)
	for i := range arr {
		arr[i] = int32(i)
	}

	// Sequential indices
	sequentialIndices := make([]int, arraySize)
	for i := range sequentialIndices {
		sequentialIndices[i] = i
	}

	// Random indices
	randomIndices := make([]int, arraySize)
	for i := range randomIndices {
		randomIndices[i] = i
	}
	rand.Shuffle(len(randomIndices), func(i, j int) {
		randomIndices[i], randomIndices[j] = randomIndices[j], randomIndices[i]
	})

	// Backward indices
	backwardIndices := make([]int, arraySize)
	for i := range backwardIndices {
		backwardIndices[i] = arraySize - 1 - i
	}

	// Interleaved indices (1st, 3rd, 2nd, 4th, ...)
	interleavedIndices := make([]int, arraySize)
	for i := 0; i < arraySize/2; i++ {
		interleavedIndices[i*2] = i
		if i*2+1 < arraySize {
			interleavedIndices[i*2+1] = i + arraySize/2
		}
	}

	// Bouncing indices (first, last, second, second-to-last, ...)
	bouncingIndices := make([]int, arraySize)
	for i := 0; i < arraySize/2; i++ {
		bouncingIndices[i*2] = i
		if i*2+1 < arraySize {
			bouncingIndices[i*2+1] = arraySize - 1 - i
		}
	}

	// Measure sequential access
	sequentialTimes := make([]float64, runs)
	for i := 0; i < runs; i++ {
		start := time.Now()
		_ = access(arr, sequentialIndices)
		sequentialTimes[i] = time.Since(start).Seconds() * 1000
	}

	// Measure random access
	randomTimes := make([]float64, runs)
	for i := 0; i < runs; i++ {
		start := time.Now()
		_ = access(arr, randomIndices)
		randomTimes[i] = time.Since(start).Seconds() * 1000
	}

	// Measure backward access
	backwardTimes := make([]float64, runs)
	for i := 0; i < runs; i++ {
		start := time.Now()
		_ = access(arr, backwardIndices)
		backwardTimes[i] = time.Since(start).Seconds() * 1000
	}

	// Measure interleaved access
	interleavedTimes := make([]float64, runs)
	for i := 0; i < runs; i++ {
		start := time.Now()
		_ = access(arr, interleavedIndices)
		interleavedTimes[i] = time.Since(start).Seconds() * 1000
	}

	// Measure bouncing access
	bouncingTimes := make([]float64, runs)
	for i := 0; i < runs; i++ {
		start := time.Now()
		_ = access(arr, bouncingIndices)
		bouncingTimes[i] = time.Since(start).Seconds() * 1000
	}

	// Calculate statistics
	var seqMin, seqMax, seqAvg, randMin, randMax, randAvg float64
	var backMin, backMax, backAvg, interMin, interMax, interAvg float64
	var bounceMin, bounceMax, bounceAvg float64

	for i := 0; i < runs; i++ {
		// Sequential stats
		if i == 0 || sequentialTimes[i] < seqMin {
			seqMin = sequentialTimes[i]
		}
		if i == 0 || sequentialTimes[i] > seqMax {
			seqMax = sequentialTimes[i]
		}
		seqAvg += sequentialTimes[i]

		// Random stats
		if i == 0 || randomTimes[i] < randMin {
			randMin = randomTimes[i]
		}
		if i == 0 || randomTimes[i] > randMax {
			randMax = randomTimes[i]
		}
		randAvg += randomTimes[i]

		// Backward stats
		if i == 0 || backwardTimes[i] < backMin {
			backMin = backwardTimes[i]
		}
		if i == 0 || backwardTimes[i] > backMax {
			backMax = backwardTimes[i]
		}
		backAvg += backwardTimes[i]

		// Interleaved stats
		if i == 0 || interleavedTimes[i] < interMin {
			interMin = interleavedTimes[i]
		}
		if i == 0 || interleavedTimes[i] > interMax {
			interMax = interleavedTimes[i]
		}
		interAvg += interleavedTimes[i]

		// Bouncing stats
		if i == 0 || bouncingTimes[i] < bounceMin {
			bounceMin = bouncingTimes[i]
		}
		if i == 0 || bouncingTimes[i] > bounceMax {
			bounceMax = bouncingTimes[i]
		}
		bounceAvg += bouncingTimes[i]
	}

	seqAvg /= runs
	randAvg /= runs
	backAvg /= runs
	interAvg /= runs
	bounceAvg /= runs

	// Print results
	fmt.Printf("Seq (ms) : min=%.2f, max=%.2f, mean=%.2f\n", seqMin, seqMax, seqAvg)
	fmt.Printf("Alea (ms) : min=%.2f, max=%.2f, mean=%.2f\n", randMin, randMax, randAvg)
	fmt.Printf("Backward (ms) : min=%.2f, max=%.2f, mean=%.2f\n", backMin, backMax, backAvg)
	fmt.Printf("Interleaved (ms) : min=%.2f, max=%.2f, mean=%.2f\n", interMin, interMax, interAvg)
	fmt.Printf("Bouncing (ms) : min=%.2f, max=%.2f, mean=%.2f\n", bounceMin, bounceMax, bounceAvg)
}
