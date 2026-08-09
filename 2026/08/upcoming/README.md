# Adversarial inputs for production substring search

Four runtimes — the macOS C library, Java, Node.js (V8) and .NET — implement
`strstr` / `indexOf` as a *filter-and-verify* loop: cheaply reject most candidate
positions with a SIMD test on one or two characters, and run a full comparison
only on the survivors. The filter is what makes these routines fast in practice.
None of them (except V8, partially) has a linear-time algorithm behind the
filter, so an input that defeats the filter costs Θ(*n*·*m*).

Each benchmark below fixes the haystack at *n* = 1 MiB and sweeps the needle
length *m*. **At fixed *n*, quadratic behaviour appears as time-per-haystack-byte
doubling when *m* doubles.** A linear searcher stays flat.

Every case ships with a *control*: an input of the same shape that the filter
does reject, which stays flat and shows the measurement is not an artifact.

Machine: Apple M4 Max, macOS 15 (Darwin 24.6.0, arm64).
Node v25.9.0 / V8 14.1, OpenJDK 25.0.1, .NET 10.0.3, Apple clang.

```sh
./run_all.sh          # all four
```

---

## 1. macOS C library — `strstr` and `memmem`

Apple's libc inherits the FreeBSD `strstr`: scan for the needle's **first** byte,
then `strncmp` the whole needle. No fall-back.

| | |
|---|---|
| haystack | `"a" * n` |
| needle | `"a"*(m-1) + "b"` |
| control | `"b" + "a"*(m-1)` (filter byte `'b'` never occurs) |

Every alignment passes the first-byte filter; every verification compares *m*−1
bytes before failing on the last one.

| m | strstr adv | strstr ctl | memmem adv | memmem ctl |
|---:|---:|---:|---:|---:|
| 8 | 3.01 | 0.25 | 1.52 | 0.22 |
| 128 | 6.27 | 0.25 | 2.52 | 0.45 |
| 512 | 13.02 | 0.23 | 9.42 | 0.44 |
| 1024 | 20.96 | 0.22 | 17.42 | 0.45 |
| 2048 | 49.80 | 0.23 | 42.25 | 0.45 |
| 4096 | 91.60 | 0.23 | 81.57 | 0.44 |

(ns per haystack byte.) Clean doubling above *m* = 512; **405× slower** than the
control at *m* = 4096. Below *m* ≈ 256 the growth is sublinear in *m* because the
verification is a vectorized `memcmp`, so the per-candidate cost is ⌈m/16⌉ vector
operations rather than *m* byte operations — the asymptotics are unchanged.

## 2. Java — `String.indexOf`

Java has three searchers and **which one runs depends on the architecture**, so
two different adversaries are needed. On AArch64 the HotSpot intrinsic uses
Horspool with a bad-character table when 8 ≤ *m* < 256 and *n* ≥ 4*m*, and a NEON
block scan otherwise.

| adversary | needle | defeats |
|---|---|---|
| first-character | `"a"*(m-1) + "b"` | the portable scan and the NEON block compare |
| last-character | `"b" + "a"*(m-1)` | Horspool (bad-character shift for `'a'` is exactly 1, and each window matches *m*−1 characters before failing at index 0) |

Haystack `"a" * n` for both. Each adversary is the other's control.

| m | first-char adv | last-char adv |
|---:|---:|---:|
| 8 | 0.44 | 0.67 |
| 32 | 0.44 | 6.80 |
| 128 | 0.44 | 35.68 |
| **256** | **73.20** | **0.03** |
| 1024 | 257.89 | 0.03 |
| 4096 | **1027.69** | 0.04 |

The two columns swap at exactly *m* = 256, the documented Horspool cutoff. Below
it the bad-character rule is the vulnerability; above it the block compare is.
At *m* = 4096 a single `indexOf` over 1 MiB takes **1.08 seconds** — roughly
29 000× the control.

Disabling the intrinsics (`-XX:DisableIntrinsic=_indexOfL,...`) forces the
portable `StringLatin1.indexOf`, a plain first-character scan, and the first
column then degrades over the *whole* range — 2.62 ns/char at *m* = 8 rising to
679.71 at *m* = 4096 — while the last-character column stays flat at 0.10. That
is the architecture dependence made visible.

## 3. Node.js / V8 — `String.prototype.indexOf`

V8 is the only one of the four that escalates. For *m* ≥ 7 it starts naive with a
work counter, switches to Horspool when the counter goes positive, then to full
Boyer–Moore when Horspool's own counter does. Boyer–Moore is linear for a
first-occurrence search, so most adversaries are absorbed.

The hole is that V8 builds the good-suffix table for only the last
`kBMMaxShift = 250` characters. A window that matches backwards *past* that
window has no good-suffix entry and falls back to a Horspool shift.

| | |
|---|---|
| haystack | `"a" * n` |
| needle | `"a" + "b" + "a"*(m-2)` |
| control | `"a"*(m-2) + "b" + "a"` (backward match stops after one character) |

The leading `'a'` keeps the naive stage from bailing out early; the trailing run
makes the bad-character shift for `'a'` equal to 1; the `'b'` at index 1 puts the
mismatch *m*−2 characters into the backward scan.

| m | adversary | control |
|---:|---:|---:|
| 128 | 0.28 | 0.33 |
| **256** | **72.33** | 0.33 |
| 1024 | 275.83 | 0.34 |
| 4096 | **1087.53** | 0.34 |

Scanning *m* one at a time locates the cliff exactly:

| m | 249 | 250 | 251 | **252** | 253 | 254 |
|---|---:|---:|---:|---:|---:|---:|
| ns/char | 0.30 | 0.29 | 0.28 | **69.60** | 74.68 | 73.01 |

