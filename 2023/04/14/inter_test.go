package inter

import (
	"testing"
)

var array IntArray
var count = 0

func populate() {
	array = IntArray{make([]uint32, 1000000), 0}
}

func BenchmarkCount(b *testing.B) {
	populate()
	b.ResetTimer()
	for n := 0; n < b.N; n++ {
		count += Count(&array)
	}
}

func BenchmarkCountExplicit(b *testing.B) {
	populate()
	b.ResetTimer()
	for n := 0; n < b.N; n++ {
		count += CountExplicit(&array)
	}
}

func BenchmarkCountArray(b *testing.B) {
	populate()
	b.ResetTimer()
	for n := 0; n < b.N; n++ {
		count += CountArray(&array)
	}
}

func BenchmarkCountArrayFast(b *testing.B) {
	populate()
	b.ResetTimer()
	for n := 0; n < b.N; n++ {
		count += CountArrayFast(&array)
	}
}
