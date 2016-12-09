import Foundation
import Dispatch

// http://stackoverflow.com/questions/24026510/how-do-i-shuffle-an-array-in-swift
extension MutableCollection where Indices.Iterator.Element == Index {
    /// Shuffles the contents of this collection.
    mutating func shuffle() {
        let c = count
        guard c > 1 else { return }

        for (firstUnshuffled , unshuffledCount) in zip(indices, stride(from: c, to: 1, by: -1)) {
            let d: IndexDistance = numericCast(arc4random_uniform(numericCast(unshuffledCount)))
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
    let shuffledarray = test_array.shuffled()
    let sortedarray = array.sorted()

    var t0 = start()
    let x = shuffledarray.sorted()
    let ΔE1 = lap(t0)
    t0 = start()
    let y = sortedarray.sorted()
    let ΔE2 = lap(t0)

    if x != y {
      print ("bug")
    }
    let Denom = Double(test_array.count)

    return (Double(ΔE1)/Denom, Double(ΔE2)/Denom)
}

var array: [Int] = []
for x in 0..<10000 {
    array.append(x)
}
for i in 0..<10 {
  print(time(test_array: array))
}
