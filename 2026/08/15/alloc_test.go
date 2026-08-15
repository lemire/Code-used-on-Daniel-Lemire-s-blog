package alloc

import "testing"

// Pointer-free structs of increasing size.
type S16 struct{ a, b int64 }
type S32 struct{ a, b, c, d int64 }
type S64 struct{ a [8]int64 }
type S80 struct{ a [10]int64 }
type S96 struct{ a [12]int64 }

// A 16-byte struct that contains a pointer.
type Node struct {
	value int64
	next  *Node
}

var sink any

func BenchmarkAlloc16(b *testing.B) {
	for b.Loop() {
		sink = &S16{}
	}
}

func BenchmarkAlloc32(b *testing.B) {
	for b.Loop() {
		sink = &S32{}
	}
}

func BenchmarkAlloc64(b *testing.B) {
	for b.Loop() {
		sink = &S64{}
	}
}

func BenchmarkAlloc80(b *testing.B) {
	for b.Loop() {
		sink = &S80{}
	}
}

func BenchmarkAlloc96(b *testing.B) {
	for b.Loop() {
		sink = &S96{}
	}
}

func BenchmarkAllocNode16(b *testing.B) {
	for b.Loop() {
		sink = &Node{}
	}
}
