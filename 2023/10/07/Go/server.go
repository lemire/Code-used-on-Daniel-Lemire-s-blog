package main

import (
    "fmt"
    "log"
    "net/http"
)

func main() {
    http.HandleFunc("/simple", func(w http.ResponseWriter, r *http.Request){
        fmt.Fprintf(w, "Hello!")
    })
    fmt.Printf("Starting server at port 3000\n")
    if err := http.ListenAndServe(":3000", nil); err != nil {
        log.Fatal(err)
    }
}