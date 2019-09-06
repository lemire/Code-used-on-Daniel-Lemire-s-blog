import Foundation
import Dispatch

public struct Swimsuit {
    public static func nanotime(block: () -> Void) -> UInt64 {
        let t1 = DispatchTime.now()
        block()
        let t2 = DispatchTime.now()
        let delay = t2.uptimeNanoseconds - t1.uptimeNanoseconds
        return delay
    }
}

func fx(_ allints: inout [Int],  _ j : inout Int) {
      for k in allints.indices {
        allints[k] = j
        j &+= 1
      }
 }

 func f(_ allints: inout [Int],  _ i : inout Int) {
      var j = i
      for k in allints.indices {
        allints[k] = j
        j &+= 1
      }
      i = j
 }
let size = 1000_000
var allints = Array((0..<size).lazy.map { 100 * $0 })
var i = 0
let nano1 = Swimsuit.nanotime() {
   fx(&allints,&i)
}
let ts1 = Double(nano1) / Double(size)
print("ref \(ts1) ns")


let nano2 = Swimsuit.nanotime() {
   f(&allints,&i)
}
let ts2 = Double(nano2) / Double(size)

print("hard \(ts2) ns")
