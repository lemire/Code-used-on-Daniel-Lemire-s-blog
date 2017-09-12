import Foundation
import Dispatch

// These are experiments in how well the swift compiler can optimize matrix multiplications.
//
// We know: if one is interested in performance, one should not code
// one's own linear algebra functions. Yes, yes.

func scalarproduct(_ array1: [Float], _ array2: [Float]) -> Float {
    var s = Float(0)
    let mylength = min(array1.count, array2.count) // please don't mess with me
    for i in 0..<mylength {
      s = s + (array1[i] * array2[i])
    }
    return s
}

let a =  Array(repeating: Float(12), count: 4*1024)
let b =  Array(repeating: Float(42), count: 4*1024)

print(scalarproduct(a, b))
