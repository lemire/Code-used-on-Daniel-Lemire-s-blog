import CSwiftCallOverhead

var i: Int32 = 1
var j: Int32 = 1
var counter = 0
// fibonacci
while ( j != 0 ) {
  CSwiftCallOverhead.fibo(&i, &j)
  counter += 1
}
print (counter)
