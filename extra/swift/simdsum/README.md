## SIMD sum performance in Swift

Swift SIMD or Accelerate Sum UInt32
http://stackoverflow.com/questions/41257678/swift-simd-or-accelerate-sum-uint32
Requirement: Swift 3.0 or better.

```
$ swift build --configuration release
$ ./.build/release/simdsum
```

## check the assembly

```
$ swift build --configuration release  -Xswiftc -Ounchecked -Xswiftc -g
$ lldb ./.build/release/simdsum
(lldb) di -m -n simdsum.scalarsum
```

## For Xcode users (Mac only)

```
$  swift package generate-xcodeproj
generated: ./simdsum.xcodeproj
$ open ./simdsum.xcodeproj
```

You can also profile the application:
```
$ iprofiler -timeprofiler ./.build/release/simdsum
```
Then open the newly created file with Instrument.

