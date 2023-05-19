package main

/*
#cgo CFLAGS: -O3  -std=c11
#cgo CXXFLAGS: -O3 -std=c++17
#include "cgotest.h"
*/
import "C"
import (
	"fmt"
	"runtime"
)

type Cstr struct {
	cpointer *C.char
}

func Addition(a, b int) int {
	return int(C.addition(C.int(a), C.int(b)))
}

func FreeCstr(c *Cstr) {
	C.free_allocated(c.cpointer)
	runtime.KeepAlive(c)
}

func AllocateFree() {
	C.allocate_free()
}
func AllocateAuto() *Cstr {
	answer := &Cstr{C.allocate()}
	runtime.SetFinalizer(answer, func(c *Cstr) { C.free_allocated(c.cpointer); runtime.KeepAlive(c) })
	return answer
}

func AllocateDummy() *Cstr {
	answer := &Cstr{C.allocate()}
	runtime.SetFinalizer(answer, func(c *Cstr) {})
	return answer
}

func Allocate() *C.char {
	return C.allocate()
}

func Free(c *C.char) {
	C.free_allocated(c)
}

func main() {
	fmt.Println(Addition(1, 2))
}
