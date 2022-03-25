package alloc

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"fmt"
	"testing"
)
// go test -count 10 -bench=. -benchmem
var data []uint64
var buf *bytes.Buffer = new(bytes.Buffer)

var table = []struct {
	input int
}{
	{input: 100000000},
}

func BenchmarkWrite(b *testing.B) {
	for _, v := range table {
		data = make([]uint64, v.input)
		for i := 0; i < len(data); i++ {
			data[i] = uint64(i)
		}
		b.Run(fmt.Sprintf("input_size_%d", v.input), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				err := binary.Write(buf, binary.LittleEndian, data)
				if err != nil {
					fmt.Println("binary.Write failed:", err)
				}
				buf.Reset()
			}
		})
		
	}
}

func BenchmarkWriteOneByOne(b *testing.B) {
	for _, v := range table {
		data = make([]uint64, v.input)
		for i := 0; i < len(data); i++ {
			data[i] = uint64(i)
		}
		b.Run(fmt.Sprintf("input_size_%d", v.input), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				var item = make([]byte, 8)
				for _, x := range data {
					binary.LittleEndian.PutUint64(item, x)
					_, err := buf.Write(item)
					if err != nil {
						fmt.Println("binary.Write failed:", err)
						break
					}
				}
				buf.Reset()
			}
		})
		
	}
}

func BenchmarkWriteBufChunk(b *testing.B) {
	for _, v := range table {
		data = make([]uint64, v.input)
		for i := 0; i < len(data); i++ {
			data[i] = uint64(i)
		}
		b.Run(fmt.Sprintf("input_size_%d", v.input), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				writer := bufio.NewWriter(buf)
				var item = make([]byte, 8)
				for _, x := range data {
					binary.LittleEndian.PutUint64(item, x)
					_, err := writer.Write(item)
					if err != nil {
						fmt.Println("binary.Write failed:", err)
						break
					}
				}
				writer.Flush()
				buf.Reset()
			}
		})
		
	}
}
