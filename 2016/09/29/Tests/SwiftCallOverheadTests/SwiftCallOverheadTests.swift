import XCTest
@testable import SwiftCallOverhead
class SwiftCallOverheadTests : XCTestCase {
  func testNativePerformance() {
    measure() {
      var i = 1;
      var j = 1;
      // fibonacci
      while ( j < 1000000 ) {
        let c = j;
        j = i + j;
        i = j;
      }
      print (j);
    }
  }

  func testCPerformance() {
    measure() {
      var i = 1;
      var j = 1;
      // fibonacci
      while ( j < 1000000 ) {
        let ip = UnsafeMutablePointer<Int32>(i)
        let jp = UnsafeMutablePointer<Int32>(j)
        SwiftCallOverhead.fibo(ip,jp)
      }
      print (j);
    }
  }

}

#if os(Linux)
extension SwiftCallOverheadTests {
  static var allTests : [(String, (SwiftCallOverheadTests)->() throws->Void)] {
    return [
      ("testNativePerformance()", testNativePerformance),
      ("testCPerformance()", testCPerformance)
    ]
  }
}
#endif
