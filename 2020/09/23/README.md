
If you have a recent version of CMake  under linux, macOS or freeBSD,  you can simply
go in the directory and type the following commands:

```
cmake -B build .
cmake --build build
./build/benchmark 
```

It is interesting to run this benchmark using different compilers:

```
cmake -B build_clang -DCMAKE_CXX_COMPILER=clang++-9 . && cmake --build build_clang && ./build_clang/benchmark
cmake -B build_gcc -DCMAKE_CXX_COMPILER=g++ . && cmake --build build_gcc && ./build_gcc/benchmark
```
