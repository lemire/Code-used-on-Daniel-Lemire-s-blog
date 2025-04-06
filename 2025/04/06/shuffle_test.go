package main

// Nevin Brackett-Rozinsky, Daniel Lemire, Batched Ranged Random Integer Generation, Software: Practice and Experience 55 (1), 2024.
// Daniel Lemire, Fast Random Integer Generation in an Interval, ACM Transactions on Modeling and Computer Simulation, 29 (1), 2019.
import (
	"math/rand"
	"testing"
)

func BenchmarkShuffleStandard10K(b *testing.B) {
	size := 10000
	data := make([]uint64, size)
	for i := 0; i < size; i++ {
		data[i] = uint64(i)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		rand.Shuffle(len(data), func(i, j int) {
			data[i], data[j] = data[j], data[i]
		})
	}
}

func BenchmarkShuffleBatch2_10K(b *testing.B) {
	size := 10000
	data := make([]uint64, size)
	for i := 0; i < size; i++ {
		data[i] = uint64(i)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		shuffleBatch2(data)
	}
}

func BenchmarkShuffleBatch23456_10K(b *testing.B) {
	size := 10000
	data := make([]uint64, size)
	for i := 0; i < size; i++ {
		data[i] = uint64(i)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		shuffleBatch23456(data)
	}
}
