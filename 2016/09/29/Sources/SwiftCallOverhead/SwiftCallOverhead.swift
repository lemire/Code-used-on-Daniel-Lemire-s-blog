import CSwiftCallOverhead

func fibo(_ i : inout Int32, _ j : inout Int32) {
  let ip = UnsafeMutablePointer<Int32>(i)
  let jp = UnsafeMutablePointer<Int32>(j)
  CSwiftCallOverhead.fibo(ip,jp)
}
