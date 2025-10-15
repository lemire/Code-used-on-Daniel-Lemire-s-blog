package main

import (
	"math/rand/v2"
	"testing"
)

// BenchmarkChaCha measures the performance of generating uint64 values using ChaCha8.
func BenchmarkChaCha(b *testing.B) {
	r := rand.NewChaCha8([32]byte{})
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_ = r.Uint64()
	}
}

// BenchmarkPCG measures the performance of generating uint64 values using PCG.
func BenchmarkPCG(b *testing.B) {
	r := rand.NewPCG(0, 0)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_ = r.Uint64()
	}
}
