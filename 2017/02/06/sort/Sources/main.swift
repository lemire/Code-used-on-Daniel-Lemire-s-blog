import Foundation
import Swimsuit

var array = [Int]()

let howmany  = 1_000_000

for i in 0...howmany {
  array.append( ( i &* (howmany - 1) ) % howmany ) // generates somewhat random looking numbers
}

let nano1 = Swimsuit.nanotime() {
  array.sort()
}
let nano2 = Swimsuit.nanotime() {
  array.sort()
}

print(" \( nano1 / UInt64(howmany) )  \( nano2 / UInt64(howmany) ) ")



