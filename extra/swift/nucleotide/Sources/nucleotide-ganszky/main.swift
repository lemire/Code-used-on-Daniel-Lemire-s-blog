/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Ralph Ganszky
*/

import Foundation
import Dispatch

let i2c = [ 0: "A", 1: "C", 2: "T", 3: "G" ]

let c2i: [Character:Int] = [ "A": 0, "C": 1, "T": 2, "G": 3 ]

let ntasks = 8
let linewidth = 60

let mQueue = DispatchQueue(label: "mergeQueue", attributes: [])
let queue = DispatchQueue(label: "hashQueue", attributes: DispatchQueue.Attributes.concurrent)

func compress(_ n: Int, seq: ArraySlice<Int8>) -> Int {
    var res = 0
    for i in seq.indices {
        res = (res << 2) | Int(seq[i])
    }
    return res
}

func decode(_ n: Int, _ len: Int) -> String {
    var val = n
    var s = ""
    for _ in 0..<len {
        s = i2c[val & 0x3]! + s
        val >>= 2
    }
    return s
}

func encode(_ seq: String) ->  Int {
    let cSeq = seq.utf8.map{(Int8($0) & 0x6) >> 1}
    var res = 0
    for c in cSeq {
        res = res << 2 | Int(c)
    }
    return res
}

func getSequenceHash(_ n: Int, seq: [Int8]) -> [Int:Int] {
    var hash = [Int:Int]()
    let slice = (seq.count-(n-1)) / ntasks
    let remainder = (seq.count-(n-1)) % ntasks
    let mask = ~(-1 << (2*n))
    DispatchQueue.concurrentPerform(iterations: ntasks) { i in
        var lHash = [Int:Int]()
        var idx = compress(n, seq: seq[i*slice..<(i*slice+n)])
        lHash[idx] = 1
        for l in i*slice+1..<(i+1)*slice {
            idx = ((idx << 2) & mask) | Int(seq[l+n-1])
            if let value = lHash[idx] {
                lHash[idx] = value + 1
            } else {
                lHash[idx] = 1
            }
        }
        mQueue.sync {
            for (key, count) in lHash {
                if let value = hash[key] {
                    hash[key] = value + count
                } else {
                    hash[key] = count
                }
            }
        }
    }
    let startIdx = seq.count - remainder - (n - 1)
    let endIdx = seq.count - (n - 1)
    for i in startIdx..<endIdx {
        let idx = compress(n, seq: seq[i..<(i+n)])
        if let value = hash[idx] {
            hash[idx] = value + 1
        } else {
            hash[idx] = 1
        }
    }
    return hash
}

func readInputAndRewrite() -> [Int8] {
    // Read until sequence THREE
    let buf = UnsafeMutablePointer<Int8>.allocate(capacity: 100)
    defer {
        buf.deallocate(capacity: 100)
    }
    let three = ">THREE".utf8.map{Int8($0)}
    while let res = fgets(buf, 100, stdin) {
        if res[0] == three[0] && res[1] == three[1] && res[2] == three[2] {
            break
        }
    }
    // Read sequence
    var seq = [Int8]()
    while let res = fgets(buf, 100, stdin) {
        seq.append(contentsOf: UnsafeMutableBufferPointer(start: res, count: linewidth))
    }
    // Locate newline from at the end
    var nl = seq.count - 1
    var pos = nl
    while pos > seq.count - linewidth {
        if seq[pos] == 10 { // "\n"
            nl = pos
            break
        }
        pos -= 1
    }
    // Convert to bits
    var bitSeq = [Int8](repeating: 0, count: nl)
    for i in 0..<nl {
        bitSeq[i] = (seq[i] & 0x6) >> 1
    }
    return bitSeq
}

// Read sequence
var sequence = readInputAndRewrite()

let hash = getSequenceHash(1, seq: sequence)
let total = hash.reduce(0) { $0 + $1.1 }
for k in hash.keys.sorted(by: { hash[$1]! < hash[$0]! }) {
    print("\(i2c[k]!) \(String(format: "%.3f", 100.0*Double(hash[k]!)/Double(total)))")
}
print()

let hash2 = getSequenceHash(2, seq: sequence)
let total2 = hash2.reduce(0) { $0 + $1.1 }
for k in hash2.keys.sorted(by: { hash2[$1]! < hash2[$0]! }) {
    print("\(decode(k, 2)) \(String(format: "%.3f", 100.0*Double(hash2[k]!)/Double(total2)))")
}
print()

let hash3 = getSequenceHash(3, seq: sequence)
print("\(hash3[encode("GGT")] ?? 0)\tGGT")

let hash4 = getSequenceHash(4, seq: sequence)
print("\(hash4[encode("GGTA")] ?? 0)\tGGTA")

let hash6 = getSequenceHash(6, seq: sequence)
print("\(hash6[encode("GGTATT")] ?? 0)\tGGTATT")

let hash12 = getSequenceHash(12, seq: sequence)
print("\(hash12[encode("GGTATTTTAATT")] ?? 0)\tGGTATTTTAATT")

let hash18 = getSequenceHash(18, seq: sequence)
print("\(hash18[encode("GGTATTTTAATTTATAGT")] ?? 0)\tGGTATTTTAATTTATAGT")
