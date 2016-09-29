import XCTest
@testable import SwiftCallOverhead
class SwiftCallOverheadTests : XCTestCase {
  func testNativePerformance() {
    measure() {
      var i : Int32 = 1;
      var j : Int32 = 1;
      var counter = 0;
      // fibonacci
      while ( j != 0 ) {
        let c = j;
        j = i &+ j;
        i = c;
        counter += 1;
      }
      print (counter);
    }
  }

  func testCPerformance() {
    measure() {
      var i : Int32 = 1;
      var j : Int32 = 1;
      var counter = 0;
      // fibonacci
      while ( j != 0 ) {
        SwiftCallOverhead.fibo(&i,&j);
        counter += 1;
      }
      print (counter);
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
