# Using Flame Graphs in Go

The hardest problem in software performance is often to understand your code and why it might be slow. One 
approach to this problem is called profiling. Profiling tries to count the time spent in the various functions
of your program.

It can be difficult to understand the result of profiling. Furthermore, it is more complicated than it seems.
For example, how do you could the time spent in a function A that calls another function B. Does the time spent in function B count for the time spent in function A? Suppose function A executes code for 2 seconds, then calls function B which takes 3 seconds to run, and finally A continues with 1 additional second. The total time for A would be 6 seconds if everything were included, but that does not accurately reflect where the time is truly spent. We might prefer the exclusive or flat time: the time spent only in the body of the function itself, without counting calls to other functions. In our example, for A, that would be 2 + 1 = 3 seconds (the time before and after the call to B). For B, it would be 3 seconds. The cumulative time is the total time spent in the function and all the functions it calls, recursively. For A, that would include the 3 own seconds plus the 3 seconds of B, totaling 6 seconds. For B, it would just be its 3 seconds, unless it calls other functions. 

Further, profiling  is typically statistical. If your program is short-lived or has uneven load, it might provide misleading data. Thus you should always interpret the results with care.


Go has bultin support for profiling. It requires two steps.

1. Use `runtime/pprof` to collect profiles.
2. Use `go tool pprof` to review the data.


Let us consider a concrete example. We have a benchmark function parsing repeatedly strings as floating-point numbers.

```go
func BenchmarkParseFloat(b *testing.B) {
	for i := 0; i < b.N; i++ {
		idx := i % len(floatStrings)
		_, _ = strconv.ParseFloat(floatStrings[idx], 64)
	}
}
```

In effect, we want to profile the `strconv.ParseFloat` function to see where it is spending its time. We run:

```
go test -bench=. -cpuprofile=cpu.prof    
```

Notice the `-cpuprofile=cpu.prof` flag which instructs go to dump its profiling data to the file `cpu.prof`. Go profiles the code using a sampling-based approach, where it periodically interrupts the execution to capture stack traces of running goroutines. It has low overhead—only a small fraction of execution time is spent on sampling—and it works across multiple goroutines and threads.

We can call `go tool pprof` on the `cpu.prof` to examine its result. The simplest flag is `-text`, it prints out a summary in the console.

```
go tool pprof -text cpu.prof
```

You might the the following output.

```
Time: 2025-10-26 14:35:03 EDT
Duration: 1.31s, Total samples = 990ms (75.45%)
Showing nodes accounting for 990ms, 100% of 990ms total
      flat  flat%   sum%        cum   cum%
     830ms 83.84% 83.84%      830ms 83.84%  strconv.readFloat
      20ms  2.02% 92.93%      900ms 90.91%  strconv.atof64
      20ms  2.02% 94.95%       20ms  2.02%  strconv.eiselLemire64
      20ms  2.02% 96.97%       20ms  2.02%  strconv.special
      10ms  1.01% 98.99%      910ms 91.92%  strconv.ParseFloat
      10ms  1.01%   100%       10ms  1.01%  strconv.atof64exact
```

It can be difficult to interpret this result. Thus we want to visualize the result. My favorite technique is the flame graph. It is a useful technique invented by Brendan Gregg.

A flamegraph is a stacked bar chart where each bar represents a function call, and the width of the bar indicates the estimated time spent in that function. The time can be replaced by other metrics such as CPU instructions, memory
allocations or any other metrics that can be matched to a software function. The y-axis represents the call stack depth, with the top being the root of the stack. Colors can be used to make it more elegant.

The name flame graph comes from the shape that often resembles flames, with wider bases indicating more time-consuming functions and narrower tops showing less frequent paths. There are many variations such as flame charts where the x-axis represents time progression, and y-axis shows call stack at each moment. But flame graphs are more common.

Given that you have generated the file `cpu.prof`, you can visualize it by first starting a web server:

```
go tool pprof -http=:8123 cpu.prof
```

It assumes that port 8123 is free on your system: you can change the port 8123 if needed. Open your browser at the URL `http://localhost:8123/ui/flamegraph`.

You should see a graph:


Our first step is to narrow our focus on the function we care about. For more clarify, double click on `strconv.ParseFloat`, or right click on it and select focus. The bulk of the time (over 80%) of the `ParseFloat` is spent in the `readFloat` function. This function converts the string into a mantissa and an exponent. There are other functions being called such as `strconv.special`, `strconv.atof64exact`, and `strconv.eiselLemire64`. However, they account for little time in this specific benchmark.

Thus, if we wanted to improve the performance of `strconv.ParseFloat` function, we might want to focus on optimizing the `ParseFloat` function.
