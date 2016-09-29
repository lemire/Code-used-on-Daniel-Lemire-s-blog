Requirement: Swift 3.0 or better.

```
clang -Ofast  -o purec code/purec.c
./purec
swift test
```
```
$ swift build --configuration release
$ time ./.build/release/LittleSwiftTest
3221225470

real       0m2.030s
user       0m2.028s
sys        0m0.000s
$ time ./.build/release/LittleCOverheadTest
3221225470

real       0m1.778s
user       0m1.776s
sys        0m0.000s

$ clang -Ofast -o purec  code/purec.c
$ time ./purec
3221225470

real       0m1.747s
user       0m1.744s
sys        0m0.000s
```
## For XCode users (Mac only)

```
$  swift package generate-xcodeproj
generated: ./SwiftCallOverhead.xcodeproj
$ open ./SwiftCallOverhead.xcodeproj
```
