import Foundation
import Swimsuit
import Dispatch

let count = 1_000_000
var data = [Int].init(repeating: 0, count: count)

fileprivate func increment(_ offset : Int) {
  for i in data.indices { data[i] = data[i] &+ offset }
}

var best_nano = UInt64.max
for t in 1..<5 {
  best_nano = min(best_nano,Swimsuit.nanotime() {
   increment(1)
  } )
}

print("single threaded : ", Double(best_nano) / Double(count), " ns/operation")

best_nano =  UInt64.max
let queues = 100
for t in 1..<5 {
  best_nano = min(best_nano,Swimsuit.nanotime() {
  var q = OperationQueue()
  for i in 1...queues {
    q.addOperation {
      increment(i)
    }
  }
  q.waitUntilAllOperationsAreFinished()
  })
}

print("OperationQueue  : ", Double(best_nano) / Double(count * queues), " ns/operation")

