import { bench, run } from 'mitata';
import crypto from 'crypto';


function createRandomUint8Array(length) {
    const array = new Uint8Array(length);
    
    // Fill the array with random values from 0 to 255
    for (let i = 0; i < length; i++) {
        array[i] = Math.floor(Math.random() * 256);
    }

    return array;
}

function md5Hash(message) {
    return crypto.createHash('md5').update(message).digest('hex');
}

function sha256Hash(message) {
    return crypto.createHash('sha256').update(message).digest('hex');
}

const largeString = createRandomUint8Array(1000000000); // 1GB string

const md5Bench = bench('MD5 Hash', () => {
    md5Hash(largeString);
});

const sha256Bench = bench('SHA-256 Hash', () => {
    sha256Hash(largeString);
});


let z = await run();
