package main

import (
	"fmt"
	"math/rand"
)

func main() {
	counter := 0
	for n := 0; n < 1000000000; n++ {
		if rand.Int63() == 0 {
			counter += 1
		}
	}
	fmt.Printf("number of zeros: %d \n", counter)
}
