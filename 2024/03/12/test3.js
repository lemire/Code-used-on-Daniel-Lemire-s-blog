const fs = require('fs');
const readFileSync = fs.readFileSync;
let start = Date.now();
let count = 0;
const N = 50000;
for (let i = 0; i < N; i++) {
	var data = readFileSync("lipsum.txt", { encoding: 'utf-8' })
	if (data.length != 1335) throw 1;
}
console.log('time: ', Date.now() - start);

