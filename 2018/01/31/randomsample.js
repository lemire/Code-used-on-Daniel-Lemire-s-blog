'use strict';
var Benchmark = require('benchmark');
var FastBitSet = require('fastbitset');

// starting code imported from https://blog.acolyer.org/2018/01/30/a-sample-of-brilliance/
// citing A sample of brilliance Jon Bentley et al., CACM 1987

// return an integer in [0,i)
function randInt(i) {
  return Math.floor(Math.random() * i);
}

// return m values in the range [0,n)
function sampleS(m, n) {
   let s = new Set();
   while(s.size < m) {
      s.add(randInt(n));
   }
   return s;
}

// Use the "mix" approach, which goes like this:
//
// 1) l2 is the smallest value such that 2^l2 >= n
// 2) for each value x in the sequence 0..m-1 do this:
// 3) hash x, using a function similar to splitmix64,
//    which has no collision
// 4) if the hash is >= n, hash again, until < n
//
// This approach doesn't need a (bit-) set,
// as by design duplicates are avoided.
// The disadvantage is that it's a custom PRNG.
//
// See also:
// https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key/12996028#12996028
// http://xorshift.di.unimi.it/splitmix64.c
//
// At step 4, we could also skip the value - but
// then the sequence can't be generated in parallel.

function mix(m, n) {
    m = m | 0;
    n = n | 0;
    var l2 = nextLog2(n) | 0;
    var shift = (l2 >> 1) | 0;
    var mask = ((1 << l2) - 1) | 0;
    let s = new Int32Array(m);
	  // var fbs = new FastBitSet();
    for (let j = 0; (j|0) < m; j = (j+1)|0) {
        s[j] = next(j, n, shift, mask);
        // let x = fbs.checkedAdd(s[j]);
        // assert(x == 1, "Bug in add at " + j);
    }
    return s;
}

function next(x, n, shift, mask) {
    x = x | 0;
    n = n | 0;
    while (true) {
        x = hash(x, shift, mask);
        if ((x|0) < n) {
            return x;
        }
    }
}

function nextLog2(x) {
    x = x | 0;
    let n = 1;
    while ((1 << n) < x) {
        n++;
    }
    return n | 0;
}

function hash(x, shift, mask) {
    x = x | 0;
    shift = shift | 0;
    mask = mask | 0;
    x = Math.imul((x >>> shift) ^ x, 0x45d9f3b);
    x = x & mask;
    x = Math.imul((x >>> shift) ^ x, 0x45d9f3b);
    x &= mask;
    x = x ^ (x >>> shift);
    x &= mask;
    return x | 0;
}

// return m values in the range [0,n)
// attributed to Floyd
function sampleF2(m, n) {
    let s = new Set();
    for (let j = n - m; j < n; j++) {
        const t = randInt(j);
        s.add( s.has(t) ? j : t );
    }
    return s;
}


// end of imported code
function sampleBitmap(m, n) {
   var s = new FastBitSet();
   var cardinality = 0
   while(cardinality < m) {
      cardinality += s.checkedAdd(randInt(n));
   }
   return s
}


function negate(s, n) {
  var news = new FastBitSet()
  let i = 0
  s.forEach(j => {while(i<j) {news.add(i);i++}; i = j+1})
  while(i<n) {news.add(i);i++}
  return news
}

function fastsampleS(m, n) {
    if(m > n / 2 ) {
      let negatedanswer = fastsampleS(n-m, n)
      return negate(negatedanswer,n)
    }
    if(m * 1024 > n) {
      return sampleBitmap(m, n)
    }
    return sampleS(m, n)
}


function assert(condition, message) {
    if (!condition) {
        throw message || "Assertion failed";
    }
}
function Bench(m,n) {
 console.log("Sampling "+m+" values in ["+0+","+n+"). Ratio: "+m/n*100+"%")
 assert(sampleF2(m, n).size == m, "bug in sampleF2")
 assert(sampleS(m, n).size == m, "bug in sampleS")
 assert(fastsampleS(m, n).size() == m , "bug in fastsampleS")

 var suite = new Benchmark.Suite();
  // add tests
  var ms = suite.add('sampleF2', function() {
    return sampleF2(m, n);
  })
  .add('sampleS', function() {
    return sampleS(m, n);
  })
  .add('fastsampleS', function() {
    return fastsampleS(m, n);
  })
  .add('mix', function() {
    return mix(m, n);
  })
  // add listeners
  .on('cycle', function(event) {
      console.log(String(event.target));
  })
  // run async
  .run({'async': false});
  console.log()
}

var main = function() {
  let n = 1000000;
  Bench(Math.floor(n/100),n)
  Bench(Math.floor(n/10),n)
  Bench(Math.floor(n/2),n)
  Bench(Math.floor(3*n/4),n)
  Bench(n,n)
}
if (require.main === module) {
  main();
}
