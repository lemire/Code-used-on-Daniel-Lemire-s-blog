import XCTest
@testable import SwiftCallOverhead
@testable import CSwiftCallOverhead
class SwiftCallOverheadTests: XCTestCase {
  func testPureCPerformance() {
    measure {
     print(CSwiftCallOverhead.howmany())
    }
  }

  func testNativePerformance() {
    measure {
      var i: Int32 = 1
      var j: Int32 = 1
      var counter = 0
      // fibonacci
      while ( j != 0 ) {
        let c = j
        j = i &+ j
        i = c
        counter += 1
      }
      print (counter)
    }
  }

  func testDirectCPerformance() {
    measure {
      var i: Int32 = 1
      var j: Int32 = 1
      var counter = 0
      // fibonacci
      while ( j != 0 ) {
        CSwiftCallOverhead.fibo(&i, &j)
        counter += 1
      }
      print (counter)
    }
  }

  func testCPerformance() {
    measure {
      var i: Int32 = 1
      var j: Int32 = 1
      var counter = 0
      // fibonacci
      while ( j != 0 ) {
        SwiftCallOverhead.wrappedfibo(&i, &j)
        counter += 1
      }
      print (counter)
    }
  }

}

#if os(Linux)
extension SwiftCallOverheadTests {
  static var allTests: [(String, (SwiftCallOverheadTests) -> Void throws->Void)] {
    return [
      ("testDirectCPerformance()", testDirectCPerformance),
      ("testNativePerformance()", testNativePerformance),
      ("testCPerformance()", testCPerformance),
      ("testPureCPerformance()", testPureCPerformance)
    ]
  }
}
#endif
