package main

import (
	"fmt"
	"os"
	"plugin"
)

func Square(x int) int {
	return x * x
}
func Power(x int, n int) int {
	product := 1
	for i := 0; i < n; i++ {
		product *= x
	}
	return product
}
func main() {
	plug, err := plugin.Open("custom.so")
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	fastSquare, err := plug.Lookup("FastSquare")
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	var FastFunction func(int) int
	FastFunction, ok := fastSquare.(func(int) int)
	if !ok {
		fmt.Println("unexpected type from module symbol")
		os.Exit(1)
	}

	fmt.Println(FastFunction(3))

}
