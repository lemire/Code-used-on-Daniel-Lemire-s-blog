# Go GC: measuring pauses and CPU load across Go 1.25, 1.26 and 1.27

Follow-up to [How bad can Python stop-the-world pauses get?](https://lemire.me/blog/2026/02/15/how-bad-can-python-stop-the-world-pauses-get/),
which reported a 50 ms maximum delay for Go 1.25 on an Apple M4.

Go 1.26 ships a new garbage collector by default ("Green Tea"), so the question is
whether the newer Go releases do better on that benchmark. I measured both the
**stop-the-world pause distribution** and the **GC CPU load**, which turn out to be
very different quantities.

**Summary of what I found:**

1. The Go benchmark in the original post does not retain the 50-million-node list.
   One missing `runtime.KeepAlive` means Go was collecting it immediately, so the Go
   program ran with a 4 MB heap while the Python program held ~50 million live objects.
   The Go and Python numbers were not measuring the same workload. **This is a fairness
   bug, but fixing it does not change the Go 1.25 result** (0.46 ms fixed vs 1.06 ms
   buggy) — Go 1.25 absorbs a 1.9 GB live heap without a latency hit.
2. Once the list is genuinely retained, the new Green Tea collector **does not reduce**
   GC CPU or stop-the-world pauses on this benchmark — it costs about 7–8% *more* GC CPU
   and introduces a reproducible ~13 ms stall that Go 1.25 did not have.
3. Go 1.27 is ~21% faster on this benchmark, but that is entirely its new
   size-specialized allocator, not the garbage collector. Go 1.27's GC code is
   essentially identical to Go 1.26's.
4. That ~13 ms stall is a **real pause of the measured goroutine** — but it is not a
   *stop-the-world* pause. Go's true STW pauses stayed under 42 µs in every
   configuration. The distinction matters for the Python comparison: Python's pause
   freezes the whole interpreter, Go's stalls one goroutine while others keep running.

---

## 1. Setup

| | |
|---|---|
| Machine | Apple M4 Max, 14 cores, 36 GB RAM |
| OS | macOS 15.7.7 (24G720), darwin/arm64 |
| Go versions | 1.25.6, 1.26.6 (latest stable), 1.27rc3 (latest overall) |
| Runs | 5 per configuration, run sequentially |
| Workload | unchanged: one 50,000,000-node list, then 1,000,000 lists of 1,000 nodes |

At the time of writing, `https://go.dev/dl/?mode=json&include=all` lists **go1.26.6** as
the newest stable release and **go1.27rc3** as the newest release overall, so those are
"the two latest versions". Go 1.25.6 is included as the baseline from the original post.

### Green Tea is the difference between 1.25 and 1.26

From `src/internal/buildcfg/exp.go` in each SDK:

| | Go 1.25 | Go 1.26 | Go 1.27rc3 |
|---|---|---|---|
| Green Tea default | off | **on** | **on** |
| Opt out with | — | `GOEXPERIMENT=nogreenteagc` | `GOEXPERIMENT=nogreenteagc` |

Because the opt-out still works in 1.26 and 1.27, I could run every Go version both with
and without Green Tea. That separates "the new GC" from unrelated release-to-release
changes, which matters a lot here (see §5).

The Go 1.26 release notes promise "somewhere between a 10–40% reduction in garbage
collection overhead in real-world programs that heavily use the garbage collector".

---

## 2. The benchmark does not keep the big list alive

This is the most important finding, and it applies to the original post.

The Go program has:

```go
objects1 := createLinkedList(50_000_000)
_ = objects1 // prevent optimization
```

The comment is optimistic. `_ = objects1` is a no-op: it does not extend the variable's
lifetime, and Go's liveness analysis treats `objects1` as dead from that point on. The
entire 50-million-node list becomes garbage before the measurement loop even starts.

Running the original program under `GODEBUG=gctrace=1` (Go 1.26.6):

```
gc 4674 @9.589s 0%: 0.017+0.060+0.013 ms clock, ... 3->3->0 MB, 4 MB goal, ... 14 P
```

Every single cycle reports `3->3->0 MB` with a **4 MB heap goal** and **0%** GC CPU, and
there are **4,674** of them. The live heap is zero. The benchmark is allocating and
immediately discarding 1,000-node lists into a 4 MB nursery — a workload with essentially
no GC pressure at all.

Adding a single line at the end of `main` fixes it:

```go
runtime.KeepAlive(objects1)
```

Same program, same Go version (1.26.6), one line different:

| | `_ = objects1` | `runtime.KeepAlive(objects1)` |
|---|---|---|
| GC cycles | 4,674 | **18** |
| live heap after GC | 0 MB | **~1,271 MB** |
| heap goal | 4 MB | **~2,549 MB** |
| GC CPU | 0% | **4%** |

The Python program, by contrast, binds the list to `x` at module scope, so it *is* live
for the whole loop. The two programs were doing different things, and the difference
flattered Go.

### But fixing it does not change the post's numbers

It would be easy to assume this bug invalidates the post's Go column. It does not. Taking
the original program and adding *only* `runtime.KeepAlive` — no other changes — 5 runs
each:

| Go version | `_ = objects1` (4 MB heap) | `runtime.KeepAlive` (1.9 GB heap) |
|---|---|---|
| **1.25.6** | 1.056 ms  [0.191–1.631] | **0.464 ms**  [0.350–1.780] |
| **1.26.6** | 0.807 ms  [0.350–0.935] | **13.111 ms** [12.107–25.951] |

On **Go 1.25 — the version used in the post — retaining the list makes no difference**,
and if anything the median improves. The collector does vastly more work (GC CPU 0% → 4.2%,
cycles 4,674 → 12) yet the mutator barely notices. Go 1.25's concurrent collector absorbs a
1.9 GB live heap essentially for free.

The 13 ms only appears on Go 1.26, and §3 shows it is caused by Green Tea, not by the heap
size. So the KeepAlive bug and the Green Tea regression are two **independent** findings;
it would be a mistake to attribute the 13 ms to the bug fix.

The practical consequence for the original post is therefore small: the Go program should be
corrected for fairness, and the comparison with Python is only meaningful once it is, but
the reported Go 1.25 figure and the post's conclusion both survive.

Everything below uses the corrected benchmark (`stress_gc.go` in this directory), which
retains the list and additionally reports the runtime's own GC metrics.

---

## 3. Results: GC pauses

Median of 5 runs. "Max delay" is what the original benchmark measures — the longest gap
between two consecutive loop iterations. "STW" columns come from the runtime's own
`/gc/pauses:seconds` histogram.

| config | Green Tea | max delay (ms) | range (ms) | STW mean (µs) | STW p99 (µs) | STW max (µs) |
|---|---|---|---|---|---|---|
| go1.25.6        | off | **0.47** | 0.38–0.94 | 12.2 | 24.6 | 32.8 |
| go1.26.6        | **on** | **12.99** | 12.33–14.92 | 15.6 | 28.7 | 41.0 |
| go1.27rc3       | **on** | **14.85** | 12.95–21.42 | 14.6 | 28.7 | 41.0 |
| go1.26.6 `nogreenteagc`  | off | **0.61** | 0.42–5.92 | 16.0 | 32.8 | 41.0 |
| go1.27rc3 `nogreenteagc` | off | **2.31** | 0.40–17.81 | 15.7 | 28.7 | 41.0 |

Two separate things are going on.

**Real stop-the-world pauses are tiny and unchanged.** Across every version and every
configuration, mean STW pause is 12–16 µs and the maximum is 20–164 µs. Green Tea does not
move them, and neither does the version bump. If "stop-the-world pause" is what you care
about, Go's answer on this workload is "a few tens of microseconds", in all five
configurations.

**But the delay the benchmark reports got ~20× worse with Green Tea.** On Go 1.26 the
separation is clean and reproducible across all 5 runs — 12.33–14.92 ms with Green Tea
versus 0.42–5.92 ms without it, with no overlap. Go 1.25, which has no Green Tea, sits at
0.38–0.94 ms.

### Is that 13 ms "a pause"?

Yes — it is a real pause of the goroutine being measured. For 13 ms that goroutine makes no
progress, and if it were serving a request, that request would be 13 ms slower. Nothing
about the mechanism makes the delay less real, and the benchmark is measuring a genuine
latency event, not an artifact.

What it is *not* is a **stop-the-world** pause. Since STW time is only ~41 µs, the other
~13 ms is the allocating goroutine being conscripted into doing GC work itself: mark
assists, plus the mark-worker scheduling Go 1.26 reworked. Three consequences follow, and
they are why the distinction is worth keeping:

* **The rest of the program keeps running.** During a mark assist, the other 13 cores keep
  executing other goroutines. A Go server would continue serving other requests throughout.
  A stop-the-world pause freezes every goroutine at once.
* **The cost is charged to the allocator.** Go's mark assist is proportional to how fast a
  goroutine allocates: the goroutine creating the garbage pays the debt. This benchmark is a
  tight allocation loop, so it is the *worst possible victim* — it volunteers for nearly all
  the assist work. A goroutine that allocates little would see far less delay in the same
  process.
* **It is not comparable, millisecond for millisecond, with Python's number.** CPython's
  generational collection stops the interpreter: with the GIL held, all Python threads
  stop. So Python's 320 ms is a whole-program freeze, while Go's 13 ms is one goroutine
  stalling while others proceed. Python's pause is the strictly more severe kind of event.

So both statements are true and neither replaces the other: *"my goroutine paused for
13 ms"* and *"Go's stop-the-world pauses stayed under 42 µs"*. The original post's question
was specifically about **stop-the-world** pauses, and for that question Go's answer on this
workload is tens of microseconds, in every version tested. If instead the question is "how
long can a busy allocating goroutine stall because of GC", the answer is ~13 ms on Go 1.26.
Reporting only one of the two numbers would mislead in one direction or the other.

Note also that the *typical* iteration is unaffected: p99 inter-iteration delay is
0.039–0.040 ms in every configuration. This is strictly a tail-latency effect.

---

## 4. Results: GC CPU load

Measured over the loop only (the list-building phase is excluded), via
`/cpu/classes/gc/total:cpu-seconds` against `/cpu/classes/total:cpu-seconds`.

| config | Green Tea | GC CPU % | GC CPU (s) | GC cycles | heap goal (MB) |
|---|---|---|---|---|---|
| go1.25.6        | off | 4.22 | 8.02 | 12 | 2,536 |
| go1.26.6        | **on** | 4.36 | 8.64 | 12 | 2,535 |
| go1.27rc3       | **on** | 5.17 | 7.92 | 11 | 2,649 |
| go1.26.6 `nogreenteagc`  | off | 4.19 | 7.99 | 12 | 2,520 |
| go1.27rc3 `nogreenteagc` | off | 5.01 | 7.41 | 11 | 2,655 |
| go1.27rc3 `nosizespecializedmalloc` | on | 4.38 | 8.70 | 12 | 2,540 |

Comparing like with like — same Go version, Green Tea toggled — Green Tea **costs** CPU on
this benchmark:

* Go 1.26: 8.64 s vs 7.99 s → **+8.1%** GC CPU
* Go 1.27: 7.92 s vs 7.41 s → **+6.9%** GC CPU

That is the opposite sign from the advertised 10–40% reduction. Two caveats matter before
generalizing:

**This is arm64, and half of Green Tea's win is amd64-only.** The vector-scanning path
lives in `internal/runtime/gc/scan/` and ships as `scan_amd64.s`, `expand_amd64.s`,
`filter_amd64.s` — gated on AVX-512 (`HasAVX512VL && HasAVX512BW && HasGFNI && ...`). There
is no arm64 assembly counterpart, only a generic fallback. The release notes are explicit
that ~10% of the improvement requires "Intel Ice Lake or AMD Zen 4 and newer". On an M4 Max
that part is simply unavailable. Your Linux/Ice Lake box is where it would show up, and
re-running this there is the obvious next step.

**This benchmark is close to a worst case for Green Tea.** Green Tea's design improves
locality by batching scan work at span granularity. A singly-linked list of 50 million
16-byte nodes is pure pointer-chasing with no spatial locality to exploit, so there is
little for the new design to win back — while its extra bookkeeping still costs. A
"real-world program" with denser, more clustered object graphs is a much friendlier case.
So this result is a caution against assuming the 10–40% figure transfers, not a claim that
Green Tea is a regression in general.

---

## 5. Go 1.27 is faster — but not because of the GC

Go 1.27 runs the loop ~21% faster than Go 1.26. It is tempting to credit the collector.
That would be wrong.

| config | loop time (s) |
|---|---|
| go1.26.6 | 14.52 |
| go1.27rc3 | **11.44** |
| go1.27rc3 with `GOEXPERIMENT=nosizespecializedmalloc` | 14.42 |

Disabling Go 1.27's new size-specialized allocator brings it back to **14.42 s**, matching
Go 1.26's 14.52 s almost exactly. The entire speedup is the allocator, which the release
notes describe as "reducing the cost of some small (<80 byte) memory allocations by up to
30%". A `Node` is 16 bytes and the benchmark does nothing but allocate them, so it benefits
about as much as any program could.

This is consistent with the source: `src/runtime/mgcmark_greenteagc.go` is **identical**
between Go 1.26.6 and Go 1.27rc3 apart from a one-word comment typo fix, and the Go 1.27
release notes contain no GC section at all. There is no GC change between 1.26 and 1.27 to
measure.

Watch out for the interaction in the GC CPU *percentage* column of §4: Go 1.27 shows a
higher GC CPU % (5.17%) than Go 1.26 (4.36%) even though it uses fewer GC CPU seconds
(7.92 s vs 8.64 s). The percentage rose only because the denominator — total run time —
shrank. Faster allocation makes the GC a larger share of a smaller pie. Absolute GC CPU
seconds is the more honest metric here.

---

## 6. What the original benchmark reports

For completeness, the unmodified program from the original post (which, per §2, runs with a
4 MB heap), 5 runs each:

| config | median max delay | range |
|---|---|---|
| go1.25.6 | 1.056 ms | 0.191–1.631 ms |
| go1.26.6 | 0.807 ms | 0.350–0.935 ms |
| go1.27rc3 | 0.329 ms | 0.200–0.910 ms |
| go1.26.6 `nogreenteagc` | 0.454 ms | 0.331–0.974 ms |
| go1.27rc3 `nogreenteagc` | 0.248 ms | 0.176–0.320 ms |

All sub-millisecond, all versions, with no Green Tea signal — because with a 4 MB live heap
there is nearly no GC work to do. I could not reproduce the 50 ms from the original post on
this machine; those numbers are dominated by OS scheduling noise rather than by the
collector, which is what one would expect when the heap goal is 4 MB.

---

## 7. Conclusions

* **The original Go benchmark had a bug**: `_ = objects1` does not retain the list, so Go
  was measured on a 4 MB heap while Python held 50 million live objects. Use
  `runtime.KeepAlive`. It is a fairness bug worth fixing, but **it does not change the
  post's Go 1.25 number or its conclusion** — Go 1.25 handles the 1.9 GB live heap in
  0.46 ms.
* **The delay the benchmark measures is a real pause of that goroutine**, and it is fair to
  call it one. It is just not a *stop-the-world* pause: Go's true STW times are 12–16 µs
  mean and under 164 µs worst case in every version tested. Other goroutines keep running
  throughout, which is why Go's 13 ms and Python's 320 ms are not the same kind of event —
  Python's freezes the whole interpreter.
* **Green Tea did not help on this workload.** On arm64 it cost ~7–8% more GC CPU and made
  the worst-case iteration delay ~20× worse (0.5 ms → 13 ms) with a clean, reproducible
  separation. This is a pointer-chasing worst case on a platform lacking the SIMD path, so
  it is a boundary case rather than a verdict — but it does show the 10–40% improvement is
  not universal.
* **Go 1.27's 21% speedup is the allocator, not the collector.** Its GC code is byte-for-byte
  the same as Go 1.26's.
* **The obvious follow-up** is to re-run this on the Linux/Ice Lake machine, where Green
  Tea's AVX-512 scanning path is actually compiled in. That is where the advertised gains
  should appear, if they appear anywhere on this benchmark.

---

## Reproducing

```sh
go install golang.org/dl/go1.25.6@latest && go1.25.6 download
go install golang.org/dl/go1.26.6@latest && go1.26.6 download
go install golang.org/dl/go1.27rc3@latest && go1.27rc3 download

# default (Green Tea on for 1.26/1.27)
go1.26.6 build -o stress_126 ./stress_gc.go && ./stress_126

# Green Tea disabled
GOEXPERIMENT=nogreenteagc go1.26.6 build -o stress_126_nogt ./stress_gc.go && ./stress_126_nogt

# Go 1.27 without the new allocator
GOEXPERIMENT=nosizespecializedmalloc go1.27rc3 build -o stress_127_nossm ./stress_gc.go

# heap behaviour
GODEBUG=gctrace=1 ./stress_126
```

`stress_gc.go` emits one JSON object per run. Raw data for all 30 instrumented runs is in
`all.jsonl`; the unmodified-benchmark numbers are in `orig_results.txt`; the
original-plus-`KeepAlive` numbers of §2 are in `keep_results.txt`.

The original benchmark at `../../02/13/stress_gc.go` has been corrected to use
`runtime.KeepAlive`.
