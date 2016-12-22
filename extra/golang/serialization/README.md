## Comparing serialization speed in Go


http://stackoverflow.com/questions/37405162/serialization-performance-in-golang-json-gob-xml

```
$ go run serial.go
Array size:  100
trial  1
2016/12/22 09:55:54 Serialization by JSON elapsed: 637 us
2016/12/22 09:55:54 serialized size in bytes:  18781
2016/12/22 09:55:54 Serialization by GOB elapsed: 324 us
2016/12/22 09:55:54 serialized size in bytes:  9066
2016/12/22 09:55:54 Serialization by XML elapsed: 2438 us
2016/12/22 09:55:54 serialized size in bytes:  25780

trial  2
2016/12/22 09:55:54 Serialization by JSON elapsed: 602 us
2016/12/22 09:55:54 serialized size in bytes:  18781
2016/12/22 09:55:54 Serialization by GOB elapsed: 238 us
2016/12/22 09:55:54 serialized size in bytes:  9066
2016/12/22 09:55:54 Serialization by XML elapsed: 2397 us
2016/12/22 09:55:54 serialized size in bytes:  25780

trial  3
2016/12/22 09:55:54 Serialization by JSON elapsed: 599 us
2016/12/22 09:55:54 serialized size in bytes:  18781
2016/12/22 09:55:54 Serialization by GOB elapsed: 230 us
2016/12/22 09:55:54 serialized size in bytes:  9066
2016/12/22 09:55:54 Serialization by XML elapsed: 2404 us
2016/12/22 09:55:54 serialized size in bytes:  25780
```
