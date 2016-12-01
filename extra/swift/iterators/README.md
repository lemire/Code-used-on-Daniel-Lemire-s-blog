## Iterator performance in Swift

Answer to this question:
http://stackoverflow.com/questions/40689128/why-are-swift-iterators-slower-than-array-building


Requirement: Swift 3.0 or better.

```
$ swift build --configuration release
$ ./.build/release/flatteniterator
```

## For Xcode users (Mac only)

```
$  swift package generate-xcodeproj
generated: ./flatteniterator.xcodeproj
$ open ./flatteniterator.xcodeproj
```
