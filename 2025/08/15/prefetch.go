package main

import (
	"fmt"
	"math/rand" // Use modern rand package (Go 1.22+)
	"testing"
)

// DataStruct represents a 64-byte structure with 8 uint32 values
type DataStruct struct {
	a, b, c, d, e, f, g, h uint32
}

const arraySize = 1000000

// initArray initializes an array of DataStruct with sequential values
func initArray(size int) []DataStruct {
	arr := make([]DataStruct, size)
	for i := 0; i < size; i++ {
		arr[i] = DataStruct{
			a: uint32(i),
			b: uint32(i + 1),
			c: uint32(i + 2),
			d: uint32(i + 3),
			e: uint32(i + 4),
			f: uint32(i + 5),
			g: uint32(i + 6),
			h: uint32(i + 7),
		}
	}
	return arr
}

func initArrayConsecutive(size int) []int {
	arr := make([]int, size)
	for i := 0; i < size; i++ {
		arr[i] = i
	}
	return arr
}
func BenchmarkSequentialAccess(b *testing.B) {
	arr := initArray(arraySize)

	b.ResetTimer()
	var sum uint32 // Ensure sum is used to prevent optimization
	for i := 0; i < b.N; i++ {
		sum = 0
		for j := 0; j < arraySize; j++ {
			sum += arr[j].a // Accessing only the first field
		}
	}
	// Use sum to prevent compiler optimization
	b.StopTimer()
	if sum == 0 {
		b.Fatal("sum is zero, which is unexpected")
	}
}

func BenchmarkSequentialAccessWithBuffer(b *testing.B) {
	arr := initArray(arraySize)
	indices := initArrayConsecutive(arraySize)

	b.ResetTimer()
	var sum uint32 // Ensure sum is used to prevent optimization
	for i := 0; i < b.N; i++ {
		sum = 0
		for j := 0; j < arraySize; j++ {
			sum += arr[indices[j]].a // Accessing only the first field
		}
	}
	// Use sum to prevent compiler optimization
	b.StopTimer()
	if sum == 0 {
		b.Fatal("sum is zero, which is unexpected")
	}
}

func BenchmarkBackwardAccess(b *testing.B) {
	arr := initArray(arraySize)

	b.ResetTimer()
	var sum uint32 // Ensure sum is used to prevent optimization
	for i := 0; i < b.N; i++ {
		sum = 0
		for j := arraySize; j > 0; j-- {
			sum += arr[j-1].a // Accessing only the first field
		}
	}
	// Use sum to prevent compiler optimization
	b.StopTimer()
	if sum == 0 {
		b.Fatal("sum is zero, which is unexpected")
	}
}

func BenchmarkRandomAccess(b *testing.B) {
	arr := initArray(arraySize)

	// Initialize random number generator with a fixed seed for reproducibility
	indices := make([]int, arraySize)
	for i := range indices {
		indices[i] = i
	}
	// Use rand.Shuffle from math/rand/v2 with the custom RNG source
	rand.Shuffle(len(indices), func(i, j int) {
		indices[i], indices[j] = indices[j], indices[i]
	})

	b.ResetTimer()
	var sum uint32 // Ensure sum is used to prevent optimization
	for i := 0; i < b.N; i++ {
		sum = 0
		for _, idx := range indices {
			sum += arr[idx].a // Accessing only the first field
		}
	}
	// Use sum to prevent compiler optimization
	b.StopTimer()
	if sum == 0 {
		b.Fatal("sum is zero, which is unexpected")
	}
}

func BenchmarkBouncingAccess(b *testing.B) {
	arr := initArray(arraySize)

	b.ResetTimer()
	var sum uint32 // Ensure sum is used to prevent optimization
	for i := 0; i < b.N; i++ {
		sum = 0
		for j := 0; j < arraySize/2; j++ {
			sum += arr[j].a
			sum += arr[arraySize-j-1].a
		}
	}
	// Use sum to prevent compiler optimization
	b.StopTimer()
	if sum == 0 {
		b.Fatal("sum is zero, which is unexpected")
	}
}

