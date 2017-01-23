import Foundation;
import Swimsuit;

let newcount = 10000


let nano1 = Swimsuit.nanotime() {
   var z = [Int]()
   z.append(10)
   for i in 0..<newcount {
      z.append(0)
   }
}

let nano1perentry = Double(nano1)/Double(newcount + 1)
print("\(nano1perentry) ns/entry (for loop)")


let nano1r = Swimsuit.nanotime() {
   var z = [Int]()
   z.append(10)
   z.reserveCapacity(newcount + 1)
   for i in 0..<newcount {
      z.append(0)
   }
}

let nano1rperentry = Double(nano1r)/Double(newcount + 1)
print("\(nano1rperentry) ns/entry (for loop with reserve) ")


let nano2 = Swimsuit.nanotime() {
   var z = UnsafeMutablePointer<Int>.allocate(capacity:newcount + 1)
   z[0] = 10
   for i in 1...newcount {
      z[i] = 0
   }
   z.deallocate(capacity:newcount + 1)
}

let nano2perentry = Double(nano2)/Double(newcount + 1)
print("\(nano2perentry) ns/entry (unsafe)")



let nano3 = Swimsuit.nanotime() {
   let z = [10] + repeatElement(0,count:newcount)
}

let nano3perentry = Double(nano3)/Double(newcount + 1)
print("\(nano3perentry) ns/entry (one go construction) ")

let nano4 = Swimsuit.nanotime() {
   var z = [Int]()
   z.append(10)
   z += repeatElement(0,count:newcount)
}

let nano4perentry = Double(nano4)/Double(newcount + 1)
print("\(nano4perentry) ns/entry (with call to repeatElement)")

let nano4r = Swimsuit.nanotime() {
   var z = [Int]()
   z.append(10)
   z.reserveCapacity(newcount + 1)
   z += repeatElement(0,count:newcount)
}

let nano4rperentry = Double(nano4r)/Double(newcount + 1)
print("\(nano4rperentry) ns/entry (with call to repeatElement and reserve)")


let nano5 = Swimsuit.nanotime() {
   var z = Array(repeating: 0, count: newcount + 1)
   z[0] = 10
}

let nano5perentry = Double(nano5)/Double(newcount + 1)
print("\(nano5perentry) ns/entry (one big array first)")


let nano6 = Swimsuit.nanotime() {
   var z = [Int]()
   z.append(10)
   z += Array(repeating: 0, count: newcount)
}

let nano6perentry = Double(nano6)/Double(newcount + 1)
print("\(nano6perentry) ns/entry (append huge array)")


let nano7 = Swimsuit.nanotime() {
   var z = [Int]()
   z.append(10)
   z = z + repeatElement(0,count:newcount)
}

let nano7perentry = Double(nano7)/Double(newcount + 1)
print("\(nano7perentry) ns/entry (append huge array)")

func extendArray(_ x : [Int], newsize: Int) -> [Int] {
   var answer = Array(repeating: 0, count: newsize)
   for i in 0..<x.count {
     answer[i] = x[i]
   }
   return answer
}

let nano8 = Swimsuit.nanotime() {
   var z = [Int]()
   z.append(10)
   z = extendArray(z,newsize:newcount+1)
}

let nano8perentry = Double(nano8)/Double(newcount + 1)
print("\(nano8perentry) ns/entry (append huge array)")


