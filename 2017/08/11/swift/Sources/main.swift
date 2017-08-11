import Foundation;
import Swimsuit;

var someInts = [Int]()

for i in 1...100_000 {
  someInts.append( 678 &* i &+ 431232 ) // pseudo-random content
}

func simpleHash(_ array : [Int]) -> Int {
  var hash = 0
  for x in array {
    hash = hash &* 31 &+ x
  }
  return hash
}



func shiftHash(_ array : [Int]) -> Int {
  var hash = 0
  for x in array {
    hash = (hash << 5) &- hash &+ x
  }
  return hash
}



func unrolledHash(_ array : [Int]) -> Int {
  var hash = 0
  let c = 31
  let c2 = 31 &* 31
  let c3 = 31 &* c2
  let c4 = 31 &* c3
  for i in stride(from:0,to:array.count/4*4,by:4) {
    hash = hash &* c4  
    hash = hash &+ array[i]  &* c3 
    hash = hash &+ array[i + 1]  &* c2   
    hash = hash &+ array[i + 2]  &* c  
    hash = hash &+ array[i + 3]
  }
  for i in stride(from:array.count/4*4,to:array.count,by:1) {
      hash = hash &* 31 &+ array[i]
  }
  return hash
}







var bogus = 0

func testHashPerformance() {
    let nano = Swimsuit.nanotime() {
      bogus = simpleHash(someInts)
    }
    print("Simple hash ", Double(nano) / Double(someInts.count), " ns/value")
}



for _ in 1...10 {
  testHashPerformance()
  print("\(bogus)")
}


print()


func testSHashPerformance() {
    let nano = Swimsuit.nanotime() {
      bogus = shiftHash(someInts)
    }
    print("Shift hash ", Double(nano) / Double(someInts.count), " ns/value")
}



for _ in 1...10 {
  testSHashPerformance()
  print("\(bogus)")
}


print()


func testUHashPerformance() {
    let nano = Swimsuit.nanotime() {
      bogus = unrolledHash(someInts)
    }
    print("Unrolled hash ", Double(nano) / Double(someInts.count), " ns/value")
}

for _ in 1...10 {
  testUHashPerformance()
  print("\(bogus)")
}
