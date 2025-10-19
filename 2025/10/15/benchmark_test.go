package main

import (
	crand "crypto/rand"
	"encoding/binary"
	randv1 "math/rand"
	"math/rand/v2"
	"testing"
)

// BenchmarkChaCha measures the performance of generating uint64 values using ChaCha8.
func BenchmarkChaCha(b *testing.B) {
	r := rand.Ne

	ChaCha8([32]byte{})
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

// BenchmarkCryptoRand measures the performance of generating uint64 values using crypto/rand.
func BenchmarkCryptoRand(b *testing.B) {
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		var buf [8]byte
		_, _ = crand.Read(buf[:])
		_ = binary.BigEndian.Uint64(buf[:])
	}
}

// BenchmarkRandV1 measures the performance of generating uint64 values using math/rand (v1).
func BenchmarkRandV1(b *testing.B) {
	r := randv1.New(randv1.NewSource(0))
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_ = r.Uint64()
	}
}
