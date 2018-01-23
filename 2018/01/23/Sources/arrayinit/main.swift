import Swimsuit
import Foundation

let size = 1_000
let skip = 100
let maxval = skip * size

var nano : UInt64

var checker = [Int]()
for i in stride(from: 0, to: maxval, by: skip) {
      checker.append(i)
}

func pad( _ s : String) -> String {
  return  s.padding(toLength: 32, withPad: " ", startingAt: 0)
}

nano = Swimsuit.nanotime {
      var b = [Int]()
      for i in stride(from: 0, to: maxval, by: skip) {
          b.append(i)
      }
      assert( b == checker, "You have a bug!")
    }
print(pad("append "), Double(nano) / Double(size), " ns")


nano = Swimsuit.nanotime {
      var b = [Int]()
      b.reserveCapacity(size)
      for i in stride(from: 0, to: maxval, by: skip) {
          b.append(i)
      }
      assert( b == checker, "You have a bug!")
    }
print(pad("reserve + append"), Double(nano) / Double(size), " ns")



nano = Swimsuit.nanotime {
      var b = [Int](repeating: 0, count: size )
      for i in 0..<size {
          b[i] = skip * i
      }
      assert( b == checker, "You have a bug!")
    }
print(pad("init + set"), Double(nano) / Double(size), " ns")

nano = Swimsuit.nanotime {
      let b = Array((0..<size).map { skip * $0 })
      assert( b == checker, "You have a bug!")
}
print(pad("map"), Double(nano) / Double(size), " ns")


nano = Swimsuit.nanotime {
      let b = Array((0..<size).lazy.map { skip * $0 })
      assert( b == checker, "You have a bug!")
}
print(pad("lazymap"), Double(nano) / Double(size), " ns")


nano = Swimsuit.nanotime {
      let b = Array(stride(from: 0, to: maxval, by: skip).lazy)
      assert( b == checker, "You have a bug!")
}
print(pad("lazy stride"), Double(nano) / Double(size), " ns")


nano = Swimsuit.nanotime {
      let b = Array(stride(from: 0, to: maxval, by: skip))
      assert( b == checker, "You have a bug!")
}

print(pad("stride"), Double(nano) / Double(size), " ns")

public extension Array {
  public init(count: Int, generator: @escaping() -> Element) {
    precondition(count >= 0, "arrays must have non-negative size")
    self.init(AnyIterator(generator).prefix(count))
  }
}


nano = Swimsuit.nanotime {
      var runningTotal : Int = 0
      let b = Array(count: size) {() -> Int in 
           runningTotal += skip 
           return runningTotal
      }
      assert( b == checker, "You have a bug!")
}
print(pad("Erica Sedun"), Double(nano) / Double(size), " ns")
