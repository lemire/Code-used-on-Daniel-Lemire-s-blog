const fs = require('fs');
const readFileSync = fs.readFileSync;
async function f(name, options) {
    return await readFileSync(name, options);
}

let start = Date.now();
let count = 0;
const N = 50000;
for (let i = 0; i < N; i++) {
	f("lipsum.txt", { encoding: 'utf-8' })
		.then((data) => {
			if (data.length != 1335) throw 1;
			count++
			if (count == N) {
				end = Date.now();
				console.log('time: ', end - start);
			}
		})
		.catch((err) => {
			throw 1

		})
}
