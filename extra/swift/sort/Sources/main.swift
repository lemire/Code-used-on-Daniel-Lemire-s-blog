import Foundation
import Dispatch

#if os(Linux)
    import Glibc
#else
    import Darwin
#endif

func randomInt(_ max: Int) -> Int {
    #if os(Linux)
        let n = random() % max
    #else
        let n = arc4random_uniform(UInt32(max))
    #endif
    return Int(n)
}

// http://stackoverflow.com/questions/24026510/how-do-i-shuffle-an-array-in-swift
extension MutableCollection where Indices.Iterator.Element == Index {
    /// Shuffles the contents of this collection.
    mutating func shuffle() {
        let c = count
        guard c > 1 else { return }

        for (firstUnshuffled, unshuffledCount) in zip(indices, stride(from: c, to: 1, by: -1)) {
            let d: IndexDistance = numericCast(randomInt(numericCast(unshuffledCount)))
            guard d != 0 else { continue }
            let i = index(firstUnshuffled, offsetBy: d)
            swap(&self[firstUnshuffled], &self[i])
        }
    }
}

extension Sequence {
    /// Returns an array with the contents of this sequence, shuffled.
    func shuffled() -> [Iterator.Element] {
        var result = Array(self)
        result.shuffle()
        return result
    }
}

func time(test_array: [Int]) -> (Double, Double) {
    func start() -> DispatchTime { return  DispatchTime.now() }
    func lap(_ t0: DispatchTime) -> UInt64 {
        let newtime = DispatchTime.now()
        return newtime.uptimeNanoseconds - t0.uptimeNanoseconds
    }
    var shuffledarray = test_array.shuffled()
    var sortedarray = test_array.sorted()

    var t0 = start()
    shuffledarray.sort()
    let ΔE1 = lap(t0)
    t0 = start()
    sortedarray.sort()
    let ΔE2 = lap(t0)

    if shuffledarray != sortedarray {
      print ("bug")
    }
    let Denom = Double(test_array.count)

    return (Double(ΔE1)/Denom, Double(ΔE2)/Denom)
}

func runtest(_ size: Int) {
  print("array size = ", size)
  var array: [Int] = []
  for _ in 0..<size {
    array.append(randomInt(1000_000))
  }
  print(time(test_array: array))
}

runtest(100)
runtest(1000)
runtest(10000)
