import Foundation
import Dispatch



func basicsum(_ array: [Int]) -> Int {
    var s = 0
    for i in array {
      s += i
    }
    return s
}

func unsafebasicsum(_ array: [Int]) -> Int {
    var s = 0
    for i in array {
      s = s &+ i
    }
    return s
}



func reducesum(_ array: [Int]) -> Int {
    return array.reduce(0,+)
}
func unsafereducesum(_ array: [Int]) -> Int {
    return array.reduce(0,{$0 &+ $1})
}


func time() -> (Double,Double,Double,Double) {
    func start() -> DispatchTime { return  DispatchTime.now() }
    func lap(_ t0: DispatchTime) -> UInt64 {
        let newtime = DispatchTime.now()
        return newtime.uptimeNanoseconds - t0.uptimeNanoseconds
    }
    var array = [Int]()
    var expectedsum = 0
    for index in 1...100000 {
      array.append(index)
      expectedsum += index
    }
    var t0 = start()
          if basicsum(array) != expectedsum {
            print ("bug")
          }
    let ΔE1 = lap(t0)

    t0 = start()
          if reducesum(array) != expectedsum {
            print ("bug")
          }
    let ΔE2 = lap(t0)

    t0 = start()
          if unsafebasicsum(array) != expectedsum {
            print ("bug")
          }
    let ΔE3 = lap(t0)

    t0 = start()
          if unsafereducesum(array) != expectedsum {
            print ("bug")
          }
    let ΔE4 = lap(t0)



    let Denom = Double(array.count)

    return (Double(ΔE1)/Denom, Double(ΔE2)/Denom, Double(ΔE3)/Denom, Double(ΔE4)/Denom)
}

print(time())
