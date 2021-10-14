package main

import (
	"fmt"
	"os"
	"os/exec"
	"plugin"
	"testing"
)

var sum int = 0

var Count int = 1000000
var myfunction func(int) int

func BenchmarkSquare(b *testing.B) {
	b.ResetTimer()
	for n := 0; n < b.N; n++ {
		for i := 0; i < Count; i++ {
			sum += Square(i)
		}
	}
}

func BenchmarkDynamicSquare(b *testing.B) {
	myfunction = func(x int) int {
		return Power(x, 2)
	}
	b.ResetTimer()
	for n := 0; n < b.N; n++ {
		for i := 0; i < Count; i++ {
			sum += myfunction(i)
		}
	}
}

func BenchmarkLocalPluginSquare(b *testing.B) {
	file, err := os.Create("fast.go")
	if err != nil {
		fmt.Println("Unable to open file")
		os.Exit(1)
	}
	_, err = file.WriteString("package main\nfunc FastFunction(x int) int {\n  return x")
	if err != nil {
		fmt.Println("Unable to write data")
		os.Exit(1)
	}
	powers := 2
	for i := 1; i < powers; i++ {
		_, err = file.WriteString(" * x")
		if err != nil {
			fmt.Println("Unable to write data")
			os.Exit(1)
		}
	}
	_, err = file.WriteString("\n}\n")

	if err != nil {
		fmt.Println("Unable to write data")
		os.Exit(1)
	}
	file.Close()
	_, errc := exec.Command("go", "build", "-buildmode=plugin", "-o", "fast.so", "fast.go").Output()
	if errc != nil {
		fmt.Println(errc)
		os.Exit(1)
	}

	plug, err := plugin.Open("fast.so")
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	fastSquare, err := plug.Lookup("FastFunction")
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	loaded, ok := fastSquare.(func(int) int)
	if !ok {
		fmt.Println("unexpected type from module symbol")
		os.Exit(1)
	}

	b.ResetTimer()
	for n := 0; n < b.N; n++ {
		for i := 0; i < Count; i++ {
			sum += loaded(i)
		}
	}
}

func BenchmarkLocalPluginSquareSimple(b *testing.B) {
	file, _ := os.Create("fast.go")
	file.WriteString("package main\nfunc FastFunction(x int) int {\n  return x")
	powers := 2
	for i := 1; i < powers; i++ {
		file.WriteString(" * x")
	}
	file.WriteString("\n}\n")
	file.Close()
	exec.Command("go", "build", "-buildmode=plugin", "-o", "fast.so", "fast.go").Output()
	plug, _ := plugin.Open("fast.so")
	fastSquare, _ := plug.Lookup("FastFunction")
	loaded, _ := fastSquare.(func(int) int)

	b.ResetTimer()
	for n := 0; n < b.N; n++ {
		for i := 0; i < Count; i++ {
			sum += loaded(i)
		}
	}
}
