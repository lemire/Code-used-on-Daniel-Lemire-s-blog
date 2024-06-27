import { bench, run } from "mitata";
const size = 1024 * 1024 * 8;

let base64Input = Buffer.alloc(size, "latin1").toString("base64");
let base64From = Buffer.from (base64Input, "base64");
console.log(base64Input);
Buffer.from(base64Input, "base64");
base64From.toString("base64");
function makeBenchmark(size, isToString) {
 if (!isToString)
    bench(`Buffer. from(${size} bytes, 'base64')`, () => {
      Buffer.from(base64Input, "base64");
    });
   if (isToString)
bench (`Buffer(${size}).toString('base64')`, () => {
  base64From.toString("base64");
});
}
//[1024 * 1024 * 8]. forEach (s => makeBenchmark(s, true));
[1024 * 1024 * 8]. forEach (s => makeBenchmark(s, false)) ;
await run();
