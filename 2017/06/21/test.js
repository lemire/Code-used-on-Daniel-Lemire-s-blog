'use strict';

var QuickSelect = require('qselect');
var FastPriorityQueue = require('fastpriorityqueue');
var Benchmark = require('benchmark');
var os = require('os');


// very fast semi-random function
function rand(i) {
    i = i + 10000;
    i = i ^ (i << 16);
    i = (i >> 5) ^ i;
    return i & 0xFF;
}

var defaultcomparator = function(a, b) {
    return a < b;
};
var reverseddefaultcomparator = function(a, b) {
    return a > b;
};

function compare(array1, array2) {
    if (array1.length != array2.length) return false;
    for (var i = 0; i < array1.length; i++) {
        if (array1[i] != array2[i]) return false;
    }
    return true;
}

function check(streamsize, k) {
    var expectedanswer = new Array();
    for (var i = 0; i < streamsize; i++) {
        expectedanswer.push(rand(i));
    }
    expectedanswer.sort(function(a, b) {
        return a - b
    })
    expectedanswer = expectedanswer.slice(0, k);

    {

        var b = new FastPriorityQueue(reverseddefaultcomparator);
        for (var i = 0; i < k; i++) {
            b.add(rand(i));
        }
        for (i = k; i < streamsize; i++) {
            b.add(rand(i));
            b.poll();
        }
        var answer = b.array.slice(0, k).sort(function(a, b) {
            return a - b
        });
        if (!compare(answer, expectedanswer)) console.log("bug FastPriorityQueue ", answer, expectedanswer);
    } {

        var b = new FastPriorityQueue(reverseddefaultcomparator);
        for (var i = 0; i < k; i++) {
            b.add(rand(i));
        }
        for (i = k; i < streamsize; i++) {
            var x = rand(i);
            if (x < b.peek()) {
                b.replaceTop(x);
            }
        }
        var answer = b.array.slice(0, k).sort(function(a, b) {
            return a - b
        });
        if (!compare(answer, expectedanswer)) console.log("bug FastPriorityQueue-KWillets-replaceTop", answer, expectedanswer);

    } {

        var b = new FastPriorityQueue(reverseddefaultcomparator);
        for (var i = 0; i < k; i++) {
            b.add(rand(i));
        }
        for (i = k; i < streamsize; i++) {
            var x = rand(i);
            if (x < b.peek()) {
                b.add(x);
                b.poll();
            }
        }
        var answer = b.array.slice(0, k).sort(function(a, b) {
            return a - b
        });
        if (!compare(answer, expectedanswer)) console.log("bug FastPriorityQueue-KWillets", answer, expectedanswer);

    } {

        var a = new Array();
        for (var i = 0; i < streamsize; i++) {
            a.push(rand(i));
        }
        a.sort(function(a, b) {
            return a - b
        });
        var answer = a.slice(0, k);
        if (!compare(answer, expectedanswer)) console.log("bug sort");

    } {

        var a = new Array();
        for (var i = 0; i < streamsize; i++) {
            a.push(rand(i));
        }
        var answer = QuickSelect(a, k, defaultcomparator).slice(0, k).sort(function(a, b) {
            return a - b
        });
        if (!compare(answer, expectedanswer)) console.log("bug QuickSelect");
    } {

        var a = new Array();
        for (var i = 0; i < 2 * k; i++) {
            a.push(rand(i));
        }
        QuickSelect(a, k, defaultcomparator);
        var eaten = 2 * k;
        while (eaten < streamsize) {
            // assume that k divides streamsize
            for (var i = 0; i < k; i++) {
                a[k + i] = rand(i + eaten);
            }
            QuickSelect(a, k, defaultcomparator);
            eaten += k;
        }
        var answer = a.slice(0, k).sort(function(a, b) {
            return a - b
        });
        if (!compare(answer, expectedanswer)) console.log("bug naiveGoogleGuava");

    } {
        var a = new Array();
        var i = 0;
        for (; i < 2 * k; i++) {
            a.push(rand(i));
        }
        QuickSelect(a, k, defaultcomparator);
        var pivotvalue = a[k];
        var locationinbuffer = k;
        for (; i < streamsize; i += 1) {
            var newvalue = rand(i);
            if (newvalue < pivotvalue) {
                a[locationinbuffer++] = newvalue;
            }
            if (locationinbuffer == 2 * k) {
                QuickSelect(a, k, defaultcomparator);
                locationinbuffer = k;
            }
        }
        if (locationinbuffer != k) QuickSelect(a, k, defaultcomparator);
        var answer = a.slice(0, k).sort(function(a, b) {
            return a - b
        });
        if (!compare(answer, expectedanswer)) console.log("bug naiveDavidEppstein", answer, expectedanswer);
    }

}

