const fs = require('fs');
const readFile = fs.readFile;
let start = Date.now();
let count = 0;
const N = 50000;
for (let i = 0; i < N; i++) {
	fs.readFile('lipsum.txt', function read(err, data) {
		if (err) {
			throw 1;
		}
		if (data.length != 1335) throw 1;
		count++;
		if (count == N) {
			end = Date.now();
			console.log('time: ', end - start);
			start = end;
		}
	});
}
