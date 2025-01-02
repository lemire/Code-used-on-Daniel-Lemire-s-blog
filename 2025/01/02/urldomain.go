package main

import (
    "fmt"
    "net/url"
)

func main() {
    urlString := "https://microsoft.coⅯ"

    // Parse the URL
    parsedURL, err := url.Parse(urlString)
    if err != nil {
        fmt.Println("URL could not be parsed:", err)
        return
    }

    // Get the host part which is the domain
    host := parsedURL.Host

    // Convert host to UTF-32
    utf32Bytes := make([]byte, 0)
    for _, r := range host {
        // Convert each rune to its 32-bit representation
        utf32Bytes = append(utf32Bytes, byte(r>>24), byte(r>>16), byte(r>>8), byte(r))
    }

    // Convert to hexadecimal for display
    var hexUtf32 string
    for _, b := range utf32Bytes {
        hexUtf32 += fmt.Sprintf("%02x", b)
    }

    fmt.Println("UTF-32 encoding of the domain:", hexUtf32)
    fmt.Println("Note: UTF-32 encoding results in 4 bytes per character, so each character will appear as 8 hexadecimal digits.")
}
