// Adversarial input for V8's String.prototype.indexOf (Node.js, Chrome, Edge).
//
// V8 does not stay naive: it escalates on work counters. For needles of at
// least seven characters it starts in a naive first-character
// filter-and-verify loop with a counter initialized to -10-4m; when the counter
// goes positive it builds a bad-character table and switches to Horspool; when
// Horspool's own counter goes positive it builds the good-suffix table and
// switches to full Boyer-Moore. Boyer-Moore is linear for a first-occurrence
// search, so most adversaries are absorbed by the escalation.
//
// The hole is that V8 builds the good-suffix table for only the last
// kBMMaxShift = 250 characters of the needle. When a window matches backwards
// past that covered suffix, Boyer-Moore has no good-suffix entry to consult and
// falls back to a Horspool shift keyed on the last character. Choose the needle
// so that shift is 1 and the backward match is nearly the whole needle:
//
//     haystack = "a"*n
//     needle   = "a" + "b" + "a"*(m-2)
//
//   * First character is 'a', so the naive stage finds a candidate at every
//     position and escalates (it does not bail out early, which is what a
//     needle starting with a rare character would do).
//   * Last character is 'a' and the rightmost other 'a' sits at index m-2, so
//     the bad-character shift for 'a' is (m-1)-(m-2) = 1.
//   * Scanning backwards, the window matches 'a' all the way down to index 1,
//     where the needle has 'b' and the haystack has 'a'. That is a backward
//     match of m-2 characters, past the 250-character good-suffix window as
//     soon as m > 252, so the shift stays 1.
//
// Result: n alignments, m-2 comparisons each, shift 1. Theta(n*m).
//
// The control is the mirror needle "a"*(m-2) + "b" + "a", whose backward match
// stops after one character, so Horspool shifts cleanly and the search is
// linear.
//
// Run: node adversarial.mjs

function measure(haystack, needle, minSeconds) {
  let best = Infinity;
  let spent = 0;
  let rounds = 0;
  while (spent < minSeconds || rounds < 3) {
    const t0 = process.hrtime.bigint();
    const r = haystack.indexOf(needle);
    const elapsed = Number(process.hrtime.bigint() - t0) / 1e9;
    if (r === -2) console.log("unreachable"); // defeat dead-code elimination
    if (elapsed < best) best = elapsed;
    spent += elapsed;
    rounds++;
    if (rounds > 1000) break;
  }
  return (best * 1e9) / haystack.length;
}

const n = process.argv[2] ? Number(process.argv[2]) : 1 << 20;
const haystack = "a".repeat(n);

// Warm up the JIT and V8's string internals.
{
  const w = "a".repeat(1 << 16);
  for (let i = 0; i < 2000; i++) {
    if (w.indexOf("ab" + "a".repeat(300)) === -2) console.log("x");
  }
}

console.log(`Node ${process.version} (V8 ${process.versions.v8}), String.prototype.indexOf`);
console.log(`haystack = 'a' x ${n}`);
console.log("adversary: needle = 'a' + 'b' + 'a'x(m-2)   (backward match runs past the");
console.log("           250-character good-suffix window, so Boyer-Moore shifts by 1)");
console.log("control:   needle = 'a'x(m-2) + 'b' + 'a'   (backward match stops at once)");
console.log();
console.log(
  "m".padStart(8) + "adversary ns/char".padStart(20) + "control ns/char".padStart(20)
);

for (let m = 16; m <= 4096; m *= 2) {
  const adv = "a" + "b" + "a".repeat(m - 2);
  const ctl = "a".repeat(m - 2) + "b" + "a";
  const a = measure(haystack, adv, 0.05);
  const c = measure(haystack, ctl, 0.05);
  console.log(
    String(m).padStart(8) + a.toFixed(3).padStart(20) + c.toFixed(3).padStart(20)
  );
}

console.log();
console.log("V8's good-suffix table covers only the last kBMMaxShift = 250 characters,");
console.log("so the adversary only bites once the backward match runs past it. Locating");
console.log("the cliff, on a shorter haystack:");
console.log();

const shortHaystack = "a".repeat(1 << 18);
console.log("m".padStart(8) + "adversary ns/char".padStart(20));
for (let m = 246; m <= 258; m++) {
  const adv = "a" + "b" + "a".repeat(m - 2);
  const a = measure(shortHaystack, adv, 0.05);
  console.log(String(m).padStart(8) + a.toFixed(3).padStart(20));
}

// The sweeps above vary m at fixed n, establishing the m factor. Vary n at
// fixed m to establish the other one: ns/char flat in n means time ~ n, so
// together the cost is Theta(n*m) rather than, say, Theta(n^2) or Theta(m^2).
console.log();
console.log("Scaling n at fixed m = 1024 (flat ns/char means time is linear in n):");
console.log();
console.log("n".padStart(10) + "adversary ns/char".padStart(20));
const advFixed = "a" + "b" + "a".repeat(1022);
for (let k = 16; k <= 22; k++) {
  const h = "a".repeat(1 << k);
  console.log(String(1 << k).padStart(10) + measure(h, advFixed, 0.05).toFixed(3).padStart(20));
}
