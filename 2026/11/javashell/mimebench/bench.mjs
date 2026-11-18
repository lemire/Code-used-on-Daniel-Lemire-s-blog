import { readFileSync } from 'node:fs';
const [expLen, expSum] = readFileSync('expected.txt', 'latin1').trim().split(' ').map(Number);
function check(u8, name) {
  let s = 0; for (const b of u8) s += b;
  if (u8.length !== expLen || s !== expSum) throw new Error('bad decode ' + name);
}
let sink;
const runtime = globalThis.Bun ? `bun ${Bun.version}` : globalThis.Deno ? `deno ${Deno.version.deno}` : `node ${process.version}`;
function bench(label, f, inLen) {
  check(f(), label);
  const iters = 200;
  for (let w = 0; w < 30; w++) for (let i = 0; i < iters; i++) sink = f();
  let best = Infinity;
  for (let t = 0; t < 10; t++) {
    const st = process.hrtime.bigint();
    for (let i = 0; i < iters; i++) sink = f();
    best = Math.min(best, Number(process.hrtime.bigint() - st) / iters);
  }
  console.log(`${runtime} ${label}: ${(inLen / best).toFixed(2)} GB/s`);
}
for (const name of ['mime']) {
  const s = readFileSync(name + '.txt', 'latin1');
  bench(`${name} Buffer.from(str,'base64')`, () => Buffer.from(s, 'base64'), s.length);
  if (Uint8Array.fromBase64) bench(`${name} Uint8Array.fromBase64(str)`, () => Uint8Array.fromBase64(s), s.length);
}
