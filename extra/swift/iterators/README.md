## Iterator performance in Swift

Answer to this question:

Why are Swift iterators slower than array building?
http://stackoverflow.com/questions/40689128/why-are-swift-iterators-slower-than-array-building

The answer to the question is that you can have fast iterators in Swift, that provide better performance than
materializing arrays. You just have to write efficient code.

Requirement: Swift 3.0 or better.

```
$ swift build --configuration release
$ ./.build/release/flatteniterator
print time elapsed in ms for array_iterate, array_store, generate_iterate, generate_store, fastiterator, fast iterator store
(0.0073099999999999997, 0.0069870000000000002, 0.18385799999999999, 0.18745700000000001, 0.005372, 0.015883999999999999)
(2.125931, 2.1169820000000001, 15.064767, 15.45152, 1.572919, 1.964912)
(2.9140269999999999, 2.9064290000000002, 9.8297640000000008, 10.38635, 1.978038, 2.2565339999999998)
```

## For Xcode users (Mac only)

```
$  swift package generate-xcodeproj
generated: ./flatteniterator.xcodeproj
$ open ./flatteniterator.xcodeproj
```
