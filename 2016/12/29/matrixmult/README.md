## Reduce performance in Swift




Requirement: Swift 3.0 or better.

```
$ swift build --configuration release
$ ./.build/release/matrixmult
```

Try also 

```
swift build --configuration release  -Xswiftc -Ounchecked
```

To dissamble, try this:

```
$ swift build --configuration release  -Xswiftc -Ounchecked -Xswiftc -g
$ lldb ./.build/release/matrixmult
> di -m -n matrixmult.scalarproduct
```

## For Xcode users (Mac only)

```
$  swift package generate-xcodeproj
generated: ./matrixmult.xcodeproj
$ open ./matrixmult.xcodeproj
```

You can also profile the application:
```
$ iprofiler -timeprofiler ./.build/release/matrixmult
```
Then open the newly created file with Instrument.

