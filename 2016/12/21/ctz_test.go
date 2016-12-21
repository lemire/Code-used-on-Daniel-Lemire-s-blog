package ctz

/*
Portions Copyright (c) 2009 The Go Authors. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

   * Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.
   * Neither the name of Google Inc. nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import (
	"encoding/binary"
	"fmt"
	"math/rand"
	"testing"
)

//go test -bench=.

const noasm_deBruijn64 = 0x03f79d71b4ca8b09

var noasm_deBruijn64Lookup = []byte{
	0, 1, 56, 2, 57, 49, 28, 3, 61, 58, 42, 50, 38, 29, 17, 4,
	62, 47, 59, 36, 45, 43, 51, 22, 53, 39, 33, 30, 24, 18, 12, 5,
	63, 55, 48, 27, 60, 41, 37, 16, 46, 35, 44, 21, 52, 32, 23, 11,
	54, 26, 40, 15, 34, 20, 31, 10, 25, 14, 19, 9, 13, 8, 7, 6,
}

// reference: http://supertech.csail.mit.edu/papers/debruijn.pdf
// Leiserson and Prokop, Using de Bruijn Sequences to Index a 1 in a Computer Word, 1998)
//
// trailingZeroBits returns the number of consecutive least significant zero
// bits of x. If x is zero, the result is 0.
func noasm_countTrailingZeros(x uint64) int {
	// x & -x leaves only the right-most bit set in the word. Let k be the
	// index of that bit. Since only a single bit is set, the value is two
	// to the power of k. Multiplying by a power of two is equivalent to
	// left shifting, in this case by k bits. The de Bruijn constant is
	// such that all six bit, consecutive substrings are distinct.
	// Therefore, if we have a left shifted version of this constant we can
	// find by how many bits it was shifted by looking at which six bit
	// substring ended up at the top of the word.
	// (Knuth, volume 4, section 7.3.1)
	return int(noasm_deBruijn64Lookup[((x&-x)*(noasm_deBruijn64))>>58])
}

func sumoftz(input []uint64) int {
	sum := 0
	for i := range input {
		sum += noasm_countTrailingZeros(input[i])
	}
	return sum
}

func getRandomUint64Set(n int) []uint64 {
	seed := int64(42)
	rand.Seed(seed)

	var buf [8]byte
	var o []uint64
	for len(o) < n {
		rand.Read(buf[:])
		x := binary.LittleEndian.Uint64(buf[:])
		if x != 0 {
			o = append(o, binary.LittleEndian.Uint64(buf[:]))
		}
	}
	return o
}

func Benchmark100CountTrailingZerosDeBruijn(b *testing.B) {
	b.StopTimer()

	r := getRandomUint64Set(1000)
	expected := sumoftz(r) * b.N

	b.ResetTimer()
	b.StartTimer()
	sum := 0
	for i := 0; i < b.N; i++ {
		for i := range r {
			sum += noasm_countTrailingZeros(r[i])
		}
	}
	if sum != expected {
		fmt.Println("bug", sum, expected)
		panic("bug")
	}

}

func Benchmark100CountTrailingZerosAsm(b *testing.B) {
	b.StopTimer()

	r := getRandomUint64Set(1000)
	expected := sumoftz(r) * b.N

	b.ResetTimer()
	b.StartTimer()
	sum := 0
	for i := 0; i < b.N; i++ {
		for i := range r {
			sum += countTrailingZeros(r[i])
		}
	}
	if sum != expected {
		fmt.Println("bug", sum, expected)
		panic("bug")
	}
}
