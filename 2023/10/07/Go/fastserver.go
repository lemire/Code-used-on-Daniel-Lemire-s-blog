package main

import (
	"io"
	"log"

	"github.com/valyala/fasthttp"
)

func main() {

	h := requestHandler
	if err := fasthttp.ListenAndServe(":3000", h); err != nil {
		log.Fatalf("Error in ListenAndServe: %v", err)
	}
}

func requestHandler(ctx *fasthttp.RequestCtx) {
	io.WriteString(ctx, "Hello World")
}
