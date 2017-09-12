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
