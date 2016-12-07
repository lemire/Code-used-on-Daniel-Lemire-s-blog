import Foundation
import Dispatch

func loopflatmap (_ test_array: [Int]) -> [String] {
   var result: [String] = []
    for x in array {
      let tmp = "string\(x)"
      if tmp.hasSuffix("1234") {
          result.append(tmp)
      }
    }
    return result;
}

func fastloopflatmap (_ test_array: [Int]) -> [String] {
   var result: [String] = []
    for x in array {
      if x % 10000 == 1234 {
          result.append("string\(x)")
      }
    }
    return result;
}


func filtermap (_ test_array: [Int]) -> [String] {
  return array.map { "string\($0)" }
                               .filter { $0.hasSuffix("1234") }
}

func straightflatmap (_ test_array: [Int]) -> [String] {
  return array.flatMap { x -> String? in
            let tmp = "string\(x)"
            return tmp.hasSuffix("1234") ? tmp : nil
        }
}


func time(test_array: [Int]) -> (Double, Double, Double, Double) {
    func start() -> DispatchTime { return  DispatchTime.now() }
    func lap(_ t0: DispatchTime) -> UInt64 {
        let newtime = DispatchTime.now()
        return newtime.uptimeNanoseconds - t0.uptimeNanoseconds
    }
    let expectedlength = straightflatmap(test_array).count
    var t0 = start()
    if loopflatmap(test_array).count != expectedlength {
      print("bug")
    }
    let ΔE1 = lap(t0)


    t0 = start()
    if filtermap(test_array).count != expectedlength {
      print("bug")
    }
    let ΔE2 = lap(t0)

    t0 = start()
    if straightflatmap(test_array).count != expectedlength {
      print("bug")
    }
    let ΔE3 = lap(t0)

    t0 = start()
    if fastloopflatmap(test_array).count != expectedlength {
      print("bug")
    }
    let ΔE4 = lap(t0)

 
    let Denom = Double(test_array.count)

    return (Double(ΔE1)/Denom, Double(ΔE2)/Denom, Double(ΔE3)/Denom, Double(ΔE4)/Denom)
}

var array: [Int] = []
for x in 0..<10000 {
    array.append(x)
}
for i in 0..<10 {
  print(time(test_array: array))
}
