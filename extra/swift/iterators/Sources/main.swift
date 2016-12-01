import Foundation


/**
* We assume that the provided array is made of integers or arrays
* of integers.
*/
public struct FastFlattenIterator: IteratorProtocol {
   let segments: [Any]
    var i = 0 // top-level index
    var j = 0 // second-level index
    var jmax = 0 // essentially, this is currentarray.count, but we buffer it
    var currentarray : [Int]! // quick reference to an int array to be flatten

   init(_ segments: [Any]) {
       self.segments = segments
   }

   public mutating func next() -> Int? {
     if j > 0 { // we handle the case where we iterate within an array separately
       let val = currentarray[j]
       j += 1
       if j == jmax {
         j = 0
         i += 1
       }
       return val
     }
     while i < segments.count {
        switch segments[i] {
          case let e as Int: // found an integer value
            i += 1
            return e
          case let E as [Int]: // first encounter with an array
            jmax = E.count
            currentarray = E
            if jmax > 0 {
              j = 1
              return E[0]
            }
            i += 1
          default:
            return nil
        }
     }
     return nil
   }
}


func chain(_ segments: [Any]) -> AnyIterator<Int>{
    var i = 0
    var j = 0
    return AnyIterator<Int> {
        while i < segments.count {
            switch segments[i] {
                case let e as Int:
                    i += 1
                    return e
                case let E as [Int]:
                    if j < E.count {
                        let val = E[j]
                        j += 1
                        return val
                    }
                    j = 0
                    i += 1

                default:
                    return nil
            }
        }
        return nil
    }
}


func flatten_array(_ segments: [Any]) -> [Int] {
    var result = [Int]()
    for segment in segments {
        switch segment {
            case let segment as Int:
                result.append(segment)
            case let segment as [Int]:
                result.append(contentsOf: segment)
            default:
                break
        }
    }
    return result
}


func time(test_array: [Any], cycles: Int = 10) -> (array_iterate: Double,
                                                        array_store  : Double,
                                                        generate_iterate: Double,
                                                        generate_store: Double,
                                                        fastflatten_iterator: Double,
                                                        fastflatten_iterator_store: Double) {
    func start() -> DispatchTime { return  DispatchTime.now() }
    func lap(_ t0: DispatchTime) -> UInt64 {
        let newtime = DispatchTime.now()
        return newtime.uptimeNanoseconds - t0.uptimeNanoseconds
    }
    let expectedlength = flatten_array(test_array).count
    var t0 = start()

    for _ in 0..<cycles {
        for _ in flatten_array(test_array) { }
    }
    let ΔE1 = lap(t0)

    t0 = start()
    for _ in 0..<cycles {
        let myarray: [Int] = flatten_array(test_array)
        if expectedlength != myarray.count {
          print("bug")
        }
    }
    let ΔE2 = lap(t0)

    t0 = start()
    for _ in 0..<cycles {
        let G = chain(test_array)
        while let _ = G.next() { }
    }
    let ΔG1 = lap(t0)

    t0 = start()
    for _ in 0..<cycles {
        let myarray: [Int] = Array(chain(test_array))
        if expectedlength != myarray.count {
            print("bug")
        }
    }
    let ΔG2 = lap(t0)


    t0 = start()
    for _ in 0..<cycles {
        var F = FastFlattenIterator(test_array)
        while let _ = F.next() { }
    }
    let ΔFF1 = lap(t0)

    t0 = start()
    for _ in 0..<cycles {
        var myarray = [Int]()
        var F = FastFlattenIterator(test_array)
        while let x = F.next() { myarray.append(x)}
        if expectedlength != myarray.count {
          print("bug")
        }
    }
    let ΔFF2 = lap(t0)


    let Denom = 1000_000.0

    return (Double(ΔE1)/Denom, Double(ΔE2)/Denom, Double(ΔG1)/Denom, Double(ΔG2)/Denom, Double(ΔFF1)/Denom, Double(ΔFF2)/Denom)
}

let array1: [Any] = [Array(1...100), Array(101...105), 106,
                     Array(107...111), 112, 113, 114, Array(115...125)]
let array2: [Any] = Array(repeating: Array(1...5), count: 2000)
let array3: [Any] = Array(repeating: 31, count: 10000)

print("print time elapsed in ms for array_iterate, array_store, generate_iterate, generate_store, fastiterator, fast iterator store")

print(time(test_array: array1, cycles: 10))

print(time(test_array: array2, cycles: 10))

print(time(test_array: array3, cycles: 10))
