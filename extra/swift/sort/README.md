## Flatmap performance in Swift

Which general purpose sorting algorithm does Swift use? It does not perform well on sorted data
http://stackoverflow.com/questions/41031106/which-general-purpose-sorting-algorithm-does-swift-use-it-does-not-perform-well

Requirement: Swift 3.0 or better.

```
$ swift build --configuration release
$ ./.build/release/sort
```

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

