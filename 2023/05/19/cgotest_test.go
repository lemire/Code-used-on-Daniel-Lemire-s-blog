package main

import "testing"

// go test -bench BenchmarkAddition -run -
func BenchmarkAddition(b *testing.B) {
	t := 0
	for j := 0; j < b.N; j++ {
		t = Addition(t, j)
	}
}

func BenchmarkAllocate(b *testing.B) {
	for j := 0; j < b.N; j++ {
		p := Allocate()
		Free(p)
	}
}

func BenchmarkAllocateFree(b *testing.B) {
	for j := 0; j < b.N; j++ {
		AllocateFree()
	}
}
func BenchmarkAllocateAuto(b *testing.B) {
	for j := 0; j < b.N; j++ {
		AllocateAuto()
	}
}

func BenchmarkAdditionGo(b *testing.B) {
	t := 0
	for j := 0; j < b.N; j++ {
		t += j
	}
}
