const fs = require('fs/promises');
const readFile = fs.readFile;
let start = Date.now();
let count = 0;
const N = 50000;
for (let i = 0; i < N; i++) {
	readFile("lipsum.txt", { encoding: 'utf-8' })
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
