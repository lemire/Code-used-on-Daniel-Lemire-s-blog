import PackageDescription

let package = Package(
    name: "SwiftCallOverhead",
    targets: [
        Target(name: "SwiftCallOverhead", dependencies: ["CSwiftCallOverhead"]),
        Target(name: "LittleTest", dependencies: ["CSwiftCallOverhead"]),
        Target(name: "LittleCOverheadTest", dependencies: ["CSwiftCallOverhead"]),
      ],
    exclude: ["code"]
)
