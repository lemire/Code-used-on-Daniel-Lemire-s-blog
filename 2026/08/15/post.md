# Go 1.27 will make some allocations cheaper

Like most programming languages, Go has both stack allocations, whose lifetime is limited to the current function, and dynamic (or heap) allocations.

The name stack comes from the fact that the memory management is somewhat trivial. There is typically one stack per thread (or goroutine in Go). When a function needs memory, it simply appends data to the stack. When the function returns, the memory is dropped from the end of the stack. So the memory last allocated is deallocated first.

Heap memory is potentially considerably more complex. For one thing, it is meant to be accessible by several threads (or goroutines). An object can be allocated by one function and later reclaimed after an entirely different function, possibly running on a different thread (or goroutine), has dropped the last reference to it. Unlike the stack, there is no prescribed order for allocating and reclaiming heap memory. In Go, the garbage collector does the reclaiming.

Typically, stack allocations have a size known at compile time. Many systems give each thread a fixed-size stack, although Go grows goroutine stacks as needed.

There are many ways in Go to do a heap allocation. A common one is when you allocate a slice, as in this instance where you allocate memory for 100 integers:

```go
x := make([]int, 100)
```

If the slice `x` is not entirely local to a function, Go will typically just allocate it on the heap. It will do so similarly when a function returns a pointer. For example, in the following instance, I assign the value 1 to a local integer variable, but I return a pointer to it.

```go
func f() *int {
  x := 1
  return &x
}
```

In C/C++, this would be quite bad. You should get a warning such as `address of local variable 'x' returned`. In Go, the variable `x` will typically get allocated on the heap.

In many Go programs, we end up doing a lot of heap allocations of small objects. It can become a bottleneck in some cases. Think about when you are maintaining a tree or a linked list where each value (node) is an object that must live on the heap. If the data structure is highly dynamic, you will be constantly allocating these small objects.

Memory allocation on the heap is usually not done at arbitrary sizes. You often cannot get exactly, say, 13 bytes. In Go, small allocations are rounded up to a size class: 8 bytes, 16 bytes, 24 bytes, 32 bytes, and so forth. There is also some overhead to each heap allocation, from rounding and from allocator metadata.

The compiler knows the size of the object, but prior to Go 1.27, Go would call a generic function when doing a heap allocation. This generic function would then look up the size class and take the corresponding path. Starting with 1.27, for small objects (under 80 bytes), Go relies on dedicated functions.

It is easy to benchmark in Go. A basic benchmark might look as follows.

```go
type Node struct {
	value int64
	next  *Node
}

var sink any

func BenchmarkAllocNode16(b *testing.B) {
	for b.Loop() {
		sink = &Node{}
	}
}
```

On my MacBook, the results are quite telling. Go 1.27 is nearly twice as fast!

| allocation | Go 1.26 | Go 1.27 | speedup |
|---|---|---|---|
| 16 B, has pointer | 9.5 ns | 5.5 ns  | 1.8x |


This will not help all software, just the components that do many small allocations.

*The code is [available](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/08/15).*