function QueueEnqueueBench(streamsize, k) {
    if (Math.floor(streamsize / k) * k != streamsize) {
        console.log('bad streamsize ', streamsize, k);
        return;
    }
    console.log('starting dynamic queue/enqueue benchmark');
    var suite = new Benchmark.Suite();
    // add tests
    var ms = suite
        .add('FastPriorityQueue', function() {
            var b = new FastPriorityQueue(reverseddefaultcomparator);
            for (var i = 0; i < k; i++) {
                b.add(rand(i));
            }
            for (i = k; i < streamsize; i++) {
                b.add(rand(i));
                b.poll();
            }
            var answer = b.array.slice(0, k).sort(function(a, b) {
                return a - b
            });
            return answer;
        })
        .add('FastPriorityQueue-KWillets', function() {
            var b = new FastPriorityQueue(reverseddefaultcomparator);
            for (var i = 0; i < k; i++) {
                b.add(rand(i));
            }
            for (i = k; i < streamsize; i++) {
                var x = rand(i);
                if (x < b.peek()) {
                    b.add(x);
                    b.poll();
                }
            }
            var answer = b.array.slice(0, k).sort(function(a, b) {
                return a - b
            });
            return answer;
        })
        .add('FastPriorityQueue-KWillets-replaceTop', function() {
            var b = new FastPriorityQueue(reverseddefaultcomparator);
            for (var i = 0; i < k; i++) {
                b.add(rand(i));
            }
            for (i = k; i < streamsize; i++) {
                var x = rand(i);
                if (x < b.peek()) {
                    b.replaceTop(x);
                }
            }
            var answer = b.array.slice(0, k).sort(function(a, b) {
                return a - b
            });
            return answer;
        })

        .add('sort', function() {
            var a = new Array();
            for (var i = 0; i < streamsize; i++) {
                a.push(rand(i));
            }
            a.sort(function(a, b) {
                return a - b
            });
            var answer = a.slice(0, k);
            return answer;
        })
        .add('QuickSelect', function() {
            var a = new Array();
            for (var i = 0; i < streamsize; i++) {
                a.push(rand(i));
            }
            var answer = QuickSelect(a, k, defaultcomparator).slice(0, k).sort(function(a, b) {
                return a - b
            });
            return answer;
        })
        .add('QuickSelect-naiveGoogleGuava', function() {
            var a = new Array();
            for (var i = 0; i < 2 * k; i++) {
                a.push(rand(i));
            }
            QuickSelect(a, k, defaultcomparator);
            var eaten = 2 * k;
            while (eaten < streamsize) {
                var nextsize = eaten + k; // assume that streamsize is divisible by k
                for (var i = 0; i < k; i++) {
                    a[k + i] = rand(i + eaten);
                }
                QuickSelect(a, k, defaultcomparator);
                eaten += k;
            }
            var answer = a.slice(0, k).sort(function(a, b) {
                return a - b
            });
            return answer;

        })
        .add('QuickSelect-naiveDavidEppstein', function() {
            var a = new Array();
            var i = 0;
            for (; i < 2 * k; i++) {
                a.push(rand(i));
            }
            QuickSelect(a, k, defaultcomparator);
            var pivotvalue = a[k];
            var locationinbuffer = k;
            for (; i < streamsize; i += 1) {
                var newvalue = rand(i);
                if (newvalue < pivotvalue) {
                    a[locationinbuffer++] = newvalue;
                }
                if (locationinbuffer == 2 * k) {
                    QuickSelect(a, k, defaultcomparator);
                    locationinbuffer = k;
                }
            }
            if (locationinbuffer != k) QuickSelect(a, k, defaultcomparator);
            var answer = a.slice(0, k).sort(function(a, b) {
                return a - b
            });
            return answer;
        })
        // add listeners
        .on('cycle', function(event) {
            console.log(String(event.target));
        })
        // run async
        .run({
            'async': false
        });
}

var main = function() {
    console.log('Platform: ' + process.platform + ' ' + os.release() + ' ' + process.arch);
    console.log(os.cpus()[0]['model']);
    console.log('Node version ' + process.versions.node + ', v8 version ' + process.versions.v8);
    console.log();
    console.log('');
    check(1408 * 10, 128);
    QueueEnqueueBench(1408 * 10, 128);
    console.log('');
};

if (require.main === module) {
    main();
}
