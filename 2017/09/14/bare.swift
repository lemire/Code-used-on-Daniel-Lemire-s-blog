

func multiplyHigh(_ value1 : UInt64, _ value2 : UInt64) -> UInt64 {
      return value1.multipliedFullWidth(by: value2).high
}

func popCnt(_ value : UInt64) -> Int {
      return value.nonzeroBitCount
}


func popcntArray( _  value  : inout [UInt64] ) -> Int {
    return value.reduce( 0, { x, y in
        x &+ popCnt(y)
        })
}


func multiplyHighArray( _  value  : inout [UInt64] , _ multiplier : UInt64) {
    for (index,element) in value.enumerated() {
        value[index] = multiplyHigh(element, multiplier)
    }
}


func getmultiplyHighArray( _  value  :  [UInt64] , _ multiplier : UInt64) ->[UInt64] {
     return value.map{ (number) -> UInt64 in
        return multiplyHigh(number, multiplier)}
}

func multiplyHighPointer( _  value  : inout UnsafeMutablePointer<UInt64> , size: Int, _ multiplier : UInt64)  {
    for index in 0..<size {
        value[index] = multiplyHigh(value[index], multiplier)
    }
}
