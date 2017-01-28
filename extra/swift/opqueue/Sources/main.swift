import Foundation
import Swimsuit
import Dispatch

let count = 1_000_000
var data = [Int].init(repeating: 0, count: count)

fileprivate func increment(_ offset : Int) {
  for i in data.indices { data[i] = data[i] &+ offset }
}

for t in 1..<5 {
  print("single threaded trial number \(t)")
  print( Double(Swimsuit.nanotime() {
   increment(1)
  } ) / Double( count ), " ns/operation" )
}

let queues = 10
for t in 1..<5 {
  print("OperationQueue trial number \(t)")
  let nano = Swimsuit.nanotime() {
  var q = OperationQueue()
  for i in 1...queues {
    q.addOperation {
      increment(i)
    }
  }
  q.waitUntilAllOperationsAreFinished()
  }
  print(Double (nano) / Double ( queues * count), " ns/operation")
}
