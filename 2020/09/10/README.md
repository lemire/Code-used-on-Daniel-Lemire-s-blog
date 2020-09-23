
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

## Sample result

```
$ cmake -B build .
-- Building for: Visual Studio 16 2019
-- Selecting Windows SDK version 10.0.18362.0 to target Windows 10.0.18363.
-- The CXX compiler identification is MSVC 19.26.28806.0
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.26.28801/bin/Hostx64/x64/cl.exe
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.26.28801/bin/Hostx64/x64/cl.exe - works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- No build type selected, default to Release
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/User/github/Code-used-on-Daniel-Lemire-s-blog/2020/09/10/build

$ cmake --build build --config Release
Microsoft (R) Build Engine version 16.6.0+5ff7b0c9e for .NET Framework
Copyright (C) Microsoft Corporation. All rights reserved.

  Checking Build System
  Building Custom Rule C:/Users/User/github/Code-used-on-Daniel-Lemire-s-blog/2020/09/10/CMakeLists.txt
  benchmark.cpp
  benchmark.vcxproj -> C:\Users\User\github\Code-used-on-Daniel-Lemire-s-blog\2020\09\10\build\Release\benchmark.exe
  Building Custom Rule C:/Users/User/github/Code-used-on-Daniel-Lemire-s-blog/2020/09/10/CMakeLists.txt

$ ./build/Release/benchmark.exe
# parsing random integers in the range [0,1)
volume = 2.09808 MB
strtod                                  :   118.69 MB/s (+/- 2.3 %)
from_chars                              :   142.25 MB/s (+/- 2.4 %)
```