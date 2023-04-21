package inter

type IntIterable interface {
	HasNext() bool
	Next() uint32
	Reset()
}

func Count(i IntIterable) (count int) {
	count = 0
	i.Reset()
	for i.HasNext() {
		i.Next()
		count++
	}
	return
}

type IntArray struct {
	array []uint32
	index int
}

func (a *IntArray) HasNext() bool {
	return a.index < len(a.array)
}

func (a *IntArray) Next() uint32 {
	a.index++
	return a.array[a.index-1]
}

func (a *IntArray) Reset() {
	a.index = 0
}

func (a *IntArray) Count() (count int) {
	count = 0
	a.Reset()
	for a.HasNext() {
		a.Next()
		count++
	}
	return
}

func CountExplicit(i IntIterable) (count int) {
	switch c := i.(type) {
	case *IntArray:
		return c.Count()
	default:
		return Count(c)
	}
}

func CountArray(a *IntArray) (count int) {
	count = 0
	a.Reset()
	for a.HasNext() {
		a.Next()
		count++
	}
	return
}

func CountArrayFast(a *IntArray) int {
	return len(a.array)
}