A 250× jump between *m* = 251 and *m* = 252. That is precisely where the theory
puts it: the mismatch sits at index 1, the covered region starts at *m* − 250, and
the fall-back triggers when 1 < *m* − 250, i.e. *m* ≥ 252. At *m* = 4096 a single
`indexOf` over 1 MiB takes **1.14 seconds**.

## 4. .NET — `string.IndexOf(..., StringComparison.Ordinal)`

.NET filters on **two** characters: `ch1 = needle[0]`, and `ch2` = the last
character of the needle that differs from `ch1`, at distance *d*. It SIMD-scans
for positions where both match, then runs a vectorized `SequenceEqual`.

That second character defeats the textbook adversary — this is the control
column, and it is genuinely flat. Making .NET quadratic requires a haystack dense
in *both* filter characters:

| | |
|---|---|
| haystack | `"ab" * (n/2)` → `"abababab..."` |
| needle | `"ab"*(m/2 - 1) + "bb"` (*m* even) |
| control | haystack `"a"*n`, needle `"a"*(m-1) + "b"` |

`ch1 = 'a'` and the needle's last character is `'b'`, so *d* = *m*. In the
alternating haystack `haystack[i] == 'a'` iff *i* is even, and with *m* even
`haystack[i+m-1]` sits at an odd index and equals `'b'` iff *i* is even — so
**both filter characters agree at every even position**, and half the haystack
survives the filter instead of none. Each survivor then matches for *m*−2
characters and fails only at index *m*−2. `"bb"` never occurs in `"abab..."`, so
there is no match and the scan runs to the end.

| m | adversary | control |
|---:|---:|---:|
| 8 | 0.53 | 0.06 |
| 128 | 2.54 | 0.06 |
| 512 | 9.78 | 0.06 |
| 1024 | 21.06 | 0.06 |
| 2048 | 44.99 | 0.06 |
| 4096 | 80.38 | 0.06 |

A third probe confirms the filter is the last *distinct* character rather than
the literal last one: needle `"a"*(m/2) + "b" + "a"*(m/2-1)` over `"a"*n` ends in
a run of `ch1`, so a literal-last-character filter would match everywhere and go
quadratic. It stays at 0.055 ns/char for every *m* up to 4096 — the backward walk
finds the buried `'b'`, exactly as described.

Clean doubling from *m* = 32 on; **1460×** the control at *m* = 4096. The absolute
numbers are lower than Java's and V8's because .NET's verification is a
vectorized `SequenceEqual` (≈ *m*/16 vector operations per candidate) rather than
a character loop — again, the constant differs, the asymptotics do not.

---

## Both factors, not just one

The sweeps above vary *m* at fixed *n*. That establishes the *m* factor but not
the *n* factor, so each benchmark also scales *n* at fixed *m* = 1024. Flat
ns/char means time ∝ *n*; combined with the *m* sweeps, the cost is Θ(*n*·*m*)
rather than Θ(*n*²) or Θ(*m*²).

| n | Java (first-char) | Node / V8 | .NET |
|---:|---:|---:|---:|
| 65 536 | 252.4 | 250.3 | 18.8 |
| 262 144 | 259.6 | 264.8 | 19.6 |
| 1 048 576 | 255.3 | 270.5 | 21.1 |
| 4 194 304 | 258.4 | 275.3 | 21.8 |

Flat within ~10% across a 64× range of *n*.

## Summary

| runtime | filter | adversary | worst measured | vs control |
|---|---|---|---:|---:|
| macOS `strstr` | first byte | `a^(m-1) b` over `a^n` | 91.6 ns/byte | 405× |
| macOS `memmem` | first byte | `a^(m-1) b` over `a^n` | 81.6 ns/byte | 185× |
| Java, *m* < 256 | last char (Horspool) | `b a^(m-1)` over `a^n` | 35.7 ns/char | 80× |
| Java, *m* ≥ 256 | NEON block | `a^(m-1) b` over `a^n` | 1027.7 ns/char | 29 000× |
| Node / V8 | first, then last char | `a b a^(m-2)` over `a^n`, *m* ≥ 252 | 1087.5 ns/char | 3 200× |
| .NET | first + last distinct char | `(ab)^(m/2-1) bb` over `(ab)^(n/2)` | 80.4 ns/char | 1 460× |

Searching a **one-megabyte** string for a **four-kilobyte** needle takes over a
second in both Node.js and Java. These are not pathological sizes; they are the
sizes of a log file and a stack trace.

### What these measurements do and do not establish

Confirmed: the Θ(*n*·*m*) worst case for Java (both AArch64 branches and the
portable path), Node/V8, .NET and the macOS C library, in both factors. Also
confirmed are three specific structural claims — V8's 250-character good-suffix
window (the cliff lands at *m* = 252, to the character), Java's *m* = 256 branch
boundary on AArch64 (the two adversaries swap roles exactly there), and .NET's
last-*distinct*-character backward walk.

Not established here: the **Java HotSpot x86 / PCMPESTRI** row, which this
AArch64 machine cannot exercise. And these are behavioural measurements, not a
source audit — timings alone cannot uniquely identify an algorithm, though V8's
exact-to-the-character cliff is hard to explain any other way. Finally, "no
fall-back" is confirmed only within the range tested (*m* ≤ 4096, *n* ≤ 4 MiB); a
fall-back with a higher trigger would not show up.

Note that all four adversaries are two-character alphabets and take one line to
construct. Where the needle is attacker-controlled — a search box, a filter
expression, a header match — the cost is quadratic in the product of two inputs
the attacker also supplies.
