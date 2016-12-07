## Flatmap performance in Swift

Improving performance of higher order functions vs loops in Swift (3.0)
http://stackoverflow.com/questions/40789010/improving-performance-of-higher-order-functions-vs-loops-in-swift-3-0

Requirement: Swift 3.0 or better.

```
$ swift build --configuration release
$ ./.build/release/flatmap
```

## For Xcode users (Mac only)

```
$  swift package generate-xcodeproj
generated: ./flatmap.xcodeproj
$ open ./flatmap.xcodeproj
```

You can also profile the application:
```
$ iprofiler -timeprofiler ./.build/release/flatmap
```
Then open the newly created file with Instrument.

