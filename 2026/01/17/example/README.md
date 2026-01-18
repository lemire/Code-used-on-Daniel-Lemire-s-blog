# Foreign Function Interface Example

This project demonstrates how to use Java's Foreign Function & Memory API (introduced in Java 21) to call native C code from Java and pass functions between them.

## Overview

The example consists of:
- A Java class (`IntArrayExample`) that loads a native library and calls a C function.
- A C function (`sum_array`) that computes the sum of an array of integers.

## Prerequisites

- Java 22 or later
- Maven 3.6+
- A C compiler (e.g., clang on macOS/Linux, gcc on Linux)

## Building the Project

1. Clone or navigate to the project directory.

2. Build the project using Maven:
   ```
   mvn clean compile
   ```
   This will:
   - Compile the C code into a shared library (`libsum.dylib` on macOS).
   - Compile the Java code.

## Running the Example

Run the Java application:
```
java -Djava.library.path=target -cp target/classes IntArrayExample
```

Expected output:
```
The sum from C is: 150
```

This is the sum of the array `{10, 20, 30, 40, 50}`.

## Notes

- On Linux, the shared library will be `libsum.so`.
- On Windows, it would be `sum.dll` (adjust the compilation accordingly).
- The example uses `--enable-native-access=ALL-UNNAMED` to allow native access (may be required in future JDK versions).

## Code Explanation

- **Java Side**: Uses `Linker.nativeLinker()` to create downcall handles for calling C functions.
- **C Side**: Implements a simple sum function that sums the elements of an integer array.

This example showcases bidirectional FFI capabilities in modern Java.