import Foundation;
import Swimsuit;

let newcount = 10000


let nano1 = Swimsuit.nanotime() {
   var z = [Int]()
   for i in 0..<newcount {
      z.append(i)
   }
}

let nano1perentry = round(Double(nano1)/Double(newcount))
print("\(nano1perentry) ns/entry")

let nano2 = Swimsuit.nanotime() {
   var z = UnsafeMutablePointer<Int>.allocate(capacity:newcount)
   for i in 0..<newcount {
      z[i] = i
   }
   z.deallocate(capacity:newcount)
}

let nano2perentry = round(Double(nano2)/Double(newcount))
print("\(nano2perentry) ns/entry")


