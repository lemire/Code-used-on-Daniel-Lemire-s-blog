import Foundation;
import Swimsuit;

for power in 20...30 {
  let size = 1 << power
  print("trial \(size)")
  var d = Set<Int>()
  let nanobuild = Swimsuit.nanotime() {
     for i in 1...size {
       d.insert(i)
     }
  }
  print("time to build set: ", Double(nanobuild) / Double(size) , " ns/element")
  var dcopy = Set<Int>()
  dcopy.insert(1000)
  let nanocopy = Swimsuit.nanotime() {
     for i in d {
       dcopy.insert(i)
     }
  }
  print("time for naive copy: ", Double(nanocopy) / Double(size) , " ns/element")
  print("ratio build/copy : ", Double(nanocopy)/Double(nanobuild))
}
