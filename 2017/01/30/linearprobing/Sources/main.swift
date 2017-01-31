import Foundation;
import Swimsuit;

for power in 20...30 {
  let size = 1 << power
  print("trial \(size)")
  // could use an  IndexSet, but we want to illustrate a generic problem (not specific to Int)
  var d = Set<Int>()
  let nanobuild = Swimsuit.nanotime() {
     for i in 1...size {
       d.insert(i)
     }
  }
  var dump = [Int]()
  for i in d {
    dump.append(i)
  }
  print("time to build set: ", Double(nanobuild) / Double(size) , " ns/element")
  var dcopy = Set<Int>() // could make much faster by specify minimumCapacity
  dcopy.insert(1000)
  let nanocopy = Swimsuit.nanotime() {
     for i in dump {
       dcopy.insert(i)
     }
  }
  print("time for naive copy: ", Double(nanocopy) / Double(size) , " ns/element")
  print("ratio build/copy : ", Double(nanocopy)/Double(nanobuild))

  var dumpr = dump.reversed()
  var dcopyr = Set<Int>() // could make much faster by specify minimumCapacity
  dcopyr.insert(1000)
  let nanocopyr = Swimsuit.nanotime() {
     for i in dumpr {
       dcopyr.insert(i)
     }
  }
  print("time for naive copy (reversed): ", Double(nanocopyr) / Double(size) , " ns/element")

}
