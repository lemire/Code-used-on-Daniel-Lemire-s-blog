MIME base64 decoding, 1 MB decoded (76-character lines, CRLF), GB/s of input, median of 3 runs.

| rank | system | function | GB/s | runs |
|---:|---|---|---:|---|
| 1 | c++ simdutf | `base64_to_binary` | 18.37 | 18.37 18.23 18.43 |
| 2 | php 8.7.0-dev (PR #25476) | `base64_decode(str)` | 17.12 | 17.12 17.11 17.12 |
| 3 | python 3.15.2 | `fastpybase64 0.1.4 (PR #5) (icelake) b64decode` | 16.44 | 16.52 16.44 16.43 |
| 4 | bun 1.4.4 | `Buffer.from(str,'base64')` | 14.15 | 14.09 14.15 14.86 |
| 5 | node v26.13.1 | `Buffer.from(str,'base64')` | 13.26 | 13.74 13.26 13.12 |
| 6 | bun 1.4.4 | `Uint8Array.fromBase64(str)` | 9.39 | 9.30 9.39 9.79 |
| 7 | deno 2.9.8 | `Uint8Array.fromBase64(str)` | 8.37 | 8.38 8.35 8.37 |
| 8 | node v26.13.1 | `Uint8Array.fromBase64(str)` | 8.10 | 8.26 8.04 8.10 |
| 9 | deno 2.9.8 | `Buffer.from(str,'base64')` | 6.33 | 6.40 6.33 6.01 |
| 10 | .NET 11.0.0 | `Base64.DecodeFromUtf8(bytes)` | 4.91 | 4.91 4.94 4.91 |
| 11 | .NET 11.0.0 | `Convert.FromBase64String(string)` | 2.15 | 2.18 2.15 2.04 |
| 12 | go go1.27.4 | `base64.StdEncoding.Decode (preallocated)` | 1.63 | 1.63 1.63 1.63 |
| 13 | go go1.27.4 | `base64.StdEncoding.DecodeString` | 1.54 | 1.54 1.54 1.54 |
| 14 | java 27.0.1.1+4 | `Base64.decode(byte[])` | 1.20 | 1.21 1.20 1.20 |
| 15 | zig 0.17.1 | `std.base64 decoderWithIgnore("\r\n")` | 0.85 | 0.84 0.86 0.85 |
| 16 | php 8.5.12 | `base64_decode(str)` | 0.61 | 0.61 0.61 0.61 |
