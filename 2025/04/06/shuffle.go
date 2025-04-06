package main

// Nevin Brackett-Rozinsky, Daniel Lemire, Batched Ranged Random Integer Generation, Software: Practice and Experience 55 (1), 2024.
// Daniel Lemire, Fast Random Integer Generation in an Interval, ACM Transactions on Modeling and Computer Simulation, 29 (1), 2019.
import (
	"fmt"
	"math/bits"
	"math/rand/v2"
)

func shuffleBatch2(storage []uint64) {
	i := uint64(len(storage))
	indexes := [7]uint64{}
	for ; i > 1<<30; i-- {
		partialShuffle64b(storage, indexes, i, 1, i)
	}
	bound := uint64(1) << 60
	for ; i > 1; i -= 2 {
		bound = partialShuffle64b(storage, indexes, i, 2, bound)
	}
}

func shuffleBatch23456(storage []uint64) {
	i := uint64(len(storage))
	indexes := [7]uint64{}

	for ; i > 1<<30; i-- {
		partialShuffle64b(storage, indexes, i, 1, i)
	}
	bound := uint64(1) << 60
	for ; i > 1<<19; i -= 2 {
		bound = partialShuffle64b(storage, indexes, i, 2, bound)
	}
	bound = uint64(1) << 57
	for ; i > 1<<14; i -= 3 {
		bound = partialShuffle64b(storage, indexes, i, 3, bound)
	}
	bound = uint64(1) << 56
	for ; i > 1<<11; i -= 4 {
		bound = partialShuffle64b(storage, indexes, i, 4, bound)
	}
	bound = uint64(1) << 55
	for ; i > 1<<9; i -= 5 {
		bound = partialShuffle64b(storage, indexes, i, 5, bound)
	}
	bound = uint64(1) << 54
	for ; i > 6; i -= 6 {
		bound = partialShuffle64b(storage, indexes, i, 6, bound)
	}
	if i > 1 {
		partialShuffle64b(storage, indexes, i, i-1, 720)
	}
}

func partialShuffle64b(storage []uint64, indexes [7]uint64, n, k, bound uint64) uint64 {
	r := rand.Uint64()

	for i := uint64(0); i < k; i++ {
		hi, lo := bits.Mul64(n-i, r)
		r = lo
		indexes[i] = hi
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
				indexes[i] = hi
			}
		}
	}
	for i := uint64(0); i < k; i++ {
		pos1 := n - i - 1
		pos2 := indexes[i]
		storage[pos1], storage[pos2] = storage[pos2], storage[pos1]
	}
	return bound
}

func main() {
	data := []uint64{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	shuffleBatch2(data)
	fmt.Println(data)
}
