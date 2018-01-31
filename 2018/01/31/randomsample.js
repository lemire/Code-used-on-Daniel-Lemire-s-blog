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
  var news = new Set()
  let i = 0
  s.forEach(j => {while(i<j) {news.add(i);i++}; i = j+1})
  while(i<n) {news.add(i);i++}
  return news
}

function fastsampleS(m, n) {
    if(m > n / 2 ) {
      let negatedanswer = fastsampleS(n-m, n)
      return negate(negatedanswer)
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
 assert((fastsampleS(m, n).size == m) || (fastsampleS(m, n).size() == m) , "bug in fastsampleS")

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
  Bench(n/100,n)
  Bench(n/10,n)
  Bench(n/2,n)
}
if (require.main === module) {
  main();
}
