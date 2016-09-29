import CSwiftCallOverhead

func fibo(_ i : inout Int32, _ j : inout Int32) {
  CSwiftCallOverhead.fibo(&i,&j)
}
