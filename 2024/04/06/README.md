```
conan profile detect --force
conan install . --output-folder=build --build=missing
cmake -B build -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

```
./build/server
```


```
go install github.com/codesenberg/bombardier@latest
bombardier -c 1000 http://localhost:18080/simple
```
