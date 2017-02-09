import Foundation;
import Swimsuit;
let newcount = 1000

for t in 0..<10 {
print("trial \(t)")
var z = [Int]()
var nano = Swimsuit.nanotime() {
   z = Array(repeating: 0, count: newcount)
}

var nanoperentry = Double(nano)/Double(newcount)
print("\(nanoperentry) ns/entry (init)")
z[newcount-1] = 10


var z1 = [Int]()
z1.append(10)
let nano1 = Swimsuit.nanotime() {
   for i in 0..<newcount {
      z1.append(0)
   }
}

let nano1perentry = Double(nano1)/Double(newcount + 1)
print("\(nano1perentry) ns/entry (for loop)")

var z1r = [Int]()
z1r.append(10)
let nano1r = Swimsuit.nanotime() {
   z1r.reserveCapacity(newcount + 1)
   for i in 0..<newcount {
      z1r.append(0)
   }
}

let nano1rperentry = Double(nano1r)/Double(newcount + 1)
print("\(nano1rperentry) ns/entry (for loop with reserve) ")

var z2 = UnsafeMutablePointer<Int>.allocate(capacity:newcount + 1)
z2[0] = 10
let nano2 = Swimsuit.nanotime() {
   for i in 1...newcount {
      z2[i] = 0
   }
}
z2.deallocate(capacity:newcount + 1)

let nano2perentry = Double(nano2)/Double(newcount + 1)
print("\(nano2perentry) ns/entry (unsafe)")


var z4 = [Int]()
z4.append(10)
let nano4 = Swimsuit.nanotime() {
   z4 += repeatElement(0,count:newcount)
}

let nano4perentry = Double(nano4)/Double(newcount + 1)
print("\(nano4perentry) ns/entry (with call to repeatElement)")


func extendArray(_ x : inout [Int], newsize: Int) {
   var answer = Array(repeating: 0, count: newsize)
   for i in 0..<x.count {
     answer[i] = x[i]
   }
   x = answer
}

var z8 = [Int]()
z8.append(10)

let nano8 = Swimsuit.nanotime() {
   extendArray(&z8,newsize:newcount+1)
}

let nano8perentry = Double(nano8)/Double(newcount + 1)
print("\(nano8perentry) ns/entry (extendArray)")

var z9 = [Int]()
z9.append(10)

func fastExtendArray(_ x : [Int], newsize: Int) -> [Int] {
   return Array((0..<newsize).lazy.map { $0 < x.count ? x[$0] : 0 })
}



let nano9 = Swimsuit.nanotime() {
   z9 = fastExtendArray(z9,newsize:newcount+1)
}

let nano9perentry = Double(nano9)/Double(newcount + 1)
print("\(nano9perentry) ns/entry (lazy.map)")



print("bogus: \(z[0]) \(z[newcount - 1])")
print()

}


var x = Array(repeating: 1738, count: 1_000_000)
var sink = x

print("A:")
print(Swimsuit.nanotime() {
  var answer = Array(repeating: 0, count: 2_000_000)
  for i in 0 ..< x.count {
    answer[i] = x[i]
  }
  // Prevent `answer` from being DCE&#039;d
  sink = answer
})

print("B:")
print(Swimsuit.nanotime() {
  var answer = Array((0..<2_000_000).lazy.map { $0 < x.count ? x[$0] : 0 })
  sink = answer
})
