// Fixed harness for the Go roaring commit history. Built against each commit
// through a go.mod replace directive. Loads the CRoaring realdata sets and
// reports, per operation, the best-of-N time in microseconds per dataset pass.
package main

import (
	"bufio"
	"bytes"
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strconv"
	"strings"
	"time"

	"github.com/RoaringBitmap/roaring/v2"
)

const reps = 5
const queriesPerBitmap = 2000

func loadDataset(dir string) [][]uint32 {
	entries, err := os.ReadDir(dir)
	if err != nil {
		panic(err)
	}
	var names []string
	for _, e := range entries {
		if strings.HasSuffix(e.Name(), ".txt") {
			names = append(names, e.Name())
		}
	}
	sort.Strings(names)
	var out [][]uint32
	for _, n := range names {
		f, err := os.Open(filepath.Join(dir, n))
		if err != nil {
			panic(err)
		}
		r := bufio.NewReaderSize(f, 1<<20)
		var vals []uint32
		for {
			tok, err := r.ReadString(',')
			tok = strings.TrimSpace(strings.TrimSuffix(tok, ","))
			if tok != "" {
				x, perr := strconv.ParseUint(tok, 10, 32)
				if perr == nil {
					vals = append(vals, uint32(x))
				}
			}
			if err != nil {
				break
			}
		}
		f.Close()
		out = append(out, vals)
	}
	return out
}

var rngState uint64 = 88172645463325252

func rng() uint64 {
	rngState ^= rngState << 13
	rngState ^= rngState >> 7
	rngState ^= rngState << 17
	return rngState
}

func timeIt(name, ds string, body func() uint64) {
	best := 1e30
	var sink uint64
	for rep := 0; rep < reps; rep++ {
		t0 := time.Now()
		sink = body()
		t := time.Since(t0).Seconds()
		if t < best {
			best = t
		}
	}
	fmt.Printf("%-28s %-18s %12.1f us  (sink %d)\n", name, ds, best*1e6, sink)
}

func main() {
	root := os.Args[1]
	datasets := []string{"census1881", "census-income", "weather_sept_85"}
	if len(os.Args) > 2 {
		datasets = os.Args[2:]
	}
	for _, ds := range datasets {
		arrs := loadDataset(filepath.Join(root, ds))
		nb := len(arrs)
		b := make([]*roaring.Bitmap, nb)
		for i := range arrs {
			b[i] = roaring.BitmapOf(arrs[i]...)
			b[i].RunOptimize()
		}
		q := make([]uint32, nb*queriesPerBitmap)
		for i := range arrs {
			var lo, hi uint32 = 0, 1
			if len(arrs[i]) > 0 {
				lo, hi = arrs[i][0], arrs[i][len(arrs[i])-1]
			}
			for k := 0; k < queriesPerBitmap; k++ {
				var v uint32
				if k&1 == 1 && len(arrs[i]) > 0 {
					v = arrs[i][rng()%uint64(len(arrs[i]))]
				} else {
					v = lo + uint32(rng()%uint64(hi-lo+1))
				}
				q[i*queriesPerBitmap+k] = v
			}
		}
		ser := make([][]byte, nb)
		var maxcard uint64
		for i := range b {
			var buf bytes.Buffer
			b[i].WriteTo(&buf)
			ser[i] = buf.Bytes()
			if c := b[i].GetCardinality(); c > maxcard {
				maxcard = c
			}
		}
		many := make([]uint32, 4096)

		timeIt("or_successive", ds, func() uint64 {
			var s uint64
			for i := 0; i+1 < nb; i++ {
				s += roaring.Or(b[i], b[i+1]).GetCardinality()
			}
			return s
		})
		timeIt("and_successive", ds, func() uint64 {
			var s uint64
			for i := 0; i+1 < nb; i++ {
				s += roaring.And(b[i], b[i+1]).GetCardinality()
			}
			return s
		})
		timeIt("and_cardinality", ds, func() uint64 {
			var s uint64
			for i := 0; i+1 < nb; i++ {
				s += b[i].AndCardinality(b[i+1])
			}
			return s
		})
		timeIt("fast_or", ds, func() uint64 {
			return roaring.FastOr(b...).GetCardinality()
		})
		timeIt("contains", ds, func() uint64 {
			var s uint64
			for i := 0; i < nb; i++ {
				for k := 0; k < queriesPerBitmap; k++ {
					if b[i].Contains(q[i*queriesPerBitmap+k]) {
						s++
					}
				}
			}
			return s
		})
		timeIt("iterate", ds, func() uint64 {
			var s uint64
			for i := 0; i < nb; i++ {
				it := b[i].Iterator()
				for it.HasNext() {
					s += uint64(it.Next())
				}
			}
			return s
		})
		timeIt("iterate_many", ds, func() uint64 {
			var s uint64
			for i := 0; i < nb; i++ {
				it := b[i].ManyIterator()
				for {
					n := it.NextMany(many)
					if n == 0 {
						break
					}
					s += uint64(many[n-1])
				}
			}
			return s
		})
		timeIt("to_array", ds, func() uint64 {
			var s uint64
			for i := 0; i < nb; i++ {
				a := b[i].ToArray()
				if len(a) > 0 {
					s += uint64(a[0])
				}
			}
			return s
		})
		timeIt("deserialize", ds, func() uint64 {
			var s uint64
			for i := 0; i < nb; i++ {
				r := roaring.New()
				if _, err := r.ReadFrom(bytes.NewReader(ser[i])); err != nil {
					panic(err)
				}
				s += r.GetCardinality()
			}
			return s
		})
	}
}
