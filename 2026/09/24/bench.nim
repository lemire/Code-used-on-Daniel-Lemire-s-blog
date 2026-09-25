import std/[monotimes, times, strformat, strutils]

var buf = newSeq[string](1024)

proc fromInt(n: int) {.noinline.} =
  for i in 0 ..< n:
    buf[i and 1023] = $i

const N = 100_000_000
fromInt(1_000_000) # warm up
var best = float.high
for r in 0 ..< 5:
  let t0 = getMonoTime()
  fromInt(N)
  let dt = float(inNanoseconds(getMonoTime() - t0))
  if dt < best: best = dt
var total = 0
for s in buf: total += s.len
let ns = best / N
echo "Nim ", NimVersion
echo alignLeft("$i", 24), &" {ns:7.2f} ns/string  {1e3/ns:8.1f} M/s   (check {total})"
