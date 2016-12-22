import Foundation
import simd

func scalarsum (_ test_array: [UInt32]) -> UInt32 {
   var result : UInt32 = 0
   for x in test_array {
     result = result &+ x
   }
   return result
}

func simdsum (_ test_array: [UInt32]) -> UInt32 {
   var tmpvector=uint4(0)
   // assume test_array.count is divisible by four
   let limit = test_array.count/4
   for i in 0..<limit {
     let thisvector = uint4(test_array[4*i],test_array[4*i+1],test_array[4*i+2],test_array[4*i+3])
     tmpvector = tmpvector &+ thisvector
   }
   return tmpvector[0] + tmpvector[1] + tmpvector[2] + tmpvector[3]
}

func time(test_array: [UInt32]) -> (Double, Double) {
    func start() -> DispatchTime { return  DispatchTime.now() }
    func lap(_ t0: DispatchTime) -> UInt64 {
        let newtime = DispatchTime.now()
        return newtime.uptimeNanoseconds - t0.uptimeNanoseconds
    }
    let expectedsum = scalarsum(test_array)
    var t0 = start()
    if scalarsum(test_array) != expectedsum {
      print("bug")
    }
    let ΔE1 = lap(t0)


    t0 = start()
    if scalarsum(test_array) != expectedsum {
      print("bug")
    }
    let ΔE2 = lap(t0)


    let Denom = Double(test_array.count)

    return (Double(ΔE1)/Denom, Double(ΔE2)/Denom)
}

var array: [UInt32] = []
for x in 0..<100000 {
    array.append(UInt32(x))
}
for i in 0..<10 {
  print(time(test_array: array))
}