func BenchmarkInterleavedAccess(b *testing.B) {
	arr := initArray(arraySize)

	b.ResetTimer()
	var sum uint32 // Ensure sum is used to prevent optimization
	for i := 0; i < b.N; i++ {
		sum = 0
		for j := 0; j < arraySize/2; j++ {
			sum += arr[j].a
			sum += arr[arraySize/2+j].a
		}
	}
	// Use sum to prevent compiler optimization
	b.StopTimer()
	if sum == 0 {
		b.Fatal("sum is zero, which is unexpected")
	}
}

func BenchmarkBackwardAccessWithBuffer(b *testing.B) {
	arr := initArray(arraySize)
	indices := initArrayConsecutive(arraySize)

	b.ResetTimer()
	var sum uint32
	for i := 0; i < b.N; i++ {
		sum = 0
		for j := arraySize; j > 0; j-- {
			sum += arr[indices[j-1]].a
		}
	}
	b.StopTimer()
	if sum == 0 {
		b.Fatal("sum is zero, which is unexpected")
	}
}

// Version bufferisée des accès bouncing
func BenchmarkBouncingAccessWithBuffer(b *testing.B) {
	arr := initArray(arraySize)
	indices := initArrayConsecutive(arraySize)

	b.ResetTimer()
	var sum uint32
	for i := 0; i < b.N; i++ {
		sum = 0
		for j := 0; j < arraySize/2; j++ {
			sum += arr[indices[j]].a
			sum += arr[indices[arraySize-j-1]].a
		}
	}
	b.StopTimer()
	if sum == 0 {
		b.Fatal("sum is zero, which is unexpected")
	}
}

// Version bufferisée des accès interleaved
func BenchmarkInterleavedAccessWithBuffer(b *testing.B) {
	arr := initArray(arraySize)
	indices := initArrayConsecutive(arraySize)

	b.ResetTimer()
	var sum uint32
	for i := 0; i < b.N; i++ {
		sum = 0
		for j := 0; j < arraySize/2; j++ {
			sum += arr[indices[j]].a
			sum += arr[indices[arraySize/2+j]].a
		}
	}
	b.StopTimer()
	if sum == 0 {
		b.Fatal("sum is zero, which is unexpected")
	}
}

func main() {
	resconsecutive := testing.Benchmark(BenchmarkSequentialAccessWithBuffer)
	fmt.Println("BenchmarkSequentialAccessWithBuffer", resconsecutive)
	resrandom := testing.Benchmark(BenchmarkRandomAccess)
	fmt.Println("BenchmarkRandomAccess", resrandom)
	resbackwardbuf := testing.Benchmark(BenchmarkBackwardAccessWithBuffer)
	fmt.Println("BenchmarkBackwardAccessWithBuffer", resbackwardbuf)

	resbouncingbuf := testing.Benchmark(BenchmarkBouncingAccessWithBuffer)
	fmt.Println("BenchmarkBouncingAccessWithBuffer", resbouncingbuf)
	resinterleavedbuf := testing.Benchmark(BenchmarkInterleavedAccessWithBuffer)
	fmt.Println("BenchmarkInterleavedAccessWithBuffer", resinterleavedbuf)

	ratiobuf := float64(resrandom.NsPerOp()) / float64(resconsecutive.NsPerOp())
	fmt.Printf("Ratio (Random/SequentialBuffer): %.2f\n", ratiobuf)
	fmt.Printf("\n\n")

	res := testing.Benchmark(BenchmarkSequentialAccess)
	fmt.Println("BenchmarkSequentialAccess", res)
	resbackward := testing.Benchmark(BenchmarkBackwardAccess)
	fmt.Println("BenchmarkBackwardAccess", resbackward)
	resbouncing := testing.Benchmark(BenchmarkBouncingAccess)
	fmt.Println("BenchmarkBouncingAccess", resbouncing)
	resinterleaved := testing.Benchmark(BenchmarkInterleavedAccess)
	fmt.Println("BenchmarkInterleavedAccess", resinterleaved)
}
