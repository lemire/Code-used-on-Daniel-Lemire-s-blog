## Flatmap performance in Swift

Which general purpose sorting algorithm does Swift use? It does not perform well on sorted data
http://stackoverflow.com/questions/41031106/which-general-purpose-sorting-algorithm-does-swift-use-it-does-not-perform-well

Requirement: Swift 3.0 or better.

```
$ swift build --configuration release
$ ./.build/release/sort

array size =  100
(31.84, 83.930000000000007)
array size =  1000
(44.564, 113.7)
array size =  10000
(59.107599999999998, 145.21690000000001)
```

Thus sorting an already sorted array over 2x more expensive than sorting a shuffled array.

## For Xcode users (Mac only)

```
$  swift package generate-xcodeproj
generated: ./sort.xcodeproj
$ open ./sort.xcodeproj
```

You can also profile the application:
```
$ iprofiler -timeprofiler ./.build/release/sort
```
Then open the newly created file with Instrument.

