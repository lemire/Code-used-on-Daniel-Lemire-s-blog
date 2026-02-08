
Under Linux and macOS, you may run:

```
cmake -B build
cmake --build build
./build/benchmark
```

## Examining Assembly Code

To examine the assembly code of the sum functions (`sum_with_add_func`, `sum_with_inline_add_func`, `sum_with_inline_add_no_vec_func`) directly:


### Using gdb (batch mode, requires gdb installed)

Compile: `cmake -B build && cmake --build build`

Then: `gdb -batch ./build/benchmark -ex "disassemble sum_with_add_func"`

Repeat for other functions by replacing the function name.

### Using lldb (batch mode, requires lldb installed)

Compile: `cmake -B build && cmake --build build`

Then: `lldb -b -o "disassemble -n sum_with_add_func" ./build/benchmark`

Repeat for other functions by replacing the function name.


