import Swimsuit
import Foundation

class Triple :  Hashable, Equatable {
      let x, y, z : Int
      init(x: Int, y:Int, z:Int) {
        self.x = x
        self.y = y
        self.z = z
      }
      final var hashValue: Int {
          return x + 31 &* y + 961 &* z
      }
      static func == (lhs: Triple, rhs: Triple) -> Bool {
        return
            lhs.x == rhs.x &&
            lhs.y == rhs.y &&
            lhs.z == rhs.z
       }
}

class BufferedTriple :  Hashable, Equatable {
      let x, y, z : Int
      private let hash : Int
      init(x: Int, y:Int, z:Int) {
        self.x = x
        self.y = y
        self.z = z
        hash =  x + 31 &* y + 961 &* z
      }

      final var hashValue: Int {
          return hash
      }
      static func == (lhs: BufferedTriple, rhs: BufferedTriple) -> Bool {
        return
            lhs.x == rhs.x &&
            lhs.y == rhs.y &&
            lhs.z == rhs.z
       }
}

var bs = Set<Triple>()
var bbs = Set<BufferedTriple>()

for k in 0..<10000000 {
  bs.insert(Triple(x:-2,y:k - 2,z:2 * k - 2))
  bbs.insert(BufferedTriple(x:-2,y:k - 2,z:2 * k - 2))
}




var counter = 0
var nano : UInt64

for time in 0...10 {
  print("test \(time + 1)")
  let N = 100
  var a = [Triple]()
  a.reserveCapacity(N)
  var ba = [BufferedTriple]()
  ba.reserveCapacity(N)
  nano = Swimsuit.nanotime {
      for k in 1...N {
         a.append(Triple(x:k,y:2*time*k+1,z:3*time*k+1))
      }
  }
  print("Construct triples \(nano) ns, \(counter) ")
  nano = Swimsuit.nanotime {
      for k in 1...N {
          ba.append(BufferedTriple(x:k,y:2*time*k+1,z:3*time*k+1))
      }
  }
  print("Construct buffered triples \(nano) ns, \(counter) ")


  nano = Swimsuit.nanotime {
      for i in a {
          counter += i.hashValue
      }
  }
  print("Just hash     \(nano) ns, \(counter) ")

  nano = Swimsuit.nanotime {
      for i in a {
          if bs.contains(i) {
            counter += 1
          }
      }
  }
  print("Triple         \(nano) ns, \(counter) ")


  counter = 0
  nano = Swimsuit.nanotime {
      for i in ba {
          if bbs.contains(i) {
            counter += 1
          }
      }
  }
  print("BufferedTriple \(nano) ns, \(counter) ")
  print("")
}
