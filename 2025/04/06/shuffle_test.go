package main

// Nevin Brackett-Rozinsky, Daniel Lemire, Batched Ranged Random Integer Generation, Software: Practice and Experience 55 (1), 2024.
// Daniel Lemire, Fast Random Integer Generation in an Interval, ACM Transactions on Modeling and Computer Simulation, 29 (1), 2019.
import (
	"math/bits"
	"math/rand/v2"
	"testing"
)

func shuffleBatch2(storage []uint64) {
	i := uint64(len(storage))
	for ; i > 1<<30; i-- {
		partialShuffle64b(storage, i, 1, i)
	}
	bound := uint64(1) << 60
	for ; i > 1; i -= 2 {
		bound = partialShuffle64b(storage, i, 2, bound)
	}
}

func shuffleBatch23456(storage []uint64) {
	i := uint64(len(storage))
	for ; i > 1<<30; i-- {
		partialShuffle64b(storage, i, 1, i)
	}
	bound := uint64(1) << 60
	for ; i > 1<<19; i -= 2 {
		bound = partialShuffle64b(storage, i, 2, bound)
	}
	bound = uint64(1) << 57
	for ; i > 1<<14; i -= 3 {
		bound = partialShuffle64b(storage, i, 3, bound)
	}
	bound = uint64(1) << 56
	for ; i > 1<<11; i -= 4 {
		bound = partialShuffle64b(storage, i, 4, bound)
	}
	bound = uint64(1) << 55
	for ; i > 1<<9; i -= 5 {
		bound = partialShuffle64b(storage, i, 5, bound)
	}
	bound = uint64(1) << 54
	for ; i > 6; i -= 6 {
		bound = partialShuffle64b(storage, i, 6, bound)
	}
	if i > 1 {
		partialShuffle64b(storage, i, i-1, 720)
	}
}

func partialShuffle64b(result []uint64, n, k, bound uint64) uint64 {
	r := rand.Uint64()

	for i := uint64(0); i < k; i++ {
		hi, lo := bits.Mul64(n-i, r)
		r = lo
		result[i] = hi
	}

	if r < bound {
		bound = n
		for i := uint64(1); i < k; i++ {
			bound *= (n - i)
		}
		t := (-bound) % bound

		for r < t {
			r = rand.Uint64()
			for i := uint64(0); i < k; i++ {
				hi, lo := bits.Mul64(n-i, r)
				r = lo
				result[i] = hi
			}
		}
	}

	return bound
}

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

func main() {
}
