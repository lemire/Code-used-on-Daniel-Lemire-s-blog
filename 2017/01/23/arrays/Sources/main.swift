
import Swimsuit;

let newcount = 10000


let nano1 = Swimsuit.nanotime() {
   var z = [Int]()
   while z.count < newcount {
      z.append(z.count)
   }
}

print(nano1)

let nano2 = Swimsuit.nanotime() {
   var z = UnsafeMutablePointer<Int>.allocate(capacity:newcount)
   for i in 0..<newcount {
      z[i] = i
   }
   z.deallocate(capacity:newcount)
}

print(nano2)

