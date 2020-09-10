
If you have a recent version of CMake  under linux, macOS or freeBSD,  you can simply
go in the directory and type the following commands:

```
cmake -B build .
cmake --build build
./build/benchmark 
```

Under Windows, the process is similar:

```
cmake -B build .
cmake --build build --config Release
./build/Release/benchmark.exe
```