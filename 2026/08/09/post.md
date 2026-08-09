# Profile-guided optimization in Go

When a compiler optimizes your program, it has to guess. Which functions are worth inlining? Which side of a branch is the common one? Which method does this interface call actually reach? At compile time it cannot know, so it uses heuristics. Profile-guided optimization (PGO) replaces the guessing with measurement: you run your program, record where it spends its time, and hand that recording back to the compiler for a second build.

PGO is a common feature of compiler systems. Google applied PGO to Chrome under Windows in 2016, [reporting gains of up to 15%](https://blog.chromium.org/2016/10/making-chrome-on-windows-faster-with-pgo.html). I expect [all mainstream Web browsers to be built with PGO](https://webkit.org/blog/15249/).


Go has supported PGO since version 1.20. You collect a profile, and pass it to the compiler. 

A CPU profile is a statistical record of where a program spends its time. While the program runs, the Go runtime interrupts it about a hundred times a second and writes down the call stack at that instant. After a few seconds you have thousands of such samples, and counting them tells you which functions were executing and who called them. In Go you produce one by wrapping the work you care about:

```go
f, _ := os.Create("cpu.pprof")
pprof.StartCPUProfile(f)   // from runtime/pprof
defer pprof.StopCPUProfile()
```

The compiler reads the call-stack counts and uses them for two things above all: inlining call sites that turn out to be hot, and devirtualizing interface calls whose target is nearly always the same concrete type.

I took three JSON documents that I wanted to parse:

- `twitter.json` (632 kB), a nest of small objects with short string keys
- `canada.json` (2.25 MB), essentially one enormous array of floating-point coordinates
- `citm_catalog.json` (1.73 MB), deeply nested objects with numeric keys

I parse each of them with the standard library's `encoding/json` into an `interface{}`. The baseline, with no profile, parses at 112 MB/s for `twitter.json`, 74 MB/s for `canada.json` and 116 MB/s for `citm_catalog.json`.


The procedure is three commands:

```sh
go build -o bench .                        # ordinary build
./bench -profile cpu.pprof -train twitter.json   # collect a CPU profile
go build -pgo=cpu.pprof -o bench_pgo .     # build again, with the profile
```

I did it three times, profiling each document on its own, and then measured all three documents against each of the three builds.

![Go PGO on encoding/json](go-pgo.png)

The gains are modest. The best result is `canada.json` at +4.7%, and most differences are in the 2–3% range. Profiling one document usually helps the others, but not reliably. Profiling `twitter.json` gave a decent improvement everywhere: +3.1%, +2.0%, +2.8%. But profiling `canada.json` bought 4.7% on `canada.json` and essentially nothing anywhere else. Interestingly, profiling `citm_catalog.json` produced a mere +0.8% on its own document while helping `twitter.json` more.

A 3% speedup is not exciting in isolation, but it may come nearly for free. Observe how you may get slightly negative results for cases you did not train for. That's expected generally, but the effect is modest in the case of Go because its optimizations are themselves modest in the first pace. That is, you are not getting a much an effect, but the process is less likely to backfire for other workloads.

[The code is available.](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/08/09)
